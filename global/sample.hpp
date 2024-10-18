#pragma once

namespace OKps::sample
{
    /*
    命令行程序示例
    读取文本文件，以用户指定的编码输出到标准输出std::cout。

    此示例的用意是让使用者理解从标准输入获得的字符串的编码问题。
    在 windows 上，终端模拟器和系统api使用utf-16编码，而即使将程序的全局本地环境设为 ".utf-8"，从std::cin输入的字符串并不会由标准库自动转换成utf-8编码，仍然需要使用者注意转换编码。
    */
    void read_and_output_text_file();

}