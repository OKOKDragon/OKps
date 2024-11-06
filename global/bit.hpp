#pragma once

#include <cstddef>
#include <limits>
#include <type_traits>

namespace OKps
{
    inline constexpr std::size_t const bit_per_byte = std::numeric_limits<std::underlying_type_t<std::byte>>::digits;//每字节有多少位
    template<typename any_type>
    inline constexpr std::size_t const bit_length = sizeof(any_type) * bit_per_byte;
}