#pragma once
#include <optional>

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
        std::optional<RSA::byte_device> MEMBER_rsa;
        std::vector<AES::byte_device> MEMBER_aes;
        std::vector<std::array<integer, AES::byte_device::key_length>> MEMBER_crypt_aes;

    public:
        using path_type = std::filesystem::path;
        bool operator ==(cipher const & right)const
            noexcept(noexcept(std::declval<RSA::byte_device const &>() == std::declval<RSA::byte_device const &>())
and noexcept(std::declval<std::vector<AES::byte_device> const &>() == std::declval<std::vector<AES::byte_device> const &>()));
        bool operator !=(cipher const & right)const
            noexcept(noexcept(std::declval<cipher const &>() == std::declval<cipher const &>()));
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
        void save(path_type const & route)const;
        //从密钥文件读取密钥
        cipher(path_type const & route);
        /*
        参数 temp_route 是存放临时文件的目录，在调用此函数时它必须已经存在，否则抛出异常。
        函数执行后会清理临时文件，但不会删除 temp_route 这个目录。该目录由调用者负责管理。
        */
        void encrypt(path_type const & origin_route, path_type const & result_route, path_type const & temp_route, const std::size_t thread_count = 0)const;
        void decrypt(path_type const & origin_route, path_type const & result_route, path_type const & temp_route, const std::size_t thread_count = 0)const;
    };

    /*
    密码生成器
    生成符合要求的字符串密码
    */
    class password_generator final
    {
    private:
        class implement;

        std::size_t MEMBER_length_lower;//长度下限
        std::size_t MEMBER_length_upper;//长度上限
        /*
        要从生成的密码中排除的子串
        例如生日等敏感信息
        */
        std::vector<std::string> MEMBER_eliminate_list;
    public:
        /*
        从基本ASCII字符中排除哪些类型的字符
        */
        class flag_type final
        {
        public:
            //构造各个标志位都是false的标志
            flag_type()noexcept;
            flag_type(
                bool const FLAG_Arabic_numerals,//阿拉伯数字
                bool const FLAG_capital_English_letter,//大写英文字母
                bool const FLAG_English_letter,//小写英文字母
                bool const FLAG_special_character//特殊字符
            )noexcept;
            ~flag_type()noexcept;
            flag_type(flag_type const &)noexcept;
            void operator =(flag_type const &)noexcept;
            flag_type(flag_type &&)noexcept;
            void operator =(flag_type &&) noexcept;

        private:

            bool MEMBER_Arabic_numerals;//阿拉伯数字
            bool MEMBER_capital_English_letter;//大写英文字母
            bool MEMBER_English_letter;//小写英文字母
            bool MEMBER_special_character;//特殊字符
        public:
            bool const & Arabic_numerals()const noexcept;//阿拉伯数字
            bool const & capital_English_letter()const noexcept;//大写英文字母
            bool const & English_letter()const noexcept;//小写英文字母
            bool const & special_character()const noexcept;//特殊字符
            bool & Arabic_numerals()noexcept;//阿拉伯数字
            bool & capital_English_letter()noexcept;//大写英文字母
            bool & English_letter()noexcept;//小写英文字母
            bool & special_character()noexcept;//特殊字符
        };

        static std::string const Arabic_numerals;//阿拉伯数字
        static std::string const capital_English_letter;//大写英文字母
        static std::string const English_letter;//小写英文字母
        static std::string const special_character;//特殊字符
    private:
        std::string MEMBER_valid_character_set;
    public:
        password_generator
        (std::size_t const length_lower,//密码长度下限
        std::size_t const length_upper,//密码长度上限
        /*
        要从生成的密码中排除的子串
        例如生日、姓名、电话号码、邮箱等敏感信息
        */
        std::vector<std::string> const & eliminate_list,
        //要从基本ASCII字符中排除哪些类型的字符
        flag_type const & flag
        );
        ~password_generator()noexcept;
        password_generator(password_generator &&)noexcept;
        void operator =(password_generator &&)noexcept;
        password_generator(password_generator const &);
        void operator =(password_generator const &);

        std::string generate()const;
    private:

        void INNER_eliminate(std::string &)const;
    };

    class password_pool final
    {
    public:
        class password_block final
        {
            friend class password_pool;
        private:
            std::string MEMBER_name;
            std::string MEMBER_account;
            std::string MEMBER_password;

            operator field()const;
            password_block(field const &);

            password_block(
                std::string const &,//名称
                std::string const &,//账号
                std::string &&//密码
            );

            password_block(
                std::string const &,//名称
                std::string const &,//账号
                std::string const &//密码
            );

        public:
            password_block(password_block const &);
            password_block(password_block &&)noexcept;
            void operator =(password_block const &);
            void operator =(password_block &&)noexcept;
            ~password_block()noexcept;

            std::string const & name()const noexcept;
            std::string const & account()const noexcept;
            std::string const & password()const noexcept;
            std::string & name() noexcept;
            std::string & account() noexcept;
            std::string & password() noexcept;

            //空构造
            password_block()noexcept;
        };
    private:
        std::vector<password_block> MEMBER_pool;
        std::filesystem::path MEMBER_key_file;
        std::filesystem::path MEMBER_storage_file;
        std::optional<cipher> MEMBER_cipher;
        std::filesystem::path MEMBER_temporary_directory;
        bool MEMBER_temporary_flag;
    public:
        password_pool(
            std::filesystem::path const &,//密钥文件位置
            std::filesystem::path const &, //数据文件位置
            /*
            加密和解密过程中的临时目录
            如果此路径不存在，则创建目录，且在析构时销毁
            如果此路径已经存在，则在析构时不销毁
            */
            std::filesystem::path const & = ".\\临时文件"
        );
        ~password_pool()noexcept(false);
        password_pool(password_pool const &) = delete;
        void operator =(password_pool const &) = delete;
        password_pool(password_pool &&) noexcept;
        void operator =(password_pool &&)noexcept;
        void erase(std::size_t const);
        //删除闭区间范围的条目
        void erase(std::size_t const begin_position, std::size_t const end_position);
        void clear();
        void add(
            std::string const &,//名称
            std::string const &,//账号
            password_generator const &//密码生成器
        );
        void add(
            std::string const &,//名称
            std::string const &,//账号
            std::string const &//密码
        );
        password_block const & operator [](std::size_t const)const noexcept;
        password_block & operator [](std::size_t const) noexcept;
        std::size_t size()const noexcept;
    };

}