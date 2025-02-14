#include ".\decimal.hpp"

namespace OKps
{
    integer const decimal::MEMBER_base = integer(2, integer::sign_type::positive);

    decimal::decimal(integer const & mantissa, integer const & exponent)
        :MEMBER_exponent(exponent)
        , MEMBER_mantissa(mantissa)
    {
    }
}