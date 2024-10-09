#pragma once

#include <vector>
#include <string>

#include ".\number.hpp"

/*
可以用 std::locale::global(std::locale("zh_CN.UTF-8")); 将本地环境设置为utf8编码、简体中文
*/

namespace OKps
{
    /*
    c++20标准要求，char8_t与unsigned char具有相同的符号性、大小和对齐，
    又要求一个值从char转换到unsigned char再转换回char，值不变，
    故std::string和std::u8string之间的相互转换必然成立，且值不变。
    */

    /*
    标准库定义的字符串类型
    */
    template<typename string_type>
    concept stl_string_type = std::is_same_v<string_type, std::string>
        or std::is_same_v<string_type, std::wstring>
        or std::is_same_v<string_type, std::u8string>
        or std::is_same_v<string_type, std::u16string>
        or std::is_same_v<string_type, std::u32string>;

    template<stl_string_type origin_type, stl_string_type target_type>
    typename std::enable_if_t<sizeof(typename origin_type::value_type) == sizeof(typename target_type::value_type), target_type> value_cast(origin_type const & input);

    template std::wstring value_cast<std::u16string, std::wstring>(std::u16string const &);
    template std::u16string value_cast<std::wstring, std::u16string>(std::wstring const &);
    template std::wstring value_cast<std::u32string, std::wstring>(std::u32string const &);
    template std::u32string value_cast<std::wstring, std::u32string>(std::wstring const &);
    template std::wstring value_cast<std::u8string, std::wstring>(std::u8string const &);
    template std::u8string value_cast<std::wstring, std::u8string>(std::wstring const &);
    template std::wstring value_cast<std::string, std::wstring>(std::string const &);
    template std::string value_cast<std::wstring, std::string>(std::wstring const &);
    template std::u8string value_cast<std::string, std::u8string>(std::string const &);
    template std::string value_cast<std::u8string, std::string>(std::u8string const &);

        /*
        此函数仅用于向系统终端输出程序接受的命令行参数
        在main函数开头将命令行参数转发给此函数

        因为不同系统、不同终端模拟器可能对命令的格式规定和处理不同，用户实际输入给程序的命令行参数可能并不是他们所想的
        */
    void echo_command_line(int const argc, char const * const argv[]);

    void wait_input(std::string const & hint = "输入回车以继续运行程序。在输入回车以前，输入任何字符都会被程序忽略。\n输入回车 ││ ", char const input = '\n');
    /*
    此函数将单个数字，如 0b1、0xa，转换成字符，如'1'、'a'
    如果不能转换，则抛出异常
    */
    char to_char(unsigned int const number, number_system const number_base = number_system::dec);
    /*
    此函数将表示数字的字符，如 '1'、'a'，转换成整数类型，不区分大小写字母
    如果不能转换，则抛出异常
    c++标准规定，整数类型的内建算术运算符的操作数至少是int类型，char、short会被隐式转换到int再参与整数运算，故此函数返回int类型
    */
    unsigned int from_char(char const number, number_system const number_base = number_system::dec);
    /*
    命令语句分离器
    输入任意字符串，将它转换为命令语句
    也就是，头部和尾部没有空格，中间也不会出现连续多个空格
    也就是说，任何字符串都会被转换成 "命令 参数1 参数2 参数3 ···"的形式
    这里所说的空格，只是普通半角空格，不支持空字符、制表符、全角空格或其他特殊字符
    */
    class command_statement final
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
        命令，指输入的命令语句的第一个元素
        */
        const std::string & command() const noexcept;

        /*
        参数表
        */
        const std::vector<std::string> & argument_list() const noexcept;

        /*
        完整的命令语句字符串
        */
        std::string command_string() const;

        command_statement(const std::string & origin_statement);

        ~command_statement()
            noexcept(std::is_nothrow_destructible_v<std::string>
            and std::is_nothrow_destructible_v<std::vector<std::string>>);

        command_statement(command_statement const & origin)
            noexcept(std::is_nothrow_copy_constructible_v<std::string>
            and std::is_nothrow_copy_constructible_v<std::vector<std::string>>);
        command_statement(command_statement && origin)
            noexcept(std::is_nothrow_move_constructible_v<std::string>
            and std::is_nothrow_move_constructible_v<std::vector<std::string>>);
        void operator =(command_statement const & origin)
            noexcept(std::is_nothrow_copy_assignable_v<std::string>
            and std::is_nothrow_copy_assignable_v<std::vector<std::string>>);
        void operator =(command_statement && origin)
            noexcept(std::is_nothrow_move_assignable_v<std::string>
            and std::is_nothrow_move_assignable_v<std::vector<std::string>>);
    };
}
