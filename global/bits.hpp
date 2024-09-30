#pragma once

#include <climits>
#include <cstddef>

namespace OKps
{
    inline constexpr std::size_t const bit_per_byte = CHAR_BIT/*c标准库定义的宏*/;// 每字节有多少位
}