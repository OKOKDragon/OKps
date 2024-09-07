#pragma once

#include <string>

#include ".\number.hpp"

namespace OKps
{

    inline const constexpr std::size_t bit_per_byte = CHAR_BIT/*c标准库定义的宏*/;// 每字节有多少位

    /*
    虽然标准库引入了类型安全的，语义更明确的std::byte来替代char和unsigned char作为单字节类型
    但是<fstream>仍然使用char类型作读写缓存，无法避免使用char类型字符串作为字节流的情况，也无法避免char、unsigned char、std::byte之间转换
    */
    static_assert(sizeof(std::byte) == 1, "要求std::byte表示1字节");
    static_assert(sizeof(unsigned char) == sizeof(char), "要求unsigned char和char大小相同");
    static_assert(sizeof(unsigned char) == sizeof(std::byte), "要求char的大小是1字节");
    static_assert(bit_per_byte == 8, "要求每字节占8个2进制位");

    //表示单个字节数据的类型
    class byte final
    {
    private:
        std::byte data;
    public:
        class reference final
        {
            friend class byte;
        private:
            std::size_t position;
            byte & data;
            reference(byte & owner, std::size_t const & pos)noexcept;
        public:
            reference(reference const & origin)noexcept;
            void operator =(reference const & origin)noexcept;
            reference(reference && origin)noexcept = delete;
            void operator =(reference && origin)noexcept = delete;
            void operator =(bool const value)noexcept;
            operator bool()const noexcept;
            void operator =(char const value);
            void operator =(wchar_t const value);
            void operator =(char8_t const value);
            void operator =(char16_t const value);
            void operator =(char32_t const value);

            ~reference()noexcept;
        };
    private:

        bool INNER_at(std::size_t const & position)const noexcept;
        reference INNER_at(std::size_t const & position)noexcept;
    public:
        byte(unsigned long long const & input);

        byte(std::byte const & input)noexcept;
        void operator =(std::byte const & origin)noexcept;
        bool operator ==(std::byte const & right)const noexcept;
        bool operator !=(std::byte const & right)const noexcept;
        operator std::byte()const noexcept;
        /*
        内容只含'0'和'1'的字符串转换为1字节数据
        字符串的顺序按照人类阅读顺序，即高位在前，低位在后
        */
        byte(std::string const & input);
        void operator =(std::string const & origin);
        bool operator ==(std::string const & right)const;
        bool operator !=(std::string const & right)const;
        operator std::string()const noexcept;

        byte(std::u8string const & input);
        void operator =(std::u8string const & origin);
        bool operator ==(std::u8string const & right)const;
        bool operator !=(std::u8string const & right)const;
        operator std::u8string()const noexcept;

        byte(std::u16string const & input);
        void operator =(std::u16string const & origin);
        bool operator ==(std::u16string const & right)const;
        bool operator !=(std::u16string const & right)const;
        operator std::u16string()const noexcept;

        byte(std::u32string const & input);
        void operator =(std::u32string const & origin);
        bool operator ==(std::u32string const & right)const;
        bool operator !=(std::u32string const & right)const;
        operator std::u32string()const noexcept;

        byte(std::wstring const & input);
        void operator =(std::wstring const & origin);
        bool operator ==(std::wstring const & right)const;
        bool operator !=(std::wstring const & right)const;
        operator std::wstring()const noexcept;

        byte(byte const & origin)noexcept;
        void operator =(byte const & origin)noexcept;
        bool operator ==(byte const & right)const noexcept;
        bool operator !=(byte const & right)const noexcept;


        //下标顺序按照习惯，低位的下标小于高位的下标
        bool operator [](std::size_t const position)const;
        reference operator [](std::size_t const position);

        byte operator &(byte const & right) noexcept;
        byte operator |(byte const & right) noexcept;
        byte operator ^(byte const & right) noexcept;
        byte operator ~() noexcept;
        byte operator <<(std::size_t const shift);
        byte operator >>(std::size_t const shift);

        void operator &=(byte const & right) noexcept;
        void operator |=(byte const & right) noexcept;
        void operator ^=(byte const & right) noexcept;
        void operator <<=(std::size_t const shift);
        void operator >>=(std::size_t const shift);
        ~byte()noexcept;
        byte(byte &&) = delete;
        void operator =(byte &&) = delete;
    };

    static_assert(sizeof(byte) == 1);

    char value_cast(std::byte const value)noexcept;
    std::byte value_cast(char const value)noexcept;

}