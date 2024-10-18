#pragma once

#include ".\integer.hpp"

namespace OKps::RSA
{
    class byte_device
    {
    private:
        integer MEMBER_public_key;
        integer MEMBER_private_key;
        integer MEMBER_key;
        bool MEMBER_valid;
    public:
        bool operator ==(byte_device const & right)const;
        bool operator !=(byte_device const & right)const;

        byte_device(integer const & key, integer const & public_key, integer const & private_key);

    /*
    生成rsa密钥
    length参数指定生成密钥时所用素数的长度
    注意长度是存储素数大整数所用数组的长度，详细解释查看OKps::integer类的注释
    生成密钥时会检测其正确性，key_check参数指定检测的次数
    prime_check参数指定素数检测算法的二次筛查次数
    */
        byte_device(std::size_t const length = 32, std::uintmax_t const key_check = 128, std::uintmax_t const prime_check = 128);

        ~byte_device()noexcept;

        byte_device(byte_device const & origin);
        void operator =(byte_device const & origin);

        //移动构造会导致原来的对象origin不可用
        byte_device(byte_device && origin)noexcept;
        void operator =(byte_device && origin)  noexcept;
        integer encrypt(integer const & input)const;
        integer decrypt(integer const & input)const;
        integer const & key()const;
        integer const & public_key()const;
        integer const & private_key()const;
        //检查当前rsa对象是否有效
        bool is_valid()const noexcept;

    private:
        /*
        检查生成的密钥是否有效
        因为使用Miller Rabin算法判断素数，有小概率出错，可能导致密钥无效
        */
        bool INNER_test_keys(std::size_t const length, std::uintmax_t const times/*测试次数*/)const;
        /*
        生成一组密钥
        */
        void INNER_generate_keys(std::size_t const length, std::uintmax_t const check_times);
    };
}