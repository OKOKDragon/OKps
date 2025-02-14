#pragma once

#include <compare>
#include <atomic>

#include ".\bit.hpp"
#include ".\number.hpp"
#include ".\storage.hpp"
#include ".\lock.hpp"

namespace OKps
{
    /*
    无限精度整数

    虽然有很多成熟的数学库，但它们大多数是c语言的，或是几乎完全用c语言风格写的所谓“C with class”而非真正的 C++。
    我希望能充分利用C++的运算符重载、类型别名、枚举类、异常和标准库，而不要像c语言一样手动创建和销毁上下文、使用复杂的宏定义和宏函数。
    因此我从头实现了这个高精度整数类。
    */
    class integer final
    {
    public:
        /*
        内置无符号整数类型。
        用此类型的数组存储大整数。
        */
        using value_type = std::uint32_t;
    private:
        //value_type 占的二进制位
        static std::size_t const half_length;
    public:
        /*
        最大的内置无符号整数类型
        用此类型可以存储任意两个 value_type 类型的加法或乘法结果。
        */
        using holder_type = std::uint64_t;
    private:
        //holder_type 占的二进制位
        static std::size_t const full_length;
    public:
        using number_type = std::vector<value_type>;

        //正负性符号
        enum class sign_type :signed char
        {
            zero = 0,//零
            positive = 1,//正
            negative = -1//负
        };


    private:
        /*
        value_type类型所能表示的不同无符号整数的数量
        也就是大整数数组使用的进制
        */
        static holder_type const base;
        static value_type const half_base;//base的一半
        sign_type MEMBER_sign;//符号位
        number_type MEMBER_number;//用2进制存储的整数，无符号
    public:
        //构造值为0的大整数
        integer();
        ~integer()noexcept;
        /*
        如果输入的数值 number 是0，则不论符号位 sign 是什么，此构造函数生成值为0的大整数。
        如果输入的符号位 sign 是0，但数值 number 不是0，则抛出异常。
        */
        integer(holder_type const number, sign_type const sign);
        integer(number_type const & number, sign_type sign);
        sign_type const & sign()const noexcept;
        number_type const & number()const noexcept;
    private:

        class known_prime_holder final
        {
        public:
            known_prime_holder(known_prime_holder const &) = delete;
            known_prime_holder(known_prime_holder &&) = delete;
            void operator =(known_prime_holder const &) = delete;
            void operator =(known_prime_holder &&) = delete;
        private:
            mutable std::atomic<bool> MEMBER_flag;//是否已经初始化
            mutable lock_proxy<std::binary_semaphore> MEMBER_lock;
            mutable std::vector<integer> MEMBER_known_prime;//已知素数数组
        public:
            known_prime_holder()
                noexcept(std::is_nothrow_default_constructible_v<std::vector<integer>>
                and noexcept(std::atomic<bool>(false))
                and noexcept(std::is_nothrow_default_constructible_v<lock_proxy<std::binary_semaphore>>));
            ~known_prime_holder()
                noexcept(std::is_nothrow_destructible_v<std::vector<integer>>
                and std::is_nothrow_destructible_v<std::atomic<bool>>
                and std::is_nothrow_destructible_v<lock_proxy<std::binary_semaphore>>);
            integer const & operator [](std::size_t const position)const;
            std::size_t size()const;
        private:
            static std::vector<holder_type> INNER_get_known_prime();
            static std::vector<integer> INNER_init_known_prime();
        };
        /*
        提供对已知素数表的访问
        在首次访问时从文件加载到内存中，若无法加载文件则抛出异常。
        */
        static known_prime_holder const MEMBER_known_prime;
        integer(number_type && number, sign_type const sign)noexcept;

        /*
        实现Miller Rabin算法判断素数
        此算法有小概率出错
        */
        static bool INNER_Miller_Rabin(integer const & num, std::uintmax_t const check_times = 1024);
        //两个无符号数相加
        static number_type INNER_add_number(number_type const & left, number_type const & right);

        /*
        较大数减去较小数，双方均无符号
        会去除结果中的前置0
        */
        static number_type INNER_subtract_number(number_type const & big, number_type const & little);

        /*
        比较this的绝对值与right的绝对值
        */
        std::strong_ordering INNER_compare_number(integer const & right)const noexcept;
        //比较this和right
        std::strong_ordering INNER_compare(integer const & right)const noexcept;
        /*
        去掉数组中所有前置的0
        如果数组中原来全是0，也不会留下一个0来占位，也就是说会得到空数组，这是因为我们规定用空数组来唯一地表示0
        */
        static void INNER_erase_leading_zero(number_type & val)noexcept;

    public:

        bool operator >(integer const & right)const noexcept;
        bool operator <(integer const & right)const noexcept;
        bool operator >=(integer const & right)const noexcept;
        bool operator <=(integer const & right)const noexcept;

        /*
        输入字符串，转换为大整数
        字符串只表示绝对值，符号由sign参数指定
        字符串的字符排列顺序是人类阅读顺序，也就是以下标0为数字的最高位
        字符串中所有的字符都必须是表示数字的
        如果字符串格式错误，抛出异常
        如果字符串为空，生成值为0的大整数
        如果字符串有前置0，忽略
        字符串采用的进制由system参数指定，默认为10进制
        */
        integer(std::string const & input, sign_type const sign, number_system const system = number_system::dec);

        integer(integer const & origin);
        void operator =(integer const & origin);

        //原来的大数对象origin被移动后会置为0，可以安全使用
        integer(integer && origin)noexcept;
        //原来的大数对象origin被移动后会置为0，可以安全使用
        void operator =(integer && origin)  noexcept;

        integer operator +(integer const & right)const;
        //取相反数
        integer operator -()const;

        integer operator -(integer const & right)const;

        bool operator ==(integer const & right)const noexcept;
        bool operator !=(integer const & right)const noexcept;

        integer operator *(integer const & right)const;

        /*
        将大整数转换为2进制流的形式，用于输出到文件
        */
        field field()const;
        /*
        读取2进制流并生成大整数
        */
        integer(OKps::field const & input);
        /*
        将大整数转换为字符串
        如果是负数，则前面加负号
        如果不是负数，则不会加正号
        system参数指定字符串采用的进制
        */
        std::string string(number_system const system = number_system::dec)const;
    private:
        /*
        用简单的模拟竖式方法实现的乘法
        */
        static number_type INNER_simple_multiply_number(number_type const & left, number_type const & right);

        //两个大整数的乘法
        static number_type INNER_multiply_number(number_type const & left, number_type const & right);
        //大整数和无符号内置类型整数之间的乘法
        static number_type INNER_multiply_number(number_type const & left, value_type const right);
        static number_type INNER_simple_multiply_number(number_type const & left, value_type const right, std::size_t const after_zero/*后置0的数量*/);
    public:
        /*
        判断大整数是不是素数
        只关心绝对值，不关心符号
        如果cheat参数为false，则使用暴力试除法，非常慢但结果一定正确

        如果cheat参数为true，则使用其他素数判断算法，比较快但有一定概率出错
        出错的概率取决于check_times参数，这个参数指定算法进行二次筛查的次数
        */
        bool is_prime(bool const cheat = false, std::uintmax_t const check_times = 1024)const;
        /*
        生成随机素数
        sign参数决定生成的素数的正负性，而此函数只关心绝对值
        length参数决定值数组的长度，即生成的素数的绝对值的二进制长度是 length * half_length
        */
        static integer random_prime(std::size_t const length, sign_type const sign, bool const cheat /*是否使用素数判断算法*/ = false, std::uintmax_t const check_times = 1024);
        /*
        生成随机数
        sign参数决定生成的素数的正负性，而此函数只关心绝对值
        length参数决定值数组的长度，即生成的随机数的绝对值的二进制长度是 length * half_length
        */
        static integer random(std::size_t const length, sign_type const sign);
        /*
        求最大公约数
        使用欧几里得算法
        */
        static integer gcd(integer const & left, integer const & right);
        /*
        判断两个数是否互素
        只关心两个参数的绝对值
        */
        static bool is_coprime(integer const & left, integer const & right);
        /*
        求num关于mod的模逆元
        即求 r 满足  (num * r) % mod == 1
        模逆元存在的充要条件是num和mod互素，如果模逆元不存在，则此函数抛出异常
        */
        static integer inverse_mod(integer const & num, integer const & mod);
        /*
        模幂运算
        令 x = this 的 pow 次方，求 x % mod
        */
        integer power_mod(integer const & pow/*指数*/, integer const & mod/*除数*/)const;
        integer power(integer const & pow)const;
        /*
        扩展欧几里得算法
        返回值是left和right的最大公约数
        并且令x和y满足a*x+b*y=gcd(a,b)
        */
        static integer extra_gcd(integer const & left, integer const & right, integer & x, integer & y);
        /*
        求大整数的平方根
        精确度为1，即将结果向0取整
        使用牛顿迭代法实现
        */
        integer square_root()const;
        //取绝对值
        integer absolute()const;
        integer operator /(integer const & right)const;
        /*
        根据c++标准，在内置类型中(a / b) * b + a % b == a
        我们规定大整数的取余运算也满足该公式
        */
        integer operator %(integer const & right)const;
        //operator std::string()const;
    private:
        //存储大整数除法结果的类型
        class INNER_divide_result final
        {
        private:
            number_type MEMBER_divide;//除法结果，即商
            number_type MEMBER_module;//取模结果，即余数
        public:
            INNER_divide_result(number_type && divide, number_type && mod)noexcept;
            INNER_divide_result(number_type const & divide, number_type const & mod);
            number_type const & divide()const noexcept;
            number_type const & mod()const noexcept;
            INNER_divide_result(INNER_divide_result && origin)noexcept;
            void operator =(INNER_divide_result && origin)noexcept;
            INNER_divide_result(INNER_divide_result const &) = delete;
            void operator =(INNER_divide_result const &) = delete;
            ~INNER_divide_result()noexcept;
        };
        /*
        无符号非0大数的除法
        left必须比right大
        */
        static INNER_divide_result INNER_divide_number(number_type const & left, number_type const & right);
        /*
        无符号大整数left除以无符号内置类型整数right
        不检查right是否为0
        要求left>right
        */
        static INNER_divide_result INNER_divide_number(number_type const & left, value_type const right);
        /*
        使用Knuth提出的试商法实现的大数除法
        除数right的绝对值数组的长度必须超过2
        */
        static INNER_divide_result INNER_Knuth_divide(number_type const & left, number_type const & right, bool debug = false);
        static std::pair<integer, integer> INNER_divide_and_module(integer const & left, integer const & right);
    public:
        class divide_result final
        {
            friend class integer;
        private:
            std::unique_ptr<integer const> MEMBER_divide;//除法结果，即商
            std::unique_ptr<integer const> MEMBER_module;//取模结果，即余数

            divide_result(integer && divide, integer && mod);
            divide_result(integer const & divide, integer const & mod);
        public:
            integer const & divide()const noexcept;
            integer const & mod()const noexcept;
            divide_result(divide_result && origin)noexcept;
            divide_result(divide_result const & origin);
            void operator =(divide_result && origin)noexcept;
            void operator =(divide_result const & origin);
        };
        divide_result divide_and_module(integer const & right)const;

        void operator +=(integer const & right);
        void operator ++();
        void operator -=(integer const & right);
        void operator --();
        void operator *=(integer const & right);
        void operator /=(integer const & right);
        void operator %=(integer const & right);
    };

}