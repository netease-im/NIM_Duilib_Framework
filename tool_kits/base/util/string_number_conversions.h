// String number conversion

#ifndef BASE_UTIL_STRING_NUMBER_CONVERSIONS_H_
#define BASE_UTIL_STRING_NUMBER_CONVERSIONS_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/base_types.h"

// ----------------------------------------------------------------------------
// IMPORTANT MESSAGE FROM YOUR SPONSOR
//
// This file contains no "wstring" variants. New code should use std::wstring. If
// you need to make old code work, use the UTF8 version and convert. Please do
// not add wstring variants.
//
// Please do not add "convenience" functions for converting strings to integers
// that return the value and ignore success/failure. That encourages people to
// write code that doesn't properly handle the error conditions.
// ----------------------------------------------------------------------------

namespace nbase {

// Number -> string conversions ------------------------------------------------

BASE_EXPORT std::string IntToString(int value);
BASE_EXPORT std::wstring IntToString16(int value);

BASE_EXPORT std::string UintToString(unsigned value);
BASE_EXPORT std::wstring UintToString16(unsigned value);

BASE_EXPORT std::string Int64ToString(int64_t value);
BASE_EXPORT std::wstring Int64ToString16(int64_t value);

BASE_EXPORT std::string Uint64ToString(uint64_t value);
BASE_EXPORT std::wstring Uint64ToString16(uint64_t value);

// DoubleToString converts the double to a string format that ignores the
// locale. If you want to use locale specific formatting, use ICU.
BASE_EXPORT std::string DoubleToString(double value);

// String -> number conversions ------------------------------------------------

// Perform a best-effort conversion of the input string to a numeric type,
// setting |*output| to the result of the conversion.  Returns true for
// "perfect" conversions; returns false in the following cases:
//  - Overflow/underflow.  |*output| will be set to the maximum value supported
//    by the data type.
//  - Trailing characters in the string after parsing the number.  |*output|
//    will be set to the value of the number that was parsed.
//  - Leading whitespace in the string before parsing the number. |*output| will
//    be set to the value of the number that was parsed.
//  - No characters parseable as a number at the beginning of the string.
//    |*output| will be set to 0.
//  - Empty string.  |*output| will be set to 0.
BASE_EXPORT bool StringToInt(const std::string& input, int* output);
BASE_EXPORT bool StringToInt(const std::wstring& input, int* output);

BASE_EXPORT bool StringToUint(const std::string& input, unsigned* output);
BASE_EXPORT bool StringToUint(const std::wstring& input, unsigned* output);

BASE_EXPORT bool StringToInt64(const std::string& input, int64_t* output);
BASE_EXPORT bool StringToInt64(const std::wstring& input, int64_t* output);

BASE_EXPORT bool StringToUint64(const std::string& input, uint64_t* output);
BASE_EXPORT bool StringToUint64(const std::wstring& input, uint64_t* output);

BASE_EXPORT bool StringToSizeT(const std::string& input, size_t* output);
BASE_EXPORT bool StringToSizeT(const std::wstring& input, size_t* output);

// For floating-point conversions, only conversions of input strings in decimal
// form are defined to work.  Behavior with strings representing floating-point
// numbers in hexadecimal, and strings representing non-fininte values (such as
// NaN and inf) is undefined.  Otherwise, these behave the same as the integral
// variants.  This expects the input string to NOT be specific to the locale.
// If your input is locale specific, use ICU to read the number.
BASE_EXPORT bool StringToDouble(const std::string& input, double* output);

// Hex encoding ----------------------------------------------------------------

// Best effort conversion, see StringToInt above for restrictions.
BASE_EXPORT bool HexStringToInt(const std::string& input, int* output);

}  // namespace nbase

#endif  // BASE_UTIL_STRING_NUMBER_CONVERSIONS_H_

