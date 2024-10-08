﻿#include <stdexcept>
#include <iostream>
#include <fstream>

#include ".\string.hpp"

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

    template<stl_string_type origin_type, stl_string_type target_type>
    typename std::enable_if_t<sizeof(typename origin_type::value_type) == sizeof(typename target_type::value_type), target_type> value_cast(origin_type const & input)

    {
        target_type result;
        result.resize(input.size());
        for (std::size_t i = 0; i < input.size(); ++i)
        {
            result[i] = static_cast<typename target_type::value_type>(input[i]);
        }
        return result;
    }



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
                    std::locale::global(std::locale::classic());
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
                    std::locale::global(std::locale::classic());
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
                    std::locale::global(std::locale::classic());
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
                    std::locale::global(std::locale::classic());
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
                    std::locale::global(std::locale::classic());
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
                    std::locale::global(std::locale::classic());
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
                    std::locale::global(std::locale::classic());
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
                    std::locale::global(std::locale::classic());
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
        if (this != (&origin))
        {
            this->MEMBER_command = origin.MEMBER_command;
            this->MEMBER_argument_list = origin.MEMBER_argument_list;
        }
    }
    void command_statement::operator =(command_statement && origin)
        noexcept(std::is_nothrow_move_assignable_v<std::string>
        and std::is_nothrow_move_assignable_v<std::vector<std::string>>)
    {
        if (this != (&origin))
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
    void echo_command_line(int const argc, char const * const argv[])
    {
        auto const origin_locale = std::locale::global(std::locale::classic());
        for (int counter = 0; counter < argc; counter++)
        {
            std::cout << "命令行参数" << counter << " ││ " << argv[counter] << "\n";
        }
        std::locale::global(origin_locale);
    }
    void wait_input(std::string const & hint, char const signal)
    {
        auto const origin_locale = std::locale::global(std::locale::classic());

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
        std::locale::global(origin_locale);
    }

    std::string read(std::fstream & file, std::uintmax_t const length)
    {
        if (not file.is_open())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象并没有打开文件");
        }
        if (file.fail())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象中有异常状态");
        }
        if (file.bad())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象中有不可恢复的严重异常状态");
        }
        char * buffer = new char[length];
        if (not file.read(buffer, length))
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("读取文件失败");
        }
        std::string result;
        result.resize(length);
        for (std::uintmax_t i = 0;i < length;i++)
        {
            result[i] = buffer[i];
        }
        delete[] buffer;
        return result;
    }

    void write(std::ofstream & file, std::string const & data)
    {
        if (not file.is_open())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象并没有打开文件");
        }
        if (file.fail())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象中有异常状态");
        }
        if (file.bad())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象中有不可恢复的严重异常状态");
        }
        if (not file.write(data.c_str(), data.size()))
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("写入文件失败");
        }
    }

    void write(std::fstream & file, std::string const & data)
    {
        if (not file.is_open())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象并没有打开文件");
        }
        if (file.fail())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象中有异常状态");
        }
        if (file.bad())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("文件对象中有不可恢复的严重异常状态");
        }
        if (not file.write(data.c_str(), data.size()))
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("写入文件失败");
        }
    }
}
