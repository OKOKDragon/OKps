﻿#include <thread>
#include <random>
#include <fstream>

#include ".\integer.hpp"
#include ".\string.hpp"

namespace OKps
{
    integer::integer()
        :MEMBER_sign(sign_type::zero)
    {
    }
    integer::~integer()noexcept
    {
    }
    integer::integer(number_type && number, sign_type sign)noexcept
        :MEMBER_sign(sign)
        , MEMBER_number(std::move(number))
    {
        if (sign == sign_type::zero)
        {
            this->MEMBER_number.clear();
            return;
        }
        INNER_erase_leading_zero(this->MEMBER_number);
        if (this->MEMBER_number.size() == 0)
        {
            this->MEMBER_sign = sign_type::zero;
        }
    }
    integer::integer(number_type const & number, sign_type sign)
        :MEMBER_sign(sign)
        , MEMBER_number(number)
    {
        if (sign == sign_type::zero)
        {
            this->MEMBER_number.clear();
            return;
        }
        INNER_erase_leading_zero(this->MEMBER_number);
        if (this->MEMBER_number.size() == 0)
        {
            this->MEMBER_sign = sign_type::zero;
        }
    }
    integer::INNER_divide_result::~INNER_divide_result()noexcept
    {
    }
    integer::divide_result::divide_result(integer && divide, integer && mod)noexcept
        :MEMBER_divide(std::make_unique<integer>(std::move(divide)))
        , MEMBER_module(std::make_unique<integer>(std::move(mod)))
    {
    }
    integer::divide_result::divide_result(integer const & divide, integer const & mod)
        :MEMBER_divide(std::make_unique<integer>(divide))
        , MEMBER_module(std::make_unique<integer>(mod))
    {
    }
    integer::divide_result integer::divide_and_module(integer const & right)const
    {
        auto result = INNER_divide_and_module(*this, right);
        return integer::divide_result(std::move(result.first), std::move(result.second));
    }
    integer const & integer::divide_result::divide()const noexcept
    {
        return *(this->MEMBER_divide);
    }
    integer const & integer::divide_result::mod()const noexcept
    {
        return *(this->MEMBER_module);
    }
    integer::divide_result::divide_result(divide_result && origin)noexcept
        :MEMBER_divide(std::move(origin.MEMBER_divide))
        , MEMBER_module(std::move(origin.MEMBER_module))
    {
    }
    integer::divide_result::divide_result(divide_result const & origin)
        :MEMBER_divide(std::make_unique<integer>(*(origin.MEMBER_divide)))
        , MEMBER_module(std::make_unique<integer>(*(origin.MEMBER_module)))
    {
    }
    integer::number_type integer::INNER_add_number(number_type const & left, number_type const & right)
    {
        if (left.size() == 0)
        {
            return right;
        }
        else if (right.size() == 0)
        {
            return left;
        }
        number_type const * REF_long;
        number_type const * REF_short;

        // 若left数组长度短于right数组, 则交换两个数组
        if (left.size() < right.size())
        {
            REF_long = &right;
            REF_short = &left;
        }
        else
        {
            REF_long = &left;
            REF_short = &right;
        }
        auto xIndex = REF_long->size();
        auto yIndex = REF_short->size();
        number_type result;
        result.resize(xIndex);
        TYPE_full_number sum = 0;
        /*
        将半长数转为全长数再相加并处理进位，进位保存在sum的高位。
        */

            //将公共部分相加
        while (yIndex > 0)
        {
            xIndex--;
            yIndex--;
            sum = (TYPE_full_number)((*REF_long)[xIndex]) + (TYPE_full_number)((*REF_short)[yIndex]) + (sum >> half_length);
            result[xIndex] = (value_type)sum;
        }

        //复制较长的数，并传播进位
        while (xIndex > 0)
        {
            xIndex--;
            sum = (TYPE_full_number)((*REF_long)[xIndex]) + (sum >> half_length);
            result[xIndex] = (value_type)sum;
        }

        //必要时扩充数组
        if ((value_type)(sum >> half_length) != 0)
        {
            result.insert(result.begin(), (value_type)(sum >> half_length));
        }
        return result;
    }

    integer::number_type integer::INNER_subtract_number(number_type const & big, number_type const & little)
    {
        auto bigIndex = big.size();
        number_type result;
        result.resize(bigIndex);
        auto littleIndex = little.size();
        bool carry = false;//是否有借位

        // 公共部分相减
        while (littleIndex > 0)
        {
            bool have_carry = false;//本次运算是否需要向高位借位
            bigIndex--;
            littleIndex--;
            TYPE_full_number TEMP_big;
            if (carry)
            {
                if ((TYPE_full_number)(big[bigIndex]) == 0)
                {
                    TEMP_big = base - (TYPE_full_number)1;
                    have_carry = true;
                }
                else
                {
                    TEMP_big = (TYPE_full_number)(big[bigIndex]) - (TYPE_full_number)1;
                }
            }
            else
            {
                TEMP_big = (TYPE_full_number)(big[bigIndex]);
            }
            if (TEMP_big < (TYPE_full_number)(little[littleIndex]))
            {
                have_carry = true;
                result[bigIndex] = base - (TYPE_full_number)(little[littleIndex]) + TEMP_big;
            }
            else
            {
                result[bigIndex] = TEMP_big - (TYPE_full_number)(little[littleIndex]);
            }
            carry = have_carry;
        }

        // 复制较长的数，同时传播借位
        while (bigIndex > 0)
        {
            bigIndex--;
            bool have_carry = false;//本次运算是否需要向高位借位
            TYPE_full_number TEMP_big;
            if (carry)
            {
                if ((TYPE_full_number)(big[bigIndex]) == 0)
                {
                    TEMP_big = base - (TYPE_full_number)1;
                    have_carry = true;
                }
                else
                {
                    TEMP_big = (TYPE_full_number)(big[bigIndex]) - (TYPE_full_number)1;
                }
            }
            else
            {
                TEMP_big = (TYPE_full_number)(big[bigIndex]);
            }
            result[bigIndex] = TEMP_big;
            carry = have_carry;
        }
        INNER_erase_leading_zero(result);
        return result;
    }
    integer::sign_type const & integer::sign()const noexcept
    {
        return this->MEMBER_sign;
    }
    integer::number_type const & integer::number()const noexcept
    {
        return this->MEMBER_number;
    }
    integer::compare_result integer::INNER_compare_number(integer const & right)const
    {
        //先比较数组长度
        if (this->MEMBER_number.size() < right.MEMBER_number.size())
        {
            return compare_result::smaller;
        }
        if (this->MEMBER_number.size() > right.MEMBER_number.size())
        {
            return compare_result::bigger;
        }
        for (decltype(this->MEMBER_number.size()) i = 0; i < this->MEMBER_number.size(); i++)
        {//从高位依次比较
            if (this->MEMBER_number[i] < right.MEMBER_number[i])
            {
                return compare_result::smaller;
            }
            else if (this->MEMBER_number[i] > right.MEMBER_number[i])
            {
                return compare_result::bigger;
            }
        }
        return compare_result::same;
    }

    void integer::INNER_erase_leading_zero(number_type & val)noexcept
    {
        auto itor = val.begin();
        while (itor != val.end() and *itor == 0)
        {
            ++itor;
        }
        val.erase(val.begin(), itor);
    }

    field_stream integer::field_stream()const
    {
        std::string result;
        for (std::size_t count_1 = 0;count_1 < sizeof(sign_type) / sizeof(char);count_1++)
        {
            result.push_back(((char const *)(&this->MEMBER_sign))[count_1]);
        }
        for (std::size_t count_1 = 0;count_1 < this->MEMBER_number.size();count_1++)
        {
            auto const temp_number = this->MEMBER_number[count_1];
            for (std::size_t count_2 = 0;count_2 < sizeof(decltype(temp_number)) / sizeof(char);count_2++)
            {
                result.push_back(((char const *)(&temp_number))[count_2]);
            }
        }
        return result;
    }

    integer::integer(OKps::field_stream const & input)
    {
        auto const temp_input = input.raw_string();
        std::size_t position = 0;
        for (std::size_t count_1 = 0;count_1 < sizeof(sign_type) / sizeof(char);count_1++)
        {
            if (position >= temp_input.size())
            {
                throw std::invalid_argument("输入的流格式错误");
            }
            ((char *)(&this->MEMBER_sign))[count_1] = temp_input[position];
            position++;
        }
        while (true)
        {
            if (position >= temp_input.size())
            {
                break;
            }
            value_type temp_number;
            for (std::size_t count_2 = 0;count_2 < sizeof(value_type) / sizeof(char);count_2++)
            {
                if (position >= temp_input.size())
                {
                    throw std::invalid_argument("输入的流格式错误");
                }
                ((char *)(&temp_number))[count_2] = temp_input[position];
                position++;
            }
            this->MEMBER_number.push_back(temp_number);
        }
    }
    integer::integer(integer const & origin)
        :MEMBER_sign(origin.MEMBER_sign)
        , MEMBER_number(origin.MEMBER_number)
    {
    }
    void integer::operator =(integer const & origin)
    {
        this->MEMBER_number = origin.MEMBER_number;//std::vector的复制赋值操作符没有声明为noexcept，所以我们也无法声明noexcept
        this->MEMBER_sign = origin.MEMBER_sign;
    }
    integer integer::operator +(integer const & right)const
    {
        if (right.MEMBER_sign == sign_type::zero)
        {
            return *this;
        }
        if (this->MEMBER_sign == sign_type::zero)
        {
            return right;
        }
        if (right.MEMBER_sign == this->MEMBER_sign)//同号相加
        {
            return integer(INNER_add_number(this->MEMBER_number, right.MEMBER_number), this->MEMBER_sign);
        }
        //异号相减
        auto const cmp = this->INNER_compare_number(right);
        if (cmp == compare_result::same)
        {
            return integer();//返回0
        }
        number_type resultMag;
        //用较大的数减较小的数
        if (cmp == compare_result::bigger)
        {
            resultMag = INNER_subtract_number(this->MEMBER_number, right.MEMBER_number);
        }
        else
        {
            resultMag = INNER_subtract_number(right.MEMBER_number, this->MEMBER_number);
        }

        if ((cmp == compare_result::bigger and this->MEMBER_sign == sign_type::positive)
        or (cmp == compare_result::smaller and this->MEMBER_sign == sign_type::negative))
        {
            return integer(resultMag, sign_type::positive);
        }
        else
        {
            return integer(resultMag, sign_type::negative);
        }
    }
    integer integer::operator - ()const
    {
        if (this->MEMBER_sign == sign_type::zero)
        {
            return integer();
        }
        if (this->MEMBER_sign == sign_type::positive)
        {
            return integer(this->MEMBER_number, sign_type::negative);
        }
        else
        {
            return integer(this->MEMBER_number, sign_type::positive);
        }
    }
    integer::integer(integer && origin)noexcept
        :MEMBER_sign(origin.MEMBER_sign)
        , MEMBER_number(std::move(origin.MEMBER_number))
    {
        origin.MEMBER_sign = sign_type::zero;
    }
    void  integer::operator =(integer && origin)noexcept
    {
        this->MEMBER_sign = origin.MEMBER_sign;
        this->MEMBER_number = std::move(origin.MEMBER_number);
        origin.MEMBER_sign = sign_type::zero;
    }
    integer integer::operator -(integer const & right)const
    {
        if (right.MEMBER_sign == sign_type::zero)
        {
            return *this;
        }
        if (this->MEMBER_sign == sign_type::zero)
        {
            return -right;
        }
        if (right.MEMBER_sign != this->MEMBER_sign)//异号则加
        {
            return integer(INNER_add_number(this->MEMBER_number, right.MEMBER_number), this->MEMBER_sign);
        }
        //同号则减
        auto const cmp = this->INNER_compare_number(right);
        if (cmp == compare_result::same)
        {
            return integer();
        }
        number_type resultMag;
        if (cmp == compare_result::bigger)
        {
            resultMag = INNER_subtract_number(this->MEMBER_number, right.MEMBER_number);
        }
        else
        {
            resultMag = INNER_subtract_number(right.MEMBER_number, this->MEMBER_number);
        };

        if ((cmp == compare_result::bigger and this->MEMBER_sign == sign_type::positive)
        or (cmp == compare_result::smaller and this->MEMBER_sign == sign_type::negative))
        {
            return integer(resultMag, sign_type::positive);
        }
        else
        {
            return integer(resultMag, sign_type::negative);
        }
    }

    bool integer::operator ==(integer const & right)const
    {
        if (this->MEMBER_sign != right.MEMBER_sign)
        {
            return false;
        }
        if (this->MEMBER_number.size() != right.MEMBER_number.size())
        {
            return false;
        }
        for (decltype(this->MEMBER_number.size()) count = 0;count < this->MEMBER_number.size();count++)
        {
            if (this->MEMBER_number[count] != right.MEMBER_number[count])
            {
                return false;
            }
        }
        return true;
    }
    bool integer::operator !=(integer const & right)const
    {
        return not((*this) == right);
    }
    integer integer::operator *(integer const & right)const
    {
        if (this->MEMBER_sign == sign_type::zero or right.MEMBER_sign == sign_type::zero)
        {
            return integer();
        }
        if (this->MEMBER_sign == right.MEMBER_sign)
        {
            return integer(INNER_multiply_number(this->MEMBER_number, right.MEMBER_number), sign_type::positive);
        }
        else
        {
            return integer(INNER_multiply_number(this->MEMBER_number, right.MEMBER_number), sign_type::negative);
        }
    }
    integer::number_type integer::INNER_multiply_number(number_type const & left, number_type const & right)
    {
        /*
        java标准库的BigInteger类会根据大数的长度选择不同的乘法实现
        但其中的Toom Cook算法等算法都是递归的，很容易造成栈溢出
        我们目前选择只使用最简单的模拟竖式方法
        */
        return INNER_simple_multiply_number(left, right);
    }
    integer::number_type integer::INNER_simple_multiply_number(number_type const & left, number_type const & right)
    {
        number_type const * REF_long;
        number_type const * REF_short;
        if (left.size() < right.size())
        {
            REF_short = &left;
            REF_long = &right;
        }
        else
        {
            REF_short = &right;
            REF_long = &left;
        }
        number_type result;
        auto position = REF_short->size();
        while (position > 0)
        {
            auto const after_zero = REF_short->size() - position;
            position--;
            result = INNER_add_number(result, INNER_simple_multiply_number(*REF_long, (*REF_short)[position], after_zero));
        }
        return result;
    }
    integer::number_type integer::INNER_simple_multiply_number(number_type const & left, value_type const right, std::size_t const after_zero/*后置0的数量*/)
    {
        if (right == 0)
        {
            number_type result;
            return result;
        }
        TYPE_full_number carrier = 0;
        number_type result;
        result.resize(left.size());
        auto position = left.size();
        while (position > 0)
        {
            position--;
            carrier = ((TYPE_full_number)(left[position])) * ((TYPE_full_number)right) + (carrier >> half_length);
            result[position] = (value_type)carrier;
        }
        if ((value_type)(carrier >> half_length) != 0)
        {
            result.insert(result.begin(), (value_type)(carrier >> half_length));
        }
        result.insert(result.end(), after_zero, 0);
        return result;
    }
    integer::INNER_divide_result::INNER_divide_result(number_type && divide, number_type && module)noexcept
        :MEMBER_divide(divide)
        , MEMBER_module(module)
    {
    }
    integer::number_type const & integer::INNER_divide_result::divide()const noexcept
    {
        return this->MEMBER_divide;
    }
    integer::number_type const & integer::INNER_divide_result::mod()const noexcept
    {
        return this->MEMBER_module;
    }
    integer::INNER_divide_result::INNER_divide_result(INNER_divide_result && origin)noexcept
        :MEMBER_divide(std::move(origin.MEMBER_divide))
        , MEMBER_module(std::move(origin.MEMBER_module))
    {
    }
    integer integer::operator / (integer const & right)const
    {
        if (right.MEMBER_sign == sign_type::zero)
        {
            throw std::invalid_argument("除数为0，引发除0错误");
        }
        if (this->MEMBER_sign == sign_type::zero)
        {//被除数为0，则结果为0
            return integer();
        }
        auto const cmp = this->INNER_compare_number(right);//this和right的值比大小的结果
        if (cmp == compare_result::smaller)
        {
            return integer();
        }
        sign_type sign;//结果的符号
        if (this->MEMBER_sign == right.MEMBER_sign)
        {
            sign = sign_type::positive;
        }
        else
        {
            sign = sign_type::negative;
        }
        number_type mag;//结果的值
        if (cmp == compare_result::same)
        {
            mag.resize(1);
            mag[0] = 1;
        }
        else
        {
            mag = INNER_divide_number(this->MEMBER_number, right.MEMBER_number).divide();
        }
        return integer(mag, sign);
    }
    integer integer::operator %(integer const & right)const
    {
        if (right.MEMBER_sign == sign_type::zero)
        {
            throw std::invalid_argument("除数为0，引发除0错误");
        }
        if (this->MEMBER_sign == sign_type::zero)
        {//被除数为0，则结果为0
            return integer();
        }
        auto const cmp = this->INNER_compare_number(right);//this和right的值比大小的结果
        if (cmp == compare_result::smaller)
        {
            return *this;
        }
        if (cmp == compare_result::same)
        {
            return integer();
        }
        else
        {
            auto const mag = INNER_divide_number(this->MEMBER_number, right.MEMBER_number).mod();
            if (mag.size() != 0)
            {
                return integer(mag, this->MEMBER_sign);
            }
            else
            {
                return integer();
            }
        }

    }
    integer integer::power(integer const & pow)const
    {
        if (pow.MEMBER_sign == sign_type::negative)
        {
            throw std::invalid_argument("此函数不接受负数作为指数");
        }
        integer result(1, sign_type::positive);
        auto exp = pow;
        auto base = *this;
        auto const two = integer(2, sign_type::positive);
        while (exp.MEMBER_sign != sign_type::zero)
        {
            auto const temp = INNER_divide_and_module(exp, two);
            if (temp.second.MEMBER_sign != sign_type::zero)
            {
                result = result * base;
            }
            base = base * base;
            exp = temp.first;
        }
        return result;
    }
    std::pair<integer, integer> integer::INNER_divide_and_module(integer const & left, integer const & right)
    {
        if (right.MEMBER_sign == sign_type::zero)
        {
            throw std::invalid_argument("除数为0，引发除0错误");
        }
        if (left.MEMBER_sign == sign_type::zero)
        {
            return std::make_pair(integer(), integer());
        }
        auto const cmp = left.INNER_compare_number(right);//left和right的值比大小的结果
        if (cmp == compare_result::smaller)
        {
            return std::make_pair(integer(), left);
        }
        sign_type sign;//商的符号
        if (left.MEMBER_sign == right.MEMBER_sign)
        {
            sign = sign_type::positive;
        }
        else
        {
            sign = sign_type::negative;
        }
        number_type div_mag;//商的值
        if (cmp == compare_result::same)
        {
            div_mag.resize(1);
            div_mag[0] = 1;
            return std::make_pair(integer(div_mag, sign), integer());
        }
        else
        {
            auto const mag = INNER_divide_number(left.MEMBER_number, right.MEMBER_number);
            div_mag = mag.divide();
            //auto const mod_mag = mag.module();//余数的值
            if (mag.mod().size() != 0)
            {
                return std::make_pair(integer(div_mag, sign), integer(mag.mod(), left.MEMBER_sign));
            }
            else
            {
                return std::make_pair(integer(div_mag, sign), integer());
            }
        }
    }
    integer::INNER_divide_result integer::INNER_divide_number(number_type const & left, value_type const right)
    {
        number_type divide;
        divide.resize(left.size());

        TYPE_full_number carrier = 0;
        for (decltype(left.size()) count = 0;count < left.size();count++)
        {
            carrier <<= half_length;
            carrier += (TYPE_full_number)(left[count]);
            divide[count] = (value_type)(carrier / (TYPE_full_number)right);
            carrier %= (TYPE_full_number)right;
        }
        INNER_erase_leading_zero(divide);
        number_type mod;
        if (carrier != 0)
        {
            mod.resize(1);
            mod[0] = (value_type)carrier;
        }
        return INNER_divide_result(divide, mod);
    }
    integer::INNER_divide_result::INNER_divide_result(number_type const & divide, number_type const & mod)
        :MEMBER_divide(divide)
        , MEMBER_module(mod)
    {
    }
    integer::INNER_divide_result integer::INNER_divide_number(number_type const & left, number_type const & right)
    {
        if (right.size() == 1)
        {
            return INNER_divide_number(left, right[0]);
        }
        return INNER_Knuth_divide(left, right);
    }
    integer::INNER_divide_result integer::INNER_Knuth_divide(number_type const & left, number_type const & right, bool debug)
    {
        auto u = left;//被除数
        auto v = right;//除数
        auto const n = v.size();//n是数组v的长度
        auto const m = u.size() - n;//m+n是数组u的长度
        auto const d = base / ((TYPE_full_number)(v[0]) + 1);//因为v[0] > 0，所以 base / (v[0]+1) <= half_base，也就是说d可以用half_number类型存储
        if (debug and (value_type)(d >> half_length) != 0)
        {
            throw std::logic_error("d值不能用half_number类型表示，不符合预期。检查Knuth除法算法的实现。");
        }
        {
            number_type TEMP_d;
            TEMP_d.resize(1);
            TEMP_d[0] = (value_type)d;
            u = INNER_multiply_number(u, TEMP_d);
            if (u.size() == m + n)
            {
                u.insert(u.begin(), 0);
            }
            //现在要求数组u的长度是m+n+1
            v = INNER_multiply_number(v, TEMP_d);
            if (debug and v.size() != n)
            {
                throw std::logic_error("数组v的长度不符合预期。检查Knuth除法算法的实现。");
            }
            if (debug and v[0] < half_base)
            {
                throw std::logic_error("数组v的最高位小于half_base，没有完成规格化。检查Knuth除法算法的实现。");
            }
        }
        decltype(u.size()) j = 0;//循环变量
        number_type q;//商
        q.resize(m + 1);
        TYPE_full_number q_;//每一位商的估计值
        //每一位商的准确值就在q_、q_1和q_2之中
        TYPE_full_number r_;//每一步余数的估计值

        auto const TEMP_v = integer(v, sign_type::positive);//值为v的大整数
        while (j <= m)
        {
            q_ = ((TYPE_full_number)(u[j]) * base + (TYPE_full_number)(u[j + 1])) / (TYPE_full_number)(v[0]);
            r_ = ((TYPE_full_number)(u[j]) * base + (TYPE_full_number)(u[j + 1])) % (TYPE_full_number)(v[0]);
        FLOW_test:
            if (q_ == base or q_ * v[1] > base * r_ + u[j + 2])
            {
                if (debug and q_ == 0)
                {
                    throw std::logic_error("商的估计值q_为0，无法执行自减运算。检查Knuth除法的实现");
                }
                q_--;
                r_ += v[0];
                if (r_ < base)
                {
                    goto FLOW_test;
                }
            }
            number_type TEMP_u_mag;//复制数组{u[j],···,u[j+n]}
            TEMP_u_mag.resize(n + 1);
            for (std::size_t count_1 = 0;count_1 <= n;count_1++)
            {
                TEMP_u_mag[count_1] = u[count_1 + j];
            }
            INNER_erase_leading_zero(TEMP_u_mag);
            auto TEMP_u = integer(TEMP_u_mag, sign_type::positive);//数组{u[j],···,u[j + n]}组成的无符号大整数，已经去除前置0
            auto const TEMP_q_ = integer(q_, sign_type::positive);//值为q_的大整数
            TEMP_u = TEMP_u - (TEMP_q_ * TEMP_v);
            q[j] = q_;
            if (TEMP_u.MEMBER_sign == sign_type::negative)
            {
                if (debug and q[j] == 0)
                {
                    throw std::logic_error("商的估计值q_为0，无法执行自减运算。检查Knuth除法的实现");
                }
                q[j]--;
                TEMP_u = TEMP_u + TEMP_v;
            }
            //将TEMP_u赋还给数组u的对应位置
            TEMP_u_mag = TEMP_u.MEMBER_number;
            if (TEMP_u_mag.size() < n + 1)
            {
                auto const u_length = TEMP_u_mag.size();
                TEMP_u_mag.insert(TEMP_u_mag.begin(), n + 1 - u_length, 0);
            }
            for (std::size_t count_2 = 0;count_2 <= n;count_2++)
            {
                u[j + count_2] = TEMP_u_mag[count_2];
            }
            j++;
        }
        INNER_erase_leading_zero(q);
        INNER_erase_leading_zero(u);
        auto TEMP_1 = INNER_divide_number(u, (value_type)d);
        if (debug and TEMP_1.mod().size() != 0)
        {
            throw std::logic_error("数组u进行反规范化时，余数不为0。检查Knuth试商算法的实现");
        }
        u = TEMP_1.divide();
        if (debug and integer(u, sign_type::positive).INNER_compare_number(integer(right, sign_type::positive)) != compare_result::smaller)
        {
            throw std::logic_error("余数不小于除数right。检查Knuth除法算法的实现");
        }
        return INNER_divide_result(q, u);
    }
    integer::compare_result integer::INNER_compare(integer const & right)const
    {
        compare_result result;
        switch (this->MEMBER_sign)
        {
            case sign_type::negative:
            {
                if (right.MEMBER_sign != sign_type::negative)
                {
                    result = compare_result::smaller;
                }
                else
                {
                    result = right.INNER_compare_number(*this);
                }
                break;
            }
            case sign_type::zero:
            {
                switch (right.MEMBER_sign)
                {
                    case sign_type::negative:
                    {
                        result = compare_result::bigger;
                        break;
                    }
                    case sign_type::zero:
                    {
                        result = compare_result::same;
                        break;
                    }
                    case sign_type::positive:
                    {
                        result = compare_result::smaller;
                        break;
                    }
                }
                break;
            }
            case sign_type::positive:
            {
                if (right.MEMBER_sign != sign_type::positive)
                {
                    result = compare_result::bigger;
                }
                else
                {
                    result = this->INNER_compare_number(right);
                }
                break;
            }
        }
        return result;
    }
    integer::integer(std::string const & input, sign_type const sign, number_system const system)
        :integer()
    {
        integer input_base;
        switch (system)
        {
            case number_system::bin:
            {
                input_base = integer((value_type)2, sign_type::positive);
                break;
            }
            case number_system::dec:
            {
                input_base = integer((value_type)10, sign_type::positive);
                break;
            }
            case number_system::oct:
            {
                input_base = integer((value_type)8, sign_type::positive);
                break;
            }
            case number_system::hex:
            {
                input_base = integer((value_type)10, sign_type::positive);
                break;
            }
        }
        for (size_t i = 0; i < input.size(); ++i)
        {
            (*this) = (*this) * input_base;
            (*this) = (*this) + integer((value_type)(OKps::from_char(input[i], system)), sign_type::positive);//如果input有非数字的字符，则会在此处抛出异常
        }
        if (this->MEMBER_sign != sign_type::zero)
        {
            this->MEMBER_sign = sign;
        }
    }
    std::string integer::string(number_system const system)const
    {
        if (this->MEMBER_sign == sign_type::zero)
        {
            return "0";
        }
        integer input_base;
        switch (system)
        {
            case number_system::bin:
            {
                input_base = integer((value_type)2, sign_type::positive);
                break;
            }
            case number_system::dec:
            {
                input_base = integer((value_type)10, sign_type::positive);
                break;
            }
            case number_system::oct:
            {
                input_base = integer((value_type)8, sign_type::positive);
                break;
            }
            case number_system::hex:
            {
                input_base = integer((value_type)10, sign_type::positive);
                break;
            }
        }
        integer ubig = *this;
        if (ubig.MEMBER_sign == sign_type::negative)
        {
            ubig.MEMBER_sign = sign_type::positive;
        }
        std::string result;
        while (ubig > integer())
        {
            auto temp = ubig % input_base;
            if (temp.MEMBER_number.size() > 1 or temp.INNER_compare_number(input_base) != compare_result::smaller)
            {
                throw std::runtime_error("取余运算结果不符合预期。检查实现代码。");
            }
            if (temp.MEMBER_sign == sign_type::zero)
            {
                result.push_back(OKps::to_char(0, system));
            }
            else
            {
                result.push_back(OKps::to_char(temp.MEMBER_number[0], system));
            }
            ubig = ubig / input_base;
        }
        if (this->MEMBER_sign == sign_type::negative)
        {
            result.push_back('-');
        }
        std::reverse(result.begin(), result.end());
        return result;
    }
    bool integer::operator >(integer const & right)const
    {
        return this->INNER_compare(right) == compare_result::bigger;
    }
    bool integer::operator <(integer const & right)const
    {
        return this->INNER_compare(right) == compare_result::smaller;
    }
    bool integer::operator >=(integer const & right)const
    {
        return this->INNER_compare(right) != compare_result::smaller;
    }
    bool integer::operator <=(integer const & right)const
    {
        return this->INNER_compare(right) != compare_result::bigger;
    }
    integer::integer(TYPE_full_number const number, sign_type const sign)noexcept
        :MEMBER_sign(sign)
    {
        if (sign == sign_type::zero)
        {
            return;
        }
        if (number == 0)
        {
            this->MEMBER_sign = sign_type::zero;
            return;
        }
        value_type const high = (value_type)(number >> half_length);
        value_type const low = (value_type)number;
        if (high == 0)
        {
            this->MEMBER_number.resize(1);
            this->MEMBER_number[0] = low;
        }
        else
        {
            this->MEMBER_number.resize(2);
            this->MEMBER_number[0] = high;
            this->MEMBER_number[1] = low;
        }

    }
    integer::number_type integer::INNER_multiply_number(number_type const & left, value_type const right)
    {
        number_type TEMP_right;
        if (right == 0)
        {
            return TEMP_right;
        }
        TEMP_right.resize(1);
        TEMP_right[0] = right;
        return INNER_multiply_number(left, TEMP_right);
    }
    bool integer::INNER_Miller_Rabin(integer const & num, std::uintmax_t const check_times)
    {
        std::uintmax_t actual_check_times = check_times;
        if (actual_check_times == 0 or actual_check_times > MEMBER_known_prime.size())
        {
            actual_check_times = MEMBER_known_prime.size();
        }

        //实现Miller Rabin算法的二次筛查
        class impl final
        {
        private:
            integer const & checker/*用于筛查的数*/;
            integer const & num;
            std::atomic_bool & done;
            std::thread worker;

            void check()noexcept
            {
                if (this->done)
                {
                    return;
                }
                auto const one = integer(1, sign_type::positive);
                auto const two = integer(2, sign_type::positive);
                //把n-1  转化成 (2^r)*d
                integer s = num - one;integer r = integer();
                while ((s % two).MEMBER_sign == sign_type::zero and (not this->done))
                {
                    s = s / two;r = r + one;
                }

                //算出 2^d  存在 k 里
                integer k = checker.power_mod(s, num);

                //二次探测  看变化过程中是不是等于1 或 n-1
                if (k == one)
                {
                    return;
                }
                for (integer i;i < r and (not this->done);i = i + one, k = k * k % num)
                {
                    if (k == num - one)
                    {
                        return;
                    }
                }
                this->done = true;//未通过检测
                return;
            }
        public:
            impl(integer const & checker/*用于筛查的数*/, integer const & num, std::atomic_bool & done)
                : checker(checker)
                , num(num)
                , done(done)
            {
                this->worker = std::thread(&impl::check, this);
            }
            ~impl()
            {
                this->worker.join();
            }
            impl(impl const &) = delete;
            impl(impl &&) = delete;
        };

        for (std::uintmax_t i = 0;i < MEMBER_known_prime.size();i++)
        {
            if (num == MEMBER_known_prime[i])
            {
                return true;
            }
        }
        std::atomic_bool done = false;
        auto const thread_number = std::thread::hardware_concurrency();
        for (std::uintmax_t counter_1 = 0;counter_1 < actual_check_times;counter_1 += thread_number)
        {
            {
                std::vector<std::shared_ptr<impl>> workers;
                workers.resize(thread_number);
                for (std::uintmax_t counter_2 = 0;(counter_2 < thread_number) and (counter_1 + counter_2 < actual_check_times);counter_2++)
                {
                    workers[counter_2] = std::make_shared<impl>(MEMBER_known_prime[MEMBER_known_prime.size() - 1 - (counter_1 + counter_2)], num, done);
                }
            }
            if (done)
            {
                return false;
            }
        }

        return true;//所有探测结束 返回真

    }
    bool integer::is_prime(bool const cheat, std::uintmax_t const check_times)const
    {
        if (this->MEMBER_sign == sign_type::zero)
        {
            return false;
        }
        auto const one = integer(1, sign_type::positive);
        auto const two = integer(2, sign_type::positive);
        auto const three = integer(3, sign_type::positive);
        auto const num = this->absolute();//要判断的数
        if (num == one)
        {
            return false;
        }
        else if (num == two)
        {
            return true;
        }
        if ((num % two).MEMBER_sign == sign_type::zero)
        {
            return false;
        }
        if (cheat)
        {
            return INNER_Miller_Rabin(*this, check_times);
        }
        auto const root = num.square_root();//this的绝对值的平方根
        for (integer count = three;count <= root;count = count + two)
        {
            if ((num % count).MEMBER_sign == sign_type::zero)
            {
                return false;
            }
        }
        return true;
    }
    integer integer::random_prime(std::size_t const length, sign_type const sign, bool const cheat /*是否使用素数判断算法*/, std::uintmax_t const check_times)
    {
        if (sign == sign_type::zero or length == 0)
        {
            throw std::invalid_argument("素数不能为0");
        }
        auto result = random(length, sign);
        while (not result.is_prime(cheat, check_times))
        {
            result = random(length, sign);
        }
        return result;
    }

    integer integer::gcd(integer const & left, integer const & right)
    {
        auto a = left;
        auto b = right;
        while (b.MEMBER_sign != sign_type::zero)
        {
            auto t = b;
            b = a % b;
            a = t;
        }
        return a;
    }
    bool integer::is_coprime(integer const & left, integer const & right)
    {
        auto const one = integer(1, sign_type::positive);
        auto const result = gcd(left.absolute(), right.absolute());
        if (result == one)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    integer integer::inverse_mod(integer const & num, integer const & mod)
    {
        integer x;
        integer y;
        integer gcd = extra_gcd(num, mod, x, y);
        auto const one = integer(1, sign_type::positive);
        if (gcd != one)
        {
            throw std::invalid_argument("输入的参数不存在模逆元");
        }
        if (x.MEMBER_sign == sign_type::negative)
        {
            x = x + mod;
        }
        if ((num * x) % mod != one)
        {
            throw std::runtime_error("求模逆运算结果错误，检查扩展欧几里得算法的实现");
        }
        return x;
    }
    integer integer::power_mod(integer const & pow/*指数*/, integer const & mod/*除数*/)const
    {
        if (pow.MEMBER_sign == sign_type::negative)
        {
            throw std::invalid_argument("此函数不接受负数作为指数");
        }
        auto const one = integer(1, sign_type::positive);
        auto const two = integer(2, sign_type::positive);
        integer exponent = pow;
        integer result = one;
        integer TEMP_base = *this;
        while (exponent.MEMBER_sign == sign_type::positive)
        {
            if (exponent % two == one)
            {
                result = (result * TEMP_base) % mod;
            }
            TEMP_base = (TEMP_base * TEMP_base) % mod;
            exponent = exponent / two;
        }
        return result;
    }
    integer integer::extra_gcd(integer const & left, integer const & right, integer & x, integer & y)
    {
        auto m = left;
        auto n = right;
        auto const one = integer(1, sign_type::positive);
        if (n.MEMBER_sign == sign_type::zero)
        {
            x = one; y = integer();
            return m;
        }
        integer t;
        integer a1 = one; integer b = one;
        integer a;integer b1;
        integer c = m; integer d = n;
        integer q = c / d; integer r = c % d;
        while (r.MEMBER_sign != sign_type::zero)
        {
            c = d;
            d = r;
            t = a1;
            a1 = a;
            a = t - q * a;
            t = b1;
            b1 = b;
            b = t - q * b;

            q = c / d;
            r = c % d;
        }
        x = a; y = b;
        return d;
    }
    integer integer::random(std::size_t const length, sign_type const sign)
    {
        if (sign == sign_type::zero or length == 0)
        {
            return integer();
        }
        OKps::integer::number_type num;
        std::random_device seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(seed());                      // 随机数生成器
        std::uniform_int_distribution<OKps::integer::value_type> distribution_1(0, std::numeric_limits<OKps::integer::value_type>::max()); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间
        std::uniform_int_distribution<OKps::integer::value_type> distribution_2(1, std::numeric_limits<OKps::integer::value_type>::max()); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间
        num.resize(length);
        num[0] = distribution_2(random_engine);
        for (std::size_t count = 1;count < length;count++)
        {
            num[count] = distribution_1(random_engine);
        }
        return OKps::integer(num, sign);
    }
    integer integer::square_root()const
    {
        if (this->MEMBER_sign == sign_type::negative)
        {
            throw std::invalid_argument("此函数不能对负数求平方根。未来可能会实现复数运算，但目前没有。");
        }
        if (this->MEMBER_sign == sign_type::zero)
        {
            return integer();
        }
        auto const one = integer(1, sign_type::positive);
        auto const two = integer(2, sign_type::positive);
        if ((*this) == one)
        {
            return one;
        }
        integer root = *this;

        while (true)
        {
            integer temp = root;
            root = ((*this) / root + root) / two;
            if (root == temp)
            {
                break;
            }
        }
        return root;
    }
    integer integer::absolute()const
    {
        auto result = integer();
        switch (this->MEMBER_sign)
        {
            case sign_type::negative:
            {
                result = (-(*this));
                break;
            }
            case sign_type::positive:
            {
                result = (*this);
                break;
            }
            case sign_type::zero:
            {
                break;
            }
        }
        return result;
    }
    std::vector<integer> integer::INNER_init_known_prime(std::uintmax_t const prime_count)
    {
        std::vector<integer> result;
        if (prime_count == 0)
        {
            return result;
        }
        auto const two = integer(2, sign_type::positive);
        if (prime_count == 1)
        {
            result.resize(1);
            result[0] = two;
            return result;
        }
        auto const three = integer(3, sign_type::positive);
        if (prime_count == 2)
        {
            result.resize(2);
            result[0] = two;
            result[1] = three;
            return result;
        }

        std::uintmax_t temp;
        result.resize(prime_count);
        std::ifstream prime_file;
        prime_file.open(".\\prime", std::ios::in | std::ios::binary);
        if (not prime_file.is_open())
        {
            throw std::runtime_error("没有找到素数文件");
        }

        for (std::size_t count = 0;count < prime_count;count++)
        {
            prime_file.read((char *)(&temp), sizeof(std::uintmax_t));
            result[count] = integer(temp, sign_type::positive);
        }

        return result;
    }

    void integer::operator +=(integer const & right)
    {
        (*this) = (*this) + right;
    }
    void integer::operator ++()
    {
        (*this) += integer(1, sign_type::positive);
    }
    void integer::operator -=(integer const & right)
    {
        (*this) = (*this) - right;
    }
    void integer::operator --()
    {
        (*this) -= integer(1, sign_type::positive);
    }
    void integer::operator *=(integer const & right)
    {
        (*this) = (*this) * right;
    }
    void integer::operator /=(integer const & right)
    {
        (*this) = (*this) / right;
    }
    void integer::operator %=(integer const & right)
    {
        (*this) = (*this) % right;
    }
}