#ifndef LIBXL_IFONTT_H
#define LIBXL_IFONTT_H

#include "setup.h"
#include "enum.h"

namespace libxl {

    template<class TCHAR>
    struct IFontT
    {
        virtual          int XLAPIENTRY size() const = 0;
        virtual         void XLAPIENTRY setSize(int size) = 0;

        virtual         bool XLAPIENTRY italic() const = 0;
        virtual         void XLAPIENTRY setItalic(bool italic = true) = 0;

        virtual         bool XLAPIENTRY strikeOut() const = 0;
        virtual         void XLAPIENTRY setStrikeOut(bool strikeOut = true) = 0;

        virtual        Color XLAPIENTRY color() const = 0;
        virtual         void XLAPIENTRY setColor(Color color) = 0;

        virtual         bool XLAPIENTRY bold() const = 0;
        virtual         void XLAPIENTRY setBold(bool bold = true) = 0;

        virtual       Script XLAPIENTRY script() const = 0;
        virtual         void XLAPIENTRY setScript(Script script) = 0;

        virtual    Underline XLAPIENTRY underline() const = 0;
        virtual         void XLAPIENTRY setUnderline(Underline underline) = 0;

        virtual const TCHAR* XLAPIENTRY name() const = 0;
        virtual         bool XLAPIENTRY setName(const TCHAR* name) = 0;

        virtual                         ~IFontT() {}
    };

}

#endif
