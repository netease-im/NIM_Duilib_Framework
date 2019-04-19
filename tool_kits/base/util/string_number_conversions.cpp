// Copyright (c) 2013, NetEase Inc. All rights reserved.
//
// wrt(guangguang)
// 2013/8/28
//
// String number conversion

#include "base/util/string_number_conversions.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <wctype.h>
#include <limits>

namespace nbase {

namespace {

template <typename STR, typename INT, typename UINT, bool NEG>
struct IntToStringT {
  // This is to avoid a compiler warning about unary minus on unsigned type.
  // For example, say you had the following code:
  //   template <typename INT>
  //   INT abs(INT value) { return value < 0 ? -value : value; }
  // Even though if INT is unsigned, it's impossible for value < 0, so the
  // unary minus will never be taken, the compiler will still generate a
  // warning.  We do a little specialization dance...
  template <typename INT2, typename UINT2, bool NEG2>
  struct ToUnsignedT {};

  template <typename INT2, typename UINT2>
  struct ToUnsignedT<INT2, UINT2, false> {
    static UINT2 ToUnsigned(INT2 value) {
      return static_cast<UINT2>(value);
    }
  };

  template <typename INT2, typename UINT2>
  struct ToUnsignedT<INT2, UINT2, true> {
    static UINT2 ToUnsigned(INT2 value) {
      return static_cast<UINT2>(value < 0 ? -value : value);
    }
  };

  // This set of templates is very similar to the above templates, but
  // for testing whether an integer is negative.
  template <typename INT2, bool NEG2>
  struct TestNegT {};
  template <typename INT2>
  struct TestNegT<INT2, false> {
    static bool TestNeg(INT2 value) {
      // value is unsigned, and can never be negative.
      return false;
    }
  };
  template <typename INT2>
  struct TestNegT<INT2, true> {
    static bool TestNeg(INT2 value) {
      return value < 0;
    }
  };

  static STR IntToString(INT value) {
    // log10(2) ~= 0.3 bytes needed per bit or per byte log10(2**8) ~= 2.4.
    // So round up to allocate 3 output characters per byte, plus 1 for '-'.
    const int kOutputBufSize = 3 * sizeof(INT) + 1;

    // Allocate the whole string right away, we will right back to front, and
    // then return the substr of what we ended up using.
    STR outbuf(kOutputBufSize, 0);

    bool is_neg = TestNegT<INT, NEG>::TestNeg(value);
    // Even though is_neg will never be true when INT is parameterized as
    // unsigned, even the presence of the unary operation causes a warning.
    UINT res = ToUnsignedT<INT, UINT, NEG>::ToUnsigned(value);

    for (typename STR::iterator it = outbuf.end();;) {
      --it;
      assert(it != outbuf.begin());
      *it = static_cast<typename STR::value_type>((res % 10) + '0');
      res /= 10;

      // We're done..
      if (res == 0) {
        if (is_neg) {
          --it;
          assert(it != outbuf.begin());
          *it = static_cast<typename STR::value_type>('-');
        }
        return STR(it, outbuf.end());
      }
    }
    // Unreachable
  }
};

// Utility to convert a character to a digit in a given base
template<typename CHAR, int BASE, bool BASE_LTE_10> class BaseCharToDigit {
};

// Faster specialization for bases <= 10
template<typename CHAR, int BASE> class BaseCharToDigit<CHAR, BASE, true> {
 public:
  static bool Convert(CHAR c, uint8_t* digit) {
    if (c >= '0' && c < '0' + BASE) {
      *digit = c - '0';
      return true;
    }
    return false;
  }
};

// Specialization for bases where 10 < base <= 36
template<typename CHAR, int BASE> class BaseCharToDigit<CHAR, BASE, false> {
 public:
  static bool Convert(CHAR c, uint8_t* digit) {
    if (c >= '0' && c <= '9') {
      *digit = c - '0';
    } else if (c >= 'a' && c < 'a' + BASE - 10) {
      *digit = c - 'a' + 10;
    } else if (c >= 'A' && c < 'A' + BASE - 10) {
      *digit = c - 'A' + 10;
    } else {
      return false;
    }
    return true;
  }
};

template<int BASE, typename CHAR> bool CharToDigit(CHAR c, uint8_t* digit) {
  return BaseCharToDigit<CHAR, BASE, BASE <= 10>::Convert(c, digit);
}

// There is an IsWhitespace for wchars defined in string_util.h, but it is
// locale independent, whereas the functions we are replacing were
// locale-dependent. TBD what is desired, but for the moment let's not introduce
// a change in behaviour.
template<typename CHAR> class WhitespaceHelper {
};

template<> class WhitespaceHelper<char> {
 public:
  static bool Invoke(char c) {
    return 0 != isspace(static_cast<unsigned char>(c));
  }
};

template<> class WhitespaceHelper<wchar_t> {
 public:
  static bool Invoke(wchar_t c) {
    return 0 != iswspace(c);
  }
};

template<typename CHAR> bool LocalIsWhitespace(CHAR c) {
  return WhitespaceHelper<CHAR>::Invoke(c);
}

// IteratorRangeToNumberTraits should provide:
//  - a typedef for iterator_type, the iterator type used as input.
//  - a typedef for value_type, the target numeric type.
//  - static functions min, max (returning the minimum and maximum permitted
//    values)
//  - constant kBase, the base in which to interpret the input
template<typename IteratorRangeToNumberTraits>
class IteratorRangeToNumber {
 public:
  typedef IteratorRangeToNumberTraits traits;
  typedef typename traits::iterator_type const_iterator;
  typedef typename traits::value_type value_type;

  // Generalized iterator-range-to-number conversion.
  //
  static bool Invoke(const_iterator begin,
                     const_iterator end,
                     value_type* output) {
    bool valid = true;

    while (begin != end && LocalIsWhitespace(*begin)) {
      valid = false;
      ++begin;
    }

    if (begin != end && *begin == '-') {
      if (!Negative::Invoke(begin + 1, end, output)) {
        valid = false;
      }
    } else {
      if (begin != end && *begin == '+') {
        ++begin;
      }
      if (!Positive::Invoke(begin, end, output)) {
        valid = false;
      }
    }

    return valid;
  }

 private:
  // Sign provides:
  //  - a static function, CheckBounds, that determines whether the next digit
  //    causes an overflow/underflow
  //  - a static function, Increment, that appends the next digit appropriately
  //    according to the sign of the number being parsed.
  template<typename Sign>
  class Base {
   public:
    static bool Invoke(const_iterator begin, const_iterator end,
                       typename traits::value_type* output) {
      *output = 0;

      if (begin == end) {
        return false;
      }

      // Note: no performance difference was found when using template
      // specialization to remove this check in bases other than 16
      if (traits::kBase == 16 && end - begin > 2 && *begin == '0' &&
          (*(begin + 1) == 'x' || *(begin + 1) == 'X')) {
        begin += 2;
      }

      for (const_iterator current = begin; current != end; ++current) {
        uint8_t new_digit = 0;

        if (!CharToDigit<traits::kBase>(*current, &new_digit)) {
          return false;
        }

        if (current != begin) {
          if (!Sign::CheckBounds(output, new_digit)) {
            return false;
          }
          *output *= traits::kBase;
        }

        Sign::Increment(new_digit, output);
      }
      return true;
    }
  };

  class Positive : public Base<Positive> {
   public:
    static bool CheckBounds(value_type* output, uint8_t new_digit) {
      if (*output > static_cast<value_type>(traits::max() / traits::kBase) ||
          (*output == static_cast<value_type>(traits::max() / traits::kBase) &&
           new_digit > traits::max() % traits::kBase)) {
        *output = traits::max();
        return false;
      }
      return true;
    }
    static void Increment(uint8_t increment, value_type* output) {
      *output += increment;
    }
  };

  class Negative : public Base<Negative> {
   public:
    static bool CheckBounds(value_type* output, uint8_t new_digit) {
      if (*output < traits::min() / traits::kBase ||
          (*output == traits::min() / traits::kBase &&
           new_digit > 0 - traits::min() % traits::kBase)) {
        *output = traits::min();
        return false;
      }
      return true;
    }
    static void Increment(uint8_t increment, value_type* output) {
      *output -= increment;
    }
  };
};

template<typename ITERATOR, typename VALUE, int BASE>
class BaseIteratorRangeToNumberTraits {
 public:
  typedef ITERATOR iterator_type;
  typedef VALUE value_type;
  static value_type min() {
    return std::numeric_limits<value_type>::min();
  }
  static value_type max() {
    return std::numeric_limits<value_type>::max();
  }
  static const int kBase = BASE;
};

template<typename ITERATOR>
class BaseHexIteratorRangeToIntTraits
    : public BaseIteratorRangeToNumberTraits<ITERATOR, int, 16> {
 public:
  // Allow parsing of 0xFFFFFFFF, which is technically an overflow
  static unsigned int max() {
    return std::numeric_limits<unsigned int>::max();
  }
};

typedef BaseHexIteratorRangeToIntTraits<std::string::const_iterator>
    HexIteratorRangeToIntTraits;

template <typename VALUE, int BASE>
class StringPieceToNumberTraits
    : public BaseIteratorRangeToNumberTraits<std::string::const_iterator,
                                             VALUE,
                                             BASE> {};

template <typename VALUE>
bool StringToIntImpl(const std::string& input, VALUE* output) {
  return IteratorRangeToNumber<StringPieceToNumberTraits<VALUE, 10> >::Invoke(
      input.begin(), input.end(), output);
}

template <typename VALUE, int BASE>
class StringPiece16ToNumberTraits
    : public BaseIteratorRangeToNumberTraits<std::wstring::const_iterator,
                                             VALUE,
                                             BASE> {};

template <typename VALUE>
bool String16ToIntImpl(const std::wstring& input, VALUE* output) {
  return IteratorRangeToNumber<StringPiece16ToNumberTraits<VALUE, 10> >::Invoke(
      input.begin(), input.end(), output);
}

}  // namespace

std::string IntToString(int value) {
  return IntToStringT<std::string, int, unsigned int, true>::
      IntToString(value);
}

std::wstring IntToString16(int value) {
  return IntToStringT<std::wstring, int, unsigned int, true>::
      IntToString(value);
}

std::string UintToString(unsigned int value) {
  return IntToStringT<std::string, unsigned int, unsigned int, false>::
      IntToString(value);
}

std::wstring UintToString16(unsigned int value) {
  return IntToStringT<std::wstring, unsigned int, unsigned int, false>::
      IntToString(value);
}

std::string Int64ToString(int64_t value) {
  return IntToStringT<std::string, int64_t, uint64_t, true>::
      IntToString(value);
}

std::wstring Int64ToString16(int64_t value) {
  return IntToStringT<std::wstring, int64_t, uint64_t, true>::IntToString(value);
}

std::string Uint64ToString(uint64_t value) {
  return IntToStringT<std::string, uint64_t, uint64_t, false>::
      IntToString(value);
}

std::wstring Uint64ToString16(uint64_t value) {
  return IntToStringT<std::wstring, uint64_t, uint64_t, false>::
      IntToString(value);
}

std::string DoubleToString(double value) {
  char buffer[64];
#ifdef COMPILER_MSVC
  _snprintf(buffer, sizeof(buffer), "%lf", value);
#else
  snprintf(buffer, sizeof(buffer), "%lf", value);
#endif
  return std::string(buffer);
}

bool StringToInt(const std::string& input, int* output) {
  return StringToIntImpl(input, output);
}

bool StringToInt(const std::wstring& input, int* output) {
  return String16ToIntImpl(input, output);
}

bool StringToUint(const std::string& input, unsigned* output) {
  return StringToIntImpl(input, output);
}

bool StringToUint(const std::wstring& input, unsigned* output) {
  return String16ToIntImpl(input, output);
}

bool StringToInt64(const std::string& input, int64_t* output) {
  return StringToIntImpl(input, output);
}

bool StringToInt64(const std::wstring& input, int64_t* output) {
  return String16ToIntImpl(input, output);
}

bool StringToUint64(const std::string& input, uint64_t* output) {
  return StringToIntImpl(input, output);
}

bool StringToUint64(const std::wstring& input, uint64_t* output) {
  return String16ToIntImpl(input, output);
}

bool StringToSizeT(const std::string& input, size_t* output) {
  return StringToIntImpl(input, output);
}

bool StringToSizeT(const std::wstring& input, size_t* output) {
  return String16ToIntImpl(input, output);
}

bool StringToDouble(const std::string& input, double* output) {
	if (input.empty() || !output)
		return false;
	if (sscanf(input.c_str(), "%lf", output) == 1)
		return true;
	return false;
}

bool HexStringToInt(const std::string& input, int* output) {
	return IteratorRangeToNumber<HexIteratorRangeToIntTraits>::Invoke(
		input.begin(), input.end(), output);
}

}  // namespace nbase
