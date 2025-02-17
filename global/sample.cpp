
#include <fstream>
#include <random>

#include ".\string.hpp"
#include ".\timer.hpp"
#include ".\cipher.hpp"

#include ".\sample.hpp"

namespace OKps::sample
{
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