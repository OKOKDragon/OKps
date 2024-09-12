#pragma once

#include <climits>
#include <cstddef>

namespace OKps
{

    inline const constexpr std::size_t bit_per_byte = CHAR_BIT/*c标准库定义的宏*/;// 每字节有多少位

    /*
    虽然标准库引入了类型安全的，语义更明确的std::byte来替代char和unsigned char作为单字节类型
    但是<fstream>仍然使用char类型作读写缓存，无法避免使用char类型字符串作为字节流的情况，也无法避免char、unsigned char、std::byte之间转换
    */

    char value_cast(std::byte const value)noexcept;
    std::byte value_cast(char const value)noexcept;

}