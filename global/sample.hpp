#pragma once

#include <filesystem>
#include <cstdint>
#include <cstddef>

namespace OKps::sample
{
    /*
    dir是一个目录，此函数比较并清除dir目录下的重复文件
    此函数只比较直接属于dir目录的文件，不比较dir目录下的子目录中的文件
    此函数只比较被std::filesystem判定为regular_file的文件
    重复指的是两个文件按字节比较完全相同，不考虑其文件名、修改时间等
    */
    void erase_duplicate_files(std::filesystem::path const & dir);
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