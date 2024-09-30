#pragma once

#include <bitset>
#include <type_traits>

#include ".\fundamental.hpp"

namespace OKps
{
    template<typename origin_type, typename result_type>
    constexpr bool const safe_convertible;

    template<>
    constexpr bool const safe_convertible<std::byte, int> = noexcept(static_cast<base::integer<int> const &>(std::declval<base::integer<std::underlying_type_t<std::byte>> const &>()));
    template<>
    constexpr bool const safe_convertible<std::byte, unsigned int> = noexcept(static_cast<base::integer<unsigned int> const &>(std::declval<base::integer<std::underlying_type_t<std::byte>> const &>()));
    template<>
    constexpr bool const safe_convertible<int, std::byte> = noexcept(static_cast<base::integer<std::underlying_type_t<std::byte>> const &>(std::declval<base::integer<int> const &>()));
    template<>
    constexpr bool const safe_convertible<unsigned int, std::byte> = noexcept(static_cast<base::integer<std::underlying_type_t<std::byte>> const &>(std::declval<base::integer<unsigned int> const &>()));
    template<>
    constexpr bool const safe_convertible<std::byte, char> = true;
    template<>
    constexpr bool const safe_convertible<std::byte, std::bitset<bit_per_byte>> = std::is_nothrow_default_constructible_v<std::bitset<bit_per_byte>>
        and noexcept(std::declval<std::bitset<bit_per_byte>>()[std::declval<std::size_t>()] = std::declval<bool>());

    /*
    虽然标准库引入了类型安全的，语义更明确的std::byte来替代char和unsigned char作为单字节类型
    但是<fstream>仍然使用char类型作读写缓存，无法避免使用char类型字符串作为字节流的情况，也无法避免char、unsigned char、std::byte之间转换
    */
    template<typename result_type>
    result_type value_cast(std::byte const value)
        noexcept(safe_convertible<result_type>);

    std::byte value_cast(char const value)noexcept;
    std::byte value_cast(std::bitset<bit_per_byte> const & value)noexcept;
    std::byte value_cast(int const value)
        noexcept(safe_convertible<int, std::byte>);
    std::byte value_cast(unsigned int const & value)
        noexcept(safe_convertible<unsigned int, std::byte>);

    template<>
    char value_cast(std::byte const value)
        noexcept(safe_convertible<std::byte, char>);
    template<>
    std::bitset<bit_per_byte> value_cast(std::byte const value)
        noexcept(safe_convertible<std::byte, std::bitset<bit_per_byte>>);
    template<>
    int value_cast(std::byte const value)
        noexcept(safe_convertible<std::byte, int>);
    template<>
    unsigned int value_cast(std::byte const value)
        noexcept(safe_convertible<std::byte, unsigned int>);
}
