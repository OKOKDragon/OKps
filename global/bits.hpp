#pragma once

#include ".\base.hpp"

namespace OKps
{

    const constexpr std::size_t bit_per_byte = 8;// 每字节有多少位

    /*
    二进制流容器
    标准库的 bitset 容器必须在编译期确定长度，而本容器是动态长度的
    但只有相同长度的对象之间可以做位操作
    */
    class bits : public base
    {
    private:
        using TYPE_bit = unsigned int;// 直接参与位操作的类型（即整型）中，大小最小的
        static const constexpr auto DATA_bit_len = sizeof(TYPE_bit) * bit_per_byte;// TYPE_bit类型的位数
        using TYPE_bit_op = std::bitset<DATA_bit_len>;// 位操作的最小单位
        std::deque<TYPE_bit_op> MEMBER_data;
        std::size_t MEMBER_size;

        std::size_t MEMBER_front_pos; // 前端在 MEMBER_data 第0块中的下标
        class TYPE_pos final : public base_no_allocate
        {
        public:
            const std::size_t block; // 块的位置
            const std::size_t pos;   // 块内的位置
            explicit TYPE_pos(const std::size_t ARG_block, const std::size_t ARG_pos) noexcept;
            TYPE_pos(TYPE_pos &&) = delete;
            TYPE_pos operator=(TYPE_pos &&) = delete;
            TYPE_pos(TYPE_pos const & ARG_origin) noexcept;
            void operator=(TYPE_pos const & origin) = delete;
        };

        /*
        位置下标转换
        将 对外模拟出的逻辑位置 转换为真实位置
        */
        const TYPE_pos INNER_trans_pos(const std::size_t ARG_pos) const noexcept;
        /*
        不检查输入的位置下标
        仅限本类内部使用，节省越界检查的开销
        */
        TYPE_bit_op::reference INNER_unsafe_at(const std::size_t ARG_pos) noexcept;
        bool INNER_unsafe_at(const std::size_t ARG_pos) const noexcept;

        // 抛出越界异常
        static void ERROR_board();
        void ERROR_board(const std::size_t ARG_pos) const;

        void ERROR_empty() const;

    public:
        static char value_to_char(const bool ARG_input) noexcept;
        static wchar_t value_to_wchar(const bool ARG_input) noexcept;
        static char16_t value_to_char16(const bool ARG_input) noexcept;
        static char32_t value_to_char32(const bool ARG_input) noexcept;

        static bool char_to_value(const char ARG_input);
        static bool char_to_value(const wchar_t ARG_input);
        static bool char_to_value(const char16_t ARG_input);
        static bool char_to_value(const char32_t ARG_input);

    private:
        // 抛出异常，内容是字符不能表示二进制位
        static void ERROR_bit_char();

    public:
        bits() noexcept;

    private:
        bits(const std::size_t ARG_size);

    public:
        /*
        按下标顺序构造
        一般不会在构造函数中抛异常，如果输入错误的参数，则返回默认构造的对象。
        但我不认同这种做法，我坚持要抛出异常。
        因为默认就代表不会出任何问题，默认构造不是其他代码出问题时的接盘侠。
        异常就是用来报告异常的，有异常而不用，何必写c++呢？请回去写c语言
        */
        bits(const std::string & ARG_input);
        void operator=(const std::string & ARG_input);
        // 按下标顺序构造
        bits(const std::wstring & ARG_input);
        void operator=(const std::wstring & ARG_input);
        // 按下标顺序构造
        bits(const std::u16string & ARG_input);
        void operator=(const std::u16string & ARG_input);
        // 按下标顺序构造
        bits(const std::u32string & ARG_input);
        void operator=(const std::u32string & ARG_input);
        // 按下标顺序构造
        bits(const std::deque<bool> & ARG_input);
        // 按出队顺序构造
        bits(const std::queue<bool> & ARG_input);
        // 按下标顺序构造
        bits(const std::vector<bool> & ARG_input);
        // 按迭代器顺序构造
        bits(const std::list<bool> & ARG_input);
        // 按迭代器顺序构造
        bits(const std::forward_list<bool> & ARG_input);
        // 按下标顺序构造
        template <std::size_t ARG_size>
        bits(const std::array<bool, ARG_size> & ARG_input);
        // 按出栈顺序构造
        bits(const std::stack<bool> & ARG_input);
        template <std::size_t ARG_size>
        bits(const std::bitset<ARG_size> & ARG_input);
        bits(const bits & ARG_origin);
        void operator=(const bits & ARG_origin);
        bits(bits && ARG_origin) noexcept;
        void operator=(bits && ARG_origin) noexcept;
        ~bits();

        void clear() noexcept;
        bool empty() const noexcept;
        std::size_t size() const noexcept;

        TYPE_bit_op::reference operator[](const std::size_t ARG_pos);
        TYPE_bit_op::reference at(const std::size_t ARG_pos);
        bool operator[](const std::size_t ARG_pos) const;
        bool at(const std::size_t ARG_pos) const;
        TYPE_bit_op::reference back();
        TYPE_bit_op::reference front();
        bool back() const;
        bool front() const;
        /*
        从begin到end，截取片段
        如果begin<end，则结果为顺序，否则为逆序
        */
        bits frag(const std::size_t ARG_begin, const std::size_t ARG_end) const;

        operator std::string() const;
        operator std::wstring() const;
        operator std::u16string() const;
        operator std::u32string() const;

        std::string str(const std::size_t ARG_begin, const std::size_t ARG_end) const;
        std::wstring wstr(const std::size_t ARG_begin, const std::size_t ARG_end) const;
        std::u16string u16str(const std::size_t ARG_begin, const std::size_t ARG_end) const;
        std::u32string u32str(const std::size_t ARG_begin, const std::size_t ARG_end) const;

        // 按下标顺序转换
        operator std::deque<bool>() const;
        // 按入队顺序转换
        operator std::queue<bool>() const;
        // 按下标顺序构造
        operator std::vector<bool>() const;
        // 按迭代器顺序构造
        operator std::list<bool>() const;
        // 按迭代器顺序构造
        operator std::forward_list<bool>() const;
        // 按入栈顺序构造
        operator std::stack<bool>() const;

        // 按下标顺序转换
        std::deque<bool> deque(const std::size_t ARG_begin, const std::size_t ARG_end) const;
        // 按入队顺序转换
        std::queue<bool> queue(const std::size_t ARG_begin, const std::size_t ARG_end) const;
        // 按下标顺序构造
        std::vector<bool> vector(const std::size_t ARG_begin, const std::size_t ARG_end) const;
        // 按迭代器顺序构造
        std::list<bool> list(const std::size_t ARG_begin, const std::size_t ARG_end) const;
        // 按迭代器顺序构造
        std::forward_list<bool> forward_list(const std::size_t ARG_begin, const std::size_t ARG_end) const;
        // 按入栈顺序构造
        std::stack<bool> stack(const std::size_t ARG_begin, const std::size_t ARG_end) const;

        void push_back(const bool ARG_value);
        void push_back(const bits & ARG_input);
        void push_front(const bool ARG_value);
        void push_front(const bits & ARG_input);
        bool pop_back();
        void pop_back(const std::size_t ARG_length);
        bool pop_front();
        void pop_front(const std::size_t ARG_length);

        // 反转每一位
        void flip() noexcept;
        // 反转指定的1位
        void flip(const std::size_t ARG_pos);
        // 反转指定的一段
        void flip(const std::size_t ARG_begin, const std::size_t ARG_end);
        std::size_t count(const bool ARG_value) const noexcept;
        bool any(const bool ARG_value) const noexcept;
        bool all(const bool ARG_value) const noexcept;

        bool operator==(const bits & ARG_right) const noexcept;
        bool operator!=(const bits & ARG_right) const noexcept;
        bool operator==(const std::string & ARG_right) const;
        bool operator!=(const std::string & ARG_right) const;
        bool operator==(const std::wstring & ARG_right) const;
        bool operator!=(const std::wstring & ARG_right) const;
        bool operator==(const std::u16string & ARG_right) const;
        bool operator!=(const std::u16string & ARG_right) const;
        bool operator==(const std::u32string & ARG_right) const;
        bool operator!=(const std::u32string & ARG_right) const;

        bits operator~() const noexcept;

    private:
        // 位操作的2个操作数长度不同
        void ERROR_diff_len(const bits & ARG_right) const;

    public:
        bits operator&(const bits & ARG_right) const;
        void operator&=(const bits & ARG_right);
        bits operator|(const bits & ARG_right) const;
        void operator|=(const bits & ARG_right);
        bits operator^(const bits & ARG_right) const;
        void operator^=(const bits & ARG_right);
        bits operator<<(const std::size_t & ARG_right) const noexcept;
        void operator<<=(const std::size_t & ARG_right) noexcept;
        bits operator>>(const std::size_t & ARG_right) const noexcept;
        void operator>>=(const std::size_t & ARG_right) noexcept;
    };
    template <std::size_t ARG_size>
    bits::bits(const std::array<bool, ARG_size> & ARG_input)
        : bits(ARG_size)
    {
        for (std::size_t TEMP_count = 0; TEMP_count < ARG_size; TEMP_count++)
        {
            this->INNER_unsafe_at(TEMP_count) = ARG_input[TEMP_count];
        }
    }
    template <std::size_t ARG_size>
    bits::bits(const std::bitset<ARG_size> & ARG_input)
        : bits(ARG_size)
    {
        for (std::size_t TEMP_count = 0; TEMP_count < ARG_size; TEMP_count++)
        {
            this->INNER_unsafe_at(TEMP_count) = ARG_input[TEMP_count];
        }
    }
}