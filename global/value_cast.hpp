#pragma once

/*
此文件声明一个函数模板 value_cast，用于多种类型转换。
此文件的模板声明比较复杂，智能感知可能误报语法错误和类型错误，以编译器输出为准。
*/

#include <type_traits>
#include <string>
#include <vector>

#include ".\fundamental.hpp"

namespace OKps
{
    //标准库的字符串类型
    template<typename string_type>
    concept stl_string_type = std::is_same_v<string_type, std::string>
        or std::is_same_v<string_type, std::wstring>
        or std::is_same_v<string_type, std::u8string>
        or std::is_same_v<string_type, std::u16string>
        or std::is_same_v<string_type, std::u32string>;

    //确定两个类型能否被转换函数value_cast转换
    class is_convertible final
    {
    public:
        is_convertible() = delete;
        ~is_convertible() = delete;
        is_convertible(is_convertible const &) = delete;
        is_convertible(is_convertible &&) = delete;
        void operator =(is_convertible const &) = delete;
        void operator =(is_convertible &&) = delete;
    private:
        template<typename target_type, typename origin_type>
        static inline constexpr bool INNER_value()noexcept
        {
            if constexpr (std::is_same_v<target_type, origin_type>)
            {
                return false;
            }
            if constexpr (std::is_same_v<target_type, std::byte>)
            {
                return (
                std::is_same_v<origin_type, std::bitset<bit_per_byte>>
                or std::is_same_v<origin_type, unsigned long long>
                or std::is_same_v<origin_type, long long>
                or std::is_same_v<origin_type, int>
                or std::is_same_v<origin_type, unsigned int>
                or std::is_same_v<origin_type, char>);
            }
            else if constexpr (std::is_same_v<origin_type, std::byte>)
            {
                return (
                std::is_same_v<target_type, std::bitset<bit_per_byte>>
                or std::is_same_v<target_type, int>
                or std::is_same_v<target_type, unsigned int>
                or std::is_same_v<target_type, char>);
            }
            else if constexpr (
                (std::is_same_v<origin_type, std::string> and std::is_same_v<target_type, std::vector<std::byte>>)
                or (std::is_same_v<origin_type, std::vector<std::byte>> and std::is_same_v<target_type, std::string>))
            {
                return true;
            }
            else if constexpr (stl_string_type<origin_type> and stl_string_type<target_type>)
            {
                return (sizeof(typename origin_type::value_type) == sizeof(typename target_type::value_type));
            }
            else
            {
                return false;
            }
        }
    public:
        /*
        若允许 value_cast 函数将 origin_type 类型转换到 target_type 类型，则此常量表达式为true，否则为false。
        */
        template<typename target_type, typename origin_type>
        static inline constexpr bool const value = is_convertible::INNER_value<target_type, origin_type>();
    };

    class stream_position;

    class safe_convertible final
    {
    public:
        safe_convertible() = delete;
        ~safe_convertible() = delete;
        safe_convertible(safe_convertible const &) = delete;
        safe_convertible(safe_convertible &&) = delete;
        void operator =(safe_convertible const &) = delete;
        void operator =(safe_convertible &&) = delete;
    private:
        template<typename target_type, typename origin_type>
        static inline constexpr bool INNER_value()noexcept
        {
            static_assert(not std::is_same_v<target_type, origin_type>, "不允许目标类型与原类型相同");
            if constexpr (
                (std::is_same_v<origin_type, std::string> and std::is_same_v<target_type, std::vector<std::byte>>)
                or (std::is_same_v<origin_type, std::vector<std::byte>> and std::is_same_v<target_type, std::string>)
                or (stl_string_type<target_type> and stl_string_type<origin_type>))
            {
                return (
                    std::is_nothrow_default_constructible_v<target_type>
                    and noexcept(std::declval<target_type>().resize(std::declval<origin_type const &>().size())));
            }
            else if constexpr (std::is_same_v<target_type, std::byte>)
            {
                if constexpr (std::is_same_v<origin_type, int>
                    or std::is_same_v<origin_type, unsigned int>
                    or std::is_same_v<origin_type, unsigned long long>
                    or std::is_same_v<origin_type, long long>)
                {
                    return noexcept(static_cast<base::integer<std::underlying_type_t<std::byte>>>(std::declval<base::integer<origin_type> const &>()));
                }
                else if constexpr (std::is_same_v<origin_type, char>
                or std::is_same_v<origin_type, std::bitset<bit_per_byte>>)
                {
                    return true;
                }
                else
                {
                    static_assert(false, "不允许这两个类型之间的转换");
                }
            }
            else if constexpr (std::is_same_v<origin_type, std::byte>)
            {
                if constexpr (std::is_same_v<target_type, int>
                                or std::is_same_v<target_type, unsigned int>)
                {
                    return noexcept(static_cast<base::integer<target_type>>(std::declval<base::integer<std::underlying_type_t<std::byte>> const &>()));
                }
                else if constexpr (std::is_same_v<target_type, char>)
                {
                    return true;
                }
                else if constexpr (std::is_same_v<target_type, std::bitset<bit_per_byte>>)
                {
                    return (std::is_nothrow_default_constructible_v<std::bitset<bit_per_byte>>
                    and noexcept(std::declval<std::bitset<bit_per_byte>>()[std::declval<std::size_t>()] = std::declval<bool>()));
                }
                else
                {
                    static_assert(false, "不允许这两个类型之间的转换");
                }
            }
            else if constexpr (std::is_same_v<target_type, stream_position>)
            {
                if constexpr (std::is_same_v<origin_type, int>
                or std::is_same_v<origin_type, unsigned int>
                or std::is_same_v<origin_type, long>
                or std::is_same_v<origin_type, unsigned long>
                or std::is_same_v<origin_type, long long>
                or std::is_same_v<origin_type, unsigned long long>)
                {
                    return noexcept(std::streampos(static_cast<base::integer<std::streamoff>>(base::integer<origin_type>(std::declval<origin_type const &>())).value()));
                }
                else
                {
                    static_assert(false, "不允许这两个类型之间的转换");
                }
            }
            else if constexpr (std::is_same_v<origin_type, stream_position>)
            {
                if constexpr (std::is_same_v<target_type, int>
                or std::is_same_v<target_type, unsigned int>
                or std::is_same_v<target_type, long>
                or std::is_same_v<target_type, unsigned long>
                or std::is_same_v<target_type, long long>
                or std::is_same_v<target_type, unsigned long long>)
                {
                    return noexcept(static_cast<base::integer<target_type>>(base::integer<std::streamoff>(std::declval<std::streampos const &>())));
                }
                else
                {
                    static_assert(false, "不允许这两个类型之间的转换");
                }
            }
            else
            {
                static_assert(false, "不允许这两个类型之间的转换");
            }

        }
    public:
        /*
        若从 origin_type 类型转换到 target_type 类型不会抛异常，则此常量表达式为 true。
        */
        template<typename target_type, typename origin_type>
        static inline constexpr bool const value = safe_convertible::INNER_value<target_type, origin_type>();
    };

    /*
    复制传递的参数类型
    */
    template<typename origin_type>
    concept copy_passing = (not std::is_void_v<origin_type>) and (std::is_fundamental_v<origin_type> or std::is_pointer_v<origin_type> or std::is_enum_v<origin_type> or std::is_member_pointer_v<origin_type>);
    /*
    引用传递的参数类型
    */
    template<typename origin_type>
    concept reference_passing = std::is_class_v<origin_type> or std::is_union_v<origin_type>;

    /*
    从 origin_type 类型转换到 target_type 类型
    保证转换前后值不变
    */
    template<typename target_type, copy_passing origin_type>
    typename std::enable_if_t<is_convertible::value<target_type, origin_type>, target_type> value_cast(origin_type const value)
        noexcept(safe_convertible::value<target_type, origin_type>);
    /*
    从 origin_type 类型转换到 target_type 类型
    保证转换前后值不变
    */
    template<typename target_type, reference_passing origin_type>
    typename std::enable_if_t<is_convertible::value<target_type, origin_type>, target_type> value_cast(origin_type const & value)
        noexcept(safe_convertible::value<target_type, origin_type>);

/*
虽然标准库引入了类型安全的，语义更明确的std::byte来替代char和unsigned char存储单字节2进制数据，
但是<fstream>仍然使用char类型字符串作读写缓存，故需要char和unsigned char与std::byte之间的转换。
*/

    template
        std::byte value_cast<std::byte, unsigned long long>(unsigned long long const value)
        noexcept(safe_convertible::value<std::byte, unsigned long long>);
    template
        std::byte value_cast<std::byte, long long>(long long const value)
        noexcept(safe_convertible::value<std::byte, long long>);
    template
        std::byte value_cast<std::byte, char>(char const value)
        noexcept(safe_convertible::value<std::byte, char>);
    template
        std::byte value_cast<std::byte, std::bitset<bit_per_byte>>(std::bitset<bit_per_byte> const & value)
        noexcept(safe_convertible::value<std::byte, std::bitset<bit_per_byte>>);
    template
        std::byte value_cast<std::byte, int>(int const value)
        noexcept(safe_convertible::value<std::byte, int>);
    template
        std::byte value_cast<std::byte, unsigned int>(unsigned int const value)
        noexcept(safe_convertible::value<std::byte, unsigned int>);

    template
        char value_cast<char, std::byte>(std::byte const value)
        noexcept(safe_convertible::value<char, std::byte>);
    template
        std::bitset<bit_per_byte> value_cast<std::bitset<bit_per_byte>, std::byte>(std::byte const value)
        noexcept(safe_convertible::value<std::bitset<bit_per_byte>, std::byte>);
    template
        int value_cast<int, std::byte>(std::byte const value)
        noexcept(safe_convertible::value<int, std::byte>);
    template
        unsigned int value_cast<unsigned int, std::byte>(std::byte const value)
        noexcept(safe_convertible::value<unsigned int, std::byte>);

    template
        std::string value_cast<std::string, std::wstring>(std::wstring const &)
        noexcept(safe_convertible::value<std::string, std::wstring>);
    template
        std::wstring value_cast<std::wstring, std::string>(std::string const &)
        noexcept(safe_convertible::value<std::wstring, std::string>);
    /*
    c++20标准要求，char8_t与unsigned char具有相同的符号性、大小和对齐，
    又要求一个值从char转换到unsigned char再转换回char，值不变，
    故std::string和std::u8string之间的相互转换必然成立，且值不变。
    */
    template
        std::string value_cast<std::string, std::u8string>(std::u8string const &)
        noexcept(safe_convertible::value<std::string, std::u8string>);
    /*
    c++20标准要求，char8_t与unsigned char具有相同的符号性、大小和对齐，
    又要求一个值从char转换到unsigned char再转换回char，值不变，
    故std::string和std::u8string之间的相互转换必然成立，且值不变。
    */
    template
        std::u8string value_cast<std::u8string, std::string>(std::string const &)
        noexcept(safe_convertible::value<std::u8string, std::string>);
    template
        std::string value_cast<std::string, std::u16string>(std::u16string const &)
        noexcept(safe_convertible::value<std::string, std::u16string>);
    template
        std::u16string value_cast<std::u16string, std::string>(std::string const &)
        noexcept(safe_convertible::value<std::u16string, std::string>);
    template
        std::string value_cast<std::string, std::u32string>(std::u32string const &)
        noexcept(safe_convertible::value<std::string, std::u32string>);
    template
        std::u32string value_cast<std::u32string, std::string>(std::string const &)
        noexcept(safe_convertible::value<std::u32string, std::string>);

    template
        std::wstring value_cast<std::wstring, std::u8string>(std::u8string const &)
        noexcept(safe_convertible::value<std::wstring, std::u8string>);
    template
        std::u8string value_cast<std::u8string, std::wstring>(std::wstring const &)
        noexcept(safe_convertible::value<std::u8string, std::wstring>);
    template
        std::wstring value_cast<std::wstring, std::u16string>(std::u16string const &)
        noexcept(safe_convertible::value<std::wstring, std::u16string>);
    template
        std::u16string value_cast<std::u16string, std::wstring>(std::wstring const &)
        noexcept(safe_convertible::value<std::u16string, std::wstring>);
    template
        std::wstring value_cast<std::wstring, std::u32string>(std::u32string const &)
        noexcept(safe_convertible::value<std::wstring, std::u32string>);
    template
        std::u32string value_cast<std::u32string, std::wstring>(std::wstring const &)
        noexcept(safe_convertible::value<std::u32string, std::wstring>);

    template
        std::string value_cast<std::string, std::vector<std::byte>>(std::vector<std::byte> const & data)
        noexcept(safe_convertible::value<std::string, std::vector<std::byte>>);
    template
        std::vector<std::byte> value_cast<std::vector<std::byte>, std::string>(std::string const & data)
        noexcept(safe_convertible::value<std::vector<std::byte>, std::string>);
}