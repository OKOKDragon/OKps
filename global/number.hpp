#pragma once

#include <cstdint>

namespace OKps
{
    //进制系统
    enum class number_system :std::uintmax_t
    {
        dec = 10,//10进制
        bin = 2,//2进制
        oct = 8,//8进制
        hex = 16//16进制
    };
}
