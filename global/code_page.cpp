#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include ".\fundamental.hpp"
#include ".\code_page.hpp"

namespace OKps
{
    class code_page::implement final
    {
        static_assert(std::is_same_v<wchar_t const *, LPCWSTR>);
    private:
        //windows系统控制台默认的输入代码页
        UINT MEMBER_default_input_code_page;
        //windows系统控制台默认的输出代码页
        UINT MEMBER_default_output_code_page;
    public:
        implement()
        {
            // 获取标准输入句柄
            HANDLE console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
            if (console_input_handle == INVALID_HANDLE_VALUE)
            {
                throw std::runtime_error("无法获取标准输入句柄");
            }
            // 获取控制台输入代码页
            this->MEMBER_default_input_code_page = GetConsoleCP();
            // 获取控制台输出句柄
            HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (console_handle == INVALID_HANDLE_VALUE)
            {
                throw std::runtime_error("无法获取标准输出句柄");
            }
            // 获取控制台输出代码页
            this->MEMBER_default_output_code_page = GetConsoleOutputCP();
            //将程序代码页设置为utf-8
            if (not SetConsoleOutputCP(CP_UTF8))
            {
                throw std::runtime_error("无法设置标准输出代码页");
            }
            if (not SetConsoleCP(CP_UTF8))
            {
                throw std::runtime_error("无法设置标准输入代码页");
            }
            //std::cout << "原代码页是(" << this->MEMBER_default_input_code_page << "," << this->MEMBER_default_output_code_page << ")\n";
        }
        ~implement()
        {
            if (not SetConsoleOutputCP(this->MEMBER_default_output_code_page))
            {
                throw std::runtime_error("无法设置标准输出代码页");
            }
            if (not SetConsoleCP(this->MEMBER_default_input_code_page))
            {
                throw std::runtime_error("无法设置标准输入代码页");
            }
        }
        implement(implement const &) = delete;
        void operator =(implement const &) = delete;
        implement(implement &&) = delete;
        void operator =(implement &&) = delete;
        UINT const & get_default_input_code_page()const noexcept
        {
            return this->MEMBER_default_input_code_page;
        }
    };
    code_page::implement code_page::MEMBER_implement = code_page::implement();
    std::string code_page::default_input_convert(std::string const & content)
    {
        if (code_page::MEMBER_implement.get_default_input_code_page() == CP_UTF8)
        {
            return content;
        }
        int wide_length = MultiByteToWideChar(code_page::MEMBER_implement.get_default_input_code_page(), 0, content.c_str(), -1, NULL, 0);
        //std::cout << "wide_length=" << wide_length << "\n";
        std::wstring wide_buffer;
        wide_buffer.resize(wide_length);
        MultiByteToWideChar(code_page::MEMBER_implement.get_default_input_code_page(), 0, content.c_str(), content.length(), wide_buffer.data(), wide_length);

        int length = WideCharToMultiByte(CP_UTF8, 0, wide_buffer.data(), -1, NULL, NULL, NULL, NULL);
        //std::cout << "length=" << length << "\n";
        std::string buffer;
        buffer.resize(length);

        WideCharToMultiByte(CP_UTF8, 0, wide_buffer.data(), wide_length, buffer.data(), length, NULL, NULL);

        return buffer;
    }
    std::string code_page::UTF_16_to_UTF_8(std::wstring const & content)
    {
        std::string result;
        if (content.size() == 0)
        {
            return result;
        }
        int content_length = static_cast<base::integer<int>>(base::integer<std::size_t>(content.size())).value();
        int length = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, content.data(), content_length, nullptr, 0, nullptr, nullptr);
        if (length == 0 and GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
        {
            throw std::invalid_argument("无法转换");
        }
        result.resize(length);
        WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, content.data(), content_length, result.data(), length, nullptr, nullptr);
        return result;
    }
}
