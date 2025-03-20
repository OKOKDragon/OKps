
#include <random>

#include ".\error.hpp"
#include ".\cipher.hpp"

namespace OKps::crypt
{

    cipher::cipher(RSA::byte_device && rsa, std::uintmax_t const aes_count)
        :MEMBER_rsa(std::move(rsa))
    {
        if (aes_count < 1)
        {
            throw error(std::invalid_argument("aes密钥数量至少为1"));
        }
        for (std::uintmax_t counter_1 = 0;counter_1 < aes_count;counter_1++)
        {
            this->MEMBER_aes.push_back(AES::byte_device());
            std::array<integer, AES::byte_device::key_length> TEMP_aes_crypt;
            for (std::uintmax_t counter_2 = 0;counter_2 < AES::byte_device::key_length;counter_2++)
            {
                TEMP_aes_crypt[counter_2] = this->MEMBER_rsa->encrypt(integer((std::underlying_type_t<std::byte>)(this->MEMBER_aes[counter_1].key()[counter_2]), integer::sign_type::positive));
            }
            this->MEMBER_crypt_aes.push_back(TEMP_aes_crypt);
        }
    }
    cipher::~cipher()
        noexcept(std::is_nothrow_destructible_v<RSA::byte_device>
        and std::is_nothrow_destructible_v<std::vector<AES::byte_device>>
        and std::is_nothrow_destructible_v<std::vector<std::array<integer, AES::byte_device::key_length>>>)
    {
    }
    cipher::cipher(cipher const & origin)
        noexcept(std::is_nothrow_copy_constructible_v<RSA::byte_device>
and std::is_nothrow_copy_constructible_v<std::vector<AES::byte_device>>
and std::is_nothrow_copy_constructible_v<std::vector<std::array<integer, AES::byte_device::key_length>>>)
    :MEMBER_rsa(origin.MEMBER_rsa)
        , MEMBER_aes(origin.MEMBER_aes)
        , MEMBER_crypt_aes(origin.MEMBER_crypt_aes)
    {
    }
    void cipher::operator =(cipher const & origin)
        noexcept(std::is_nothrow_copy_assignable_v<RSA::byte_device>
        and std::is_nothrow_copy_assignable_v<std::vector<AES::byte_device>>
        and std::is_nothrow_copy_assignable_v<std::vector<std::array<integer, AES::byte_device::key_length>>>)
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_rsa = origin.MEMBER_rsa;
            this->MEMBER_aes = origin.MEMBER_aes;
            this->MEMBER_crypt_aes = origin.MEMBER_crypt_aes;
        }
    }

    cipher::cipher(cipher && origin)
        noexcept(std::is_nothrow_move_constructible_v<RSA::byte_device>
        and std::is_nothrow_move_constructible_v<std::vector<AES::byte_device>>
        and std::is_nothrow_move_constructible_v<std::vector<std::array<integer, AES::byte_device::key_length>>>)
        :MEMBER_rsa(std::move(origin.MEMBER_rsa))
        , MEMBER_aes(std::move(origin.MEMBER_aes))
        , MEMBER_crypt_aes(std::move(origin.MEMBER_crypt_aes))
    {
    }
    void cipher::operator =(cipher && origin)
        noexcept(std::is_nothrow_move_assignable_v<RSA::byte_device>
        and std::is_nothrow_move_assignable_v<std::vector<AES::byte_device>>
        and std::is_nothrow_move_assignable_v<std::vector<std::array<integer, AES::byte_device::key_length>>>)
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_rsa = std::move(origin.MEMBER_rsa);
            this->MEMBER_aes = std::move(origin.MEMBER_aes);
            this->MEMBER_crypt_aes = std::move(origin.MEMBER_crypt_aes);
        }
    }
    void cipher::encrypt(path_type const & origin_route, path_type const & result_route, path_type const & temp_route, const std::size_t thread_count)const
    {
        namespace fs = std::filesystem;
        if (not fs::is_directory(temp_route))
        {
            std::string const hint = std::string("路径 ")
                + temp_route.string()
                + " 不是既存目录，无法作为临时目录使用";
            throw error(std::invalid_argument(hint));
        }
        AES::file_device TEMP_cipher;

        if (this->MEMBER_aes.size() == 1)
        {
            TEMP_cipher.encrypt(origin_route, result_route, this->MEMBER_aes[0].key(), thread_count);
            return;
        }
        std::random_device random_seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(random_seed());                      // 随机数生成器
        std::uniform_int_distribution<std::uintmax_t> distribution(std::numeric_limits<std::uintmax_t>::min(), std::numeric_limits<std::uintmax_t>::max()); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间


        path_type TEMP_route = temp_route / std::to_string(distribution(random_engine));
        while (fs::exists(TEMP_route))
        {
            TEMP_route = temp_route / std::to_string(distribution(random_engine));
        }
        TEMP_cipher.encrypt(origin_route, TEMP_route, this->MEMBER_aes[0].key(), thread_count);
        for (std::size_t count_1 = 1;count_1 < this->MEMBER_aes.size() - 1;count_1++)
        {
            path_type TEMP_receiver = temp_route / std::to_string(distribution(random_engine));
            while (fs::exists(TEMP_receiver))
            {
                TEMP_receiver = temp_route / std::to_string(distribution(random_engine));
            }
            TEMP_cipher.encrypt(TEMP_route, TEMP_receiver, this->MEMBER_aes[count_1].key(), thread_count);
            fs::remove(TEMP_route);
            TEMP_route = TEMP_receiver;
        }
        TEMP_cipher.encrypt(TEMP_route, result_route, this->MEMBER_aes[this->MEMBER_aes.size() - 1].key(), thread_count);
        fs::remove(TEMP_route);

    }
    void cipher::decrypt(path_type const & origin_route, path_type const & result_route, path_type const & temp_route, const std::size_t thread_count)const
    {
        namespace fs = std::filesystem;
        if (not fs::is_directory(temp_route))
        {
            std::string const hint = std::string("路径 ")
                + temp_route.string()
                + " 不是既存目录，无法作为临时目录使用";
            throw error(std::invalid_argument(hint));
        }
        AES::file_device TEMP_cipher;

        if (this->MEMBER_aes.size() == 1)
        {
            TEMP_cipher.decrypt(origin_route, result_route, this->MEMBER_aes[0].key(), thread_count);
            return;
        }
        std::random_device random_seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(random_seed());                      // 随机数生成器
        std::uniform_int_distribution<std::uintmax_t> distribution(std::numeric_limits<std::uintmax_t>::min(), std::numeric_limits<std::uintmax_t>::max()); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间

        path_type TEMP_route = temp_route / std::to_string(distribution(random_engine));
        while (fs::exists(TEMP_route))
        {
            TEMP_route = temp_route / std::to_string(distribution(random_engine));
        }
        TEMP_cipher.decrypt(origin_route, TEMP_route, this->MEMBER_aes[this->MEMBER_aes.size() - 1].key(), thread_count);
        for (std::size_t count_1 = 1;count_1 < this->MEMBER_aes.size() - 1;count_1++)
        {
            path_type TEMP_receiver = temp_route / std::to_string(distribution(random_engine));
            while (fs::exists(TEMP_receiver))
            {
                TEMP_receiver = temp_route / std::to_string(distribution(random_engine));
            }
            TEMP_cipher.decrypt(TEMP_route, TEMP_receiver, this->MEMBER_aes[this->MEMBER_aes.size() - 1 - count_1].key(), thread_count);
            fs::remove(TEMP_route);
            TEMP_route = TEMP_receiver;
        }
        TEMP_cipher.decrypt(TEMP_route, result_route, this->MEMBER_aes[0].key(), thread_count);
        fs::remove(TEMP_route);

    }
    cipher::cipher(path_type const & route)
    {
        std::ifstream file;
        file.open(route, std::ios::in | std::ios::binary);
        if (not file.is_open())
        {
            std::string hint = "文件“";
            hint += route.string();
            hint += "”打开失败";
            throw error(std::runtime_error(hint));
        }
        std::string buffer;
        char reader;
        while (file.read(&reader, 1))
        {
            buffer.push_back(reader);
        }
        file.close();
        auto const data = field::parse(buffer);
        if (data.size() <= 3)
        {
            std::string hint = "密钥文件“";
            hint += route.string();
            hint += "”格式错误";
            throw error(std::invalid_argument(hint));
        }
        this->MEMBER_rsa = RSA::byte_device(integer(data[0]), integer(data[1]), integer(data[2]));
        for (std::size_t count_1 = 3;count_1 < data.size();count_1 += AES::byte_device::key_length)
        {
            std::array<integer, AES::byte_device::key_length> temp_aes_key;
            for (std::size_t count_2 = 0;count_2 < AES::byte_device::key_length;count_2++)
            {
                if (count_1 + count_2 >= data.size())
                {
                    std::string hint = "密钥文件“";
                    hint += route.string();
                    hint += "”格式错误";
                    throw error(std::invalid_argument(hint));
                }
                temp_aes_key[count_2] = integer(data[count_1 + count_2]);
            }
            this->MEMBER_crypt_aes.push_back(temp_aes_key);
        }
        this->MEMBER_aes.resize(this->MEMBER_crypt_aes.size());
        for (std::size_t count_1 = 0;count_1 < this->MEMBER_aes.size();count_1++)
        {
            AES::byte_device::key_type temp_aes_key;
            for (std::size_t count_2 = 0;count_2 < AES::byte_device::key_length;count_2++)
            {
                auto temp_real_aes_key = this->MEMBER_rsa->decrypt(this->MEMBER_crypt_aes[count_1][count_2]);
                if (temp_real_aes_key.sign() == integer::sign_type::zero)
                {
                    temp_aes_key[count_2] = static_cast<std::byte>(static_cast<std::underlying_type_t<std::byte>>(0));
                }
                else
                {
                    temp_aes_key[count_2] = static_cast<std::byte>(static_cast<std::underlying_type_t<std::byte>>(temp_real_aes_key.number()[0]));
                }
            }
            this->MEMBER_aes[count_1] = temp_aes_key;
        }

    }
    bool cipher::operator ==(cipher const & right)const
        noexcept(noexcept(std::declval<RSA::byte_device const &>() == std::declval<RSA::byte_device const &>())
        and noexcept(std::declval<std::vector<AES::byte_device> const &>() == std::declval<std::vector<AES::byte_device> const &>()))
    {
        if (this->MEMBER_rsa == right.MEMBER_rsa and this->MEMBER_aes == right.MEMBER_aes)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool cipher::operator !=(cipher const & right)const
        noexcept(noexcept(std::declval<cipher const &>() == std::declval<cipher const &>()))
    {
        return not((*this) == right);
    }
    void cipher::save(path_type const & route)const
    {
        /*
        密钥文件的结构
        用std::size_t表示一段数据的长度，后接数据，称为一个字段
        字段和字段紧密衔接组成文件
        字段的顺序是：rsa密钥，rsa公钥，rsa私钥，aes密钥
        */
        std::string result
            = this->MEMBER_rsa->key().field().field_string()
            + this->MEMBER_rsa->public_key().field().field_string()
            + this->MEMBER_rsa->private_key().field().field_string();

        for (std::size_t count_1 = 0;count_1 < this->MEMBER_crypt_aes.size();count_1++)
        {
            for (std::size_t count_2 = 0;count_2 < this->MEMBER_crypt_aes[count_1].size();count_2++)
            {
                result += this->MEMBER_crypt_aes[count_1][count_2].field().field_string();
            }
        }
        std::ofstream file;
        file.open(route, std::ios::binary | std::ios::out | std::ios::trunc);
        if (not file.is_open())
        {
            std::string hint = "文件“";
            hint += route.string();
            hint += "”打开失败";
            throw error(std::runtime_error(hint));
        }
        if (not file.write(result.c_str(), result.size()))
        {
            std::string hint = "文件“";
            hint += route.string();
            hint += "”写入失败";
            file.close();
            throw error(std::runtime_error(hint));
        }
        file.close();
    }

    password_generator::flag_type::flag_type()noexcept
        :flag_type(false, false, false, false)
    {
    }
    password_generator::flag_type::~flag_type()noexcept
    {
    }
    password_generator::flag_type::flag_type(flag_type const & origin)noexcept
        :MEMBER_Arabic_numerals(origin.MEMBER_Arabic_numerals)
        , MEMBER_capital_English_letter(origin.MEMBER_capital_English_letter)
        , MEMBER_English_letter(origin.MEMBER_English_letter)
        , MEMBER_special_character(origin.MEMBER_special_character)
    {
    }
    void password_generator::flag_type::operator =(flag_type const & origin)noexcept
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_Arabic_numerals = origin.MEMBER_Arabic_numerals;
            this->MEMBER_capital_English_letter = origin.MEMBER_capital_English_letter;
            this->MEMBER_English_letter = origin.MEMBER_English_letter;
            this->MEMBER_special_character = origin.MEMBER_special_character;
        }
    }
    password_generator::flag_type::flag_type(flag_type && origin) noexcept
        :flag_type(static_cast<flag_type const &>(origin))
    {
    }
    void password_generator::flag_type::operator =(flag_type && origin) noexcept
    {
        (*this) = static_cast<flag_type const &>(origin);
    }
    password_generator::flag_type::flag_type(
        bool const FLAG_Arabic_numerals,
        bool const FLAG_capital_English_letter,
        bool const FLAG_English_letter,
        bool const FLAG_special_character
    )noexcept
        :MEMBER_Arabic_numerals(FLAG_Arabic_numerals)
        , MEMBER_capital_English_letter(FLAG_capital_English_letter)
        , MEMBER_English_letter(FLAG_English_letter)
        , MEMBER_special_character(FLAG_special_character)
    {
    }
    bool const & password_generator::flag_type::Arabic_numerals()const noexcept
    {
        return this->MEMBER_Arabic_numerals;
    }
    bool const & password_generator::flag_type::capital_English_letter()const noexcept
    {
        return this->MEMBER_capital_English_letter;
    }
    bool const & password_generator::flag_type::English_letter()const noexcept
    {
        return this->MEMBER_English_letter;
    }
    bool const & password_generator::flag_type::special_character()const noexcept
    {
        return this->MEMBER_special_character;
    }
    bool & password_generator::flag_type::Arabic_numerals()noexcept
    {
        return this->MEMBER_Arabic_numerals;
    }
    bool & password_generator::flag_type::capital_English_letter()noexcept
    {
        return this->MEMBER_capital_English_letter;
    }
    bool & password_generator::flag_type::English_letter()noexcept
    {
        return this->MEMBER_English_letter;
    }
    bool & password_generator::flag_type::special_character()noexcept
    {
        return this->MEMBER_special_character;
    }
    class password_generator::implement final
    {
    public:
        implement() = delete;
        ~implement() = delete;
        implement(implement const &) = delete;
        void operator =(implement const &) = delete;
        implement(implement &&) = delete;
        void operator =(implement &&) = delete;

        static std::string Arabic_numerals()
        {
            std::string result;
            result.resize(10);
            for (std::size_t i = 0;i < 10;++i)
            {
                result[i] = '0' + i;
            }
            return result;
        }
        static std::string capital_English_letter()
        {
            std::string result;
            result.resize(26);
            for (std::size_t i = 0;i < 26;++i)
            {
                result[i] = 'A' + i;
            }
            return result;
        }
        static std::string English_letter()
        {
            std::string result;
            result.resize(26);
            for (std::size_t i = 0;i < 26;++i)
            {
                result[i] = 'a' + i;
            }
            return result;
        }
        static std::string special_character()
        {
            std::string result;
            result.resize(32);
            for (std::size_t i = 0;i < 15;++i)
            {
                result[i] = '!' + i;
            }
            for (std::size_t i = 0;i < 7;++i)
            {
                result[i + 15] = ':' + i;
            }
            for (std::size_t i = 0;i < 6;++i)
            {
                result[i + 22] = '[' + i;
            }
            for (std::size_t i = 0;i < 4;++i)
            {
                result[i + 28] = '{' + i;
            }
            return result;
        }
    };
    std::string const password_generator::Arabic_numerals = implement::Arabic_numerals();
    std::string const password_generator::capital_English_letter = implement::capital_English_letter();
    std::string const password_generator::English_letter = implement::English_letter();
    std::string const password_generator::special_character = implement::special_character();

    password_generator::password_generator
    (std::size_t const length_lower,//密码长度下限
    std::size_t const length_upper,//密码长度上限
    /*
    要从生成的密码中排除的子串
    例如生日、姓名、电话号码、邮箱等敏感信息
    */
    std::vector<std::string> const & eliminate_list,
    //要从基本ASCII字符中排除哪些类型的字符
    flag_type const & flag
    )
        :MEMBER_length_lower(length_lower)
        , MEMBER_length_upper(length_upper)
        , MEMBER_eliminate_list(eliminate_list)
        , MEMBER_valid_character_set()
    {
        if (length_lower > length_upper)
        {
            this->MEMBER_length_upper = length_lower;
            this->MEMBER_length_lower = length_upper;
        }
        if (not flag.Arabic_numerals())
        {
            this->MEMBER_valid_character_set += password_generator::Arabic_numerals;
        }
        if (not flag.capital_English_letter())
        {
            this->MEMBER_valid_character_set += password_generator::capital_English_letter;
        }
        if (not flag.English_letter())
        {
            this->MEMBER_valid_character_set += password_generator::English_letter;
        }
        if (not flag.special_character())
        {
            this->MEMBER_valid_character_set += password_generator::special_character;
        }
        if (this->MEMBER_valid_character_set.size() == 0)
        {
            throw error(std::invalid_argument("此生成器中没有任何字符"));
        }
    }
    password_generator::~password_generator()noexcept
    {
    }
    password_generator::password_generator(password_generator && origin)noexcept
        :MEMBER_length_lower(origin.MEMBER_length_lower)
        , MEMBER_length_upper(origin.MEMBER_length_upper)
        , MEMBER_eliminate_list(std::move(origin.MEMBER_eliminate_list))
        , MEMBER_valid_character_set(std::move(origin.MEMBER_valid_character_set))
    {
    }
    void password_generator::operator =(password_generator && origin)noexcept
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_length_lower = origin.MEMBER_length_lower;
            this->MEMBER_length_upper = origin.MEMBER_length_upper;
            this->MEMBER_eliminate_list = std::move(origin.MEMBER_eliminate_list);
            this->MEMBER_valid_character_set = std::move(origin.MEMBER_valid_character_set);
        }
    }
    password_generator::password_generator(password_generator const & origin)
        :MEMBER_length_lower(origin.MEMBER_length_lower)
        , MEMBER_length_upper(origin.MEMBER_length_upper)
        , MEMBER_eliminate_list(origin.MEMBER_eliminate_list)
        , MEMBER_valid_character_set(origin.MEMBER_valid_character_set)
    {
    }
    void password_generator::operator =(password_generator const & origin)
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_length_lower = origin.MEMBER_length_lower;
            this->MEMBER_length_upper = origin.MEMBER_length_upper;
            this->MEMBER_eliminate_list = origin.MEMBER_eliminate_list;
            this->MEMBER_valid_character_set = origin.MEMBER_valid_character_set;
        }
    }

    std::string password_generator::generate()const
    {
        std::string result;

        std::random_device seed;
        std::mt19937_64 engine(seed());
        std::uniform_int_distribution<std::size_t> generator(0, this->MEMBER_valid_character_set.size() - 1);
        std::size_t length;
        {
            std::uniform_int_distribution<std::size_t> length_generator(this->MEMBER_length_lower, this->MEMBER_length_upper);
            length = length_generator(engine);
        }
        if (length == 0)
        {
            return result;
        }
        bool FLAG_done = false;
        while (not FLAG_done)
        {
            result += this->MEMBER_valid_character_set[generator(engine)];
            this->INNER_eliminate(result);
            if (result.size() == length)
            {
                FLAG_done = true;
            }
        }
        return result;
    }


    void password_generator::INNER_eliminate(std::string & content)const
    {
        for (std::size_t i = 0;i < this->MEMBER_eliminate_list.size();++i)
        {
            std::size_t position = content.find(this->MEMBER_eliminate_list[i]);
            while (position != std::string::npos)
            {
                content.erase(position, this->MEMBER_eliminate_list[i].size());
                position = content.find(this->MEMBER_eliminate_list[i], position);
            }
        }
    }


    password_pool::password_block::operator field()const
    {
        std::vector<field> TEMP;
        TEMP.resize(3);
        TEMP[0] = this->MEMBER_name;
        TEMP[1] = this->MEMBER_account;
        TEMP[2] = this->MEMBER_password;
        return field::connect<std::string>(TEMP);
    }
    password_pool::password_block::password_block(field const & data)
    {
        auto TEMP = field::parse(data.raw_string());
        if (TEMP.size() != 3)
        {
            throw error(std::invalid_argument("字段格式错误"));
        }
        this->MEMBER_name = std::move(TEMP[0].raw_string());
        this->MEMBER_account = std::move(TEMP[1].raw_string());
        this->MEMBER_password = std::move(TEMP[2].raw_string());
    }

    password_pool::password_block::password_block(
        std::string const & name,
        std::string const & account,
        std::string && password
    )
        :MEMBER_name(name)
        , MEMBER_account(account)
        , MEMBER_password(std::move(password))
    {
    }

    password_pool::password_block::password_block(
        std::string const & name,
        std::string const & account,
        std::string const & password
    )
        :MEMBER_name(name)
        , MEMBER_account(account)
        , MEMBER_password(password)
    {
    }

    password_pool::password_block::password_block(password_block const & origin)
        :MEMBER_name(origin.MEMBER_name)
        , MEMBER_account(origin.MEMBER_account)
        , MEMBER_password(origin.MEMBER_password)
    {
    }
    password_pool::password_block::password_block(password_block && origin)noexcept
        :MEMBER_name(std::move(origin.MEMBER_name))
        , MEMBER_account(std::move(origin.MEMBER_account))
        , MEMBER_password(std::move(origin.MEMBER_password))
    {
    }
    void password_pool::password_block::operator =(password_block const & origin)
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_name = origin.MEMBER_name;
            this->MEMBER_account = origin.MEMBER_account;
            this->MEMBER_password = origin.MEMBER_password;
        }
    }
    void password_pool::password_block::operator =(password_block && origin)noexcept
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_name = std::move(origin.MEMBER_name);
            this->MEMBER_account = std::move(origin.MEMBER_account);
            this->MEMBER_password = std::move(origin.MEMBER_password);
        }
    }
    password_pool::password_block::~password_block()noexcept
    {
    }

    std::string const & password_pool::password_block::name()const noexcept
    {
        return this->MEMBER_name;
    }
    std::string const & password_pool::password_block::account()const noexcept
    {
        return this->MEMBER_account;
    }
    std::string const & password_pool::password_block::password()const noexcept
    {
        return this->MEMBER_password;
    }
    std::string & password_pool::password_block::name() noexcept
    {
        return this->MEMBER_name;
    }
    std::string & password_pool::password_block::account() noexcept
    {
        return this->MEMBER_account;
    }
    std::string & password_pool::password_block::password() noexcept
    {
        return this->MEMBER_password;
    }


    password_pool::password_pool(
    std::filesystem::path const & key_file,
    std::filesystem::path const & storage_file,
    std::filesystem::path const & temporary_directory
    )
        :MEMBER_key_file(std::filesystem::absolute(key_file))
        , MEMBER_storage_file(std::filesystem::absolute(storage_file))
        , MEMBER_temporary_directory(std::filesystem::absolute(temporary_directory))
        , MEMBER_temporary_flag(false)
    {
        auto const storage_file_status = std::filesystem::symlink_status(this->MEMBER_storage_file);
        auto const key_file_status = std::filesystem::symlink_status(this->MEMBER_key_file);

        auto const TEMP_directory_status = std::filesystem::symlink_status(this->MEMBER_temporary_directory);
        if (TEMP_directory_status.type() == std::filesystem::file_type::not_found)
        {
            this->MEMBER_temporary_flag = true;
            if (not std::filesystem::create_directory(this->MEMBER_temporary_directory))
            {
                std::string const hint = std::string("创建临时目录 ") + this->MEMBER_temporary_directory.string() + " 失败";
                throw error(std::runtime_error(hint));
            }
        }
        else if (TEMP_directory_status.type() != std::filesystem::file_type::directory)
        {
            std::string const hint = std::string("路径 ") + this->MEMBER_temporary_directory.string() + " 存在且不是目录，创建临时目录失败";
            throw error(std::runtime_error(hint));
        }
        if (key_file_status.type() == std::filesystem::file_type::not_found)
        {
            //密钥文件不存在

            if (storage_file_status.type() == std::filesystem::file_type::regular)
            {
                //数据文件存在

                std::string const hint = std::string("密钥文件 ") + this->MEMBER_key_file.string() + " 丢失";
                throw error(std::invalid_argument(hint));
            }
            else if (storage_file_status.type() != std::filesystem::file_type::not_found)
            {
                //数据文件路径存在，但不是常规文件

                std::string const hint = std::string("路径 ") + this->MEMBER_storage_file.string() + " 不是常规文件";
                throw error(std::invalid_argument(hint));
            }
        }
        else if (key_file_status.type() == std::filesystem::file_type::regular)
        {
            //密钥文件存在

            this->MEMBER_cipher = cipher(this->MEMBER_key_file);
            if (storage_file_status.type() == std::filesystem::file_type::regular)
            {
                //数据文件存在

                password_generator const TEMP_path_generator(16, 16, std::vector<std::string>(), password_generator::flag_type(false, false, false, true));
                std::filesystem::path TEMP_data_file = this->MEMBER_temporary_directory / TEMP_path_generator.generate();
                while (std::filesystem::exists(TEMP_data_file))
                {
                    TEMP_data_file = this->MEMBER_temporary_directory / TEMP_path_generator.generate();
                }
                this->MEMBER_cipher->decrypt(storage_file, TEMP_data_file, this->MEMBER_temporary_directory);
                std::ifstream TEMP_reader;
                TEMP_reader.open(TEMP_data_file, std::ios::in | std::ios::binary);
                if (not TEMP_reader.is_open())
                {
                    std::filesystem::remove(TEMP_data_file);
                    std::string const hint = std::string("读取临时数据文件 ") + TEMP_data_file.string() + " 失败";
                    throw error(std::runtime_error(hint));
                }
                auto const TEMP_begin = TEMP_reader.tellg();
                TEMP_reader.seekg(0, std::ios::end);
                auto const TEMP_end = TEMP_reader.tellg();
                TEMP_reader.seekg(0, std::ios::beg);
                std::string TEMP_data_buffer;
                TEMP_data_buffer.resize(TEMP_end - TEMP_begin);
                if (not TEMP_reader.read(TEMP_data_buffer.data(), TEMP_data_buffer.size()))
                {
                    TEMP_reader.close();
                    std::filesystem::remove(TEMP_data_file);
                    std::string const hint = std::string("读取临时数据文件 ") + TEMP_data_file.string() + " 失败";
                    throw error(std::runtime_error(hint));
                }
                TEMP_reader.close();
                std::filesystem::remove(TEMP_data_file);
                auto const TEMP_data_result = field::parse(TEMP_data_buffer);
                TEMP_data_buffer.resize(0);
                this->MEMBER_pool.resize(TEMP_data_result.size());
                for (std::size_t i = 0;i < this->MEMBER_pool.size();++i)
                {
                    this->MEMBER_pool[i] = TEMP_data_result[i];
                }
            }
            else if (storage_file_status.type() != std::filesystem::file_type::not_found)
            {
                //数据文件路径存在，但不是常规文件

                std::string const hint = std::string("路径 ") + this->MEMBER_storage_file.string() + " 不是常规文件";
                throw error(std::invalid_argument(hint));
            }
        }
        else
        {
            //密钥文件路径存在，但不是常规文件

            std::string const hint = std::string("路径 ") + this->MEMBER_key_file.string() + " 不是常规文件";
            throw error(std::invalid_argument(hint));
        }
    }
    password_pool::password_block::password_block()noexcept
    {
    }
    password_pool::~password_pool()noexcept(false)
    {
        password_generator const TEMP_path_generator(16, 16, std::vector<std::string>(), password_generator::flag_type(false, false, false, true));
        std::filesystem::path TEMP_data_file = this->MEMBER_temporary_directory / TEMP_path_generator.generate();
        while (std::filesystem::exists(TEMP_data_file))
        {
            TEMP_data_file = this->MEMBER_temporary_directory / TEMP_path_generator.generate();
        }
        std::ofstream TEMP_writter;
        TEMP_writter.open(TEMP_data_file, std::ios::out | std::ios::binary | std::ios::trunc);
        if (not TEMP_writter.is_open())
        {
            std::string const hint = std::string("写入临时数据文件 ") + TEMP_data_file.string() + " 失败";
            throw error(std::runtime_error(hint));
        }
        std::vector<field> TEMP_data_holder;
        TEMP_data_holder.resize(this->MEMBER_pool.size());
        for (std::size_t i = 0;i < TEMP_data_holder.size();++i)
        {
            TEMP_data_holder[i] = this->MEMBER_pool[i];
        }
        auto TEMP_data_buffer = field::connect<std::string>(TEMP_data_holder);
        TEMP_data_holder.resize(0);
        if (not TEMP_writter.write(TEMP_data_buffer.data(), TEMP_data_buffer.size()))
        {
            TEMP_writter.close();
            std::filesystem::remove(TEMP_data_file);
            std::string const hint = std::string("写入临时数据文件 ") + TEMP_data_file.string() + " 失败";
            throw error(std::runtime_error(hint));
        }
        TEMP_data_buffer.resize(0);
        TEMP_writter.close();
        if (not this->MEMBER_cipher)
        {
            this->MEMBER_cipher = cipher();
            this->MEMBER_cipher->save(this->MEMBER_key_file);
        }
        this->MEMBER_cipher->encrypt(TEMP_data_file, this->MEMBER_storage_file, this->MEMBER_temporary_directory);
        std::filesystem::remove(TEMP_data_file);
        if (this->MEMBER_temporary_flag)
        {
            std::filesystem::remove(this->MEMBER_temporary_directory);
        }
    }
    password_pool::password_pool(password_pool && origin) noexcept
        :MEMBER_pool(std::move(origin.MEMBER_pool))
        , MEMBER_key_file(std::move(origin.MEMBER_key_file))
        , MEMBER_storage_file(std::move(origin.MEMBER_storage_file))
        , MEMBER_cipher(std::move(origin.MEMBER_cipher))
        , MEMBER_temporary_directory(std::move(origin.MEMBER_temporary_directory))
        , MEMBER_temporary_flag(origin.MEMBER_temporary_flag)
    {
    }
    void password_pool::operator =(password_pool && origin)noexcept
    {
        if (this != std::addressof(origin))
        {
            this->MEMBER_pool = std::move(origin.MEMBER_pool);
            this->MEMBER_key_file = std::move(origin.MEMBER_key_file);
            this->MEMBER_storage_file = std::move(origin.MEMBER_storage_file);
            this->MEMBER_cipher = std::move(origin.MEMBER_cipher);
            this->MEMBER_temporary_directory = std::move(origin.MEMBER_temporary_directory);
            this->MEMBER_temporary_flag = origin.MEMBER_temporary_flag;
        }
    }
    void password_pool::clear()
    {
        this->MEMBER_pool.clear();
    }
    void password_pool::erase(std::size_t const position)
    {
        this->MEMBER_pool.erase(this->MEMBER_pool.begin() + position);
    }
    void password_pool::erase(std::size_t const begin_position, std::size_t const end_position)
    {
        std::size_t lower_position = begin_position;
        std::size_t upper_position = end_position;
        if (begin_position > end_position)
        {
            lower_position = end_position;
            upper_position = begin_position;
        }
        this->MEMBER_pool.erase(this->MEMBER_pool.begin() + lower_position, this->MEMBER_pool.begin() + (upper_position + 1));
    }
    void password_pool::add(
    std::string const & name,
    std::string const & account,
    password_generator const & password
    )
    {
        this->MEMBER_pool.push_back(password_block(name, account, password.generate()));
    }
    void password_pool::add(
    std::string const & name,
    std::string const & account,
    std::string const & password
    )
    {
        this->MEMBER_pool.push_back(password_block(name, account, password));
    }
    password_pool::password_block const & password_pool::operator [](std::size_t const position)const noexcept
    {
        return this->MEMBER_pool[position];
    }
    password_pool::password_block & password_pool::operator [](std::size_t const position) noexcept
    {
        return this->MEMBER_pool[position];
    }
    std::size_t password_pool::size()const noexcept
    {
        return this->MEMBER_pool.size();
    }


}
