
#include <type_traits>

#include ".\bits.hpp"

namespace OKps
{
    char value_cast(std::byte const data)noexcept
    {
        static_assert(sizeof(std::byte) == sizeof(char));
        using middle_type = std::underlying_type<std::byte>::type;
        return static_cast<char>(static_cast<middle_type>(data));
    }
    std::byte value_cast(char const data)noexcept
    {
        static_assert(sizeof(std::byte) == sizeof(char));
        using middle_type = std::underlying_type<std::byte>::type;
        return static_cast<std::byte>(static_cast<middle_type>(data));
    }

}
