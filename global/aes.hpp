#pragma once

#include <memory>
#include <filesystem>
#include <thread>
#include <fstream>
#include <array>
#include <vector>



/*aes加密模块*/
namespace OKps::AES
{

    /*
    AES 字节流加密
    */
    class byte_device final
    {

    public:
        //判断两个AES对象的密钥是否相等
        bool operator==(byte_device const & right)const noexcept;
        bool operator!=(byte_device const & right)const noexcept;

        //生成随机aes密钥，构造aes对象
        byte_device();
        byte_device(byte_device const & origin)noexcept;
        void operator=(byte_device const & origin)noexcept;

        /*
        移动构造会导致原来的对象所保存的密钥变成随机值，对象仍然可以安全使用
        原来的密钥会转移给新生成的aes对象
        */
        byte_device(byte_device &&) noexcept;
        void operator=(byte_device &&) noexcept;
    private:
        byte_device(std::byte const * key) noexcept;
    public:
        static inline constexpr std::size_t const key_length = 16;
        using key_type = std::array<std::byte, key_length>;
        using byte_type = std::vector<std::byte>;
    private:
        key_type MEMBER_key;
    public:
        byte_device(key_type const & key)noexcept;

        ~byte_device()noexcept;

    private:
        /*
        aes加密
        输入必须是 长度 16 的字符串
        加密后的密文会替换输入字符串，不会分配新内存空间
        */
        void encrypt(std::byte * const input)const;

    public:
        key_type const & key()const noexcept;
        /*
        加密
        直接修改原字符串
        */
        void encrypt(std::string & origin)const;
        void encrypt(byte_type & origin)const;

    private:
        /*
        aes解密
        输入必须是 长度 16 的字符串
        解密后的明文会替换输入字符串，不会分配新内存空间
        */
        void decrypt(std::byte * const input)const;

    public:
        /*
        解密
        直接修改原字符串
        */
        void decrypt(std::string & origin)const;
        void decrypt(byte_type & origin)const;
        /*
        末尾对齐
        */
        static void make_align(std::byte * buffer, const unsigned int count);

        static void make_align(char * buffer, const unsigned int count);

        /*
        对齐加密
        */
        void align_encrypt(std::string & origin)const;
        void align_encrypt(byte_type & origin)const;
        /*
        对齐解密
        输入的必须是对齐加密产生的格式密文串，否则报错
        */
        void align_decrypt(std::string & origin)const;
        void align_decrypt(byte_type & origin)const;

        /*
        随机生成16字节aes密钥
        */
        static key_type random_aes_key();
        /*
        生成随机密钥，并用该密钥进行对齐加密
        所得密文应该用对齐解密
        返回值是生成的随机密钥
        */
        static key_type random_encrypt(std::string & origin);
        static key_type random_encrypt(byte_type & origin);

    private:
        static const std::byte Sbox[256];
        static const std::byte InvSbox[256];
        std::byte w[11][4][4];

        void KeyExpansion(std::byte const * key) noexcept;

        static std::byte FFmul(std::byte const & a, std::byte const & b)noexcept;

        static void SubBytes(std::byte state[][4])noexcept;

        static void ShiftRows(std::byte state[][4])noexcept;

        static void MixColumns(std::byte state[][4])noexcept;

        static void AddRoundKey(std::byte state[][4], std::byte const k[][4])noexcept;

        static void InvSubBytes(std::byte state[][4])noexcept;

        static void InvShiftRows(std::byte state[][4])noexcept;

        static void InvMixColumns(std::byte state[][4])noexcept;

        // 为了通过循环展开优化运算速度而定义的函数
        void INNER_KeyExpansion(int const i, std::byte  const rc[]) noexcept;


    };

    /*
    aes多线程文件加密
    注意，本类析构时，一定会等待加密或解密完成后，线程才会结束
    所以，只要不是操作系统强制结束了进程，那么在用户发出关闭程序的命令后，尽管UI界面已经关闭，后台仍有可能在运行aes线程，并持续占用CPU、内存和文件
    */
    class file_device final
    {
    public:
        using key_type = byte_device::key_type;

    private:
        using TYPE_path = std::filesystem::path;
        using TYPE_reader = std::shared_ptr<std::ifstream>;
        using TYPE_writter = std::shared_ptr<std::ofstream>;

        /*
        本设备的主线程
        加密器线程由这个线程管理
        所有加密器线程结束以后，这个线程才结束
        本设备析构时一定会等待这个线程结束
        */
        std::thread MEMBER_worker;

        std::exception_ptr MEMBER_error;

        TYPE_path MEMBER_origin_route; // 被加密的文件路径
        TYPE_path MEMBER_result_route; // 加密后生成的文件路径
    public:

    private:
        std::uintmax_t MEMBER_file_size; // 输入文件的大小

        // 加密器线程数
        std::uintmax_t MEMBER_thread_number;

        key_type MEMBER_key;

        static std::string INNER_buffer_string(const char * const buffer, std::size_t const size);

    public:
        file_device()noexcept;

        // 禁用拷贝构造函数，因为 std::thread 不应该复制
        file_device(file_device const &) = delete;
        void operator=(file_device const &) = delete;

        file_device(file_device &&) noexcept = delete;
        void operator=(file_device &&) noexcept = delete;
        ~file_device() noexcept;

        /*
        作为对外接口，启动一个加密过程
        如果输入的线程数为0，则根据std::thread::hardware_concurrency()函数的返回值自动调整线程数

        注意，线程数不要过大，因为操作系统对一个进程能拥有的文件句柄的数量有限制，而本类会给每一个线程申请2个文件句柄，如果线程数太大，则有些线程无法访问文件。如果出现这种情况，则抛出异常。
        */

        void encrypt(TYPE_path const & origin_route, TYPE_path const & result_route, const key_type & key, const std::size_t thread_count = 0);
        void decrypt(TYPE_path const & origin_route, TYPE_path const & result_route, const key_type & key, const std::size_t thread_count = 0);

    private:
        // aes对称加密，密钥必须是长度16的unsigned char字符串
        void encrypt_work();

        // aes对称解密，密钥必须是长度16的unsigned char字符串
        void decrypt_work();

        class cipher final
        {
        private:

            // 分配的任务起始位置，是文件指针的值
            std::size_t const begin;
            // 分配的任务长度，注意不是字节数，是16字节块的块数
            std::size_t const block_number;

            TYPE_reader origin;  // 被加密的文件
            TYPE_writter result; // 加密后生成的文件

        private:

            // 密钥
            key_type const & key;

        public:
            cipher() = delete;
            cipher(TYPE_reader const & origin, TYPE_writter const & result, const std::size_t begin, const std::size_t block_number, const key_type & key);
            cipher(cipher const &) = delete;
            void operator=(cipher const &) = delete;
            cipher(cipher &&) = delete;
            void operator=(cipher &&) = delete;

            ~cipher()noexcept;


            // 作为加密器的线程函数
            void encrypt();
            void decrypt();
        private:
            std::thread MEMBER_worker;

            void encrypt_work();
            void decrypt_work();
        public:
            void join();
        };

    };
}