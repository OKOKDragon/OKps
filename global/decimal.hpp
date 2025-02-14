

#include ".\integer.hpp"

namespace OKps
{
    /*
    指定精度的小数
    */
    class decimal final
    {
    private:
        static integer const MEMBER_base;//底数
        integer MEMBER_exponent;//指数部分
        integer MEMBER_mantissa;//尾数部分

    public:
        ~decimal()noexcept;
        /*
        mantissa 参数指定尾数部分
        对于整数 mantissa，此构造函数生成的尾数部分是 0.mantissa
        */
        decimal(integer const & mantissa, integer const & exponent = integer(0, integer::sign_type::zero));
        decimal(decimal const & origin);
        void operator =(decimal const & origin);
        decimal(decimal && origin)noexcept;
        void operator =(decimal && origin)noexcept;
    };
}