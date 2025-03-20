#include <bit>

#include ".\error.hpp"
#include ".\decimal.hpp"

namespace OKps
{
    //integer const decimal::MEMBER_base = integer(2, integer::sign_type::positive);
    decimal decimal::operator -()const
    {
        return decimal(-(this->MEMBER_mantissa), this->MEMBER_exponent);
    }
    decimal::decimal(decimal const & origin)
        :MEMBER_exponent(origin.MEMBER_exponent)
        , MEMBER_mantissa(origin.MEMBER_mantissa)
    {
    }
    void decimal::operator =(decimal const & origin)
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_exponent = origin.MEMBER_exponent;
            this->MEMBER_mantissa = origin.MEMBER_mantissa;
        }
    }
    decimal::decimal(decimal && origin)noexcept
        :MEMBER_exponent(origin.MEMBER_exponent)
        , MEMBER_mantissa(std::move(origin.MEMBER_mantissa))
    {
        origin.MEMBER_exponent = 0;
    }
    void decimal::operator =(decimal && origin)noexcept
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_exponent = origin.MEMBER_exponent;
            origin.MEMBER_exponent = 0;
            this->MEMBER_mantissa = std::move(origin.MEMBER_mantissa);
        }
    }
    decimal::decimal()
        :MEMBER_mantissa()
        , MEMBER_exponent(0)
    {
    }

    decimal const decimal::one = integer(1, integer::sign_type::positive);
    decimal const decimal::two = integer(2, integer::sign_type::positive);
    decimal const decimal::zero = integer(0, integer::sign_type::positive);
    decimal const decimal::negative_one = integer(1, integer::sign_type::negative);
    decimal const decimal::eight = integer(8, integer::sign_type::positive);
    decimal const decimal::ten = integer(10, integer::sign_type::positive);
    decimal const decimal::sixteen = integer(16, integer::sign_type::positive);
    decimal::decimal(integer const & number)
        :MEMBER_mantissa(number)
        , MEMBER_exponent(0)
    {

    }
    decimal::decimal(std::string const & input, exponent_type const exponent, number_system const system)
        :decimal()
    {
        if (input.empty())
        {
            return;
        }
        std::size_t point_position = 0;
        while (true)
        {
            if (point_position == input.size())
            {
                break;
            }
            if (input[point_position] == '.')
            {
                break;
            }
            else
            {
                ++point_position;
            }
        }
        integer TEMP_mas;
        {
            std::string TEMP_input = input;
            TEMP_input.erase(TEMP_input.begin() + point_position);
            TEMP_mas = integer(TEMP_input, system);
        }
        if (TEMP_mas.MEMBER_sign == integer::sign_type::zero)
        {
            return;
        }
        exponent_type TEMP_exponent;
        if (point_position == input.size() or point_position == input.size() - 1)
        {
            TEMP_exponent = exponent;
        }
        else
        {
            TEMP_exponent = (base::integer<exponent_type>(exponent) - (base::integer<std::size_t>(input.size() - point_position - 1))).value();
        }
        this->MEMBER_mantissa = std::move(TEMP_mas);
        this->INNER_erase_post_zero();
        if (TEMP_exponent == 0)
        {
            return;
        }
        decimal const * input_base = nullptr;
        switch (system)
        {
            case number_system::bin:
            {
                input_base = std::addressof(decimal::two);
                break;
            }
            case number_system::dec:
            {
                input_base = std::addressof(decimal::ten);
                break;
            }
            case number_system::oct:
            {
                input_base = std::addressof(decimal::eight);
                break;
            }
            case number_system::hex:
            {
                input_base = std::addressof(decimal::sixteen);
                break;
            }
        }
        if (TEMP_exponent > 0)
        {
            for (exponent_type i = 0;i < TEMP_exponent;++i)
            {
                (*this) *= (*input_base);
            }
        }
        else
        {
            for (exponent_type i = 0;i < (-TEMP_exponent);++i)
            {
                (*this) /= (*input_base);
            }
        }
    }
    decimal::~decimal()noexcept
    {
    }
    void decimal::operator /=(decimal const & right)
    {
        (*this) = (*this) / right;
    }
    decimal decimal::operator +(decimal const & right)const
    {
        decimal result;
        if (this->MEMBER_exponent > right.MEMBER_exponent)
        {
            result = (*this);
            decimal::INNER_align_point(result, right);
            result.MEMBER_mantissa += right.MEMBER_mantissa;
        }
        else if (this->MEMBER_exponent < right.MEMBER_exponent)
        {
            result = right;
            decimal::INNER_align_point(result, *this);
            result.MEMBER_mantissa += this->MEMBER_mantissa;
        }
        else
        {
            result.MEMBER_mantissa = this->MEMBER_mantissa + right.MEMBER_mantissa;
            result.MEMBER_exponent = this->MEMBER_exponent;
        }
        result.INNER_erase_post_zero();
        return result;
    }
    void decimal::operator +=(decimal const & right)
    {
        if (this->MEMBER_exponent > right.MEMBER_exponent)
        {
            decimal::INNER_align_point(*this, right);
            this->MEMBER_mantissa += right.MEMBER_mantissa;
            this->INNER_erase_post_zero();
        }
        else if (this->MEMBER_exponent < right.MEMBER_exponent)
        {
            decimal result = right;
            decimal::INNER_align_point(result, *this);
            result.MEMBER_mantissa += this->MEMBER_mantissa;
            result.INNER_erase_post_zero();
            (*this) = std::move(result);
        }
        else
        {
            this->MEMBER_mantissa += right.MEMBER_mantissa;
            this->INNER_erase_post_zero();
        }
    }
    decimal decimal::operator -(decimal const & right)const
    {
        decimal result;
        if (this->MEMBER_exponent > right.MEMBER_exponent)
        {
            result = (*this);
            decimal::INNER_align_point(result, right);
            result.MEMBER_mantissa -= right.MEMBER_mantissa;
        }
        else if (this->MEMBER_exponent < right.MEMBER_exponent)
        {
            result = right;
            decimal::INNER_align_point(result, *this);
            result.MEMBER_mantissa = this->MEMBER_mantissa - result.MEMBER_mantissa;
        }
        else
        {
            result.MEMBER_mantissa = this->MEMBER_mantissa - right.MEMBER_mantissa;
            result.MEMBER_exponent = this->MEMBER_exponent;
        }
        result.INNER_erase_post_zero();
        return result;
    }
    void decimal::operator -=(decimal const & right)
    {
        if (this->MEMBER_exponent > right.MEMBER_exponent)
        {
            decimal::INNER_align_point(*this, right);
            this->MEMBER_mantissa -= right.MEMBER_mantissa;
            this->INNER_erase_post_zero();
        }
        else if (this->MEMBER_exponent < right.MEMBER_exponent)
        {
            decimal result = right;
            decimal::INNER_align_point(result, *this);
            result.MEMBER_mantissa = this->MEMBER_mantissa - result.MEMBER_mantissa;
            result.INNER_erase_post_zero();
            (*this) = std::move(result);
        }
        else
        {
            this->MEMBER_mantissa -= right.MEMBER_mantissa;
            this->INNER_erase_post_zero();
        }
    }
    decimal decimal::operator *(decimal const & right)const
    {
        decimal result;
        auto const TEMP_mas = this->MEMBER_mantissa * right.MEMBER_mantissa;
        if (TEMP_mas.MEMBER_sign != integer::sign_type::zero)
        {
            auto const TEMP_exp = (base::integer<exponent_type>(this->MEMBER_exponent) + right.MEMBER_exponent).value();
            result = decimal(std::move(TEMP_mas), TEMP_exp);
            result.INNER_erase_post_zero();
        }
        return result;
    }
    void decimal::operator *=(decimal const & right)
    {
        (*this) = (*this) * right;
    }
    decimal decimal::operator /(decimal const & right)const
    {

        if (right.MEMBER_mantissa == integer::zero)
        {
            throw error(std::logic_error("除数不能为0"));
        }
        if (this->MEMBER_mantissa == integer::zero)
        {
            return (*this);
        }
        decimal result = (*this);
        std::size_t left_length = result.MEMBER_mantissa.MEMBER_number.size();
        if (left_length < 4)
        {
            result.MEMBER_mantissa.MEMBER_number.resize(4, 0);
            result.MEMBER_exponent = (base::integer<exponent_type>(result.MEMBER_exponent) - (4 - left_length)).value();
            left_length = 4;
        }
        base::integer<std::size_t> TEMP_bigger_length;

        std::size_t const right_length = right.MEMBER_mantissa.MEMBER_number.size();
        if (left_length > right_length)
        {
            TEMP_bigger_length = left_length;
        }
        else
        {
            TEMP_bigger_length = right_length;
        }
        result.MEMBER_mantissa.MEMBER_number.resize((TEMP_bigger_length + 1 + right_length).value(), 0);
        result.MEMBER_exponent = (base::integer<exponent_type>(result.MEMBER_exponent) - (TEMP_bigger_length - left_length + right_length) - right.MEMBER_exponent).value();
        result.MEMBER_mantissa /= right.MEMBER_mantissa;
        {
            auto const TEMP_length = result.MEMBER_mantissa.MEMBER_number.size();
            bool const FLAG_debug = false;
            if (FLAG_debug and TEMP_length == 0)
            {
                throw error(std::logic_error("实现错误"));
            }
            if (TEMP_length > (TEMP_bigger_length + 1).value())
            {
                if (result.MEMBER_mantissa.MEMBER_number[TEMP_length - 2] >= integer::half_base)
                {
                    integer TEMP_carry = integer(integer::base, integer::sign_type::positive);
                    TEMP_carry.MEMBER_number.push_back(0);
                    result.MEMBER_mantissa += TEMP_carry;
                }
                result.MEMBER_mantissa.MEMBER_number.pop_back();
                result.MEMBER_mantissa.MEMBER_number.pop_back();
                ++result.MEMBER_exponent;
            }
            else
            {
                if (result.MEMBER_mantissa.MEMBER_number[TEMP_length - 1] >= integer::half_base)
                {
                    result.MEMBER_mantissa += integer(integer::base, integer::sign_type::positive);
                }
                result.MEMBER_mantissa.MEMBER_number.pop_back();
            }
        }
        result.INNER_erase_post_zero();
        return result;
    }
    void decimal::INNER_erase_post_zero()
    {
        std::size_t const TEMP_length_before = this->MEMBER_mantissa.MEMBER_number.size();
        std::size_t TEMP_position = TEMP_length_before;
        bool FLAG_found = false;
        while (TEMP_position > 0)
        {
            --TEMP_position;
            if (this->MEMBER_mantissa.MEMBER_number[TEMP_position] != 0)
            {
                FLAG_found = true;
                break;
            }
        }
        if (not FLAG_found)
        {
            (*this) = decimal();
        }
        else
        {
            this->MEMBER_exponent = (base::integer<exponent_type>(this->MEMBER_exponent) + base::integer<std::size_t>(TEMP_length_before - TEMP_position - 1)).value();
            this->MEMBER_mantissa.MEMBER_number.resize(TEMP_position + 1);
        }
    }
    void decimal::INNER_align_point(decimal & longer, decimal const & shorter)
    {
        auto const shift_length = static_cast<base::integer<std::size_t>>(base::integer<exponent_type>(longer.MEMBER_exponent) - shorter.MEMBER_exponent);
        longer.MEMBER_mantissa.MEMBER_number.resize((shift_length + longer.MEMBER_mantissa.MEMBER_number.size()).value(), 0);
        longer.MEMBER_exponent = shorter.MEMBER_exponent;
    }

    decimal::decimal(integer const & mantissa, exponent_type const exponent)
        :MEMBER_exponent(exponent)
        , MEMBER_mantissa(mantissa)
    {
    }
    decimal::decimal(integer && mantissa, exponent_type const exponent)
        :MEMBER_exponent(exponent)
        , MEMBER_mantissa(std::move(mantissa))
    {
    }

    integer::holder_type const & decimal::base = integer::base;
    integer decimal::integer_part()const
    {
        integer result;
        if (this->MEMBER_exponent == 0)
        {
            result = this->MEMBER_mantissa;
        }
        else if (this->MEMBER_exponent > 0)
        {
            result = this->MEMBER_mantissa;
            result.MEMBER_number.resize((base::integer<std::size_t>(result.MEMBER_number.size()) + base::integer<exponent_type>(this->MEMBER_exponent)).value(), 0);
        }
        else
        {
            auto const exponent_positive = static_cast<base::integer<std::size_t>>(-base::integer<exponent_type>(this->MEMBER_exponent)).value();
            auto const TEMP_length = this->MEMBER_mantissa.MEMBER_number.size();
            if (exponent_positive >= TEMP_length)
            {
            }
            else
            {
                result = this->MEMBER_mantissa;
                result.MEMBER_number.resize(TEMP_length - exponent_positive);
            }
        }
        return result;
    }
    std::string decimal::string(number_system const system)const
    {

        if (this->MEMBER_mantissa.MEMBER_sign == integer::sign_type::zero)
        {
            return "0";
        }
        exponent_type result_valid_numbers = 0;
        exponent_type TEMP_result_exponent = 0;
        {
            integer valid_numbers;
            valid_numbers.MEMBER_number.resize(this->MEMBER_mantissa.MEMBER_number.size() + 1, 0);
            valid_numbers.MEMBER_number[0] = 1;
            valid_numbers.MEMBER_sign = integer::sign_type::positive;
            integer const * input_base = nullptr;
            switch (system)
            {
                case number_system::bin:
                {
                    input_base = std::addressof(integer::two);
                    break;
                }
                case number_system::dec:
                {
                    input_base = std::addressof(integer::ten);
                    break;
                }
                case number_system::oct:
                {
                    input_base = std::addressof(integer::eight);
                    break;
                }
                case number_system::hex:
                {
                    input_base = std::addressof(integer::sixteen);
                    break;
                }
            }
            while (valid_numbers.MEMBER_sign != integer::sign_type::zero)
            {
                ++result_valid_numbers;
                valid_numbers /= (*input_base);
            }
            --result_valid_numbers;
        }
        decimal TEMP_value_holder = (*this);
        std::string result;
        decimal const * input_base = nullptr;
        switch (system)
        {
            case number_system::bin:
            {
                input_base = std::addressof(decimal::two);
                break;
            }
            case number_system::dec:
            {
                input_base = std::addressof(decimal::ten);
                break;
            }
            case number_system::oct:
            {
                input_base = std::addressof(decimal::eight);
                break;
            }
            case number_system::hex:
            {
                input_base = std::addressof(decimal::sixteen);
                break;
            }
        }
        integer result_integer_part = TEMP_value_holder.integer_part();
        std::string result_integer_part_string = result_integer_part.string(system);
        bool FLAG_done_integer_part = false;
        while (not FLAG_done_integer_part)
        {
            if (result_integer_part_string == "0")
            {
                TEMP_value_holder *= (*input_base);
                result_integer_part = TEMP_value_holder.integer_part();
                result_integer_part_string = result_integer_part.string(system);
                --TEMP_result_exponent;
            }
            else
            {
                auto const TEMP_length = result_integer_part_string.size();
                if (TEMP_length == 1)
                {
                    FLAG_done_integer_part = true;
                }
                else if (TEMP_length == 2 and result_integer_part_string[0] == '-')
                {
                    FLAG_done_integer_part = true;
                }
                else
                {
                    TEMP_value_holder /= (*input_base);
                    result_integer_part = TEMP_value_holder.integer_part();
                    result_integer_part_string = result_integer_part.string(system);
                    ++TEMP_result_exponent;
                }
            }
        }
        for (exponent_type i = 0;i < result_valid_numbers;++i)
        {
            TEMP_value_holder *= (*input_base);
        }
        result = TEMP_value_holder.integer_part().string(system);
        {
            auto TEMP_position = result.begin();
            if (*TEMP_position == '-')
            {
                TEMP_position += 2;
            }
            else
            {
                TEMP_position += 1;
            }
            if (TEMP_position != result.end())
            {
                result.insert(TEMP_position, '.');
            }
        }
        result += " * ";
        result += std::to_string(static_cast<std::underlying_type_t<number_system>>(system));
        result += " ^ ";
        result += std::to_string(TEMP_result_exponent);
        return result;
    }
}