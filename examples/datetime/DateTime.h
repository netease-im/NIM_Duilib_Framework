#pragma once

namespace ui
{
    class DateTimeWnd;

    class DateTime : public ui::Label
    {

    public:
        friend class DateTimeWnd;
        DateTime();

        const SYSTEMTIME& GetTime() const;
        void SetTime(const SYSTEMTIME& val);

        void UpdateText();

        const std::wstring& GetFormat() const;
        void SetFormat(std::wstring val);

        const std::wstring& GetDateTimeFormat() const;
        void SetDateTimeFormat(std::wstring val);


        virtual void HandleMessage(EventArgs& msg) override;


        virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

    protected:
        SYSTEMTIME m_sysTime;
        int m_nDTUpdateFlag;

        /*
        %a 星期几的简写
        %A 星期几的全称
        %b 月分的简写
        %B 月份的全称
        %c 标准的日期的时间串
        %C 年份的后两位数字
        %d 十进制表示的每月的第几天
        %D 月/天/年
        %e 在两字符域中，十进制表示的每月的第几天
        %F 年-月-日
        %g 年份的后两位数字，使用基于周的年
        %G 年分，使用基于周的年
        %h 简写的月份名
        %H 24小时制的小时
        %I 12小时制的小时
        %j 十进制表示的每年的第几天
        %m 十进制表示的月份
        %M 十时制表示的分钟数
        %n 新行符
        %p 本地的AM或PM的等价显示
        %r 12小时的时间
        %R 显示小时和分钟：hh:mm
        %S 十进制的秒数
        %t 水平制表符
        %T 显示时分秒：hh:mm:ss
        %u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
        %U 第年的第几周，把星期日做为第一天（值从0到53）
        %V 每年的第几周，使用基于周的年
        %w 十进制表示的星期几（值从0到6，星期天为0）
        %W 每年的第几周，把星期一做为第一天（值从0到53）
        %x 标准的日期串
        %X 标准的时间串
        %y 不带世纪的十进制年份（值从0到99）
        %Y 带世纪部分的十进制年份
        %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
        %% 百分号
        */
        std::wstring m_sFormat;

        /*
        “ dd”两位数的日期。一位数字的日期值前面有一个零。
        “ ddd”是三个字符的工作日缩写。
        “ dddd”完整的工作日名称。
        “ h” 12小时格式的一位或两位数小时。
        “ hh” 12小时格式的两位数小时。一位数值前加零。
        “ H” 24小时格式的一位或两位数小时。
        “ HH” 24小时格式的两位数小时。一位数值前加零。
        “ m”一位或两位数字分钟。
        “ mm”两位数分钟。一位数值前加零。
        “ M”一位或两位数的月份号。
        “ MM”两位数的月份号。一位数值前加零。
        “ MMM”是三个字符的月份缩写。
        “ MMMM”完整的月份名称。
        “ t” AM / PM一个字母的缩写（即AM显示为“ A”）。
        “ tt”两个字母的AM / PM缩写（即AM显示为“ AM”）。
        “ yy”年份的最后两位数字（即1996将显示为“ 96”）。
        “ yyyy”全年（即1996年将显示为“ 1996”）。
        */
        std::wstring m_sDateTimeFormat;

        DateTimeWnd* m_pDateTimeWnd;
    };
}

