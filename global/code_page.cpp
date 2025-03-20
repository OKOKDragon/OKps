#include <Windows.h>
#include <stdexcept>
#include <locale>

#include ".\fundamental.hpp"
#include ".\error.hpp"
#include ".\code_page.hpp"

namespace OKps
{
    class code_page::implement final
    {
        static_assert(std::is_same_v<wchar_t const *, LPCWSTR>);
        static_assert(std::is_same_v<wchar_t *, LPWSTR>);
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
                throw error(std::runtime_error("无法获取标准输入句柄"));
            }
            // 获取控制台输入代码页
            this->MEMBER_default_input_code_page = GetConsoleCP();
            // 获取控制台输出句柄
            HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (console_handle == INVALID_HANDLE_VALUE)
            {
                throw error(std::runtime_error("无法获取标准输出句柄"));
            }
            // 获取控制台输出代码页
            this->MEMBER_default_output_code_page = GetConsoleOutputCP();
            //将程序代码页设置为utf-8
            if (not SetConsoleOutputCP(CP_UTF8))
            {
                throw error(std::runtime_error("无法设置标准输出代码页"));
            }
            if (not SetConsoleCP(CP_UTF8))
            {
                throw error(std::runtime_error("无法设置标准输入代码页"));
            }
        }
        ~implement()
        {
            if (not SetConsoleOutputCP(this->MEMBER_default_output_code_page))
            {
                throw error(std::runtime_error("无法设置标准输出代码页"));
            }
            if (not SetConsoleCP(this->MEMBER_default_input_code_page))
            {
                throw error(std::runtime_error("无法设置标准输入代码页"));
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
    std::string code_page::default_input_to_UTF_8(std::string const & content)
    {
        if (code_page::MEMBER_implement.get_default_input_code_page() == CP_UTF8)
        {
            return content;
        }
        /*
        MultiByteToWideChar函数以int类型接受字符串的长度参数
        但是在windows上，int是32位，只能表示长度为2GB的字符串，而实际上是可以一次性申请超过2GB内存的，故 content 可能是一个超过2GB的字符串
        */
        int content_length = static_cast<base::integer<int>>(base::integer<std::size_t>(content.size())).value();
        int wide_length = MultiByteToWideChar(code_page::MEMBER_implement.get_default_input_code_page(), 0, content.data(), content_length, nullptr, 0);
        std::wstring wide_buffer;
        wide_buffer.resize(wide_length);
        MultiByteToWideChar(code_page::MEMBER_implement.get_default_input_code_page(), 0, content.data(), content_length, wide_buffer.data(), wide_length);

        int length = WideCharToMultiByte(CP_UTF8, 0, wide_buffer.data(), wide_length, nullptr, 0, nullptr, nullptr);
        std::string buffer;
        buffer.resize(length);

        WideCharToMultiByte(CP_UTF8, 0, wide_buffer.data(), wide_length, buffer.data(), length, nullptr, nullptr);

        return buffer;
    }
    std::string code_page::UTF_16_to_UTF_8(std::wstring const & content)
    {
        //WC_ERR_INVALID_CHARS
        std::string result;
        if (content.size() == 0)
        {
            return result;
        }
        int content_length = static_cast<base::integer<int>>(base::integer<std::size_t>(content.size())).value();
        int length = WideCharToMultiByte(CP_UTF8, 0, content.data(), content_length, nullptr, 0, nullptr, nullptr);
        //if (length == 0 and GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
        //{
            //throw std::invalid_argument("无法转换");
        //}
        result.resize(length);
        WideCharToMultiByte(CP_UTF8, 0, content.data(), content_length, result.data(), length, nullptr, nullptr);
        return result;
    }
    std::wstring code_page::get_process_directory()
    {
        //获取程序路径
        std::wstring buffer;
        buffer.resize(MAX_PATH);
        DWORD result = GetModuleFileNameW(nullptr, buffer.data(), buffer.size());
        if (result == 0)
        {
            throw error(std::runtime_error("获取进程路径失败"));
        }
        std::size_t last_slash = buffer.find_last_of(L"\\/");
        if (last_slash != std::wstring::npos)
        {
            return buffer.substr(0, last_slash + 1); // 包括最后一个斜杠
        }
        else
        {
            return L""; // 如果没有找到斜杠，返回空字符串}     
        }
    }
}
