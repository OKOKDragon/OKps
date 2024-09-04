#include <stdexcept>

#include ".\bits.hpp"

namespace OKps
{
    char value_cast(std::byte const data)noexcept
    {
        static_assert(sizeof(std::byte) == sizeof(char));
        using middle_type = std::underlying_type<std::byte>::type;
        return (char)((middle_type)data);
    }
    std::byte value_cast(char const data)noexcept
    {
        static_assert(sizeof(std::byte) == sizeof(char));
        using middle_type = std::underlying_type<std::byte>::type;
        return (std::byte)((middle_type)data);
    }
    byte::~byte()noexcept
    {
    }
    byte byte::operator &(byte const & right) noexcept
    {
        return this->data & right.data;
    }
    byte byte::operator |(byte const & right) noexcept
    {
        return this->data | right.data;
    }
    byte byte::operator ^(byte const & right) noexcept
    {
        return this->data ^ right.data;
    }
    byte byte::operator ~() noexcept
    {
        return ~(this->data);
    }
    byte byte::operator <<(std::size_t const shift)
    {
        if (shift >= bit_per_byte)
        {
            throw std::out_of_range("二进制位移的位数超出范围");
        }
        return this->data << shift;
    }
    byte byte::operator >>(std::size_t const shift)
    {
        if (shift >= bit_per_byte)
        {
            throw std::out_of_range("二进制位移的位数超出范围");
        }
        return this->data >> shift;
    }

    void byte::operator &=(byte const & right) noexcept
    {
        this->data &= right.data;
    }
    void byte::operator |=(byte const & right) noexcept
    {
        this->data |= right.data;
    }
    void byte::operator ^=(byte const & right) noexcept
    {
        this->data ^= right.data;
    }
    void byte::operator <<=(std::size_t const shift)
    {
        if (shift >= bit_per_byte)
        {
            throw std::out_of_range("二进制位移的位数超出范围");
        }
        this->data <<= shift;
    }
    void byte::operator >>=(std::size_t const shift)
    {
        {
            if (shift >= bit_per_byte)
            {
                throw std::out_of_range("二进制位移的位数超出范围");
            }
            this->data >>= shift;
        }
    }
    bool byte::INNER_at(std::size_t const & position)const noexcept
    {
        return (bool)((this->data >> position) & (std::byte)1);
    }
    byte::reference byte::INNER_at(std::size_t const & position)noexcept
    {
        return reference(*this, position);
    }
    byte::byte(unsigned long long const & input)
    {

        if (input > std::numeric_limits<std::underlying_type_t<std::byte>>::max())
        {
            throw std::invalid_argument("输入的数超出1个字节的表示范围");
            return;
        }
        this->data = (std::byte)((std::underlying_type_t<std::byte>)input);
    }
    byte::byte(std::byte const & input)noexcept
        :data(input)
    {
    }
    byte::operator std::byte()const noexcept
    {
        return this->data;
    }
    byte::reference::reference(byte & owner, std::size_t const & pos)noexcept
        :position(pos), data(owner)
    {
    }
    byte::reference::reference(reference const & origin)noexcept
        :position(origin.position), data(origin.data)
    {
    }
    void byte::reference::operator =(reference const & origin)noexcept
    {
        this->position = origin.position;
        this->data = origin.data;
    }
    byte::reference::operator bool()const noexcept
    {
        auto const & temp = this->data;
        return temp.INNER_at(this->position);
    }
    void byte::reference::operator =(bool const value)noexcept
    {
        if (value)
        {
            this->data.data |= ((std::byte)1 << this->position);
        }
        else
        {
            this->data.data &= ~((std::byte)1 << this->position);
        }
    }
    void byte::reference::operator =(char const value)
    {
        switch (value)
        {
            case '0':
            {
                (*this) = false;
                break;
            }
            case '1':
            {
                (*this) = true;
                break;
            }
            default:
            {
                throw std::invalid_argument("输入的字符不能表示二进制位");
                break;
            }
        }
    }
    void byte::reference::operator =(wchar_t const value)
    {
        switch (value)
        {
            case L'0':
            {
                (*this) = false;
                break;
            }
            case L'1':
            {
                (*this) = true;
                break;
            }
            default:
            {
                throw std::invalid_argument("输入的字符不能表示二进制位");
                break;
            }
        }
    }
    void byte::reference::operator =(char8_t const value)
    {
        switch (value)
        {
            case u8'0':
            {
                (*this) = false;
                break;
            }
            case u8'1':
            {
                (*this) = true;
                break;
            }
            default:
            {
                throw std::invalid_argument("输入的字符不能表示二进制位");
                break;
            }
        }
    }
    void byte::reference::operator =(char16_t const value)
    {
        switch (value)
        {
            case u'0':
            {
                (*this) = false;
                break;
            }
            case u'1':
            {
                (*this) = true;
                break;
            }
            default:
            {
                throw std::invalid_argument("输入的字符不能表示二进制位");
                break;
            }
        }
    }
    void byte::reference::operator =(char32_t const value)
    {
        switch (value)
        {
            case U'0':
            {
                (*this) = false;
                break;
            }
            case U'1':
            {
                (*this) = true;
                break;
            }
            default:
            {
                throw std::invalid_argument("输入的字符不能表示二进制位");
                break;
            }
        }
    }
    byte::reference::~reference()noexcept
    {
    }
    byte::operator std::string()const noexcept
    {
        std::string result;
        result.resize(bit_per_byte);
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = this->INNER_at(count);
            switch (temp)
            {
                case false:
                {
                    result[bit_per_byte - 1 - count] = '0';
                    break;
                }
                case true:
                {
                    result[bit_per_byte - 1 - count] = '1';
                    break;
                }
            }
        }
        return result;
    }
    byte::operator std::wstring()const noexcept
    {
        std::wstring result;
        result.resize(bit_per_byte);
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = this->INNER_at(count);
            switch (temp)
            {
                case false:
                {
                    result[bit_per_byte - 1 - count] = L'0';
                    break;
                }
                case true:
                {
                    result[bit_per_byte - 1 - count] = L'1';
                    break;
                }
            }
        }
        return result;
    }
    byte::operator std::u8string()const noexcept
    {
        std::u8string result;
        result.resize(bit_per_byte);
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = this->INNER_at(count);
            switch (temp)
            {
                case false:
                {
                    result[bit_per_byte - 1 - count] = u8'0';
                    break;
                }
                case true:
                {
                    result[bit_per_byte - 1 - count] = u8'1';
                    break;
                }
            }
        }
        return result;
    }
    byte::operator std::u16string()const noexcept
    {
        std::u16string result;
        result.resize(bit_per_byte);
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = this->INNER_at(count);
            switch (temp)
            {
                case false:
                {
                    result[bit_per_byte - 1 - count] = u'0';
                    break;
                }
                case true:
                {
                    result[bit_per_byte - 1 - count] = u'1';
                    break;
                }
            }
        }
        return result;
    }
    byte::operator std::u32string()const noexcept
    {
        std::u32string result;
        result.resize(bit_per_byte);
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = this->INNER_at(count);
            switch (temp)
            {
                case false:
                {
                    result[bit_per_byte - 1 - count] = U'0';
                    break;
                }
                case true:
                {
                    result[bit_per_byte - 1 - count] = U'1';
                    break;
                }
            }
        }
        return result;
    }
    bool byte::operator ==(std::byte const & right)const noexcept
    {
        return this->data == right;
    }
    bool byte::operator !=(std::byte const & right)const noexcept
    {
        return this->data != right;
    }
    byte::byte(std::string const & input)
    {
        if (input.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = input[bit_per_byte - 1 - count];
        }
    }
    byte::byte(std::u8string const & input)
    {
        if (input.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = input[bit_per_byte - 1 - count];
        }
    }
    byte::byte(std::u16string const & input)
    {
        if (input.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = input[bit_per_byte - 1 - count];
        }
    }
    byte::byte(std::u32string const & input)
    {
        if (input.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = input[bit_per_byte - 1 - count];
        }
    }
    byte::byte(std::wstring const & input)
    {
        if (input.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = input[bit_per_byte - 1 - count];
        }
    }
    void byte::operator =(std::string const & origin)
    {
        if (origin.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = origin[bit_per_byte - 1 - count];
        }
    }
    bool byte::operator ==(std::string const & right)const
    {
        return (*this) == byte(right);
    }
    bool byte::operator !=(std::string const & right)const
    {
        return (*this) != byte(right);
    }
    byte::byte(byte const & origin)noexcept
        :data(origin.data)
    {
    }
    void byte::operator =(byte const & origin)noexcept
    {
        this->data = origin.data;
    }
    bool byte::operator ==(byte const & right)const noexcept
    {
        return (this->data == right.data);
    }
    bool byte::operator !=(byte const & right)const noexcept
    {
        return (this->data != right.data);
    }
    bool byte::operator [](std::size_t const position)const
    {
        if (position >= bit_per_byte)
        {
            throw std::out_of_range("输入的下标超出范围");
        }
        return this->INNER_at(position);

    }
    byte::reference byte::operator [](std::size_t const position)
    {
        if (position >= bit_per_byte)
        {
            throw std::out_of_range("输入的下标超出范围");
        }
        return reference(*this, position);
    }
    void byte::operator =(std::u16string const & origin)
    {
        if (origin.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = origin[bit_per_byte - 1 - count];
        }
    }
    bool byte::operator ==(std::u16string const & right)const
    {
        return (*this) == byte(right);
    }
    bool byte::operator !=(std::u16string const & right)const
    {
        return (*this) != byte(right);
    }
    
    void byte::operator =(std::u8string const & origin)
    {
        if (origin.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = origin[bit_per_byte - 1 - count];
        }
    }
    bool byte::operator ==(std::u8string const & right)const
    {
        return (*this) == byte(right);
    }
    bool byte::operator !=(std::u8string const & right)const
    {
        return (*this) != byte(right);
    }
    void byte::operator =(std::u32string const & origin)
    {
        if (origin.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = origin[bit_per_byte - 1 - count];
        }
    }
    bool byte::operator ==(std::u32string const & right)const
    {
        return (*this) == byte(right);
    }
    bool byte::operator !=(std::u32string const & right)const
    {
        return (*this) != byte(right);
    }
    void byte::operator =(std::wstring const & origin)
    {
        if (origin.size() != bit_per_byte)
        {
            throw std::invalid_argument("输入的字符串长度不是1字节能表示的二进制位数");
            return;
        }
        for (std::size_t count = 0; count < bit_per_byte; count++)
        {
            auto temp = reference(*this, count);
            temp = origin[bit_per_byte - 1 - count];
        }
    }
    bool byte::operator ==(std::wstring const & right)const
    {
        return (*this) == byte(right);
    }
    bool byte::operator !=(std::wstring const & right)const
    {
        return (*this) != byte(right);
    }
    void byte::operator =(std::byte const & origin)noexcept
    {
        this->data = origin;
    }

 

}
