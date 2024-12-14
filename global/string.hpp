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
    namespace text_encoding
    {
        /*
        utf-8字符编码的本地环境
        */
        extern std::locale const utf_8;
    }

    /*
    此函数仅用于向系统终端输出程序接受的命令行参数，
    在main函数开头将命令行参数转发给此函数。

    因为不同系统、不同终端模拟器可能对命令的格式规定和处理不同，用户实际输入给程序的命令行参数可能并不是他们所想的。
    */
    void echo_command_line(int const argc, char const * const argv[]);
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



    class string;

    template<>
    constexpr bool const safe_convertible<std::string, string> = noexcept(std::locale::global(std::declval<std::locale>()))
        and noexcept(value_cast<std::string>(std::declval<std::filesystem::path>().u8string()))
        and noexcept(std::filesystem::path(std::declval<std::string const>()));
    template<>
    constexpr bool const safe_convertible<std::u8string, string> = noexcept(std::locale::global(std::declval<std::locale>()))
        and noexcept(std::declval<std::filesystem::path>().u8string())
        and noexcept(std::filesystem::path(std::declval<std::string const>()));

    /*
    带编码信息的字符串
    */
    class string final
    {
    private:
        std::string MEMBER_content;
        std::locale MEMBER_page;
    public:
        /*
        默认编码为 utf-8
        */
        string(std::string const & content = "", std::locale const & page = text_encoding::utf_8)
            noexcept(std::is_nothrow_copy_constructible_v<std::string>
            and std::is_nothrow_copy_constructible_v<std::locale>);
        string(string const &)
            noexcept(std::is_nothrow_copy_constructible_v<std::string>
and std::is_nothrow_copy_constructible_v<std::locale>);
        void operator =(string const &)
            noexcept(std::is_nothrow_copy_assignable_v<std::string>
    and std::is_nothrow_copy_assignable_v<std::locale>);
        string(string &&)
            noexcept(std::is_nothrow_move_constructible_v<std::string>
            and std::is_nothrow_copy_constructible_v<std::locale>
            and std::is_nothrow_copy_assignable_v<std::locale>);
        void operator =(string &&)
            noexcept(std::is_nothrow_move_assignable_v<std::string>
and std::is_nothrow_copy_assignable_v<std::locale>);
        ~string()
            noexcept(std::is_nothrow_destructible_v<std::string>
            and std::is_nothrow_destructible_v<std::locale>);

        /*
        返回当前对象保存的字符串。
        如果 set_locale 为 true，则返回之前，先将全局本地环境设为当前对象保存的本地环境。
        */
        std::string & content(bool const set_locale = false)
            noexcept(noexcept(std::locale::global(std::declval<std::locale>())));
        std::string const & content(bool const set_locale = false)const
            noexcept(noexcept(std::locale::global(std::declval<std::locale>())));

        std::locale & page()noexcept;
        std::locale const & page()const noexcept;

    public:

        /*
        将当前对象保存的字符串转换到utf-8编码

        此函数执行过程中会改变全局本地环境，但最终会恢复原样。
        */
        template<typename target_type>
        target_type utf_8()const
            noexcept(safe_convertible<target_type, string>);

        template<>
        std::string utf_8<std::string>()const
            noexcept(safe_convertible<std::string, string>);
        template<>
        std::u8string utf_8<std::u8string>()const
            noexcept(safe_convertible<std::u8string, string>);

        /*
        通过std::getline函数以 std::locale::classic() 格式从标准输入流 input 输入一行字符串，覆盖当前对象。
        使用此函数后，当前对象的编码将改成 std::locale::classic()

        此函数执行过程中会改变全局本地环境，但最终会恢复原样。
        */
        void get_line(std::istream &)
            noexcept(noexcept(std::getline(std::declval<std::istream &>(), std::declval<std::string &>()))
            and noexcept(std::declval<std::locale &>() = std::locale::classic()));

        template<typename string_type>
        class requirement
        {
        public:
            requirement()noexcept;
            requirement(requirement const &)noexcept;
            requirement(requirement &&)noexcept;
            void operator =(requirement const &)noexcept;
            void operator =(requirement &&)noexcept;
            virtual ~requirement()noexcept;

            virtual bool operator ()(string_type const &) = 0;
        };

        /*
        参数 judge 是判断函数，对于字符串 str，符合要求返回 true，不符合要求返回 false
        从标准输入流输入 input ，调用 std::getline，输入一行字符串，然后调用 judge 判断该行字符串是否符合要求。
        如果符合要求，则将字符串转换到utf-8编码写入 result；否则抛弃这一行输入，且不更改 result。
        此函数的返回值是 judge 函数的判断结果。
        */
        template<typename string_type>
        static bool get_line(std::istream & input, string_type & result, requirement<string_type> & judge);
        /*
        从输入流 input 输入一行字符串，转换到utf-8编码
        */
        template<typename string_type>
        static void get_line(std::istream & input, string_type & result);

    };
    template
        class string::requirement<std::string>;
    template
        class string::requirement<std::u8string>;
    template
        bool string::get_line<std::string>(std::istream & input, std::string & result, requirement<std::string> & judge);
    template
        bool string::get_line<std::u8string>(std::istream & input, std::u8string & result, requirement<std::u8string> & judge);
    template
        void string::get_line<std::string>(std::istream &, std::string &);
    template
        void string::get_line<std::u8string>(std::istream &, std::u8string &);

   /*
   将字符串输出到标准输出流

   此函数执行过程中会改变全局本地环境，但最终会恢复原样。
   */
    std::ostream & operator <<(std::ostream &, string const &)
        noexcept(noexcept(std::declval<std::ostream &>() << std::declval<string const &>().utf_8<std::string>()));
    /*
    从标准输入流以 std::locale::classic() 格式输入字符串，
    即使用此函数后，object 的编码将无条件改成 std::locale::classic()，其内容则被标准输入流输入的字符串覆盖。

    此函数执行过程中会改变全局本地环境，但最终会恢复原样。
    */
    std::istream & operator >>(std::istream &, string & object)
        noexcept(noexcept(std::declval<std::istream &>() >> std::declval<std::string &>())
        and noexcept(std::declval<std::locale &>() = std::locale::classic()));


    /*
    命令语句分离器
    输入任意字符串，将它转换为命令语句，
    也就是，头部和尾部没有空格，中间也不会出现连续多个空格。
    也就是说，任何字符串都会被转换成 "命令 参数1 参数2 参数3 ···"的形式。
    这里所说的空格，仅包括char类型的普通半角空格，即字符字面量 ' '，不支持空字符、制表符、全角空格或其他特殊字符。
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
