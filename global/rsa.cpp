#include <thread>

#include ".\rsa.hpp"

namespace OKps::RSA
{

    bool byte_device::is_valid()const noexcept
    {
        return this->MEMBER_valid;
    }
    bool byte_device::INNER_test_keys(std::size_t const length, std::uintmax_t const times)const
    {
        class impl final
        {
        private:
            std::thread worker;
            byte_device const & rsa;
            std::atomic<bool> & done;
            std::size_t const length;
            void check()noexcept
            {
                integer origin = integer::random(this->length, integer::sign_type::positive);
                while ((not this->done) and origin >= this->rsa.MEMBER_key)
                {
                    origin = integer::random(this->length, integer::sign_type::positive);
                }
                if (this->done)
                {
                    return;
                }
                auto const crypt = this->rsa.encrypt(origin);
                if (this->done)
                {
                    return;
                }
                auto const checker = this->rsa.decrypt(crypt);
                if (this->done)
                {
                    return;
                }
                if (checker != origin)
                {
                    this->done = true;
                    return;
                }
            }
        public:
            impl(byte_device const & rsa, std::atomic<bool> & done, std::size_t const length)
                :rsa(rsa)
                , done(done)
                , length(length)
            {
                this->worker = std::thread(&impl::check, this);
            }
            ~impl()
            {
                this->worker.join();
            }
            impl(impl const &) = delete;
            impl(impl &&) = delete;
            void operator =(impl const &) = delete;
            void operator =(impl &&) = delete;
        };
        std::atomic<bool> done = false;
        auto const thread_number = std::thread::hardware_concurrency();

        for (std::uintmax_t counter_1 = 0;counter_1 < times;counter_1 += thread_number)
        {
            {
                std::vector<std::shared_ptr<impl>> workers;
                workers.resize(thread_number);
                for (std::uintmax_t counter_2 = 0;(counter_2 < thread_number) and (counter_1 + counter_2 < times);counter_2++)
                {
                    workers[counter_2] = std::make_shared<impl>(*this, done, length);
                }
            }
            if (done)
            {
                return false;
            }
        }
        return true;
    }

    void byte_device::INNER_generate_keys(std::size_t const length, std::uintmax_t const check_times)
    {
        bool done = false;
        while (not done)
        {
            done = true;
            auto const p = integer::random_prime(length, integer::sign_type::positive, true, check_times);
            auto const q = integer::random_prime(length, integer::sign_type::positive, true, check_times);
            this->MEMBER_key = p * q;//n
            auto const one = integer(1, integer::sign_type::positive);
            auto const eular = (p - one) * (q - one);//欧拉函数的值

            /*
            选择公钥e与eular互素
            一般使得e为一个较小的数，方便加密的一方
            而私钥d则为大数
            */
            while (true)
            {
                std::size_t public_key_length = 5;
                if (public_key_length > length / 2)
                {
                    public_key_length = length / 2;
                }
                this->MEMBER_public_key = integer::random(public_key_length, integer::sign_type::positive);//e
                if (integer::is_coprime(this->MEMBER_public_key, eular))
                {
                    break;
                }
            }

            /*
            公钥d与e关于eular互为模逆元
            即d满足 (e * d) % eualr == 1
            */
            try
            {
                this->MEMBER_private_key = integer::inverse_mod(this->MEMBER_public_key, eular);//d
            }
            catch (std::exception const & error)
            {//可能是由于素数判断算法出错，而导致不存在e关于eular的模逆元
                done = false;
            }
        }
    }
    bool byte_device::operator ==(byte_device const & right)const
    {
        if ((not this->MEMBER_valid) or (not right.MEMBER_valid))
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("rsa对象已失效，无法使用");
        }
        if (this->MEMBER_key == right.MEMBER_key and this->MEMBER_public_key == right.MEMBER_public_key and this->MEMBER_private_key == right.MEMBER_private_key)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool byte_device::operator !=(byte_device const & right)const
    {
        return not(*this == right);
    }
    void byte_device::operator =(byte_device && origin) noexcept
    {
        if (this != (&origin))
        {
            this->MEMBER_key = std::move(origin.MEMBER_key);
            this->MEMBER_public_key = std::move(origin.MEMBER_public_key);
            this->MEMBER_private_key = std::move(origin.MEMBER_private_key);
            this->MEMBER_valid = origin.MEMBER_valid;
            origin.MEMBER_valid = false;
        }
    }
    byte_device::byte_device(integer const & key, integer const & public_key, integer const & private_key)
        :MEMBER_key(key)
        , MEMBER_public_key(public_key)
        , MEMBER_private_key(private_key)
        , MEMBER_valid(true)
    {
    }
    byte_device::byte_device(std::size_t const length, std::uintmax_t const key_check, std::uintmax_t const prime_check)
        :MEMBER_valid(true)
    {
        if (length <= 5)
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("素数长度太小，安全性非常低");
            /*
            在一台使用 n 位CPU的机器上，本库使用“长为 n/2 位的c++语言内置整数类型”组成的数组来存储大数，而编译器可能会提供长为 2n 位的整数类型作为扩展支持
            所以一个大数必须至少用一个长度为5的数组来表示，才能保证它无法用编译器内置类型来存储
            */
        }
        bool checked = false;
        while (not checked)
        {
            this->INNER_generate_keys(length, prime_check);
            checked = this->INNER_test_keys(1, key_check);
        }

    }

    byte_device::~byte_device()noexcept
    {
    }
    byte_device::byte_device(byte_device const & origin)
        :MEMBER_key(origin.MEMBER_key)
        , MEMBER_public_key(origin.MEMBER_public_key)
        , MEMBER_private_key(origin.MEMBER_private_key)
        , MEMBER_valid(origin.MEMBER_valid)
    {
    }
    void  byte_device::operator =(byte_device const & origin)
    {
        if (this != (&origin))
        {
            this->MEMBER_key = origin.MEMBER_key;
            this->MEMBER_public_key = origin.MEMBER_public_key;
            this->MEMBER_private_key = origin.MEMBER_private_key;
            this->MEMBER_valid = origin.MEMBER_valid;
        }
    }
    byte_device::byte_device(byte_device && origin)noexcept
        :MEMBER_key(std::move(origin.MEMBER_key))
        , MEMBER_public_key(std::move(origin.MEMBER_public_key))
        , MEMBER_private_key(std::move(origin.MEMBER_private_key))
        , MEMBER_valid(origin.MEMBER_valid)
    {
        origin.MEMBER_valid = false;
    }
    integer byte_device::encrypt(integer const & input)const
    {
        if (not this->MEMBER_valid)
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("此rsa对象已失效，无法使用");
        }
        if (input.sign() == integer::sign_type::negative)
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("rsa加密时，原文必须为非负数");
        }
        if (input >= this->MEMBER_key)
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("rsa加密时，原文必须小于密钥n");
        }
        return input.power_mod(this->MEMBER_public_key, this->MEMBER_key);
    }
    integer byte_device::decrypt(integer const & input)const
    {
        if (not this->MEMBER_valid)
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("此rsa对象已失效，无法使用");
        }
        if (input.sign() == integer::sign_type::negative)
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("rsa解密时，密文必须为非负数");
        }
        if (input >= this->MEMBER_key)
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("rsa解密时，密文必须小于密钥n");
        }
        return input.power_mod(this->MEMBER_private_key, this->MEMBER_key);
    }
    integer const & byte_device::key()const
    {
        if (not this->MEMBER_valid)
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("此rsa对象已失效，无法使用");
        }
        return this->MEMBER_key;
    }
    integer const & byte_device::public_key()const
    {
        if (not this->MEMBER_valid)
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("此rsa对象已失效，无法使用");
        }
        return this->MEMBER_public_key;
    }
    integer const & byte_device::private_key()const
    {
        if (not this->MEMBER_valid)
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("此rsa对象已失效，无法使用");
        }
        return this->MEMBER_private_key;
    }


}