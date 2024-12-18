#pragma once

#include <filesystem>
#include <cstdint>
#include <cstddef>

namespace OKps::sample
{
    /*
    命令行程序示例
    读取文本文件，以用户指定的编码输出到标准输出std::cout。

    此示例的用意是让使用者理解从标准输入std::cin获得的字符串的编码问题。
    在 windows 上，即使将c++程序的全局本地环境设为 ".utf-8"，通过std::cin输入的字符串却不会由标准库自动转换成utf-8编码，
    例如
    {
        std::locale::global(".utf-8");
        std::string str;
        std::cin >> str;
    }，
    字符串str的编码是windows终端模拟器使用的编码，而不是utf-8，需要手动转换编码。
    */
    void read_and_output_text_file();

    /*
    在目录 holder_dir 下生成 file_count 个以随机非负整数作文件名的OKps::crypt::cipher密钥文件。
    */
    void generate_cipher_key_files(std::filesystem::path const & holder_dir, std::uintmax_t const file_count);

    /*
    测试 std::ofstream 调用 write 函数写入文件的速度

    分别采用3中方式写入，
    第1种是将数据切分成 block_count 个 block_size 大小的块，一次写入一块；
    第2种是一次写入1字节；
    第3种是一次性调用 write 函数写入全部数据。

    经测试，在windows上，当block_size超过16kb时，第1种方法总是最快，而第3种方法耗时常常达到第1种的十多倍。
    */
    void test_file_output_speed(std::filesystem::path const & file_path, std::size_t const block_count = 1024, std::size_t const block_size = static_cast<std::size_t>(64) * 1024);
}