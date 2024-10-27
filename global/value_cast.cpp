#include ".\value_cast.hpp"

namespace OKps
{
    template<>
    std::byte value_cast(int const value)
        noexcept(safe_convertible<std::byte, int>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        base::integer<int> const temp = base::integer<int>(value);
        base::integer<middle_type> const result = temp;
        return static_cast<std::byte>(result.value());
    }
    template<>
    std::byte value_cast(unsigned int const value)
        noexcept(safe_convertible<std::byte, unsigned int>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        base::integer<unsigned int> const temp = base::integer<unsigned int>(value);
        base::integer<middle_type> const result = temp;
        return static_cast<std::byte>(result.value());
    }
    template<>
    char value_cast(std::byte const value)
        noexcept(safe_convertible<char, std::byte>)
    {
        static_assert(sizeof(std::byte) == sizeof(char));
        using middle_type = std::underlying_type<std::byte>::type;
        return static_cast<char>(static_cast<middle_type>(value));
    }
    template<>
    std::bitset<bit_per_byte> value_cast(std::byte const value)
        noexcept(safe_convertible<std::bitset<bit_per_byte>, std::byte>)
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
    template<>
    std::byte value_cast(char const data)
        noexcept(safe_convertible<std::byte, char>)
    {
        static_assert(sizeof(std::byte) == sizeof(char));
        using middle_type = std::underlying_type<std::byte>::type;
        return static_cast<std::byte>(static_cast<middle_type>(data));
    }

    template<>
    int value_cast(std::byte const value)
        noexcept(safe_convertible<int, std::byte>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        base::integer<middle_type> const temp = base::integer<middle_type>(static_cast<middle_type>(value));
        base::integer<int> const result = temp;
        return result.value();
    }
    template<>
    unsigned int value_cast(std::byte const value)
        noexcept(safe_convertible<unsigned int, std::byte>)
    {
        using middle_type = std::underlying_type<std::byte>::type;
        base::integer<middle_type> const temp = base::integer<middle_type>(static_cast<middle_type>(value));
        base::integer<unsigned int> const result = temp;
        return result.value();
    }

    template<typename target_type, reference_passing origin_type>
    typename std::enable_if_t<is_convertible<target_type, origin_type>, target_type> value_cast(origin_type const & value)
        noexcept(safe_convertible<target_type, origin_type>)
    {
        if constexpr (std::is_same_v<std::byte, target_type> and std::is_same_v<origin_type, std::bitset<bit_per_byte>>)
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
        else if constexpr ((std::is_same_v<std::string, origin_type> and std::is_same_v<std::vector<std::byte>, target_type>)
        or (std::is_same_v<std::string, target_type> and std::is_same_v<std::vector<std::byte>, origin_type>))
        {
            target_type result;
            result.resize(value.size());
            for (std::size_t i = 0;i < result.size();++i)
            {
                result[i] = value_cast<typename target_type::value_type>(value[i]);
            }
            return result;
        }
        else if constexpr (implement::stl_string_type<target_type> and implement::stl_string_type<origin_type>)
        {
            target_type result;
            result.resize(value.size());
            for (std::size_t i = 0; i < value.size(); ++i)
            {
                result[i] = static_cast<typename target_type::value_type>(value[i]);
            }
            return result;
        }
    }
}