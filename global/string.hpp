#pragma once

#include <locale>
#include <filesystem>
#include <iostream>

#include ".\number.hpp"
#include ".\value_cast.hpp"

/*
可以用 std::locale::global(std::locale("zh_CN.UTF-8")); 将本地环境设置为utf8编码、简体中文
*/

namespace OKps
{
    char operator ""_utf_8(char8_t const content)noexcept;
    std::string operator ""_utf_8(char8_t const * const content, std::size_t const size)
        noexcept(std::is_nothrow_default_constructible_v<std::string>
        and noexcept(std::declval<std::string>().resize(std::declval<std::size_t const>())));

    class text_encoding final
    {
    public:
        /*
        utf-8字符编码的本地环境
        */
        static std::locale const utf_8;

    private:
        static std::locale INNER_init()
            noexcept(noexcept(std::locale(""))
        and noexcept(std::locale::global(std::declval<std::locale &>())));
    public:
        text_encoding() = delete;
        ~text_encoding() = delete;
        text_encoding(text_encoding const &) = delete;
        text_encoding(text_encoding &&) = delete;
        void operator =(text_encoding const &) = delete;
        void operator =(text_encoding &&) = delete;
    };

    /*
    此函数仅用于向系统终端输出程序接受的命令行参数，
    在main函数开头将命令行参数转发给此函数。

    因为不同系统、不同终端模拟器可能对命令的格式规定和处理不同，用户实际输入给程序的命令行参数可能并不是他们所想的。
    */
    void echo_command_line(int const argc, char const * const * const argv);
    /*
    等待用户从标准输入std::cin输入字符 signal ，在此之前不做任何事。
    hint是等待用户输入之前，向标准输出std::cout输出的提示信息，其编码必须符合 page 参数所指定的本地环境的编码，否则可能无法正常显示。
    */
    void wait_input(std::string const & hint/*提示信息*/ = "输入回车以继续运行程序。在输入回车以前，输入任何字符都会被程序忽略。\n输入回车 ││ ", char const signal/*要等待的字符*/ = '\n');
    /*
    此函数将单个数字，如 0b1、0xa，转换成字符，如'1'、'a'
    如果不能转换，则抛出异常。
    */
    char to_char(unsigned int const number, number_system const number_base = number_system::dec);
    /*
    此函数将表示数字的字符，如 '1'、'a'，转换成整数类型，不区分大小写字母
    如果不能转换，则抛出异常。
    c++标准规定，整数类型的内建算术运算符的操作数至少是int类型，char、short会被隐式转换到int再参与整数运算，故此函数返回int类型。
    */
    unsigned int from_char(char const number, number_system const number_base = number_system::dec);

    /*
    命令语句分离器
    输入任意字符串，将它转换为命令语句，
    也就是，头部和尾部没有空格，中间也不会出现连续多个空格。
    也就是说，任何字符串都会被转换成 "命令 参数1 参数2 参数3 ···"的形式。
    这里所说的空格，仅包括普通半角空格，即字符字面量 ' '，不支持全角空格或其他特殊字符。
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
