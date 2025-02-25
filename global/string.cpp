
#include <cwchar>
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <memory>

#include ".\string.hpp"
#include ".\code_page.hpp"

/*
禁用C4996警告
c++17弃用了<codecvt>库，但其替代品<text_encoding>在c++26才提上议程
windows在windows.h中提供了系统函数，可以替代标准库，但我希望此代码跨平台，所以只用标准库
*/
//#pragma warning(disable : 4996)

/*
std::string string::String_to_UTF8(const std::string &str)
{

    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    wchar_t *pwBuf = new wchar_t[nwLen + 1];
    ZeroMemory(pwBuf, nwLen * 2 + 2);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char *pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr(pBuf);

    delete[] pwBuf;
    delete[] pBuf;

    pwBuf = NULL;
    pBuf = NULL;

    return retStr;
}

std::string string::UTF8_to_String(const std::string &s)
{
    if (s.empty())
    {
        return std::string();
    }

    std::wstring result;

    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
    wchar_t *buffer = new wchar_t[n];

    ::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, n);

    result = buffer;
    delete[] buffer;

    std::string result2;
    int len = WideCharToMultiByte(CP_ACP, 0, result.c_str(), result.size(), NULL, 0, NULL, NULL);
    char *buffer2 = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, result.c_str(), result.size(), buffer2, len, NULL, NULL);
    buffer2[len] = '\0';
    result2.append(buffer2);
    delete[] buffer2;

    return result2;
}
*/

namespace OKps
{
    char operator ""_utf_8(char8_t const content)noexcept
    {
        return static_cast<char>(content);
    }
    std::string operator ""_utf_8(char8_t const * const content, std::size_t const size)
        noexcept(std::is_nothrow_default_constructible_v<std::string>
and noexcept(std::declval<std::string>().resize(std::declval<std::size_t const>())))
    {
        std::string result;
        result.resize(size);
        for (std::size_t i = 0;i < size;++i)
        {
            result[i] = static_cast<char>(content[i]);
        }
        return result;
    }

    std::locale const text_encoding::utf_8 = text_encoding::INNER_init();

    std::locale text_encoding::INNER_init()
        noexcept(noexcept(std::locale(""))
    and noexcept(std::locale::global(std::declval<std::locale &>())))
    {
        std::locale TEMP_utf_8(".utf-8");
        std::locale::global(TEMP_utf_8);
        std::filesystem::current_path(code_page::get_process_directory());
        return TEMP_utf_8;
    }

/*
    std::string string::INNER_wide_to_narrow(std::wstring const & origin)
    {

        wchar_t const * origin_string = &(origin[0]);
        std::mbstate_t state = std::mbstate_t();
        std::size_t const result_length = std::wcsrtombs(nullptr, &origin_string, 0, &state);
        std::string result;
        result.resize(result_length);
        std::size_t const convert_result = std::wcsrtombs(&(result[0]), &origin_string, result_length, &state);
        if (convert_result == static_cast<std::size_t>(-1) and errno == EILSEQ)
        {
            throw std::runtime_error(std::strerror(errno));
        }
        return result;
    }
    std::wstring string::INNER_narrow_wo_wide(std::string const & origin)
    {

        char const * origin_string = &(origin[0]);
        std::mbstate_t state = std::mbstate_t();
        std::size_t const result_length = std::mbsrtowcs(nullptr, &origin_string, 0, &state);
        std::wstring result;
        result.resize(result_length);
        std::size_t const convert_result = std::mbsrtowcs(&(result[0]), &origin_string, result_length, &state);
        if (convert_result == static_cast<std::size_t>(-1) and errno == EILSEQ)
        {
            throw std::runtime_error(std::strerror(errno));
        }
        return result;
    }
*/

    char to_char(unsigned int const number, number_system const number_base)
    {
        char result;
        switch (number_base)
        {
            case number_system::bin:
            {
                if (number >= 0 and number <= 1)
                {
                    result = static_cast<char>(number + static_cast<unsigned char>('0'));
                }
                else
                {
                    throw std::invalid_argument("输入的整数不是单个2进制数字");
                }
                break;
            }
            case number_system::dec:
            {
                if (number >= 0 and number <= 9)
                {
                    result = static_cast<char>(number + static_cast<unsigned char>('0'));
                }
                else
                {
                    throw std::invalid_argument("输入的整数不是单个10进制数字");
                }
                break;
            }
            case number_system::hex:
            {
                if (number >= 0 and number <= 9)
                {
                    result = static_cast<char>(number + static_cast<unsigned char>('0'));
                }
                else if (number >= 10 and number <= 15)
                {
                    result = static_cast<char>(number - 10 + static_cast<unsigned char>('a'));
                }
                else
                {
                    throw std::invalid_argument("输入的整数不是单个16进制数字");
                }
                break;
            }
            case number_system::oct:
            {
                if (number >= 0 and number <= 7)
                {
                    result = static_cast<char>(number + static_cast<unsigned char>('0'));
                }
                else
                {
                    throw std::invalid_argument("输入的整数不是单个8进制数字");
                }
                break;
            }
        }
        return result;
    }

    unsigned int from_char(char const number, number_system const number_base)
    {
        unsigned int result;
        switch (number_base)
        {
            case number_system::bin:
            {
                if (number >= '0' and number <= '1')
                {
                    result = static_cast<unsigned char>(number - '0');
                }
                else
                {
                    throw std::invalid_argument("输入的字符不表示单个2进制数字");
                }
                break;
            }
            case number_system::dec:
            {
                if (number >= '0' and number <= '9')
                {
                    result = static_cast<unsigned char>(number - '0');
                }
                else
                {
                    throw std::invalid_argument("输入的字符不表示单个10进制数字");
                }
                break;
            }
            case number_system::hex:
            {
                if (number >= '0' and number <= '9')
                {
                    result = static_cast<unsigned char>(number - 0);
                }
                else if (number >= 'a' and number <= 'f')
                {
                    result = static_cast<unsigned char>(number - 'a' + 10);
                }
                else if (number >= 'A' and number <= 'F')
                {
                    result = static_cast<unsigned char>(number - 'A' + 10);
                }
                else
                {
                    throw std::invalid_argument("输入的字符不表示单个16进制数字");
                }
                break;
            }
            case number_system::oct:
            {
                if (number >= '0' and number <= '7')
                {
                    result = static_cast<unsigned char>(number - '0');
                }
                else
                {
                    throw std::invalid_argument("输入的字符不表示单个8进制数字");
                }
                break;
            }
        }
        return result;
    }
    const std::string & command_statement::command() const noexcept
    {
        return this->MEMBER_command;
    }
    const std::vector<std::string> & command_statement::argument_list() const noexcept
    {
        return this->MEMBER_argument_list;
    }
    std::string command_statement::command_string() const
    {
        std::string result = this->MEMBER_command;
        for (size_t LOOP_count = 0; LOOP_count < this->MEMBER_argument_list.size(); LOOP_count++)
        {
            result = result + ' ' + this->MEMBER_argument_list[LOOP_count];
        }
        return result;
    }
    command_statement::~command_statement()
        noexcept(std::is_nothrow_destructible_v<std::string>
        and std::is_nothrow_destructible_v<std::vector<std::string>>)
    {
    }

    command_statement::command_statement(command_statement const & origin)
        noexcept(std::is_nothrow_copy_constructible_v<std::string>
        and std::is_nothrow_copy_constructible_v<std::vector<std::string>>)
        :MEMBER_command(origin.MEMBER_command)
        , MEMBER_argument_list(origin.MEMBER_argument_list)
    {
    }
    command_statement::command_statement(command_statement && origin)
        noexcept(std::is_nothrow_move_constructible_v<std::string>
        and std::is_nothrow_move_constructible_v<std::vector<std::string>>)
        :MEMBER_command(std::move(origin.MEMBER_command))
        , MEMBER_argument_list(std::move(origin.MEMBER_argument_list))
    {
    }
    void command_statement::operator =(command_statement const & origin)
        noexcept(std::is_nothrow_copy_assignable_v<std::string>
        and std::is_nothrow_copy_assignable_v<std::vector<std::string>>)
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_command = origin.MEMBER_command;
            this->MEMBER_argument_list = origin.MEMBER_argument_list;
        }
    }
    void command_statement::operator =(command_statement && origin)
        noexcept(std::is_nothrow_move_assignable_v<std::string>
        and std::is_nothrow_move_assignable_v<std::vector<std::string>>)
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_command = std::move(origin.MEMBER_command);
            this->MEMBER_argument_list = std::move(origin.MEMBER_argument_list);
        }
    }
    command_statement::command_statement(const std::string & origin_statement)
    {

        size_t position = 0;
        while (position < origin_statement.size())
        {
            if (origin_statement[position] == ' ')
            {
                position++;
            }
            else
            {
                break;
            }
        }
        /*
        现在，position是头部第1个非空格字符的下标
        */
        while (position < origin_statement.size())
        {
            if (origin_statement[position] == ' ')
            {
                break;
            }
            else
            {
                this->MEMBER_command.push_back(origin_statement[position]);
                position++;
            }
        }
        /*
        现在，position是 "命令" 后第1个空格的下标
        */
        while (position < origin_statement.size())
        {
            /*
            现在，position是 上一个参数后第1个空格的下标
            */
            while (position < origin_statement.size())
            {
                if (origin_statement[position] == ' ')
                {
                    position++;
                }
                else
                {
                    break;
                }
            }
            if (position == origin_statement.size())
            {
                return;
            }
            /*
            现在，position是 上一个参数后的第1个非空格字符的下标
            也就是下一个参数的起始下标
            */
            this->MEMBER_argument_list.push_back(std::string());
            while (position < origin_statement.size())
            {
                if (origin_statement[position] == ' ')
                {
                    break;
                }
                else
                {
                    this->MEMBER_argument_list.back().push_back(origin_statement[position]);
                    position++;
                }
            }
        }
    }
    void echo_command_line(int const argc, char const * const * const argv)
    {
        std::cout << "当前工作目录 ││ " << std::filesystem::current_path().string() << "\n";
        for (int counter = 0; counter < argc; counter++)
        {
            std::cout << "命令行参数" << counter << " ││ " << code_page::default_input_to_UTF_8(argv[counter]) << "\n";
        }
    }
    void wait_input(std::string const & hint, char const signal)
    {
        std::cout << hint;
        while (true)
        {
            char temp;
            if (std::cin.get(temp))
            {
                if (temp == signal)
                {
                    break;
                }
            }
            else
            {
                throw std::runtime_error("std::cin输入流发生错误");
            }
        }
    }

}
