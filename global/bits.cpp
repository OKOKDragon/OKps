#include ".\bits.hpp"

namespace OKps
{

    bits::TYPE_pos::TYPE_pos(const std::size_t ARG_block, const std::size_t ARG_pos) noexcept
        : block(ARG_block), pos(ARG_pos)
    {
    }

    bits::TYPE_pos::TYPE_pos(const TYPE_pos & ARG_origin) noexcept
        : block(ARG_origin.block), pos(ARG_origin.pos)
    {
    }
    const bits::TYPE_pos bits::INNER_trans_pos(const std::size_t ARG_pos) const noexcept
    {
        // 前端没有不完整的块
        if (this->MEMBER_front_pos == 0)
        {
            const bits::TYPE_pos result(ARG_pos / bits::DATA_bit_len, ARG_pos % bits::DATA_bit_len);
            return result;
        }
        // 真实位置在第0块内
        if (ARG_pos + this->MEMBER_front_pos < bits::DATA_bit_len)
        {
            const bits::TYPE_pos result(0, ARG_pos + this->MEMBER_front_pos);
            return result;
        }
        // 真实位置不在第0块
        else
        {
            std::size_t TEMP_sft = ARG_pos - (bits::DATA_bit_len - this->MEMBER_front_pos); // ARG_pos相对于第1块第0位的偏移量
            const bits::TYPE_pos result(TEMP_sft / bits::DATA_bit_len + 1, TEMP_sft % bits::DATA_bit_len);
            return result;
        }
    }
    bits::TYPE_bit_op::reference bits::INNER_unsafe_at(const std::size_t ARG_pos) noexcept
    {
        const auto TEMP_pos = bits::INNER_trans_pos(ARG_pos);
        return this->MEMBER_data[TEMP_pos.block][TEMP_pos.pos];
    }
    bool bits::INNER_unsafe_at(const std::size_t ARG_pos) const noexcept
    {
        const auto TEMP_pos = this->INNER_trans_pos(ARG_pos);
        return this->MEMBER_data[TEMP_pos.block][TEMP_pos.pos];
    }

    void bits::ERROR_board()
    {
        throw std::out_of_range("访问越界");
    }

    void bits::ERROR_board(const std::size_t ARG_pos) const
    {
        if (ARG_pos >= this->MEMBER_size)
        {
            bits::ERROR_board();
        }
    }

    void bits::ERROR_empty() const
    {
        if (this->MEMBER_size == 0)
        {
            throw std::logic_error("容器为空");
        }
    }
    char bits::value_to_char(const bool ARG_input) noexcept
    {
        char result;
        switch (ARG_input)
        {
            case true:
            {
                result = '1';
                break;
            }
            case false:
            {
                result = '0';
                break;
            }
        }
        return result;
    }
    wchar_t bits::value_to_wchar(const bool ARG_input) noexcept
    {
        wchar_t result;
        switch (ARG_input)
        {
            case true:
            {
                result = L'1';
                break;
            }
            case false:
            {
                result = L'0';
                break;
            }
        }
        return result;
    }
    char16_t bits::value_to_char16(const bool ARG_input) noexcept
    {
        char16_t result;
        switch (ARG_input)
        {
            case true:
            {
                result = u'1';
                break;
            }
            case false:
            {
                result = u'0';
                break;
            }
        }
        return result;
    }
    char32_t bits::value_to_char32(const bool ARG_input) noexcept
    {
        char32_t result;
        switch (ARG_input)
        {
            case true:
            {
                result = U'1';
                break;
            }
            case false:
            {
                result = U'0';
                break;
            }
        }
        return result;
    }
    bool bits::char_to_value(const char ARG_input)
    {
        bool result;
        switch (ARG_input)
        {
            case '0':
            {
                result = false;
                break;
            }
            case '1':
            {
                result = true;
                break;
            }
            default:
            {
                bits::ERROR_bit_char();
                break;
            }
        }
        return result;
    }
    bool bits::char_to_value(const wchar_t ARG_input)
    {
        bool result;
        switch (ARG_input)
        {
            case L'0':
            {
                result = false;
                break;
            }
            case L'1':
            {
                result = true;
                break;
            }
            default:
            {
                bits::ERROR_bit_char();
                break;
            }
        }
        return result;
    }
    bool bits::char_to_value(const char16_t ARG_input)
    {
        bool result;
        switch (ARG_input)
        {
            case u'0':
            {
                result = false;
                break;
            }
            case u'1':
            {
                result = true;
                break;
            }
            default:
            {
                bits::ERROR_bit_char();
                break;
            }
        }
        return result;
    }
    bool bits::char_to_value(const char32_t ARG_input)
    {
        bool result;
        switch (ARG_input)
        {
            case U'0':
            {
                result = false;
                break;
            }
            case U'1':
            {
                result = true;
                break;
            }
            default:
            {
                bits::ERROR_bit_char();
                break;
            }
        }
        return result;
    }
    void bits::ERROR_bit_char()
    {
        throw std::invalid_argument("字符不能表示二进制位");
    }
    bits::bits() noexcept
        : MEMBER_size(0), MEMBER_front_pos(0)
    {
    }
    bits::bits(const std::size_t ARG_size)
        : MEMBER_front_pos(0), MEMBER_size(ARG_size)
    {
        if (ARG_size == 0)
        {
            return;
        }
        for (std::size_t TEMP_counter = 0; TEMP_counter <= bits::INNER_trans_pos(ARG_size - 1).block; TEMP_counter++)
        {
            this->MEMBER_data.push_back(bits::TYPE_bit_op());
        }
    }
    bits::bits(const std::string & ARG_input)
        : bits(ARG_input.size())
    {

        for (std::size_t TEMP_counter = 0; TEMP_counter < ARG_input.size(); TEMP_counter++)
        {

            this->INNER_unsafe_at(TEMP_counter) = bits::char_to_value(ARG_input[TEMP_counter]);
        }
    }
    void bits::operator=(const std::string & ARG_input)
    {
        (*this) = bits(ARG_input);
    }
    bits::bits(const std::wstring & ARG_input)
        : bits(ARG_input.size())
    {

        for (std::size_t TEMP_counter = 0; TEMP_counter < ARG_input.size(); TEMP_counter++)
        {

            this->INNER_unsafe_at(TEMP_counter) = bits::char_to_value(ARG_input[TEMP_counter]);
        }
    }
    void bits::operator=(const std::wstring & ARG_input)
    {
        (*this) = bits(ARG_input);
    }
    bits::bits(const std::u16string & ARG_input)
        : bits(ARG_input.size())
    {

        for (std::size_t TEMP_counter = 0; TEMP_counter < ARG_input.size(); TEMP_counter++)
        {

            this->INNER_unsafe_at(TEMP_counter) = bits::char_to_value(ARG_input[TEMP_counter]);
        }
    }

    void bits::operator=(const std::u16string & ARG_input)
    {
        (*this) = bits(ARG_input);
    }

    bits::bits(const std::u32string & ARG_input)
        : bits(ARG_input.size())
    {

        for (std::size_t TEMP_counter = 0; TEMP_counter < ARG_input.size(); TEMP_counter++)
        {

            this->INNER_unsafe_at(TEMP_counter) = bits::char_to_value(ARG_input[TEMP_counter]);
        }
    }

    void bits::operator=(const std::u32string & ARG_input)
    {
        (*this) = bits(ARG_input);
    }

    bits::bits(const std::deque<bool> & ARG_input)
        : bits(ARG_input.size())
    {
        for (std::size_t TEMP_count = 0; TEMP_count < ARG_input.size(); TEMP_count++)
        {
            this->INNER_unsafe_at(TEMP_count) = ARG_input[TEMP_count];
        }
    }
    bits::bits(const std::queue<bool> & ARG_input)
        : bits(ARG_input.size())
    {
        auto TEMP_copy = ARG_input;
        for (std::size_t TEMP_count = 0; TEMP_count < ARG_input.size(); TEMP_count++)
        {
            this->INNER_unsafe_at(TEMP_count) = TEMP_copy.front();
            TEMP_copy.pop();
        }
    }
    bits::bits(const std::vector<bool> & ARG_input)
        : bits(ARG_input.size())
    {

        for (std::size_t TEMP_count = 0; TEMP_count < ARG_input.size(); TEMP_count++)
        {
            this->INNER_unsafe_at(TEMP_count) = ARG_input[TEMP_count];
        }
    }
    bits::bits(const std::list<bool> & ARG_input)
        : bits(ARG_input.size())
    {
        std::size_t TEMP_count = 0;
        for (auto TEMP_itor = ARG_input.cbegin(); TEMP_itor != ARG_input.cend(); TEMP_itor++)
        {
            this->INNER_unsafe_at(TEMP_count) = *TEMP_itor;
            TEMP_count++;
        }
    }
    bits::bits(const std::forward_list<bool> & ARG_input)
        : bits()
    {
        for (auto TEMP_itor = ARG_input.cbegin(); TEMP_itor != ARG_input.cend(); TEMP_itor++)
        {
            auto TEMP_pos = bits::INNER_trans_pos(this->MEMBER_size);
            if (TEMP_pos.block >= this->MEMBER_data.size())
            {
                this->MEMBER_data.push_back(bits::TYPE_bit_op());
            }
            this->INNER_unsafe_at(this->MEMBER_size) = *TEMP_itor;
            this->MEMBER_size++;
        }
    }
    bits::bits(const std::stack<bool> & ARG_input)
        : bits(ARG_input.size())
    {
        auto TEMP_copy = ARG_input;
        for (std::size_t TEMP_count = 0; TEMP_count < ARG_input.size(); TEMP_count++)
        {
            this->INNER_unsafe_at(TEMP_count) = TEMP_copy.top();
            TEMP_copy.pop();
        }
    }
    bits::bits(const bits & ARG_origin)
        : MEMBER_size(ARG_origin.MEMBER_size), MEMBER_data(ARG_origin.MEMBER_data), MEMBER_front_pos(ARG_origin.MEMBER_front_pos)
    {
    }
    void bits::operator=(const bits & ARG_origin)
    {
        if (this == (&ARG_origin))
        {
            return;
        }
        this->MEMBER_data = ARG_origin.MEMBER_data;
        this->MEMBER_front_pos = ARG_origin.MEMBER_front_pos;
        this->MEMBER_size = ARG_origin.MEMBER_size;
    }
    bits::bits(bits && ARG_origin) noexcept
        : MEMBER_size(ARG_origin.MEMBER_size), MEMBER_data(std::move(ARG_origin.MEMBER_data)), MEMBER_front_pos(ARG_origin.MEMBER_front_pos)
    {
        ARG_origin.MEMBER_size = 0;
        ARG_origin.MEMBER_front_pos = 0;
    }
    void bits::operator=(bits && ARG_origin) noexcept
    {
        if (this == (&ARG_origin))
        {
            return;
        }
        this->MEMBER_data = std::move(ARG_origin.MEMBER_data);
        this->MEMBER_front_pos = ARG_origin.MEMBER_front_pos;
        this->MEMBER_size = ARG_origin.MEMBER_size;
        ARG_origin.MEMBER_front_pos = 0;
        ARG_origin.MEMBER_size = 0;
    }
    bits::~bits()
    {
    }
    void bits::clear() noexcept
    {
        this->MEMBER_data.clear();
        this->MEMBER_size = 0;
        this->MEMBER_front_pos = 0;
    }
    bool bits::empty() const noexcept
    {
        if (this->MEMBER_size == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    std::size_t bits::size() const noexcept
    {

        return this->MEMBER_size;
    }
    bits::TYPE_bit_op::reference bits::operator[](const std::size_t ARG_pos)
    {
        return this->at(ARG_pos);
    }
    bits::TYPE_bit_op::reference bits::at(const std::size_t ARG_pos)
    {

        this->ERROR_board(ARG_pos);

        return this->INNER_unsafe_at(ARG_pos);
    }
    bool bits::operator[](const std::size_t ARG_pos) const
    {
        return this->at(ARG_pos);
    }
    bool bits::at(const std::size_t ARG_pos) const
    {
        this->ERROR_board(ARG_pos);

        return this->INNER_unsafe_at(ARG_pos);
    }
    bits::TYPE_bit_op::reference bits::back()
    {
        this->ERROR_empty();
        return this->INNER_unsafe_at(this->MEMBER_size - 1);
    }
    bits::TYPE_bit_op::reference bits::front()
    {
        this->ERROR_empty();

        return this->INNER_unsafe_at(0);
    }
    bool bits::back() const
    {
        this->ERROR_empty();

        return this->INNER_unsafe_at(this->MEMBER_size - 1);
    }
    bool bits::front() const
    {
        this->ERROR_empty();

        return this->INNER_unsafe_at(0);
    }
    bits bits::frag(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);

        if (ARG_begin > ARG_end)
        {
            bits result(ARG_begin - ARG_end + 1);

            for (std::size_t TEMP_count = 0; TEMP_count <= ARG_begin - ARG_end; TEMP_count++)
            {
                result.INNER_unsafe_at(TEMP_count) = this->INNER_unsafe_at(ARG_begin - TEMP_count);
            }

            return result;
        }
        else
        {
            bits result(ARG_end - ARG_begin + 1);
            for (std::size_t TEMP_count = 0; TEMP_count <= ARG_end - ARG_begin; TEMP_count++)
            {
                result.INNER_unsafe_at(TEMP_count) = this->INNER_unsafe_at(ARG_begin + TEMP_count);
            }
            return result;
        }
    }
    bits::operator std::string() const
    {
        std::string result;
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {

            result.push_back(bits::value_to_char(this->INNER_unsafe_at(TEMP_count)));
        }
        return result;
    }
    bits::operator std::wstring() const
    {
        std::wstring result;
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            result.push_back(bits::value_to_wchar(this->INNER_unsafe_at(TEMP_count)));
        }
        return result;
    }
    bits::operator std::u16string() const
    {
        std::u16string result;
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            result.push_back(bits::value_to_char16(this->INNER_unsafe_at(TEMP_count)));
        }
        return result;
    }
    bits::operator std::u32string() const
    {
        std::u32string result;
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            result.push_back(bits::value_to_char32(this->INNER_unsafe_at(TEMP_count)));
        }
        return result;
    }
    std::string bits::str(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);

        std::string result;
        if (ARG_begin > ARG_end)
        {

            for (std::size_t TEMP_count = ARG_begin; TEMP_count >= ARG_end; TEMP_count--)
            {
                result.push_back(bits::value_to_char(this->INNER_unsafe_at(TEMP_count)));
            }
        }
        else
        {

            for (std::size_t TEMP_count = ARG_begin; TEMP_count <= ARG_end; TEMP_count++)
            {
                result.push_back(bits::value_to_char(this->INNER_unsafe_at(TEMP_count)));
            }
        }
        return result;
    }
    std::wstring bits::wstr(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);

        std::wstring result;
        if (ARG_begin > ARG_end)
        {

            for (std::size_t TEMP_count = ARG_begin; TEMP_count >= ARG_end; TEMP_count--)
            {
                result.push_back(bits::value_to_wchar(this->INNER_unsafe_at(TEMP_count)));
            }
        }
        else
        {

            for (std::size_t TEMP_count = ARG_begin; TEMP_count <= ARG_end; TEMP_count++)
            {
                result.push_back(bits::value_to_wchar(this->INNER_unsafe_at(TEMP_count)));
            }
        }
        return result;
    }
    std::u16string bits::u16str(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);

        std::u16string result;
        if (ARG_begin > ARG_end)
        {

            for (std::size_t TEMP_count = ARG_begin; TEMP_count >= ARG_end; TEMP_count--)
            {
                result.push_back(bits::value_to_char16(this->INNER_unsafe_at(TEMP_count)));
            }
        }
        else
        {

            for (std::size_t TEMP_count = ARG_begin; TEMP_count <= ARG_end; TEMP_count++)
            {
                result.push_back(bits::value_to_char16(this->INNER_unsafe_at(TEMP_count)));
            }
        }
        return result;
    }
    std::u32string bits::u32str(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);

        std::u32string result;
        if (ARG_begin > ARG_end)
        {

            for (std::size_t TEMP_count = ARG_begin; TEMP_count >= ARG_end; TEMP_count--)
            {
                result.push_back(bits::value_to_char32(this->INNER_unsafe_at(TEMP_count)));
            }
        }
        else
        {

            for (std::size_t TEMP_count = ARG_begin; TEMP_count <= ARG_end; TEMP_count++)
            {
                result.push_back(bits::value_to_char32(this->INNER_unsafe_at(TEMP_count)));
            }
        }
        return result;
    }

    bits::operator std::vector<bool>() const
    {
        std::vector<bool> result;
        for (std::size_t TEMP_counter = 0; TEMP_counter < this->MEMBER_size; TEMP_counter++)
        {
            result.push_back(this->INNER_unsafe_at(TEMP_counter));
        }
        return result;
    }

    bits::operator std::deque<bool>() const
    {
        std::deque<bool> result;
        for (std::size_t TEMP_counter = 0; TEMP_counter < this->MEMBER_size; TEMP_counter++)
        {
            result.push_back(this->INNER_unsafe_at(TEMP_counter));
        }
        return result;
    }

    bits::operator std::queue<bool>() const
    {
        std::queue<bool> result;
        for (std::size_t TEMP_counter = 0; TEMP_counter < this->MEMBER_size; TEMP_counter++)
        {
            result.push(this->INNER_unsafe_at(TEMP_counter));
        }
        return result;
    }

    bits::operator std::list<bool>() const
    {
        std::list<bool> result;
        for (std::size_t TEMP_counter = 0; TEMP_counter < this->MEMBER_size; TEMP_counter++)
        {
            result.push_back(this->INNER_unsafe_at(TEMP_counter));
        }
        return result;
    }

    bits::operator std::forward_list<bool>() const
    {
        std::forward_list<bool> result;
        for (std::size_t TEMP_counter = 0; TEMP_counter < this->MEMBER_size; TEMP_counter++)
        {
            result.push_front(this->INNER_unsafe_at(TEMP_counter));
        }
        return result;
    }

    bits::operator std::stack<bool>() const
    {
        std::stack<bool> result;
        for (std::size_t TEMP_counter = 0; TEMP_counter < this->MEMBER_size; TEMP_counter++)
        {
            result.push(this->INNER_unsafe_at(TEMP_counter));
        }
        return result;
    }

    void bits::push_back(const bool ARG_value)
    {
        const auto TEMP_pos = bits::INNER_trans_pos(this->MEMBER_size);
        if (TEMP_pos.block >= this->MEMBER_data.size())
        {
            this->MEMBER_data.push_back(bits::TYPE_bit_op());
        }
        this->INNER_unsafe_at(this->MEMBER_size) = ARG_value;
        this->MEMBER_size++;
    }
    void bits::push_back(const bits & ARG_input)
    {
        if (ARG_input.MEMBER_size == 0)
        {
            return;
        }
        const auto TEMP_pos = bits::INNER_trans_pos(this->MEMBER_size + ARG_input.MEMBER_size - 1);
        while (TEMP_pos.block >= this->MEMBER_data.size())
        {
            this->MEMBER_data.push_back(bits::TYPE_bit_op());
        }

        for (std::size_t TEMP_count = 0; TEMP_count < ARG_input.size(); TEMP_count++)
        {
            this->INNER_unsafe_at(this->MEMBER_size + TEMP_count) = ARG_input.INNER_unsafe_at(TEMP_count);
        }
        this->MEMBER_size = this->MEMBER_size + ARG_input.MEMBER_size;
    }
    void bits::push_front(const bool ARG_value)
    {
        if (this->MEMBER_front_pos == 0)
        {
            this->MEMBER_data.push_front(bits::TYPE_bit_op());
            this->MEMBER_front_pos = bits::DATA_bit_len - 1;
        }
        else
        {
            this->MEMBER_front_pos--;
        }
        this->INNER_unsafe_at(0) = ARG_value;
        this->MEMBER_size++;
    }
    void bits::push_front(const bits & ARG_input)
    {
        if (ARG_input.MEMBER_size <= this->MEMBER_front_pos)
        {
            this->MEMBER_front_pos = this->MEMBER_front_pos - ARG_input.MEMBER_size;
        }
        else
        {
            for (std::size_t TEMP_count = 0; TEMP_count < (ARG_input.MEMBER_size - this->MEMBER_front_pos) / bits::DATA_bit_len + 1; TEMP_count++)
            {
                this->MEMBER_data.push_front(bits::TYPE_bit_op());
            }
            this->MEMBER_front_pos = bits::DATA_bit_len - ((ARG_input.MEMBER_size - this->MEMBER_front_pos) % bits::DATA_bit_len);
        }
        for (std::size_t TEMP_count = 0; TEMP_count < ARG_input.MEMBER_size; TEMP_count++)
        {
            this->INNER_unsafe_at(TEMP_count) = ARG_input.INNER_unsafe_at(TEMP_count);
        }
        this->MEMBER_size = this->MEMBER_size + ARG_input.MEMBER_size;
    }
    bool bits::pop_back()
    {
        this->ERROR_empty();

        bool result = this->INNER_unsafe_at(this->MEMBER_size - 1);

        this->MEMBER_size--;
        if (this->MEMBER_size == 0)
        {
            this->MEMBER_data.clear();
            this->MEMBER_front_pos = 0;
        }
        else
        {
            const auto TEMP_pos = this->INNER_trans_pos(this->MEMBER_size - 1);
            if (TEMP_pos.block + 1 < this->MEMBER_data.size())
            {
                this->MEMBER_data.pop_back();
            }
        }
        return result;
    }
    void bits::pop_back(const std::size_t ARG_length)
    {
        if (this->MEMBER_size < ARG_length)
        {
            bits::ERROR_board();
        }

        this->MEMBER_size = this->MEMBER_size - ARG_length;
        if (this->MEMBER_size == 0)
        {
            this->MEMBER_data.clear();
            this->MEMBER_front_pos = 0;
        }
        else
        {
            const auto TEMP_pos = this->INNER_trans_pos(this->MEMBER_size - 1);
            while (this->MEMBER_data.size() > TEMP_pos.block + 1)
            {
                this->MEMBER_data.pop_back();
            }
        }
    }
    bool bits::pop_front()
    {
        this->ERROR_empty();
        bool result = this->INNER_unsafe_at(0);
        if (this->MEMBER_front_pos + 1 == bits::DATA_bit_len)
        {
            this->MEMBER_front_pos = 0;
            this->MEMBER_data.pop_front();
        }
        else
        {
            this->MEMBER_front_pos++;
        }
        this->MEMBER_size--;
        return result;
    }
    void bits::pop_front(const std::size_t ARG_length)
    {
        if (this->MEMBER_size < ARG_length)
        {
            bits::ERROR_board();
        }

        this->MEMBER_size = this->MEMBER_size - ARG_length;
        if (this->MEMBER_size == 0)
        {
            this->MEMBER_front_pos = 0;
            this->MEMBER_data.clear();
        }
        else
        {
            if (ARG_length + this->MEMBER_front_pos < bits::DATA_bit_len)
            {
                this->MEMBER_front_pos = ARG_length + this->MEMBER_front_pos;
            }
            else
            {
                for (std::size_t TEMP_count = 0; TEMP_count < (ARG_length - (bits::DATA_bit_len - this->MEMBER_front_pos)) / bits::DATA_bit_len + 1; TEMP_count++)
                {
                    this->MEMBER_data.pop_front();
                }
                this->MEMBER_front_pos = (ARG_length - (bits::DATA_bit_len - this->MEMBER_front_pos)) % bits::DATA_bit_len;
            }
        }
    }

    void bits::flip() noexcept
    {
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            this->INNER_unsafe_at(TEMP_count) = (not this->INNER_unsafe_at(TEMP_count));
        }
    }
    void bits::flip(const std::size_t ARG_pos)
    {
        this->ERROR_board(ARG_pos);
        this->INNER_unsafe_at(ARG_pos) = (not this->INNER_unsafe_at(ARG_pos));
    }
    void bits::flip(const std::size_t ARG_begin, const std::size_t ARG_end)
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);
        if (ARG_begin <= ARG_end)
        {
            for (std::size_t TEMP_count = ARG_begin; TEMP_count < ARG_end; TEMP_count++)
            {
                this->INNER_unsafe_at(TEMP_count) = (not this->INNER_unsafe_at(TEMP_count));
            }
        }
        else
        {
            for (std::size_t TEMP_count = ARG_end; TEMP_count < ARG_begin; TEMP_count++)
            {
                this->INNER_unsafe_at(TEMP_count) = (not this->INNER_unsafe_at(TEMP_count));
            }
        }
    }
    std::size_t bits::count(const bool ARG_value) const noexcept
    {
        std::size_t result = 0;
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) == ARG_value)
            {
                result++;
            }
        }
        return result;
    }
    bool bits::any(const bool ARG_value) const noexcept
    {
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) == ARG_value)
            {
                return true;
            }
        }
        return false;
    }
    bool bits::all(const bool ARG_value) const noexcept
    {
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) != ARG_value)
            {
                return false;
            }
        }
        return true;
    }
    bool bits::operator==(const bits & ARG_right) const noexcept
    {
        if (this->MEMBER_size != ARG_right.MEMBER_size)
        {
            return false;
        }
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) != ARG_right.INNER_unsafe_at(TEMP_count))
            {
                return false;
            }
        }
        return true;
    }
    bool bits::operator!=(const bits & ARG_right) const noexcept
    {
        return not(*this == ARG_right);
    }
    bool bits::operator==(const std::string & ARG_right) const
    {
        if (this->MEMBER_size != ARG_right.size())
        {
            return false;
        }
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) != bits::char_to_value(ARG_right[TEMP_count]))
            {
                return false;
            }
        }
        return true;
    }
    bool bits::operator!=(const std::string & ARG_right) const
    {
        return not(*this == ARG_right);
    }
    bool bits::operator==(const std::wstring & ARG_right) const
    {
        if (this->MEMBER_size != ARG_right.size())
        {
            return false;
        }
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) != bits::char_to_value(ARG_right[TEMP_count]))
            {
                return false;
            }
        }
        return true;
    }
    bool bits::operator!=(const std::wstring & ARG_right) const
    {
        return not(*this == ARG_right);
    }
    bool bits::operator==(const std::u16string & ARG_right) const
    {
        if (this->MEMBER_size != ARG_right.size())
        {
            return false;
        }
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) != bits::char_to_value(ARG_right[TEMP_count]))
            {
                return false;
            }
        }
        return true;
    }
    bool bits::operator!=(const std::u16string & ARG_right) const
    {
        return not(*this == ARG_right);
    }
    bool bits::operator==(const std::u32string & ARG_right) const
    {
        if (this->MEMBER_size != ARG_right.size())
        {
            return false;
        }
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) != bits::char_to_value(ARG_right[TEMP_count]))
            {
                return false;
            }
        }
        return true;
    }
    bool bits::operator!=(const std::u32string & ARG_right) const
    {
        return not(*this == ARG_right);
    }
    bits bits::operator~() const noexcept
    {
        bits result(this->MEMBER_size);
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            result.INNER_unsafe_at(TEMP_count) = (not this->INNER_unsafe_at(TEMP_count));
        }
        return result;
    }

    void bits::ERROR_diff_len(const bits & ARG_right) const
    {
        if (this->MEMBER_size != ARG_right.MEMBER_size)
        {
            throw std::invalid_argument("位操作的2个操作数长度不同");
        }
    }
    bits bits::operator&(const bits & ARG_right) const
    {
        this->ERROR_diff_len(ARG_right);
        bits result(this->MEMBER_size);
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) == true and ARG_right.INNER_unsafe_at(TEMP_count) == true)
            {
                result.INNER_unsafe_at(TEMP_count) = true;
            }
            else
            {
                result.INNER_unsafe_at(TEMP_count) = false;
            }
        }
        return result;
    }
    void bits::operator&=(const bits & ARG_right)
    {
        this->ERROR_diff_len(ARG_right);

        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) == true and ARG_right.INNER_unsafe_at(TEMP_count) == true)
            {
                this->INNER_unsafe_at(TEMP_count) = true;
            }
            else
            {
                this->INNER_unsafe_at(TEMP_count) = false;
            }
        }
    }
    bits bits::operator|(const bits & ARG_right) const
    {
        this->ERROR_diff_len(ARG_right);
        bits result(this->MEMBER_size);
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) == false and ARG_right.INNER_unsafe_at(TEMP_count) == false)
            {
                result.INNER_unsafe_at(TEMP_count) = false;
            }
            else
            {
                result.INNER_unsafe_at(TEMP_count) = true;
            }
        }
        return result;
    }
    void bits::operator|=(const bits & ARG_right)
    {
        this->ERROR_diff_len(ARG_right);
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) == false and ARG_right.INNER_unsafe_at(TEMP_count) == false)
            {
                this->INNER_unsafe_at(TEMP_count) = false;
            }
            else
            {
                this->INNER_unsafe_at(TEMP_count) = true;
            }
        }
    }
    bits bits::operator^(const bits & ARG_right) const
    {
        this->ERROR_diff_len(ARG_right);
        bits result(this->MEMBER_size);
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) == ARG_right.INNER_unsafe_at(TEMP_count))
            {
                result.INNER_unsafe_at(TEMP_count) = false;
            }
            else
            {
                result.INNER_unsafe_at(TEMP_count) = true;
            }
        }
        return result;
    }
    void bits::operator^=(const bits & ARG_right)
    {
        this->ERROR_diff_len(ARG_right);
        for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
        {
            if (this->INNER_unsafe_at(TEMP_count) == ARG_right.INNER_unsafe_at(TEMP_count))
            {
                this->INNER_unsafe_at(TEMP_count) = false;
            }
            else
            {
                this->INNER_unsafe_at(TEMP_count) = true;
            }
        }
    }
    bits bits::operator<<(const std::size_t & ARG_right) const noexcept
    {
        bits result(this->MEMBER_size);
        if (ARG_right >= this->MEMBER_size)
        {
            for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
            {
                result.INNER_unsafe_at(TEMP_count) = false;
            }
        }
        else
        {
            for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size - ARG_right; TEMP_count++)
            {
                result.INNER_unsafe_at(TEMP_count) = this->INNER_unsafe_at(TEMP_count + ARG_right);
            }
            for (std::size_t TEMP_count = 0; TEMP_count < ARG_right; TEMP_count++)
            {
                result.INNER_unsafe_at(TEMP_count + this->MEMBER_size - ARG_right) = false;
            }
        }
        return result;
    }
    void bits::operator<<=(const std::size_t & ARG_right) noexcept
    {
        if (ARG_right >= this->MEMBER_size)
        {
            for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
            {
                this->INNER_unsafe_at(TEMP_count) = false;
            }
        }
        else
        {
            for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size - ARG_right; TEMP_count++)
            {
                this->INNER_unsafe_at(TEMP_count) = this->INNER_unsafe_at(TEMP_count + ARG_right);
            }
            for (std::size_t TEMP_count = 0; TEMP_count < ARG_right; TEMP_count++)
            {
                this->INNER_unsafe_at(TEMP_count + this->MEMBER_size - ARG_right) = false;
            }
        }
    }
    bits bits::operator>>(const std::size_t & ARG_right) const noexcept
    {
        bits result(this->MEMBER_size);
        if (ARG_right >= this->MEMBER_size)
        {
            for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
            {
                result.INNER_unsafe_at(TEMP_count) = false;
            }
        }
        else
        {
            for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size - ARG_right; TEMP_count++)
            {
                result.INNER_unsafe_at(this->MEMBER_size - 1 - TEMP_count) = this->INNER_unsafe_at(this->MEMBER_size - 1 - TEMP_count - ARG_right);
            }
            for (std::size_t TEMP_count = 0; TEMP_count < ARG_right; TEMP_count++)
            {
                result.INNER_unsafe_at(TEMP_count) = false;
            }
        }
        return result;
    }
    void bits::operator>>=(const std::size_t & ARG_right) noexcept
    {
        if (ARG_right >= this->MEMBER_size)
        {
            for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size; TEMP_count++)
            {
                this->INNER_unsafe_at(TEMP_count) = false;
            }
        }
        else
        {
            for (std::size_t TEMP_count = 0; TEMP_count < this->MEMBER_size - ARG_right; TEMP_count++)
            {
                this->INNER_unsafe_at(this->MEMBER_size - 1 - TEMP_count) = this->INNER_unsafe_at(this->MEMBER_size - 1 - TEMP_count - ARG_right);
            }
            for (std::size_t TEMP_count = 0; TEMP_count < ARG_right; TEMP_count++)
            {
                this->INNER_unsafe_at(TEMP_count) = false;
            }
        }
    }
    std::deque<bool> bits::deque(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);
        std::deque<bool> result;
        if (ARG_begin > ARG_end)
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter >= ARG_end; TEMP_counter--)
            {
                result.push_back(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        else
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter <= ARG_end; TEMP_counter++)
            {
                result.push_back(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        return result;
    }
    std::queue<bool> bits::queue(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {

        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);
        std::queue<bool> result;
        if (ARG_begin > ARG_end)
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter >= ARG_end; TEMP_counter--)
            {
                result.push(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        else
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter <= ARG_end; TEMP_counter++)
            {
                result.push(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        return result;
    }
    std::vector<bool> bits::vector(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);
        std::vector<bool> result;
        if (ARG_begin > ARG_end)
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter >= ARG_end; TEMP_counter--)
            {
                result.push_back(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        else
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter <= ARG_end; TEMP_counter++)
            {
                result.push_back(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        return result;
    }
    std::list<bool> bits::list(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);
        std::list<bool> result;
        if (ARG_begin > ARG_end)
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter >= ARG_end; TEMP_counter--)
            {
                result.push_back(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        else
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter <= ARG_end; TEMP_counter++)
            {
                result.push_back(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        return result;
    }
    std::forward_list<bool> bits::forward_list(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);
        std::forward_list<bool> result;
        if (ARG_begin > ARG_end)
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter >= ARG_end; TEMP_counter--)
            {
                result.push_front(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        else
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter <= ARG_end; TEMP_counter++)
            {
                result.push_front(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        return result;
    }
    std::stack<bool> bits::stack(const std::size_t ARG_begin, const std::size_t ARG_end) const
    {
        this->ERROR_board(ARG_begin);
        this->ERROR_board(ARG_end);
        std::stack<bool> result;
        if (ARG_begin > ARG_end)
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter >= ARG_end; TEMP_counter--)
            {
                result.push(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        else
        {
            for (std::size_t TEMP_counter = ARG_begin; TEMP_counter <= ARG_end; TEMP_counter++)
            {
                result.push(this->INNER_unsafe_at(TEMP_counter));
            }
        }
        return result;
    }

}
