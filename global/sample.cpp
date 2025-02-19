
#include <fstream>
#include <random>

#include ".\string.hpp"
#include ".\timer.hpp"
#include ".\cipher.hpp"

#include ".\sample.hpp"

namespace OKps::sample
{
    void erase_duplicate_files(std::filesystem::path const & dir)
    {
        directory_tree file_tree(dir);
        class list_type final
        {
        public:
            using group_type = std::set<std::filesystem::path const *>;
            using storage_type = std::vector<group_type>;
        private:
            storage_type MEMBER_storage;
            //std::set<group_type *> MEMBER_list;
        public:
            list_type()
            {
            }
            ~list_type()noexcept
            {
            }
            list_type(list_type const &) = delete;
            void operator =(list_type const &) = delete;
            list_type(list_type && origin)noexcept
                :MEMBER_storage(std::move(origin.MEMBER_storage))
            {
            }
            void operator =(list_type && origin)noexcept
            {
                this->MEMBER_storage = std::move(origin.MEMBER_storage);
            }
            group_type const * find_group(std::filesystem::path const & value)const
            {
                group_type const * result = nullptr;
                for (std::size_t i = 0;i < this->MEMBER_storage.size();++i)
                {
                    if (this->MEMBER_storage[i].find(&value) != this->MEMBER_storage[i].end())
                    {
                        result = (&(this->MEMBER_storage[i]));
                        break;
                    }
                }
                return result;
            }
            group_type * find_group(std::filesystem::path const & value)
            {
                group_type * result = nullptr;
                for (std::size_t i = 0;i < this->MEMBER_storage.size();++i)
                {
                    if (this->MEMBER_storage[i].find(&value) != this->MEMBER_storage[i].end())
                    {
                        result = (&(this->MEMBER_storage[i]));
                        break;
                    }
                }
                return result;
            }
            void insert(std::filesystem::path const & file_path_1, std::filesystem::path const & file_path_2)
            {
                group_type TEMP_new_group;
                TEMP_new_group.insert(&file_path_1);
                TEMP_new_group.insert(&file_path_2);
                this->MEMBER_storage.push_back(std::move(TEMP_new_group));
            }
            std::size_t size()const
            {
                return this->MEMBER_storage.size();
            }
            group_type const & operator [](std::size_t const position)const
            {
                return this->MEMBER_storage[position];
            }
            group_type & operator [](std::size_t const position)
            {
                return this->MEMBER_storage[position];
            }
        };
        class regular_file_judgement final : public OKps::directory_tree::path_judgement
        {
        private:
            using base_type = OKps::directory_tree::path_judgement;
        public:
            regular_file_judgement()noexcept
                : base_type()
            {
            }
            ~regular_file_judgement() noexcept override
            {
            }
            bool judge(std::filesystem::path const & file)noexcept override
            {
                return std::filesystem::is_regular_file(file);
            }
            regular_file_judgement(regular_file_judgement const &) = delete;
            regular_file_judgement(regular_file_judgement &&) = delete;
            void operator =(regular_file_judgement const &) = delete;
            void operator =(regular_file_judgement &&) = delete;
        };
        regular_file_judgement regular_judge;
        list_type same_list;
        auto const file_list = file_tree.root()->content(regular_judge);
        for (std::size_t i = 0;i < file_list.size();++i)
        {
            for (std::size_t j = 0;j < file_list.size();++j)
            {
                if (i != j)
                {
                    auto * const i_group = same_list.find_group(file_list[i]->path());
                    auto * const j_group = same_list.find_group(file_list[j]->path());
                    if ((not(i_group and j_group)) and compare(file_list[i]->path(), file_list[j]->path()))
                    {
                        if (i_group and (not j_group))
                        {
                            i_group->insert(&(file_list[j]->path()));
                        }
                        else if ((not i_group) and j_group)
                        {
                            j_group->insert(&(file_list[i]->path()));
                        }
                        else
                        {
                            same_list.insert(file_list[i]->path(), file_list[j]->path());
                        }
                    }
                }
            }
        }
        //OKps::wait_input();
        for (std::size_t i = 0;i < same_list.size();++i)
        {
            while (same_list[i].size() > 1)
            {
                auto const iterator = same_list[i].begin();
                std::filesystem::remove(*(*iterator));
                same_list[i].erase(iterator);
            }
        }
    }
    void test_file_output_speed(std::filesystem::path const & file_path, std::size_t const block_count, std::size_t const block_size)
    {
        if (std::filesystem::exists(file_path))
        {
            throw std::invalid_argument("此文件已存在");
        }
        OKps::time_monitor timer;
        std::random_device seed;
        std::mt19937_64 engine(seed());
        std::uniform_int_distribution<short> dis(std::numeric_limits<char>::min(), std::numeric_limits<char>::max());
        std::size_t const total_length = block_size * block_count;
        std::unique_ptr<char[]> data = std::make_unique<char[]>(total_length);
        for (std::size_t i = 0;i < total_length;++i)
        {
            data[i] = dis(engine);
        }
        std::ofstream file;
        file.open(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (not file.is_open())
        {
            throw std::runtime_error("无法打开文件");
        }
        timer.begin();
        for (std::size_t i = 0;i < block_count;++i)
        {
            file.write(std::addressof(data[i * block_size]), block_size);
        }
        auto const times_1 = timer.end<std::chrono::nanoseconds>();
        file.close();
        file.open(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (not file.is_open())
        {
            throw std::runtime_error("无法打开文件");
        }
        timer.begin();
        for (std::size_t i = 0;i < total_length;++i)
        {
            file.write(std::addressof(data[i]), 1);
        }
        auto const times_2 = timer.end<std::chrono::nanoseconds>();
        file.close();
        file.open(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (not file.is_open())
        {
            throw std::runtime_error("无法打开文件");
        }
        timer.begin();
        file.write(data.get(), total_length);
        auto const times_3 = timer.end<std::chrono::nanoseconds>();
        file.close();
        std::filesystem::remove(file_path);
        std::cout << "以块写入耗时" << times_1 << "，以字节写入耗时" << times_2 << "，一次性写入耗时" << times_3 << "\n";
        OKps::wait_input("");
    }

    void generate_cipher_key_files(std::filesystem::path const & holder_dir, std::uintmax_t const file_count)
    {
        namespace fs = std::filesystem;
        using std::chrono::nanoseconds;
        if (not fs::exists(holder_dir))
        {
            fs::create_directory(holder_dir);
        }
        time_monitor timer;
        std::random_device seed;
        std::mt19937_64 engine(seed());
        std::uniform_int_distribution<std::uintmax_t> generer(std::numeric_limits<std::uintmax_t>::min(), std::numeric_limits<std::uintmax_t>::max());
        for (std::uintmax_t i = 0;i < file_count;i++)
        {
            timer.begin();
            auto key = crypt::cipher();
            auto times = timer.end<nanoseconds>();
            std::cout << "生成密钥耗时" << times << "纳秒\n";
            timer.begin();

            fs::path route = holder_dir / std::to_string(generer(engine));
            while (fs::exists(route))
            {
                route = holder_dir / std::to_string(generer(engine));
            }
            key.save(route);
            times = timer.end<nanoseconds>();
            std::cout << "生成密钥文件 " << route.string() << " 耗时" << times << "纳秒\n";
            timer.begin();
            auto reader = crypt::cipher(route);
            times = timer.end<nanoseconds>();
            std::cout << "读取密钥文件 " << route.string() << " 耗时" << times << "纳秒\n";
            std::cout << "密钥文件 " << route.string() << " 生成";
            if (reader == key)
            {
                std::cout << "成功\n";
            }
            else
            {
                std::cout << "失败\n";
                wait_input();
            }
        }
    }
}