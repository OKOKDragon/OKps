#include ".\byte.hpp"

namespace OKps
{
    std::byte value_cast(int const value)
        noexcept(safe_convertible<int, std::byte>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        base::integer<int> const temp = base::integer<int>(value);
        base::integer<middle_type> const result = temp;
        return static_cast<std::byte>(result.value());
    }
    std::byte value_cast(unsigned int const & value)
        noexcept(safe_convertible<unsigned int, std::byte>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        base::integer<unsigned int> const temp = base::integer<unsigned int>(value);
        base::integer<middle_type> const result = temp;
        return static_cast<std::byte>(result.value());
    }
    template<>
    char value_cast(std::byte const value)
        noexcept(safe_convertible<std::byte, char>)
    {
        static_assert(sizeof(std::byte) == sizeof(char));
        using middle_type = std::underlying_type<std::byte>::type;
        return static_cast<char>(static_cast<middle_type>(value));
    }
    template<>
    std::bitset<bit_per_byte> value_cast(std::byte const value)
        noexcept(safe_convertible<std::byte, std::bitset<bit_per_byte>>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        std::byte const one = static_cast<std::byte>(static_cast<middle_type>(1));
        std::bitset<bit_per_byte> result;
        for (std::size_t i = 0;i < bit_per_byte;++i)
        {
            if (((value & (one << i)) >> i) == one)
            {
                result[i] = true;
            }
            else
            {
                result[i] = false;
            }
        }
        return result;
    }
    std::byte value_cast(char const data)noexcept
    {
        static_assert(sizeof(std::byte) == sizeof(char));
        using middle_type = std::underlying_type<std::byte>::type;
        return static_cast<std::byte>(static_cast<middle_type>(data));
    }
    std::byte value_cast(std::bitset<bit_per_byte> const & value)noexcept
    {
        using middle_type = std::underlying_type<std::byte>::type;
        std::byte result = static_cast<std::byte>(static_cast<middle_type>(0));
        std::byte const one = static_cast<std::byte>(static_cast<middle_type>(1));
        for (std::size_t i = 0;i < bit_per_byte;++i)
        {
            if (value[i])
            {
                result |= (one << i);
            }
            else
            {
                result &= (~(one << i));
            }
        }
        return result;
    }
    template<>
    int value_cast(std::byte const value)
        noexcept(safe_convertible<std::byte, int>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        base::integer<middle_type> const temp = base::integer<middle_type>(static_cast<middle_type>(value));
        base::integer<int> const result = temp;
        return result.value();
    }
    template<>
    unsigned int value_cast(std::byte const value)
        noexcept(safe_convertible<std::byte, unsigned int>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        base::integer<middle_type> const temp = base::integer<middle_type>(static_cast<middle_type>(value));
        base::integer<unsigned int> const result = temp;
        return result.value();
    }
}
