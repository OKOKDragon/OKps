
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

    void read_and_output_text_file(bool const FLAG_debug)
    {
        static_assert(bit_per_byte == 8, "此函数的前提是每字节占8位");

        std::vector<std::string> invalid_characters;
        invalid_characters.resize(90);
        invalid_characters[0] = "龦";
        invalid_characters[1] = "龧";
        invalid_characters[2] = "龨";
        invalid_characters[3] = "龩";
        invalid_characters[4] = "龪";
        invalid_characters[5] = "龫";
        invalid_characters[6] = "龬";
        invalid_characters[7] = "龭";
        invalid_characters[8] = "龮";
        invalid_characters[9] = "龯";
        invalid_characters[10] = "龰";
        invalid_characters[11] = "龱";
        invalid_characters[12] = "龲";
        invalid_characters[13] = "龳";
        invalid_characters[14] = "龴";
        invalid_characters[15] = "龵";
        invalid_characters[16] = "龶";
        invalid_characters[17] = "龷";
        invalid_characters[18] = "龸";
        invalid_characters[19] = "龹";
        invalid_characters[20] = "龺";
        invalid_characters[21] = "龻";
        invalid_characters[22] = "龼";
        invalid_characters[23] = "龽";
        invalid_characters[24] = "龾";
        invalid_characters[25] = "龿";
        invalid_characters[26] = "鿀";
        invalid_characters[27] = "鿁";
        invalid_characters[28] = "鿂";
        invalid_characters[29] = "鿃";
        invalid_characters[30] = "鿄";
        invalid_characters[31] = "鿅";
        invalid_characters[32] = "鿆";
        invalid_characters[33] = "鿇";
        invalid_characters[34] = "鿈";
        invalid_characters[35] = "鿉";
        invalid_characters[36] = "鿊";
        invalid_characters[37] = "鿋";
        invalid_characters[38] = "鿌";
        invalid_characters[39] = "鿍";
        invalid_characters[40] = "鿎";
        invalid_characters[41] = "鿏";
        invalid_characters[42] = "鿐";
        invalid_characters[43] = "鿑";
        invalid_characters[44] = "鿒";
        invalid_characters[45] = "鿓";
        invalid_characters[46] = "鿔";
        invalid_characters[47] = "鿕";
        invalid_characters[48] = "鿖";
        invalid_characters[49] = "鿗";
        invalid_characters[50] = "鿘";
        invalid_characters[51] = "鿙";
        invalid_characters[52] = "鿚";
        invalid_characters[53] = "鿛";
        invalid_characters[54] = "鿜";
        invalid_characters[55] = "鿝";
        invalid_characters[56] = "鿞";
        invalid_characters[57] = "鿟";
        invalid_characters[58] = "鿠";
        invalid_characters[59] = "鿡";
        invalid_characters[60] = "鿢";
        invalid_characters[61] = "鿣";
        invalid_characters[62] = "鿤";
        invalid_characters[63] = "鿥";
        invalid_characters[64] = "鿦";
        invalid_characters[65] = "鿧";
        invalid_characters[66] = "鿨";
        invalid_characters[67] = "鿩";
        invalid_characters[68] = "鿪";
        invalid_characters[69] = "鿫";
        invalid_characters[70] = "鿬";
        invalid_characters[71] = "鿭";
        invalid_characters[72] = "鿮";
        invalid_characters[73] = "鿯";
        invalid_characters[74] = "鿰";
        invalid_characters[75] = "鿱";
        invalid_characters[76] = "鿲";
        invalid_characters[77] = "鿳";
        invalid_characters[78] = "鿴";
        invalid_characters[79] = "鿵";
        invalid_characters[80] = "鿶";
        invalid_characters[81] = "鿷";
        invalid_characters[82] = "鿸";
        invalid_characters[83] = "鿹";
        invalid_characters[84] = "鿺";
        invalid_characters[85] = "鿻";
        invalid_characters[86] = "鿼";
        invalid_characters[87] = "鿽";
        invalid_characters[88] = "鿾";
        invalid_characters[89] = "鿿";
        std::vector<std::string> invalid_numbers;
        invalid_numbers.resize(invalid_characters.size());
        for (std::size_t i = 0;i < invalid_characters.size();++i)
        {
            std::byte const TEMP_mask = value_cast<std::byte>(0b00001111);
            invalid_numbers[i].resize(invalid_characters[i].size() * 2);
            for (std::size_t j = 0;j < invalid_characters[i].size();++j)
            {
                invalid_numbers[i][j * 2] = to_char(value_cast<unsigned int>((value_cast<std::byte>(invalid_characters[i][j]) >> 4) & TEMP_mask), number_system::hex);
                invalid_numbers[i][j * 2 + 1] = to_char(value_cast<unsigned int>(value_cast<std::byte>(invalid_characters[i][j]) & TEMP_mask), number_system::hex);
            }
        }

        std::ifstream reader;

        std::cout << "本程序以指定的用户环境显示文本文件的内容。\n"
            "有些字符可能无法在控制台显示，与字体无关，例如\n"
            "示例 ││ ";
        for (std::size_t i = 0;i < invalid_characters.size();++i)
        {
            std::cout << invalid_characters[i];
        }
        std::cout << "\n"
            "这是utf-8编码从" << invalid_numbers[0] << "到" << invalid_numbers[invalid_numbers.size() - 1] << "闭区间范围的字符。\n";
        if (FLAG_debug)
        {
            for (std::size_t i = 0;i < invalid_numbers.size();++i)
            {
                std::cout << "调试信息 ││ 失效字符[" << (i + 1) << "]=" << invalid_numbers[i] << "\n";
            }
        }

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