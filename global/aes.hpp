#pragma once

#include ".\fastIO.hpp"
#include ".\block.hpp"
#include <thread>
#include <vector>
#include <array>
#include <memory>

/*aes加密模块*/
namespace OKps
{

    /*
    AES 字符串加密
    */
    class basic_aes_device : public base
    {

    public:
        basic_aes_device() = delete;
        basic_aes_device(const basic_aes_device &) = delete;
        basic_aes_device(basic_aes_device &&) = delete;

    private:
        basic_aes_device(const unsigned char * key) noexcept;

    public:
        basic_aes_device(const std::string & key);
        basic_aes_device(const std::array<unsigned char, 16> & key) noexcept;
        ~basic_aes_device();

    private:
        /*
        aes加密
        输入必须是 长度 16 的字符串
        加密后的密文会替换输入字符串，不会分配新内存空间
        */
        void Cipher(unsigned char * const input);

    public:
        /*
        加密
        直接修改原字符串
        */
        void Cipher(std::string & origin);

    private:
        /*
        aes解密
        输入必须是 长度 16 的字符串
        解密后的明文会替换输入字符串，不会分配新内存空间
        */
        void InvCipher(unsigned char * const input);

    public:
        /*
        解密
        直接修改原字符串
        */
        void InvCipher(std::string & origin);
        /*
        末尾对齐
        */
        void make_align(unsigned char * buffer, const int count);
        /*
        对齐加密
        */
        void align_Cipher(std::string & origin);
        /*
        对齐解密
        输入的必须是 align_Cipher 函数生成的字符串格式，否则报错
        */
        void align_InvCipher(std::string & origin);

        /*
        随机生成16位unsigned char，用作aes密钥
        */
        static std::string random_aes_key();
        /*
        随机加密
        */
        static void random_Cipher(std::string & origin);

        /*
        随机解密
        */
        static void random_InvCipher(std::string & origin);
        /*
        生成OKps标准格式密文
        */
        static void OKps_Cipher(std::string & origin);
        /*
        begin_position 参数是文件位置指针
        从 begin_position 处开始，读取并解密一块OKps标准格式密文
        解密完成后，begin_position 参数会变成下一个密文块的起始位置
        */
        static std::string OKps_InvCipher(OKps::file::reader & file, std::streampos & begin_position);
        // 加密字符串
        // void* Cipher(void* input, int length = 0);

        // 解密字符串
        // void* InvCipher(void* input, int length);

    private:
        static const unsigned char Sbox[256];
        static const unsigned char InvSbox[256];
        unsigned char w[11][4][4];

        void KeyExpansion(const unsigned char * key) noexcept;

        unsigned char FFmul(const unsigned char & a, const unsigned char & b) noexcept;

        void SubBytes(unsigned char state[][4]) noexcept;

        void ShiftRows(unsigned char state[][4]) noexcept;

        void MixColumns(unsigned char state[][4]) noexcept;

        void AddRoundKey(unsigned char state[][4], const unsigned char k[][4]) noexcept;

        void InvSubBytes(unsigned char state[][4]) noexcept;

        void InvShiftRows(unsigned char state[][4]) noexcept;

        void InvMixColumns(unsigned char state[][4]) noexcept;

        // 为了通过循环展开优化运算速度而定义的函数
        void INNER_KeyExpansion(int i, unsigned char rc[]) noexcept;

        /*
        一个用于加密密钥的固定密钥
        例如，现在要用随机生成的密钥加密用户文件，因为如果用 用户信息（ID，用户名和密码等）的哈希值 当密钥，这种做法太常见，容易被猜到
        而如果用随机生成的密钥，则密钥和用户信息之间没有任何关联，只获取其一，并不能用于破解加密文件
        */
        static const std::string key_of_keys();
    };

    // aes多线程文件加密
    // 注意，本类析构时，一定会等待加密或解密完成后，线程才会结束
    // 所以，只要不是操作系统强制结束了进程，那么在用户发出关闭程序的命令后，尽管UI界面已经关闭，后台仍有可能在运行aes线程，并持续占用CPU、内存和文件
    class file_aes_device : public base
    {
    private:
        using TYPE_key = std::array<unsigned char, 16>;
        using TYPE_reader = std::shared_ptr<OKps::file::reader>;
        using TYPE_writter = std::shared_ptr<OKps::file::writter>;
        std::vector<TYPE_reader> origin;  // 被加密的文件
        std::vector<TYPE_writter> result; // 加密后生成的文件
        // 本设备的主线程
        // 加密器线程由这个线程管理
        // 所有加密器线程结束以后，这个线程才结束
        // 本设备析构时一定会等待这个线程结束
        std::thread main_thread;
        TYPE_key key; // 密钥

        bool running; // 线程正在运行
        bool used;    // 线程运行结束

    public:
        // 返回running标志
        // 如果直接公开这个字段，调用者在其他线程试图修改它们，会产生冲突
        // 为了线程安全，而设置此接口，因为即使提供锁，调用者也可以不用，而坚持作出不安全的行为
        bool is_running() const noexcept;

        // 返回used标志
        bool is_used() const noexcept;

    private:
        bool is_start; // 记录是否启动过线程，防止对空的std::thread对象调用join方法，而导致异常

        std::string origin_route; // 被加密的文件路径
        std::string result_route; // 加密后生成的文件路径
    public:
        enum class aes_mode
        {
            cipher,  // 加密
            incipher // 解密

        };

    private:
        uintmax_t MEMBER_file_size; // 文件总大小

        // 已经写入的文件大小
        // 随加密器运行而实时更新
        // 这是为了在UI界面显示进度条
        // 有多少线程，这个数组长度就是多少，每个线程写入不同下标，就不用加锁
        // 数组中所有数加起来，除以文件总大小 file_size ，就是已经完成的任务百分比
        std::vector<uintmax_t> MEMBER_write_size;

        // 加密器线程数
        int cipher_number;

    public:
        uintmax_t file_size() const noexcept;
        const std::vector<uintmax_t> write_size() const noexcept;
        int cipher_count() const noexcept;

    private:
        aes_mode aes_work_mode;
        static std::string INNER_buffer_str(const char * buffer, std::size_t size);

    public:
        file_aes_device();

        // 禁用拷贝构造函数，因为 std::thread 不应该复制
        file_aes_device(const file_aes_device &) = delete;
        // 禁用移动构造函数
        file_aes_device(file_aes_device &&) = delete;

        ~file_aes_device();

        void change_mode() noexcept;
        void change_mode(aes_mode _mode) noexcept;

        /*
        作为对外接口
        key参数不能为空指针
        */
        void aes_start(const std::string & _origin_route, const std::string & _result_route, const TYPE_key & _key, const int thread_count);
        /*
        作为对外接口
        key参数的长度必须是16
        */
        void aes_start(const std::string & _origin_route, const std::string & _result_route, const std::string & _key, const int thread_count);

    private:
        // aes对称加密，密钥必须是长度16的unsigned char字符串
        void aes_bifile();

        // aes对称解密，密钥必须是长度16的unsigned char字符串
        void deaes_bifile();

        // 作为线程函数运行
        void aes_main();

        class aes_cipher final : public base
        {
        private:
            // 分配的任务起始位置，是文件指针的值
            const unsigned long long int begin;
            // 分配的任务长度，注意不是字节数，是16字节块的块数
            const unsigned long long int block_number;

            TYPE_reader origin;  // 被加密的文件
            TYPE_writter result; // 加密后生成的文件

        private:
            // aes模式，是加密还是解密
            const aes_mode aes_work_mode;
            // 本加密器要修改的那个write_size数组位置
            uintmax_t & write_size;

            // 密钥，构造时从输入参数复制
            TYPE_key key;

        public:
            aes_cipher() = delete;
            aes_cipher(TYPE_reader origin, TYPE_writter result, const unsigned long long int begin, const unsigned long long int block_number, const aes_mode mode, uintmax_t & write_size, const TYPE_key & key);
            aes_cipher(const aes_cipher &) = delete;
            aes_cipher(const aes_cipher &&) = delete;

            ~aes_cipher();

        private:
            // 作为加密器的线程函数
            void cipher_main();

            std::thread cipher_thread;

        public:
            void join();
        };
        using TYPE_cipher_ptr = std::shared_ptr<aes_cipher>;
    };
}