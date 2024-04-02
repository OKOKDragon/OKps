#pragma once

#include <string>
#include <vector>
#include ".\base.hpp"
#include <locale>
#include <codecvt>
#include <map>

/*
可以用 std::locale::global(std::locale("zh_CN.UTF-8")); 将本地码设置为utf8编码的简体中文
*/

namespace OKps
{
    /*
    由于ImGui使用utf8编码，但windows系统不使用utf8，所以一旦涉及文件操作，就要转换编码
    本类用于管理字符串编码
    完全用标准库实现，所以这份代码是缺省跨平台的，不需要定义任何宏或编译器选项
    */
    class string final : public base
    {
    private:
        /*
        本地码
        */
        std::string MEMBER_locale;

    public:
        /*
        设置本地码
        如果输入空字符串，则由c++运行时库根据操作系统设置自动选择本地码
        一般来说，这样就够用了
        如果不了解本地码，不要乱输入，此函数不负责检查输入，也不负责处理由 不恰当的输入 引发的c++标准库异常
        */
        // void set_locale(const std::string &_locale);
        //  字符串编码
        // enum class state
        //{
        /*
        ANSI编码
        <windows.h>中，对应宏CP_ACP
        ANSI也叫本地码，其编码方式和系统语言设置有关
        对于简体中文windows系统，它是GB2312编码
        对于繁体中文windows系统，它是Big5编码
        windows系统函数使用的是utf-16编码，对应 wchar_t 类型字符串和 std::wstring 容器
        但UTF8编码才是支持最广泛、生态最完善的编码，linux系统使用UTF8编码，全网网站大部分使用UTF8编码，大量开源库使用utf8编码
        所以必然涉及到几种编码的转换
        */
        // ANSI,
        /*
        utf-8编码
        ImGui使用utf-8编码
        linux系统基本全都使用utf-8编码
        utf-8编码也是vscode等各类文本编辑器默认的编码
        可以认为utf-8是使用最广泛的编码
        在各种情况下，都应该尽量使用utf-8
        */
        // UTF8,
        /*
        unicode编码
        只有它用wchar_t类型字符串存储，通过字符串类型就可以确定
        */
        // Unicode
        //};

        string(const std::string & ARG_locale = "");

        // 复制构造
        string(const string & ARG_origin);

        // 移动构造
        string(string &&) = delete;

        ~string();

        static std::string Unicode_to_UTF8(const std::wstring & origin);
        static std::wstring UTF8_to_Unicode(const std::string & origin);

        std::string Unicode_to_ANSI(const std::wstring & origin) const;
        std::wstring ANSI_to_Unicode(const std::string & origin) const;

        static std::string Unicode_to_ANSI(const std::wstring & origin, const std::string & locale);
        static std::wstring ANSI_to_Unicode(const std::string & origin, const std::string & locale);
        static std::string UTF8_to_ANSI(const std::string & origin, const std::string & locale);
        static std::string ANSI_to_UTF8(const std::string & origin, const std::string & locale);
        std::string UTF8_to_ANSI(const std::string & origin)const;
        std::string ANSI_to_UTF8(const std::string & origin)const;

        /*输入字符，判断是否为10进制数字*/
        static bool is_number(const char input) noexcept;
        /*输入10进制单个数字，转换为字符*/
        static char to_char(const char input);
        /*输入字符，转换为10进制单个数字*/
        static char to_number(const char input);
        static char number16_to_char(const char input);
        /*
        输入1个字符，转换为16进制数字
        不分大小写
        */
        static char char16_to_number(const char input);
        /*
        输入无符号长整数，将它的内存转为16进制数，再转为字符串
        例如 size_t x = 0，则x的内存为 00 00 00 00 00 00 00 00
        */
        static std::string to_string16(const size_t input);
        /*
        将上述格式的字符串转为size_t类型
        */
        static size_t string16_to_ULL(const std::string & input);
    };
    /*
    命令语句分离器
    输入任意字符串，将它转换为命令语句
    也就是，头部和尾部没有空格，中间也不会出现连续多个空格
    也就是说，任何字符串都会被转换成 "命令 参数1 参数2 参数3 ···"的形式
    这里所说的空格，只是普通半角空格，不支持空字符、制表符、全角空格或其他特殊字符
    */
    class command_statement final : public base
    {
    private:
        /*
        命令
        */
        std::string MEMBER_command;
        /*
        参数表
        */
        std::vector<std::string> MEMBER_argument_list;

    public:
        /*
        命令
        */
        const std::string & command() const noexcept;
        /*
        参数表
        */
        const std::vector<std::string> & argument_list() const noexcept;
        /*
        命令语句字符串
        */
        std::string command_string() const;
        /*
        构造函数
        充当分离器
        */
        command_statement(const std::string & origin_statement);
    };
}
