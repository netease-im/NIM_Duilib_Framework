// name/value map
// !note: the map save the point of value, so the value's life circle must more than the valuemap

#ifndef BASE_UTIL_VALUEMAP_H_
#define BASE_UTIL_VALUEMAP_H_

#include "base/base_export.h"
#include "base/macros.h"
#include <map>
#include <vector>

namespace nbase
{
class BASE_EXPORT ValueMap
{
public:
    // 参数的访问限制
    enum
	{
        access_read  = 0x01,
        access_write = 0x02
    };

private:
    struct VToString
	{
        virtual ~VToString(){}
        virtual std::string to_string(void *v) const = 0;
        virtual bool        to_value(const char *s, void *v) = 0;
    };

    template<typename T>
    struct NumberToString : public VToString
	{
        const char *format_;

        NumberToString(const char *format) : format_(format){}
        virtual std::string to_string(void *v) const
		{
             assert(v);
             char buf[128];
             sprintf(buf, format_, *((const T *)v));
             return std::string((const char *)buf);
        }
        virtual bool to_value(const char *s, void *v)
		{
            return sscanf(s, format_, v) == 1;
        }
    };

    struct StringToString : public VToString
	{
        virtual std::string to_string(void *v) const 
		{
		    assert(v);
		    return *((const std::string *)v);
	    }
	    virtual bool to_value(const char *s, void *v)
		{
		    *((std::string *)v) = s;
		    return true;
	    }
    };

public:
    enum Type
	{
        t_char = 0 ,
        t_float,
        t_double,
        t_string,
        t_pointer,
        t_i8,
        t_u8,
        t_i16,
        t_u16,
        t_i32,
        t_u32,
        t_i64,
        t_u64,
        t_o8,
        t_o16,
        t_o32,
        t_o64,
        t_h8,
        t_h16,
        t_h32,
        t_h64,
        types_num
    };

private:
    struct Item
	{
        uint8_t access_;    // access_read | access_write
        Type    type_;
        void   *ptr_;
        Item() : access_(access_read), type_(t_pointer), ptr_(0){}
        Item(Type t, void *v, uint8_t a) : access_(a), type_(t), ptr_(v) { assert(ptr_); }
        Item(const Item & src)
		{
			access_ = src.access_;
			type_   = src.type_;
			ptr_    = src.ptr_;
		}
        Item& operator = (const Item &src)
		{
			access_ = src.access_;
			type_   = src.type_;
			ptr_    = src.ptr_;
			return *this;
		}
    };

    typedef std::map<std::string, Item> Map;
    VToString *formats_[types_num];
    Map        items_;

public:
    ValueMap()
	{
        // initialize format map
        formats_[t_char] = new NumberToString<char>(CHAR_FORMAT);
        formats_[t_float] = new NumberToString<float>(FLOAT_FORMAT);
        formats_[t_double] = new NumberToString<double>(DOUBLE_FORMAT);
        formats_[t_string] = new StringToString;
        formats_[t_pointer] = new NumberToString<void *>(POINTER_FORMAT);
        formats_[t_i8] = new NumberToString<int8_t>(INT8_FORMAT);
        formats_[t_u8] = new NumberToString<uint8_t>(UINT8_FORMAT);
        formats_[t_i16] = new NumberToString<int16_t>(INT16_FORMAT);
        formats_[t_u16] = new NumberToString<uint16_t>(UINT16_FORMAT);
        formats_[t_i32] = new NumberToString<int32_t>(INT32_FORMAT);
        formats_[t_u32] = new NumberToString<uint32_t>(UINT32_FORMAT);
        formats_[t_i64] = new NumberToString<int64_t>(INT64_FORMAT);
        formats_[t_u64] = new NumberToString<uint64_t>(UINT64_FORMAT);
        formats_[t_o8] = new NumberToString<uint8_t>(OCT8_FORMAT);
        formats_[t_o16] = new NumberToString<uint16_t>(OCT16_FORMAT);
        formats_[t_o32] = new NumberToString<uint32_t>(OCT32_FORMAT);
        formats_[t_o64] = new NumberToString<uint64_t>(OCT64_FORMAT);
        formats_[t_h8] = new NumberToString<uint8_t>(HEX8_FORMAT);
        formats_[t_h16] = new NumberToString<uint16_t>(HEX16_FORMAT);
        formats_[t_h32] = new NumberToString<uint32_t>(HEX32_FORMAT);
        formats_[t_h64] = new NumberToString<uint64_t>(HEX64_FORMAT);
    }
    ~ValueMap()
	{      
        items_.clear();
        for(size_t i = 0; i < types_num; ++i)     
            delete formats_[i];
    }
    void clear() { items_.clear(); }
    void bind(const std::string &name,
		      void *ptr,
			  Type type,
			  uint8_t access = access_read)
	{
        assert(ptr);
        assert(type >= t_char && type < types_num);
        items_[name] = Item(type, ptr, access);
    }
    void bind(const char *name,
		      void *ptr,
			  Type type,
			  uint8_t access = access_read)
	{
        bind(std::string(name), ptr, type, access);
    }
    void unbind(const std::string &name)
	{
        Map::iterator it=items_.find(name);
        if (it != items_.end())
			items_.erase(it);
    }
    void unbind(const char *name)
	{
        unbind(std::string(name));
    }
    std::string get(const std::string &name) const
	{
        Map::const_iterator it=items_.find(name);
        if (it==items_.end())
			return std::string("");
        const Item &i = it->second;
        if (i.access_ && access_read)
			return formats_[i.type_]->to_string(i.ptr_);
        else
			return std::string("");
    }
    std::string get(const char *name) const
	{
        return get(std::string(name));
    }
    bool set(const std::string &name, const char *value)
	{
        assert(value);
        Map::iterator it = items_.find(name);
        if (it==items_.end())
			return false;
        Item &i = it->second;
        // forbid set pointer or string
        if (i.type_ == t_string || i.type_ == t_pointer)
			return false;
        if (!(i.access_ & access_write))
			return false;
        return formats_[i.type_]->to_value(value,i.ptr_);
    }
    bool set(const char *name, const char *value)
	{
        return set(std::string(name), value) ;
    }
    void enumerate(std::vector<std::string> &names) const
	{
        Map::const_iterator it=items_.begin();
        for (; it != items_.end(); ++it)
			names.push_back(it->first) ;
    }
};

}  // namespace nbase

#endif  // BASE_UTIL_VALUEMAP_H_