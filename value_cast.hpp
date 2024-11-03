#pragma once

/*
此文件声明一个函数模板 value_cast，用于多种类型转换。
此文件的模板声明比较复杂，智能感知可能误报语法错误和类型错误，不要过于依赖智能感知。以编译器输出为准。
*/

#include <type_traits>
#include <string>
#include <vector>

#include ".\fundamental.hpp"

namespace OKps
{
    /*
    若允许 value_cast 函数将 origin_type 类型转换到 target_type 类型，则此常量表达式为true。
    */
    template<typename target_type, typename origin_type>
    constexpr bool const is_convertible;

    /*
    若从 origin_type 类型转换到 target_type 类型不会抛异常，则此常量表达式为 true。
    */
    template<typename target_type, typename origin_type>
    constexpr bool const safe_convertible;

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
    不要使用此命名空间下的任何代码。
    这些代码仅用于辅助实现此库的内部逻辑。
    */
    namespace implement
    {
        /*
        标准库定义的字符串类型
        */
        template<typename string_type>
        concept stl_string_type = std::is_same_v<string_type, std::string>
            or std::is_same_v<string_type, std::wstring>
            or std::is_same_v<string_type, std::u8string>
            or std::is_same_v<string_type, std::u16string>
            or std::is_same_v<string_type, std::u32string>;

        template<stl_string_type target_type, stl_string_type origin_type>
        constexpr bool const is_convertible = sizeof(typename origin_type::value_type) == sizeof(typename target_type::value_type);

        template<stl_string_type target_type, stl_string_type origin_type>
        constexpr bool const safe_convertible = std::is_nothrow_destructible_v<target_type>
            and noexcept(std::declval<target_type>().resize(std::declval<origin_type const &>().size()));

    }

    /*
    从 origin_type 类型转换到 target_type 类型
    保证转换前后值不变
    */
    template<typename target_type, copy_passing origin_type>
    typename std::enable_if_t<is_convertible<target_type, origin_type>, target_type> value_cast(origin_type const value)
        noexcept(safe_convertible<target_type, origin_type>);
    /*
    从 origin_type 类型转换到 target_type 类型
    保证转换前后值不变
    */
    template<typename target_type, reference_passing origin_type>
    typename std::enable_if_t<is_convertible<target_type, origin_type>, target_type> value_cast(origin_type const & value)
        noexcept(safe_convertible<target_type, origin_type>);

    template<>
    constexpr bool const is_convertible<std::byte, std::bitset<bit_per_byte>> = true;
    template<>
    constexpr bool const is_convertible<std::string, std::vector<std::byte>> = true;
    template<>
    constexpr bool const is_convertible<std::vector<std::byte>, std::string> = true;

    template<>
    constexpr bool const is_convertible<int, std::byte> = true;

    template<>
    constexpr bool const is_convertible<std::byte, unsigned long long> = true;
    template<>
    constexpr bool const is_convertible<std::byte, long long> = true;

    template<>
    constexpr bool const is_convertible<unsigned int, std::byte> = true;

    template<>
    constexpr bool const is_convertible<std::byte, int> = true;

    template<>
    constexpr bool const is_convertible<std::byte, unsigned int> = true;

    template<>
    constexpr bool const is_convertible<char, std::byte> = true;

    template<>
    constexpr bool const is_convertible<std::byte, char> = true;

    template<>
    constexpr bool const is_convertible<std::bitset<bit_per_byte>, std::byte> = true;

    template<>
    constexpr bool const safe_convertible<int, std::byte> = noexcept(static_cast<base::integer<int>>(std::declval<base::integer<std::underlying_type_t<std::byte>> const &>()));

    template<>
    constexpr bool const safe_convertible<unsigned int, std::byte> = noexcept(static_cast<base::integer<unsigned int>>(std::declval<base::integer<std::underlying_type_t<std::byte>> const &>()));

    template<>
    constexpr bool const safe_convertible<std::byte, int> = noexcept(static_cast<base::integer<std::underlying_type_t<std::byte>>>(std::declval<base::integer<int> const &>()));

    template<>
    constexpr bool const safe_convertible<std::byte, unsigned int> = noexcept(static_cast<base::integer<std::underlying_type_t<std::byte>>>(std::declval<base::integer<unsigned int> const &>()));

    template<>
    constexpr bool const safe_convertible<std::byte, unsigned long long> = noexcept(static_cast<base::integer<std::underlying_type_t<std::byte>>>(std::declval<base::integer<unsigned long long> const &>()));
    template<>
    constexpr bool const safe_convertible<std::byte, long long> = noexcept(static_cast<base::integer<std::underlying_type_t<std::byte>>>(std::declval<base::integer<long long> const &>()));

    template<>
    constexpr bool const safe_convertible<char, std::byte> = true;

    template<>
    constexpr bool const safe_convertible<std::byte, char> = true;

    template<>
    constexpr bool const safe_convertible<std::bitset<bit_per_byte>, std::byte> = std::is_nothrow_default_constructible_v<std::bitset<bit_per_byte>>
        and noexcept(std::declval<std::bitset<bit_per_byte>>()[std::declval<std::size_t>()] = std::declval<bool>());

    template<>
    constexpr bool const safe_convertible<std::byte, std::bitset<bit_per_byte>> = true;

/*
虽然标准库引入了类型安全的，语义更明确的std::byte来替代char和unsigned char存储单字节2进制数据，
但是<fstream>仍然使用char类型字符串作读写缓存，故需要char和unsigned char与std::byte之间的转换。
*/

    template
        std::byte value_cast<std::byte, unsigned long long>(unsigned long long const value)
        noexcept(safe_convertible<std::byte, unsigned long long>);
    template
        std::byte value_cast<std::byte, long long>(long long const value)
        noexcept(safe_convertible<std::byte, long long>);
    template
        std::byte value_cast<std::byte, char>(char const value)
        noexcept(safe_convertible<std::byte, char>);
    template
        std::byte value_cast<std::byte, std::bitset<bit_per_byte>>(std::bitset<bit_per_byte> const & value)
        noexcept(safe_convertible<std::byte, std::bitset<bit_per_byte>>);
    template
        std::byte value_cast<std::byte, int>(int const value)
        noexcept(safe_convertible<std::byte, int>);
    template
        std::byte value_cast<std::byte, unsigned int>(unsigned int const value)
        noexcept(safe_convertible<std::byte, unsigned int>);

    template
        char value_cast<char, std::byte>(std::byte const value)
        noexcept(safe_convertible<char, std::byte>);
    template
        std::bitset<bit_per_byte> value_cast<std::bitset<bit_per_byte>, std::byte>(std::byte const value)
        noexcept(safe_convertible<std::bitset<bit_per_byte>, std::byte>);
    template
        int value_cast<int, std::byte>(std::byte const value)
        noexcept(safe_convertible<int, std::byte>);
    template
        unsigned int value_cast<unsigned int, std::byte>(std::byte const value)
        noexcept(safe_convertible<unsigned int, std::byte>);

    template<>
    constexpr bool const is_convertible<std::string, std::wstring> = implement::is_convertible<std::string, std::wstring>;
    template<>
    constexpr bool const is_convertible<std::wstring, std::string> = implement::is_convertible<std::wstring, std::string>;

    template<>
    constexpr bool const is_convertible<std::string, std::u8string> = implement::is_convertible<std::string, std::u8string>;
    template<>
    constexpr bool const is_convertible<std::u8string, std::string> = implement::is_convertible<std::u8string, std::string>;
    template<>
    constexpr bool const is_convertible<std::string, std::u16string> = implement::is_convertible<std::string, std::u16string>;
    template<>
    constexpr bool const is_convertible<std::u16string, std::string> = implement::is_convertible<std::u16string, std::string>;
    template<>
    constexpr bool const is_convertible<std::string, std::u32string> = implement::is_convertible<std::string, std::u32string>;
    template<>
    constexpr bool const is_convertible<std::u32string, std::string> = implement::is_convertible<std::u32string, std::string>;

    template<>
    constexpr bool const is_convertible<std::wstring, std::u8string> = implement::is_convertible<std::wstring, std::u8string>;
    template<>
    constexpr bool const is_convertible<std::u8string, std::wstring> = implement::is_convertible<std::u8string, std::wstring>;
    template<>
    constexpr bool const is_convertible<std::wstring, std::u16string> = implement::is_convertible<std::wstring, std::u16string>;
    template<>
    constexpr bool const is_convertible<std::u16string, std::wstring> = implement::is_convertible<std::u16string, std::wstring>;
    template<>
    constexpr bool const is_convertible<std::wstring, std::u32string> = implement::is_convertible<std::wstring, std::u32string>;
    template<>
    constexpr bool const is_convertible<std::u32string, std::wstring> = implement::is_convertible<std::u32string, std::wstring>;

    template<>
    constexpr bool const safe_convertible<std::string, std::wstring> = implement::safe_convertible<std::string, std::wstring>;
    template<>
    constexpr bool const safe_convertible<std::wstring, std::string> = implement::safe_convertible<std::wstring, std::string>;

    template<>
    constexpr bool const safe_convertible<std::string, std::u8string> = implement::safe_convertible<std::string, std::u8string>;
    template<>
    constexpr bool const safe_convertible<std::u8string, std::string> = implement::safe_convertible<std::u8string, std::string>;
    template<>
    constexpr bool const safe_convertible<std::string, std::u16string> = implement::safe_convertible<std::string, std::u16string>;
    template<>
    constexpr bool const safe_convertible<std::u16string, std::string> = implement::safe_convertible<std::u16string, std::string>;
    template<>
    constexpr bool const safe_convertible<std::string, std::u32string> = implement::safe_convertible<std::string, std::u32string>;
    template<>
    constexpr bool const safe_convertible<std::u32string, std::string> = implement::safe_convertible<std::u32string, std::string>;

    template<>
    constexpr bool const safe_convertible<std::wstring, std::u8string> = implement::safe_convertible<std::wstring, std::u8string>;
    template<>
    constexpr bool const safe_convertible<std::u8string, std::wstring> = implement::safe_convertible<std::u8string, std::wstring>;
    template<>
    constexpr bool const safe_convertible<std::wstring, std::u16string> = implement::safe_convertible<std::wstring, std::u16string>;
    template<>
    constexpr bool const safe_convertible<std::u16string, std::wstring> = implement::safe_convertible<std::u16string, std::wstring>;
    template<>
    constexpr bool const safe_convertible<std::wstring, std::u32string> = implement::safe_convertible<std::wstring, std::u32string>;
    template<>
    constexpr bool const safe_convertible<std::u32string, std::wstring> = implement::safe_convertible<std::u32string, std::wstring>;

    template
        std::string value_cast<std::string, std::wstring>(std::wstring const &)
        noexcept(safe_convertible<std::string, std::wstring>);
    template
        std::wstring value_cast<std::wstring, std::string>(std::string const &)
        noexcept(safe_convertible<std::wstring, std::string>);
    /*
    c++20标准要求，char8_t与unsigned char具有相同的符号性、大小和对齐，
    又要求一个值从char转换到unsigned char再转换回char，值不变，
    故std::string和std::u8string之间的相互转换必然成立，且值不变。
    */
    template
        std::string value_cast<std::string, std::u8string>(std::u8string const &)
        noexcept(safe_convertible<std::string, std::u8string>);
    /*
    c++20标准要求，char8_t与unsigned char具有相同的符号性、大小和对齐，
    又要求一个值从char转换到unsigned char再转换回char，值不变，
    故std::string和std::u8string之间的相互转换必然成立，且值不变。
    */
    template
        std::u8string value_cast<std::u8string, std::string>(std::string const &)
        noexcept(safe_convertible<std::u8string, std::string>);
    template
        std::string value_cast<std::string, std::u16string>(std::u16string const &)
        noexcept(safe_convertible<std::string, std::u16string>);
    template
        std::u16string value_cast<std::u16string, std::string>(std::string const &)
        noexcept(safe_convertible<std::u16string, std::string>);
    template
        std::string value_cast<std::string, std::u32string>(std::u32string const &)
        noexcept(safe_convertible<std::string, std::u32string>);
    template
        std::u32string value_cast<std::u32string, std::string>(std::string const &)
        noexcept(safe_convertible<std::u32string, std::string>);

    template
        std::wstring value_cast<std::wstring, std::u8string>(std::u8string const &)
        noexcept(safe_convertible<std::wstring, std::u8string>);
    template
        std::u8string value_cast<std::u8string, std::wstring>(std::wstring const &)
        noexcept(safe_convertible<std::u8string, std::wstring>);
    template
        std::wstring value_cast<std::wstring, std::u16string>(std::u16string const &)
        noexcept(safe_convertible<std::wstring, std::u16string>);
    template
        std::u16string value_cast<std::u16string, std::wstring>(std::wstring const &)
        noexcept(safe_convertible<std::u16string, std::wstring>);
    template
        std::wstring value_cast<std::wstring, std::u32string>(std::u32string const &)
        noexcept(safe_convertible<std::wstring, std::u32string>);
    template
        std::u32string value_cast<std::u32string, std::wstring>(std::wstring const &)
        noexcept(safe_convertible<std::u32string, std::wstring>);

    template<>
    constexpr bool const safe_convertible<std::string, std::vector<std::byte>> = std::is_nothrow_default_constructible_v<std::string>
        and noexcept(std::declval<std::string>().resize(std::declval<std::vector<std::byte> const &>().size()));
    template<>
    constexpr bool const safe_convertible<std::vector<std::byte>, std::string> = std::is_nothrow_default_constructible_v<std::vector<std::byte>>
        and noexcept(std::declval<std::vector<std::byte>>().resize(std::declval<std::string const &>().size()));

    template
        std::string value_cast<std::string, std::vector<std::byte>>(std::vector<std::byte> const & data)
        noexcept(safe_convertible<std::string, std::vector<std::byte>>);
    template
        std::vector<std::byte> value_cast<std::vector<std::byte>, std::string>(std::string const & data)
        noexcept(safe_convertible<std::vector<std::byte>, std::string>);
}