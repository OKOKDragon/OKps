#include ".\string.hpp"

/*
禁用C4996警告
因为C语言库中的一些用于字符串编码转换的函数被c++标准弃用，但标准库没有提供替代品
windows在windows.h中提供了系统函数，用于替代这些库函数，但我希望此代码跨平台，所以只用标准库
*/
#pragma warning(disable : 4996)

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
    /*
    void string::set_locale(const std::string &_locale)
    {
        this->data_ANSI = this->Unicode_to_ANSI(this->data_Unicode);
    }
    */
    string::string(const std::string & ARG_locale)
    {
        this->MEMBER_locale = ARG_locale;
    }
    string::string(const string & ARG_origin)
        : MEMBER_locale(ARG_origin.MEMBER_locale)
    {
    }

    string::~string()
    {
    }

    std::string string::Unicode_to_UTF8(const std::wstring & origin)
    {

        std::wstring_convert<std::codecvt_utf8<wchar_t>> temp;
        return temp.to_bytes(origin);
    }

    std::wstring string::UTF8_to_Unicode(const std::string & origin)
    {

        std::wstring_convert<std::codecvt_utf8<wchar_t>> temp;
        return temp.from_bytes(origin);

    }
    std::string string::Unicode_to_ANSI(const std::wstring & origin) const
    {
        return this->Unicode_to_ANSI(origin, this->MEMBER_locale);
    }
    std::wstring string::ANSI_to_Unicode(const std::string & origin) const
    {
        return this->ANSI_to_Unicode(origin, this->MEMBER_locale);
    }
    std::string string::Unicode_to_ANSI(const std::wstring & origin, const std::string & locale)
    {
        using trans_rule = std::codecvt_byname<wchar_t, char, std::mbstate_t>;
        static std::wstring_convert<trans_rule> temp(new trans_rule(locale));
        return temp.to_bytes(origin);
    }

    std::wstring string::ANSI_to_Unicode(const std::string & origin, const std::string & locale)
    {
        using trans_rule = std::codecvt_byname<wchar_t, char, std::mbstate_t>;
        static std::wstring_convert<trans_rule> temp(new trans_rule(locale));
        return temp.from_bytes(origin);
    }

    std::string string::UTF8_to_ANSI(const std::string & origin, const std::string & locale)
    {
        auto temp = string::UTF8_to_Unicode(origin);

        return string::Unicode_to_ANSI(temp, locale);
    }

    std::string string::ANSI_to_UTF8(const std::string & origin, const std::string & locale)
    {
        auto temp = string::ANSI_to_Unicode(origin, locale);

        return string::Unicode_to_UTF8(temp);
    }
    std::string string::UTF8_to_ANSI(const std::string & origin)const
    {
        return string::UTF8_to_ANSI(origin, this->MEMBER_locale);
    }
    std::string string::ANSI_to_UTF8(const std::string & origin)const
    {
        return string::ANSI_to_UTF8(origin, this->MEMBER_locale);

    }

    bool string::is_number(const char input) noexcept
    {
        if (input >= '0' and input <= '9')
        {
            return true;
        }
        return false;
    }

    char string::to_char(const char input)
    {
        if (input >= 0 and input <= 9)
        {
            return (input + '0');
        }
        else
        {
            throw std::invalid_argument("输入的不是10进制数字");
        }
    }

    char string::to_number(const char input)
    {
        if (string::is_number(input))
        {
            return (input - '0');
        }
        else
        {

            throw std::invalid_argument("输入的不是10进制数字");
        }
    }
    std::string string::to_string16(const size_t input)
    {

        std::string result;
        for (size_t count = 0; count < 2 * (sizeof(size_t) / sizeof(char)); count++)
        {
            result.push_back('0');
        }
        size_t temp = input;
        int position = result.size() - 1;
        while (temp / (size_t)16 > 0)
        {
            result[position] = string::number16_to_char((char)(temp % 16));
            position = position - 1;
            temp = temp / (size_t)16;
        }
        result[position] = string::number16_to_char((char)temp);
        return result;
    }
    size_t string::string16_to_ULL(const std::string & input)
    {
        if (input.size() != 2 * (sizeof(size_t) / sizeof(char)))
        {

            throw std::invalid_argument("输入串长度错误");
        }
        size_t result = 0;
        for (size_t count = 0; count < 2 * (sizeof(size_t) / sizeof(char)); count++)
        {
            // std::cout << "count=" << count << "\n";
            size_t temp = string::char16_to_number(input[input.size() - 1 - count]);
            // std::cout << "temp=" << temp << "\n";
            for (size_t count1 = 0; count1 < count; count1++)
            {
                temp = temp * 16;
            }
            // std::cout << "temp=" << temp << "\n";

            result = result + temp;
            // std::cout << "result=" << result << "\n";
        }
        return result;
    }

    char string::number16_to_char(const char input)
    {
        if (input < 0 or input > 15)
        {

            throw std::invalid_argument("输入的不是16进制数字");
        }
        if (input >= 0 and input <= 9)
        {
            return (input + '0');
        }
        else
        {
            return (input - 10 + 'a');
        }
    }
    char string::char16_to_number(const char input)
    {
        if (input >= '0' and input <= '9')
        {
            return (input - '0');
        }
        else if (input >= 'a' and input <= 'f')
        {
            return (input - 'a' + 10);
        }
        else if (input >= 'A' and input <= 'F')
        {
            return (input - 'A' + 10);
        }
        else
        {

            throw std::invalid_argument("输入的字符不是16进制数字");
        }
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
}
