#include <fstream>
#include <iostream>
#include <filesystem>

#include ".\sample.hpp"

#include ".\string.hpp"

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
            OKps::string path_input_temp("", std::locale::classic());
            std::getline(std::cin, path_input_temp.content(true));
            std::locale::global(OKps::text_encoding::utf_8);
            file_path = std::filesystem::path(path_input_temp.utf_8());
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

        input_done = false;
        OKps::string user_page("", std::locale::classic());
        while (not input_done)
        {
            std::cout << "输入本地环境 ││ ";
            std::getline(std::cin, user_page.content(true));
            try
            {
                std::locale::global(std::locale(user_page.content()));
                input_done = true;
            }
            catch (...)
            {
                std::locale::global(OKps::text_encoding::utf_8);
                std::cout << "本地环境 " << user_page.utf_8() << " 不存在，重新输入。\n";
            }
        }

        {
            std::string buffer;
            try
            {
                buffer = std::string((std::istreambuf_iterator<char>(reader)),
                                    std::istreambuf_iterator<char>());
            }
            catch (...)
            {
                std::locale::global(OKps::text_encoding::utf_8);
                std::cout << "文件 " << file_path.string() << " 太大，无法读取。\n"
                    "程序关闭。\n";
                return;
            }
            std::locale::global(OKps::text_encoding::utf_8);
            std::cout << "文件内容 ││ \n";
            std::locale::global(std::locale(user_page.content()));
            std::cout << buffer;
            std::locale::global(OKps::text_encoding::utf_8);

            /*
            在这里添加块作用域，是为了提前释放 buffer 的内存。以免文件很大，而用户在wait_input函数等待输入时却迟迟不输入，程序占用系统太多内存。
            */
        }
        OKps::wait_input("");
        return;
    }
}