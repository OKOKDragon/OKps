#include ".\value_cast.hpp"

namespace OKps
{
    template<typename target_type, copy_passing origin_type>
    typename std::enable_if_t<is_convertible<target_type, origin_type>, target_type> value_cast(origin_type const value)
        noexcept(safe_convertible<target_type, origin_type>)
    {
        static_assert(not std::is_same_v<origin_type, target_type>);
        using middle_type = std::underlying_type<std::byte>::type;
        if constexpr (std::is_same_v<origin_type, std::byte>)
        {
            if constexpr (std::is_same_v<target_type, std::bitset<bit_per_byte>>)
            {
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
            else if constexpr (std::is_same_v<target_type, char>)
            {
                static_assert(sizeof(std::byte) == sizeof(char));
                return static_cast<char>(static_cast<middle_type>(value));
            }
            else if constexpr (base::arithmetic_integer<target_type> and (not std::is_same_v<target_type, middle_type>))
            {
                base::integer<middle_type> const temp = base::integer<middle_type>(static_cast<middle_type>(value));
                base::integer<target_type> const result = temp;
                return result.value();
            }
            else
            {
                static_assert(false, "此类型转换未实现");
            }
        }
        if constexpr (std::is_same_v<target_type, std::byte>)
        {
            if constexpr (std::is_same_v<origin_type, char>)
            {
                static_assert(sizeof(std::byte) == sizeof(char));
                return static_cast<std::byte>(static_cast<middle_type>(value));
            }
            else if constexpr (base::arithmetic_integer<origin_type> and (not std::is_same_v<origin_type, middle_type>))
            {
                base::integer<origin_type> const temp = base::integer<origin_type>(value);
                base::integer<middle_type> const result = temp;
                return static_cast<std::byte>(result.value());
            }
            else
            {
                static_assert(false, "此类型转换未实现");
            }
        }
    }

    template<typename target_type, reference_passing origin_type>
    typename std::enable_if_t<is_convertible<target_type, origin_type>, target_type> value_cast(origin_type const & value)
        noexcept(safe_convertible<target_type, origin_type>)
    {
        static_assert(not std::is_same_v<origin_type, target_type>);
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
        else
        {
            static_assert(false, "此类型转换未实现");
        }
    }
}