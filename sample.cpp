
#include <fstream>
#include <random>

#include ".\string.hpp"
#include ".\timer.hpp"
#include ".\cipher.hpp"

#include ".\sample.hpp"

namespace OKps::sample
{
    void read_and_output_text_file()
    {

        std::ifstream reader;

        std::cout << "本程序以指定的用户环境显示文本文件的内容。\n";

        std::filesystem::path file_path;
        bool input_done = false;
        while (not input_done)
        {

            std::cout << "输入文件路径 ││ ";
            string path_input_temp;
            path_input_temp.get_line(std::cin);
            file_path = std::filesystem::path(path_input_temp.utf_8<std::u8string>());
            if (not std::filesystem::exists(file_path))
            {
                std::cout << "路径 " << file_path.string() << " 不存在，重新输入。\n";
            }
            else if (not std::filesystem::is_regular_file(file_path))
            {
                std::cout << "路径 " << file_path.string() << " 不是普通文件，重新输入。\n";
            }
            else
            {
                input_done = true;
            }
        }
        reader.open(file_path, std::ios::in | std::ios::binary);
        if (not reader.is_open())
        {
            std::cout << "文件 " << file_path.string() << " 无法打开。\n"
                "程序关闭。\n";
            return;
        }
        std::unique_ptr<std::string> buffer = std::make_unique<std::string>();
        time_monitor timer;
        using std::chrono::nanoseconds;

        try
        {
            timer.begin();
            (*buffer) = std::string((std::istreambuf_iterator<char>(reader)),
                                std::istreambuf_iterator<char>());
        }
        catch (...)
        {
            std::cout << "文件 " << file_path.string() << " 太大，无法读取。\n"
                "程序关闭。\n";
            return;
        }
        auto const read_file_time = timer.end<nanoseconds>();
        input_done = false;
        string user_page;
        {
            std::string text_file_content;

            while (not input_done)
            {
                std::cout << "输入本地环境 ││ ";
                user_page.get_line(std::cin);
                try
                {
                    std::locale::global(std::locale(user_page.content(true)));

                }
                catch (...)
                {
                    std::locale::global(text_encoding::utf_8);
                    std::cout << "本地环境 " << user_page << " 不存在，重新输入。\n";
                    continue;
                }

                try
                {
                    text_file_content = string(*buffer, std::locale(user_page.content(true))).utf_8<std::string>();
                    buffer.reset();
                    input_done = true;
                }
                catch (const std::exception & e)
                {
                    std::locale::global(text_encoding::utf_8);
                    std::cout << "无法以本地环境 " << user_page << " 读取文件。重新输入本地环境。\n";
                }

            }
            std::locale::global(text_encoding::utf_8);
            std::cout << "文件内容 ││ \n";
            timer.begin();
            std::cout << text_file_content;
            auto const output_time = timer.end<nanoseconds>();

            std::locale::global(text_encoding::utf_8);

            if (text_file_content.size() != 0 and (text_file_content[text_file_content.size() - 1] != '\n'))
            {
                std::cout << "\n";
            }

            std::cout << "运行计时 ││ 读取文件耗时" << read_file_time << "纳秒，输出内容耗时" << output_time << "纳秒。\n";

            /*
            在这里添加块作用域，是为了提前释放 buffer 的内存。以免文件很大，而用户在wait_input函数等待输入时却迟迟不输入，程序占用系统太多内存。
            */
        }

        OKps::wait_input("");
        return;
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