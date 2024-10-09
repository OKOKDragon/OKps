#include <random>

#include ".\cipher.hpp"

namespace OKps::crypt
{

    cipher::cipher(RSA::byte_device && rsa, std::uintmax_t const aes_count)
        :MEMBER_rsa(std::make_unique<RSA::byte_device>(std::move(rsa)))
    {
        if (not this->MEMBER_rsa->is_valid())
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("输入的RSA密钥不可用");
        }
        if (aes_count < 1)
        {
            std::locale::global(std::locale::classic());
            throw std::invalid_argument("aes密钥数量至少为1");
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
    cipher::~cipher()noexcept
    {
    }
    cipher::cipher(cipher const & origin)
        :MEMBER_rsa(std::make_unique<RSA::byte_device>(*(origin.MEMBER_rsa)))
        , MEMBER_aes(origin.MEMBER_aes)
        , MEMBER_crypt_aes(origin.MEMBER_crypt_aes)
    {
    }
    void cipher::operator =(cipher const & origin)
    {
        if (this != (&origin))
        {
            this->MEMBER_rsa = std::make_unique<RSA::byte_device>(*(origin.MEMBER_rsa));
            this->MEMBER_aes = origin.MEMBER_aes;
            this->MEMBER_crypt_aes = origin.MEMBER_crypt_aes;
        }
    }

    cipher::cipher(cipher && origin)noexcept
        :MEMBER_rsa(std::move(origin.MEMBER_rsa))
        , MEMBER_aes(std::move(origin.MEMBER_aes))
        , MEMBER_crypt_aes(std::move(origin.MEMBER_crypt_aes))
    {
    }
    void cipher::operator =(cipher && origin)noexcept
    {
        if (this != (&origin))
        {
            this->MEMBER_rsa = std::move(origin.MEMBER_rsa);
            this->MEMBER_aes = std::move(origin.MEMBER_aes);
            this->MEMBER_crypt_aes = std::move(origin.MEMBER_crypt_aes);
        }
    }
    void cipher::encrypt(TYPE_path const & origin_route, TYPE_path const & result_route, TYPE_path const & temp_route, const std::size_t thread_count)const
    {
        if (not this->MEMBER_rsa)
        {
            std::locale::global(std::locale::classic());
            throw std::logic_error("此密钥已失效，禁止访问");
        }
        namespace fs = std::filesystem;
        if (not fs::is_directory(temp_route))
        {
            std::locale::global(std::locale::classic());
            std::string const hint = std::string("路径 ")
                + temp_route.string()
                + " 不是既存目录，无法作为临时目录使用";
            throw std::invalid_argument(hint);
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


        TYPE_path TEMP_route = temp_route / std::to_string(distribution(random_engine));
        while (fs::exists(TEMP_route))
        {
            TEMP_route = temp_route / std::to_string(distribution(random_engine));
        }
        TEMP_cipher.encrypt(origin_route, TEMP_route, this->MEMBER_aes[0].key(), thread_count);
        for (std::size_t count_1 = 1;count_1 < this->MEMBER_aes.size() - 1;count_1++)
        {
            TYPE_path TEMP_receiver = temp_route / std::to_string(distribution(random_engine));
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
    void cipher::decrypt(TYPE_path const & origin_route, TYPE_path const & result_route, TYPE_path const & temp_route, const std::size_t thread_count)const
    {
        if (not this->MEMBER_rsa)
        {
            std::locale::global(std::locale::classic());
            throw std::logic_error("此密钥已失效，禁止访问");
        }
        namespace fs = std::filesystem;
        if (not fs::is_directory(temp_route))
        {
            std::locale::global(std::locale::classic());
            std::string const hint = std::string("路径 ")
                + temp_route.string()
                + " 不是既存目录，无法作为临时目录使用";
            throw std::invalid_argument(hint);
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

        TYPE_path TEMP_route = temp_route / std::to_string(distribution(random_engine));
        while (fs::exists(TEMP_route))
        {
            TEMP_route = temp_route / std::to_string(distribution(random_engine));
        }
        TEMP_cipher.decrypt(origin_route, TEMP_route, this->MEMBER_aes[this->MEMBER_aes.size() - 1].key(), thread_count);
        for (std::size_t count_1 = 1;count_1 < this->MEMBER_aes.size() - 1;count_1++)
        {
            TYPE_path TEMP_receiver = temp_route / std::to_string(distribution(random_engine));
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
    cipher::cipher(TYPE_path const & route)
    {
        std::ifstream file;
        file.open(route, std::ios::in | std::ios::binary);
        if (not file.is_open())
        {
            std::locale::global(std::locale::classic());
            std::string hint = "文件“";
            hint += route.string();
            hint += "”打开失败";
            throw std::runtime_error(hint);
        }
        std::string buffer;
        char reader;
        while (file.read(&reader, 1))
        {
            buffer.push_back(reader);
        }
        file.close();
        auto const data = field_stream::parse(buffer);
        if (data.size() <= 3)
        {
            std::locale::global(std::locale::classic());
            std::string hint = "密钥文件“";
            hint += route.string();
            hint += "”格式错误";
            throw std::invalid_argument(hint);
        }
        this->MEMBER_rsa = std::make_unique<RSA::byte_device>(integer(data[0]), integer(data[1]), integer(data[2]));
        for (std::size_t count_1 = 3;count_1 < data.size();count_1 += AES::byte_device::key_length)
        {
            std::array<integer, AES::byte_device::key_length> temp_aes_key;
            for (std::size_t count_2 = 0;count_2 < AES::byte_device::key_length;count_2++)
            {
                if (count_1 + count_2 >= data.size())
                {
                    std::locale::global(std::locale::classic());
                    std::string hint = "密钥文件“";
                    hint += route.string();
                    hint += "”格式错误";
                    throw std::invalid_argument(hint);
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
                    temp_aes_key[count_2] = (std::byte)0;
                }
                else
                {
                    temp_aes_key[count_2] = (std::byte)((std::underlying_type_t<std::byte>)(temp_real_aes_key.number()[0]));
                }
            }
            this->MEMBER_aes[count_1] = temp_aes_key;
        }

    }
    bool cipher::operator ==(cipher const & right)const
    {
        if (not (this->MEMBER_rsa and right.MEMBER_rsa))
        {
            std::locale::global(std::locale::classic());
            throw std::runtime_error("密钥已失效，禁止访问");
        }
        if ((*(this->MEMBER_rsa)) == (*(right.MEMBER_rsa)) and this->MEMBER_aes == right.MEMBER_aes)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool cipher::operator !=(cipher const & right)const
    {
        return not(*this == right);
    }
    void cipher::save(TYPE_path const & route)const
    {
        if (not this->MEMBER_rsa)
        {
            std::locale::global(std::locale::classic());
            throw std::logic_error("此密钥已失效，禁止访问");
        }
        /*
        密钥文件的结构
        用std::size_t表示一段数据的长度，后接数据，称为一个字段
        字段和字段紧密衔接组成文件
        字段的顺序是：rsa密钥，rsa公钥，rsa私钥，aes密钥
        */
        std::string result
            = this->MEMBER_rsa->key().field_stream().field_string()
            + this->MEMBER_rsa->public_key().field_stream().field_string()
            + this->MEMBER_rsa->private_key().field_stream().field_string();

        for (std::size_t count_1 = 0;count_1 < this->MEMBER_crypt_aes.size();count_1++)
        {
            for (std::size_t count_2 = 0;count_2 < this->MEMBER_crypt_aes[count_1].size();count_2++)
            {
                result += this->MEMBER_crypt_aes[count_1][count_2].field_stream().field_string();
            }
        }
        std::ofstream file;
        file.open(route, std::ios::binary | std::ios::out | std::ios::trunc);
        if (not file.is_open())
        {
            std::locale::global(std::locale::classic());
            std::string hint = "文件“";
            hint += route.string();
            hint += "”打开失败";
            throw std::runtime_error(hint);
        }
        if (not file.write(result.c_str(), result.size()))
        {
            std::locale::global(std::locale::classic());
            std::string hint = "文件“";
            hint += route.string();
            hint += "”写入失败";
            file.close();
            throw std::runtime_error(hint);
        }
        file.close();
    }
}