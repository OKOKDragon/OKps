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

    public:
        bool operator ==(byte_device const & right)const
            noexcept(noexcept(std::declval<integer const &>() == std::declval<integer const &>()));
        bool operator !=(byte_device const & right)const
            noexcept(noexcept(std::declval<integer const &>() == std::declval<integer const &>()));

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

        byte_device(byte_device const & origin)
            noexcept(std::is_nothrow_copy_constructible_v<integer>);
        void operator =(byte_device const & origin)
            noexcept(std::is_nothrow_copy_assignable_v<integer>);

        /*
        移动后，禁止使用原来的对象 origin
        */
        byte_device(byte_device && origin)
            noexcept(std::is_nothrow_move_constructible_v<integer>);
        void operator =(byte_device && origin)
            noexcept(std::is_nothrow_move_assignable_v<integer>);
        integer encrypt(integer const & input)const;
        integer decrypt(integer const & input)const;
        integer const & key()const noexcept;
        integer const & public_key()const noexcept;
        integer const & private_key()const noexcept;

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