

#include ".\integer.hpp"

namespace OKps
{
    /*
    高精度小数
    每一次除法运算中保留的精度至少是双精度浮点数的2倍
    */
    class decimal final
    {
    public:
        //用于表示指数的类型
        using exponent_type = std::intmax_t;
        //底数
        static integer::holder_type const & base;
    private:
        exponent_type MEMBER_exponent;//指数部分
        integer MEMBER_mantissa;//尾数部分

        /*
        对齐小数点
        要求longer的指数比shorter大
        */
        static void INNER_align_point(decimal & longer, decimal const & shorter);
        /*
        去除后置零
        */
        void INNER_erase_post_zero();

    public:
        decimal(integer const & number);
        /*
        构造值为0的数
        */
        decimal();
        ~decimal()noexcept;
        /*
        mantissa 参数指定有效数字
        exponent 参数指定指数
        */
        decimal(integer const & mantissa, exponent_type const exponent);
        /*
        将字符串转换为小数
        */
        decimal(std::string const & number, exponent_type const exponent, number_system const system = number_system::dec);
        /*
        mantissa 参数指定有效数字
        exponent 参数指定指数
        */
        decimal(integer && mantissa, exponent_type const exponent);
        decimal(decimal const & origin);
        void operator =(decimal const & origin);
        decimal(decimal && origin)noexcept;
        void operator =(decimal && origin)noexcept;

        //取相反数
        decimal operator -()const;

        decimal operator +(decimal const &)const;
        decimal operator -(decimal const &)const;
        decimal operator *(decimal const &)const;
        /*
        对于除不尽的除法运算，设左操作数的位数是nl，右操作数的位数是nr，则结果保留的位数是{nl,nr,4}中的最大值。
        */
        decimal operator /(decimal const &)const;
        void operator +=(decimal const &);
        void operator -=(decimal const &);
        void operator *=(decimal const &);
        void operator /=(decimal const &);

        /*
        取整数部分
        */
        integer integer_part()const;

        std::string string(number_system const system = number_system::dec)const;

        static decimal const one;
        static decimal const two;
        static decimal const zero;
        static decimal const negative_one;
        static decimal const eight;
        static decimal const ten;
        static decimal const sixteen;
    };
}