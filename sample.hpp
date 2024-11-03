#pragma once

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



}