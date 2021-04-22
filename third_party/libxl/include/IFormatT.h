#ifndef LIBXL_IFORMATT_H
#define LIBXL_IFORMATT_H

#include "setup.h"
#include "enum.h"

namespace libxl {

    template<class TCHAR> struct IFontT;

    template<class TCHAR>
    struct IFormatT
    {
        virtual IFontT<TCHAR>* XLAPIENTRY font() const = 0;
        virtual           bool XLAPIENTRY setFont(IFontT<TCHAR>* font) = 0;

        virtual            int XLAPIENTRY numFormat() const = 0;
        virtual           void XLAPIENTRY setNumFormat(int numFormat) = 0;

        virtual         AlignH XLAPIENTRY alignH() const = 0;
        virtual           void XLAPIENTRY setAlignH(AlignH align) = 0;

        virtual         AlignV XLAPIENTRY alignV() const = 0;
        virtual           void XLAPIENTRY setAlignV(AlignV align) = 0;

        virtual           bool XLAPIENTRY wrap() const = 0;
        virtual           void XLAPIENTRY setWrap(bool wrap = true) = 0;

        virtual            int XLAPIENTRY rotation() const = 0;
        virtual           bool XLAPIENTRY setRotation(int rotation) = 0;

        virtual            int XLAPIENTRY indent() const = 0;
        virtual           void XLAPIENTRY setIndent(int indent) = 0;

        virtual           bool XLAPIENTRY shrinkToFit() const = 0;
        virtual           void XLAPIENTRY setShrinkToFit(bool shrinkToFit = true) = 0;

        virtual           void XLAPIENTRY setBorder(BorderStyle style = BORDERSTYLE_THIN) = 0;
        virtual           void XLAPIENTRY setBorderColor(Color color) = 0;

        virtual    BorderStyle XLAPIENTRY borderLeft() const = 0;
        virtual           void XLAPIENTRY setBorderLeft(BorderStyle style = BORDERSTYLE_THIN) = 0;

        virtual    BorderStyle XLAPIENTRY borderRight() const = 0;
        virtual           void XLAPIENTRY setBorderRight(BorderStyle style = BORDERSTYLE_THIN) = 0;

        virtual    BorderStyle XLAPIENTRY borderTop() const = 0;
        virtual           void XLAPIENTRY setBorderTop(BorderStyle style = BORDERSTYLE_THIN) = 0;

        virtual    BorderStyle XLAPIENTRY borderBottom() const = 0;
        virtual           void XLAPIENTRY setBorderBottom(BorderStyle style = BORDERSTYLE_THIN) = 0;

        virtual          Color XLAPIENTRY borderLeftColor() const = 0;
        virtual           void XLAPIENTRY setBorderLeftColor(Color color) = 0;

        virtual          Color XLAPIENTRY borderRightColor() const = 0;
        virtual           void XLAPIENTRY setBorderRightColor(Color color) = 0;

        virtual          Color XLAPIENTRY borderTopColor() const = 0;
        virtual           void XLAPIENTRY setBorderTopColor(Color color) = 0;

        virtual          Color XLAPIENTRY borderBottomColor() const = 0;
        virtual           void XLAPIENTRY setBorderBottomColor(Color color) = 0;

        virtual BorderDiagonal XLAPIENTRY borderDiagonal() const = 0;
        virtual           void XLAPIENTRY setBorderDiagonal(BorderDiagonal border) = 0;

        virtual    BorderStyle XLAPIENTRY borderDiagonalStyle() const = 0;
        virtual           void XLAPIENTRY setBorderDiagonalStyle(BorderStyle style) = 0;

        virtual          Color XLAPIENTRY borderDiagonalColor() const = 0;
        virtual           void XLAPIENTRY setBorderDiagonalColor(Color color) = 0;

        virtual    FillPattern XLAPIENTRY fillPattern() const = 0;
        virtual           void XLAPIENTRY setFillPattern(FillPattern pattern) = 0;

        virtual          Color XLAPIENTRY patternForegroundColor() const = 0;
        virtual           void XLAPIENTRY setPatternForegroundColor(Color color) = 0;

        virtual          Color XLAPIENTRY patternBackgroundColor() const = 0;
        virtual           void XLAPIENTRY setPatternBackgroundColor(Color color) = 0;

        virtual           bool XLAPIENTRY locked() const = 0;
        virtual           void XLAPIENTRY setLocked(bool locked = true) = 0;

        virtual           bool XLAPIENTRY hidden() const = 0;
        virtual           void XLAPIENTRY setHidden(bool hidden = true) = 0;

        virtual                           ~IFormatT() {}
    };

}

#endif
