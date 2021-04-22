#ifndef LIBXL_CPP_H
#define LIBXL_CPP_H

#define LIBXL_VERSION 0x03060500

#include "IBookT.h"
#include "ISheetT.h"
#include "IFormatT.h"
#include "IFontT.h"

namespace libxl {

    #ifdef _UNICODE
        typedef IBookT<wchar_t> Book;
        typedef ISheetT<wchar_t> Sheet;
        typedef IFormatT<wchar_t> Format;
        typedef IFontT<wchar_t> Font;
        #define xlCreateBook xlCreateBookW
        #define xlCreateXMLBook xlCreateXMLBookW
    #else
        typedef IBookT<char> Book;
        typedef ISheetT<char> Sheet;
        typedef IFormatT<char> Format;
        typedef IFontT<char> Font;
        #define xlCreateBook xlCreateBookA
        #define xlCreateXMLBook xlCreateXMLBookA
    #endif

}

#endif
