#pragma once

#include ".\aes.hpp"
#include ".\rsa.hpp"

namespace OKps::crypt
{
    /*
    采用aes和rsa双重加密实现的加密器
    */
    class cipher final
    {
    private:
        std::unique_ptr<RSA::byte_device> MEMBER_rsa;
        std::vector<AES::byte_device> MEMBER_aes;
        std::vector<std::array<integer, AES::byte_device::key_length>> MEMBER_crypt_aes;
        using TYPE_path = std::filesystem::path;
    public:

        bool operator ==(cipher const & right)const;
        bool operator !=(cipher const & right)const;
        cipher(RSA::byte_device && rsa = RSA::byte_device(), std::uintmax_t const aes_count = 2/*aes密钥的数量*/);
        ~cipher()noexcept;
        cipher(cipher const & origin);
        //移动时会造成原对象失效
        cipher(cipher && origin)noexcept;
        //将密钥输出到文件
        void save(TYPE_path const & route)const;
        //从密钥文件读取密钥
        cipher(TYPE_path const & route);
        void encrypt(TYPE_path const & origin_route, TYPE_path const & result_route, TYPE_path const & temp_route/*存放临时文件的目录*/, const std::size_t thread_count = 0)const;
        void decrypt(TYPE_path const & origin_route, TYPE_path const & result_route, TYPE_path const & temp_route/*存放临时文件的目录*/, const std::size_t thread_count = 0)const;
    };
}