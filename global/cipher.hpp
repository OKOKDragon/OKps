#pragma once

#include ".\aes.hpp"
#include ".\rsa.hpp"

namespace OKps::crypt
{
    /*
    加密器

    采用aes算法加密数据，用rsa算法加密aes密钥的经典加密方法。
    */
    class cipher final
    {
    private:
        RSA::byte_device MEMBER_rsa;
        std::vector<AES::byte_device> MEMBER_aes;
        std::vector<std::array<integer, AES::byte_device::key_length>> MEMBER_crypt_aes;
        using TYPE_path = std::filesystem::path;
    public:

        bool operator ==(cipher const & right)const
            noexcept(noexcept(std::declval<RSA::byte_device const>() == std::declval<RSA::byte_device const>())
and noexcept(std::declval<std::vector<AES::byte_device> const>() == std::declval<std::vector<AES::byte_device> const>()));
        bool operator !=(cipher const & right)const
            noexcept(noexcept(std::declval<cipher const>() == std::declval<cipher const &>()));
        cipher(RSA::byte_device && rsa = RSA::byte_device(), std::uintmax_t const aes_count = 2/*aes密钥的数量*/);
        ~cipher()
            noexcept(std::is_nothrow_destructible_v<RSA::byte_device>
and std::is_nothrow_destructible_v<std::vector<AES::byte_device>>
and std::is_nothrow_destructible_v<std::vector<std::array<integer, AES::byte_device::key_length>>>);
        cipher(cipher const & origin)
            noexcept(std::is_nothrow_copy_constructible_v<RSA::byte_device>
and std::is_nothrow_copy_constructible_v<std::vector<AES::byte_device>>
and std::is_nothrow_copy_constructible_v<std::vector<std::array<integer, AES::byte_device::key_length>>>);
        void operator =(cipher const & origin)
            noexcept(std::is_nothrow_copy_assignable_v<RSA::byte_device>
and std::is_nothrow_copy_assignable_v<std::vector<AES::byte_device>>
and std::is_nothrow_copy_assignable_v<std::vector<std::array<integer, AES::byte_device::key_length>>>);
        /*
        移动后，禁止使用原来的对象 origin
        */
        cipher(cipher && origin)
            noexcept(std::is_nothrow_move_constructible_v<RSA::byte_device>
and std::is_nothrow_move_constructible_v<std::vector<AES::byte_device>>
and std::is_nothrow_move_constructible_v<std::vector<std::array<integer, AES::byte_device::key_length>>>);
        void operator =(cipher && origin)
            noexcept(std::is_nothrow_move_assignable_v<RSA::byte_device>
and std::is_nothrow_move_assignable_v<std::vector<AES::byte_device>>
and std::is_nothrow_move_assignable_v<std::vector<std::array<integer, AES::byte_device::key_length>>>);
            //将密钥输出到文件
        void save(TYPE_path const & route)const;
        //从密钥文件读取密钥
        cipher(TYPE_path const & route);
        /*
        参数 temp_route 是存放临时文件的目录，在调用此函数时它必须已经存在，否则抛出异常。
        函数执行后会清理临时文件，但不会删除 temp_route 这个目录。该目录由调用者负责管理。
        */
        void encrypt(TYPE_path const & origin_route, TYPE_path const & result_route, TYPE_path const & temp_route, const std::size_t thread_count = 0)const;
        void decrypt(TYPE_path const & origin_route, TYPE_path const & result_route, TYPE_path const & temp_route, const std::size_t thread_count = 0)const;
    };
}