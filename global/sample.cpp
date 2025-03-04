
#include <fstream>
#include <random>

#include ".\string.hpp"
#include ".\timer.hpp"
#include ".\cipher.hpp"

#include ".\sample.hpp"

namespace OKps::sample
{

    void password_management(
        std::filesystem::path const & key_path,
        std::filesystem::path const & storage_path,
        std::filesystem::path const & temporary_directory
    )
    {
        crypt::password_pool pool(key_path, storage_path, temporary_directory);
        std::cout << "密码管理器\n";
        std::string command_input;
        bool FLAG_done = false;
        while (not FLAG_done)
        {
            std::cout << "输入命令 ││ ";
            std::getline(std::cin, command_input);
            command_statement const command(command_input);
            if (command.command() == "查询")
            {
                if (command.argument_list().size() == 0)
                {
                    if (pool.size() == 0)
                    {
                        std::cout << "查询结果 ││ 没有条目\n";
                    }
                    else
                    {
                        std::cout << "查询结果 ││ \n";
                        for (std::size_t i = 0;i < pool.size();++i)
                        {
                            std::cout << "条目[" << i << "]\n"
                                "名称 │ " << pool[i].name() << "\n"
                                "账号 │ " << pool[i].account() << "\n"
                                "密码 │ " << pool[i].password() << "\n";
                        }
                    }
                }
                else
                {
                    std::vector<std::size_t> entry_list;
                    entry_list.resize(command.argument_list().size());
                    bool failure = false;
                    for (std::size_t i = 0;(not failure) and i < entry_list.size();++i)
                    {
                        try
                        {
                            entry_list[i] = to_number<std::size_t>(command.argument_list()[i]);
                        }
                        catch (const std::exception & error)
                        {
                            failure = true;
                            std::cout << "参数错误 ││ “" << command.argument_list()[i] << "”不是有效非负整数\n";
                        }
                        if (entry_list[i] >= pool.size())
                        {
                            failure = true;
                            std::cout << "参数错误 ││ 条目[" << command.argument_list()[i] << "]不存在\n";
                        }
                    }
                    if (not failure)
                    {
                        std::cout << "查询结果 ││ \n";
                        for (std::size_t i = 0; i < entry_list.size();++i)
                        {
                            std::cout << "条目[" << entry_list[i] << "]\n"
                                "名称 │ " << pool[entry_list[i]].name() << "\n"
                                "账号 │ " << pool[entry_list[i]].account() << "\n"
                                "密码 │ " << pool[entry_list[i]].password() << "\n";
                        }
                    }
                }
            }
            else if (command.command() == "添加")
            {
                if (command.argument_list().size() < 4)
                {
                    std::cout << "参数错误 ││ 至少需要4个参数，其中参数1为名称，参数2为账号，参数3为生成密码的方式。\n";
                }
                else
                {
                    if (command.argument_list()[2] == "随机")
                    {
                        if (command.argument_list().size() < 6)
                        {
                            std::cout << "参数错误 ││ 至少需要6个参数。\n"
                                "参数4应为“是”或“否”，它决定生成的密码中是否含有特殊字符。特殊字符指的是“" << crypt::password_generator::special_character << "”。\n"
                                "参数5、参数6应为非负整数，它们决定生成的密码长度的范围。\n";
                        }
                        else
                        {
                            bool failure = false;
                            std::size_t lower_length;
                            std::size_t upper_length;
                            try
                            {
                                lower_length = to_number<std::size_t>(command.argument_list()[4]);
                            }
                            catch (const std::exception & error)
                            {
                                failure = true;
                                std::cout << "参数错误 ││ 参数5不是有效非负整数。\n";
                            }
                            if (not failure)
                            {
                                try
                                {
                                    upper_length = to_number<std::size_t>(command.argument_list()[5]);
                                }
                                catch (const std::exception & error)
                                {
                                    failure = true;
                                    std::cout << "参数错误 ││ 参数6不是有效非负整数。\n";
                                }
                            }
                            if (not failure)
                            {
                                std::optional<crypt::password_generator> TEMP_generator;
                                std::vector<std::string> TEMP_eliminate_list;
                                TEMP_eliminate_list.resize(command.argument_list().size() - 6);
                                for (std::size_t i = 0;i < TEMP_eliminate_list.size();++i)
                                {
                                    TEMP_eliminate_list[i] = command.argument_list()[i + 6];
                                }
                                if (command.argument_list()[3] == "是")
                                {
                                    TEMP_generator = crypt::password_generator(lower_length, upper_length, TEMP_eliminate_list, crypt::password_generator::flag_type());
                                    pool.add(command.argument_list()[0], command.argument_list()[1], *TEMP_generator);
                                }
                                else if (command.argument_list()[3] == "否")
                                {
                                    TEMP_generator = crypt::password_generator(lower_length, upper_length, TEMP_eliminate_list, crypt::password_generator::flag_type(false, false, false, true));
                                    pool.add(command.argument_list()[0], command.argument_list()[1], *TEMP_generator);
                                }
                                else
                                {
                                    std::cout << "参数错误 ││ 参数4应为“是”或“否”，它决定生成的密码中是否含有特殊字符。特殊字符指的是“" << crypt::password_generator::special_character << "”。\n";
                                }
                            }
                        }
                    }
                    else if (command.argument_list()[2] == "指定")
                    {
                        if (command.argument_list().size() > 4)
                        {
                            std::cout << "参数错误 ││ 参数太多。只需要4个参数，其中参数4是你指定的密码。\n";
                        }
                        else
                        {
                            pool.add(command.argument_list()[0], command.argument_list()[1], command.argument_list()[3]);
                        }
                    }
                    else
                    {
                        std::cout << "参数错误 ││ 参数3应为“随机”或“指定”\n";
                    }
                }
            }
            else if (command.command() == "修改")
            {
                if (command.argument_list().size() < 3)
                {
                    std::cout << "参数错误 ││ 至少有3个参数。其中参数1指定条目编号，参数2指定要修改的元素。\n";
                }
                else
                {
                    bool failure = false;
                    std::size_t entry_number;
                    try
                    {
                        entry_number = to_number<std::size_t>(command.argument_list()[0]);
                    }
                    catch (const std::exception & error)
                    {
                        failure = true;
                        std::cout << "参数错误 ││ “" << command.argument_list()[0] << "”不是有效非负整数\n";
                    }
                    if (not failure)
                    {
                        if (entry_number >= pool.size())
                        {
                            failure = true;
                            std::cout << "参数错误 ││ 条目[" << command.argument_list()[0] << "]不存在\n";
                        }
                    }
                    if (not failure)
                    {
                        if (command.argument_list()[1] == "名称")
                        {
                            if (command.argument_list().size() > 3)
                            {
                                failure = true;
                                std::cout << "参数错误 ││ 参数太多。应为3个参数，其中参数3是条目的新名称。\n";
                            }
                            if (not failure)
                            {
                                pool[entry_number].name() = command.argument_list()[2];
                            }
                        }
                        else if (command.argument_list()[1] == "账号")
                        {
                            if (command.argument_list().size() > 3)
                            {
                                failure = true;
                                std::cout << "参数错误 ││ 参数太多。应为3个参数，其中参数3是条目的新账号。\n";
                            }
                            if (not failure)
                            {
                                pool[entry_number].account() = command.argument_list()[2];
                            }
                        }
                        else if (command.argument_list()[1] == "密码")
                        {
                            std::cout << "错误 ││ 此功能尚未实现。\n";
                        }
                        else
                        {
                            failure = true;
                            std::cout << "参数错误 ││ 条目元素“" << command.argument_list()[1] << "”不存在\n";
                        }
                    }
                }
            }
            else if (command.command() == "删除")
            {
                if (command.argument_list().size() == 0)
                {
                    std::cout << "错误 ││ 没有指定要删除的条目。\n";
                }
                else if (command.argument_list().size() == 1)
                {
                    std::size_t entry_number;
                    bool failure = false;
                    try
                    {
                        entry_number = to_number<std::size_t>(command.argument_list()[0]);
                    }
                    catch (const std::exception & error)
                    {
                        failure = true;
                        std::cout << "参数错误 ││ “" << command.argument_list()[0] << "”不是有效非负整数\n";
                    }
                    if (not failure)
                    {
                        if (entry_number >= pool.size())
                        {
                            std::cout << "参数错误 ││ 条目[" << command.argument_list()[0] << "]不存在\n";
                        }
                        else
                        {
                            pool.erase(entry_number);
                        }
                    }
                }
                else if (command.argument_list().size() == 2)
                {
                    std::size_t entry_begin;
                    std::size_t entry_end;
                    bool failure = false;
                    try
                    {
                        entry_begin = to_number<std::size_t>(command.argument_list()[0]);
                    }
                    catch (const std::exception & error)
                    {
                        failure = true;
                        std::cout << "参数错误 ││ “" << command.argument_list()[0] << "”不是有效非负整数\n";
                    }
                    if (not failure)
                    {
                        try
                        {
                            entry_end = to_number<std::size_t>(command.argument_list()[1]);
                        }
                        catch (const std::exception & error)
                        {
                            failure = true;
                            std::cout << "参数错误 ││ “" << command.argument_list()[1] << "”不是有效非负整数\n";
                        }
                    }
                    if (not failure)
                    {
                        if (entry_begin > entry_end)
                        {
                            std::size_t TEMP_swap = entry_begin;
                            entry_begin = entry_end;
                            entry_end = TEMP_swap;
                        }
                        if (entry_end >= pool.size())
                        {
                            failure = true;
                            std::cout << "参数错误 ││ 闭区间范围[" << entry_begin << "," << entry_end << "]无效\n";
                        }
                    }
                    if (not failure)
                    {
                        pool.erase(entry_begin, entry_end);
                    }
                }
                else
                {
                    std::cout << "参数错误 ││ 参数太多。\n";
                }
            }
            else if (command.command() == "退出")
            {
                FLAG_done = true;
            }
            else
            {
                std::cout << "错误 ││ “" << command.command() << "”不是有效命令\n";
            }
        }

    }
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
        auto const file_list = file_tree.root().content(regular_judge);
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