#include ".\fastIO.hpp"
#include <iostream>

namespace OKps::file
{
    reader::reader(std::size_t size)
        : buffer_size(size), end_position(0), file_size(0), read_position(0), head_position(0), buffer(new char[size])
    {
        for (size_t TEMP_count = 0; TEMP_count < this->buffer_size; TEMP_count++)
        {
            this->buffer[TEMP_count] = (char)0;
        }
    }

    reader::~reader()
    {
        if (this->is_open())
        {
            this->close();
        }
        delete[] this->buffer;
    }
    template <typename string_type>
    void reader::INNER_open(string_type _route, std::ios_base::openmode _mode, int _prot, bool resize)
    {
        if (this->file.is_open())
        {
            throw OKps::error(u8"已经有一个打开的文件，禁止重复打开", OKps::error::error_type::DEBUG_logic);
        }
        else
        {
            this->route = _route;

            this->file.open(_route, _mode, _prot);

            if (std::filesystem::exists(this->route))
            {
                this->file_size = std::filesystem::file_size(this->route);
                // 自动调整缓冲区大小
                if (resize)
                {
                    this->auto_resizer();
                }
            }
            if (this->file.is_open())
            {
                // 根据输入的文件打开模式，调整读指针
                if (_mode == std::ios::in or _mode == (std::ios::in | std::ios::binary) or _mode == (std::ios::in | std::ios::app) or _mode == (std::ios::in | std::ios::app | std::ios::binary) or _mode == std::ios::binary or _mode == std::ios::app or _mode == (std::ios::app | std::ios::binary))
                {
                    this->read_position = 0;
                }
                else if (_mode == (std::ios::in | std::ios::ate) or _mode == (std::ios::in | std::ios::ate | std::ios::binary) or _mode == (std::ios::in | std::ios::ate | std::ios::app) or _mode == (std::ios::in | std::ios::ate | std::ios::app | std::ios::binary) or _mode == std::ios::ate or _mode == (std::ios::ate | std::ios::binary) or _mode == (std::ios::ate | std::ios::app) or _mode == (std::ios::ate | std::ios::app | std::ios::binary))
                {
                    this->read_position = this->file_size;
                }
                else
                {
                    this->file.close();
                    throw OKps::error(u8"_mode参数不合法", OKps::error::error_type::DEBUG_bad_parameter);
                }
            }
        }
    }

    void reader::open(const std::string & _route, std::ios_base::openmode _mode, int _prot, bool resize)
    {
        this->INNER_open(_route, _mode, _prot, resize);
    }

    void reader::open(const char * _route, std::ios_base::openmode _mode, int _prot, bool resize)
    {
        this->INNER_open(_route, _mode, _prot, resize);
    }

    void reader::open(const std::wstring & _route, std::ios_base::openmode _mode, int _prot, bool resize)
    {
        this->INNER_open(_route, _mode, _prot, resize);
    }

    void reader::open(const wchar_t * _route, std::ios_base::openmode _mode, int _prot, bool resize)
    {
        this->INNER_open(_route, _mode, _prot, resize);
    }

    void reader::open(const std::filesystem::path & _route, std::ios_base::openmode _mode, int _prot, bool resize)
    {
        this->INNER_open(_route, _mode, _prot, resize);
    }

    bool reader::is_open() const noexcept
    {
        return this->file.is_open();
    }

    void reader::close()
    {
        if (this->file.is_open())
        {
            this->file.close();
        }
        else
        {
            throw OKps::error(u8"没有打开的文件，无法关闭文件", OKps::error::error_type::DEBUG_logic);
        }
    }

    std::streampos reader::tellg() const noexcept
    {

        return this->read_position;
    }

    void reader::seekg(std::streampos _pos)
    {
        if (_pos >= 0)
        {
            this->read_position = _pos;
        }
        else
        {
            this->read_position = -1;
        }
    }

    void reader::seekg(std::streamoff _off, std::ios_base::seekdir _way)
    {
        if (_way == std::ios::beg)
        {
            auto result = _off;
            if (result >= 0)
            {
                this->read_position = result;
            }
            else
            {
                this->read_position = -1;
            }
        }
        else if (_way == std::ios::end)
        {
            auto result = this->file_size + _off;
            if (result >= 0)
            {
                this->read_position = result;
            }
            else
            {
                this->read_position = -1;
            }
        }
        else if (_way == std::ios::cur)
        {
            auto result = this->read_position + _off;
            if (result >= 0)
            {
                this->read_position = result;
            }
            else
            {
                this->read_position = -1;
            }
        }
        else
        {

            throw OKps::error(u8"_way 参数不合法", OKps::error::error_type::DEBUG_bad_parameter);
        }
    }

    bool reader::read(char * read_buffer, std::streamsize size)
    {
        if (size <= 0)
        {
            return true;
        }
        if (this->read_position < 0 or this->read_position > this->file_size or this->read_position + size < 0 or this->read_position + size > this->file_size)
        {
            // 申请读取的内容超过文件范围
            this->read_position = -1;
            return false;
        }
        if (this->end_position == 0)
        {

            // 缓冲区是空的
            this->refresh_buffer();
        }
        while (true)
        {
            // 这里显式调用operator +，是因为内置操作符+也符合操作数的要求，编译报错
            if (this->read_position < this->head_position or this->read_position >= (this->head_position.operator+(this->end_position)))
            {

                // 申请的读指针不在缓冲区范围内
                // 更新缓冲区
                this->refresh_buffer();
                // 现在缓冲区的头就是读指针的位置
            }
            if (this->read_position + size <= this->head_position.operator+(this->end_position))
            {

                // 要读的数据完全在缓冲区内
                for (std::size_t count = 0; count < size; count++)
                {
                    read_buffer[count] = this->buffer[this->read_position.operator+(count) -this->head_position];
                }
                this->read_position = this->read_position + size;
                return true;
            }
            else
            {

                // 先把缓冲区的相应数据全部写入
                auto to_read_size = this->head_position.operator+(this->end_position) - this->read_position;

                for (std::size_t count = 0; count < to_read_size; count++)
                {
                    read_buffer[count] = this->buffer[this->read_position.operator+(count) -this->head_position];
                }
                this->read_position = this->read_position + to_read_size;
                // this->read(&(this->buffer[_size]), size - _size);
                size = size - to_read_size;
                if (size <= 0)
                {
                    return true;
                }
                read_buffer = &(read_buffer[to_read_size]);
            }
        }
    }

    void reader::refresh_buffer()
    {
        if (this->read_position >= 0 and this->read_position <= this->file_size)
        {
            // 申请的读指针位置合法
            this->file.seekg(this->read_position);
            auto to_read_size = this->file_size - this->file.tellg(); // 文件可读的大小
            if (to_read_size <= this->buffer_size)
            {
                // 把文件读完，填不满缓冲区
                if (this->file.read(this->buffer, to_read_size))
                {
                }
                else
                {
                    goto FLOW_read_error;
                }
                this->end_position = to_read_size;
            }
            else
            {
                // 填满缓冲区
                if (this->file.read(this->buffer, this->buffer_size))
                {
                }
                else
                {
                    goto FLOW_read_error;
                }
                this->end_position = this->buffer_size;
            }
            // 把缓冲区的头定位到读指针
            this->head_position = this->read_position;
        }
        else
        {

            throw OKps::error(u8"申请的读指针位置超出文件范围", OKps::error::error_type::DEBUG_overstep);
        }
        /*
        读真实文件时出错
        */
    FLOW_read_error:
        {

            throw OKps::error(u8"读文件失败", OKps::error::error_type::IO_file);
        }
    }

    void reader::auto_resizer()
    {
        if (this->file_size <= 1000000)
        {
            this->resize_buffer(this->file_size);
        }
        else
        {
            // 如果文件大小超过缓冲区的100倍，就令缓冲区为原来的10倍
            // 循环此过程，直到文件大小不超过缓冲区的100倍，或者缓冲区长为1000000000
            size_t size1 = 100 * this->buffer_size;
            size_t size2 = 10 * this->buffer_size;
            while (this->file_size > size1)
            {
                size1 = 10 * size1;
                size2 = 10 * size2;
            }
            if (size2 <= 1000000000)
            {
                this->resize_buffer(size2);
            }
            else
            {
                this->resize_buffer(1000000000);
            }
        }
    }

    uintmax_t reader::size() const noexcept
    {
        return this->file_size;
    }

    void reader::resize_buffer(std::size_t new_size)
    {
        std::size_t _size; // 要保留的数据大小
        if (new_size >= this->end_position)
        {
            _size = this->end_position;
        }
        else
        {
            _size = new_size;
            this->end_position = new_size;
        }
        this->buffer_size = new_size;
        char * new_buffer = new char[new_size];
        for (std::size_t count = 0; count < _size; count++)
        {
            new_buffer[count] = this->buffer[count];
        }
        for (std::size_t count = _size; count < new_size; count++)
        {
            new_buffer[count] = 0;
        }
        delete[] this->buffer;
        this->buffer = new_buffer;
    }

    writter::writter(std::size_t size)
        : buffer_size(size), buffer(new char[size]), file_size(0), write_position(0), head_position(0), app_mode(false)
    {
        // 把数据全部初始化，是为了真正占有申请的内存
        // 现代操作系统，只有在进程尝试使用内存的时候，才会真正分配给进程
        // 进程可以用malloc或者new申请一大堆内存，甚至超过系统可用内存，任务管理器里是不会显示的，因为事实确实如此
        for (std::size_t count = 0; count < this->buffer_size; count++)
        {
            this->buffer[count] = 0;
            this->valid.push_back(false);
        }
    }

    writter::~writter()
    {
        if (this->is_open())
        {
            this->close();
        }

        delete[] this->buffer;
    }
    template <typename string_type>
    void writter::INNER_open(string_type _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        if (this->file.is_open())
        {

            throw OKps::error(u8"已经有一个打开的文件，禁止重复打开", OKps::error::error_type::DEBUG_logic);
        }
        else
        {
            this->route = _route;
            this->file.open(_route, _mode, _prot);
            if (std::filesystem::exists(this->route))
            {
                this->file_size = std::filesystem::file_size(this->route);
                if (resize)
                {
                    this->auto_resizer(guess_size);
                }
            }
            if (this->file.is_open())
            {
                // 根据输入的文件打开模式，调整写指针
                if (_mode == (std::ios::out | std::ios::app) or _mode == (std::ios::out | std::ios::app | std::ios::binary) or _mode == std::ios::app or _mode == (std::ios::app | std::ios::binary))
                {
                    this->write_position = 0;
                    this->app_mode = true;
                }
                else if (_mode == (std::ios::out | std::ios::ate) or _mode == (std::ios::out | std::ios::ate | std::ios::binary) or _mode == (std::ios::out | std::ios::ate | std::ios::trunc) or _mode == (std::ios::out | std::ios::ate | std::ios::trunc | std::ios::binary) or _mode == std::ios::ate or _mode == (std::ios::ate | std::ios::binary) or _mode == (std::ios::ate | std::ios::trunc) or _mode == (std::ios::ate | std::ios::trunc | std::ios::binary))
                {
                    this->write_position = this->file_size;
                    this->app_mode = false;
                }
                else if (_mode == (std::ios::out | std::ios::ate | std::ios::app) or _mode == (std::ios::out | std::ios::ate | std::ios::app | std::ios::binary) or _mode == (std::ios::ate | std::ios::app) or _mode == (std::ios::ate | std::ios::app | std::ios::binary))
                {
                    this->write_position = this->file_size;
                    this->app_mode = true;
                }
                else if (_mode == std::ios::out or _mode == (std::ios::out | std::ios::binary) or _mode == (std::ios::out | std::ios::trunc) or _mode == (std::ios::out | std::ios::trunc | std::ios::binary) or _mode == std::ios::binary or _mode == std::ios::trunc or _mode == (std::ios::trunc | std::ios::binary))
                {
                    this->write_position = 0;
                    this->app_mode = false;
                }
                else
                {
                    this->file.close();
                    throw OKps::error(u8"_mode参数不合法", OKps::error::error_type::DEBUG_bad_parameter);
                }
            }
        }
    }

    void writter::open(const std::string & _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    void writter::open(const char * _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    void writter::open(const std::wstring & _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    void writter::open(const wchar_t * _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    void writter::open(const std::filesystem::path & _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    bool writter::is_open() const noexcept
    {
        return this->file.is_open();
    }

    void writter::close()
    {
        if (this->file.is_open())
        {
            // 在关闭文件之前，更新缓冲区，写入最后一批数据
            this->refresh_buffer();
            this->file.close();
        }
        else
        {

            throw OKps::error(u8"没有打开的文件，无法关闭文件", OKps::error::error_type::DEBUG_logic);
        }
    }

    std::streampos writter::tellp() const noexcept
    {
        return this->write_position;
    }

    void writter::seekp(std::streampos _pos)
    {
        if (_pos >= 0)
        {
            this->write_position = _pos;
            if (not this->app_mode and this->write_position > this->file_size)
            {
                this->file_size = this->write_position;
            }
        }
        else
        {
            this->write_position = -1;
        }
    }

    void writter::seekp(std::streamoff _off, std::ios_base::seekdir _way)
    {
        if (_way == std::ios::beg)
        {
            if (_off >= 0)
            {
                this->write_position = _off;
                if (not this->app_mode and this->write_position > this->file_size)
                {
                    this->file_size = this->write_position;
                }
            }
            else
            {
                this->write_position = -1;
            }
        }
        else if (_way == std::ios::end)
        {
            auto result = this->file_size + _off;
            if (result >= 0)
            {
                this->write_position = result;
                if (not this->app_mode and this->write_position > this->file_size)
                {
                    this->file_size = this->write_position;
                }
            }
            else
            {
                this->write_position = -1;
            }
        }
        else if (_way == std::ios::cur)
        {
            auto result = this->write_position + _off;
            if (result >= 0)
            {
                this->write_position = result;
                if (not this->app_mode and this->write_position > this->file_size)
                {
                    this->file_size = this->write_position;
                }
            }
            else
            {
                this->write_position = -1;
            }
        }
        else
        {

            throw OKps::error(u8"不合法的参数 _way", OKps::error::error_type::DEBUG_bad_parameter);
        }
    }

    bool writter::write(const char * write_buffer, std::streamsize size)
    {
        if (size <= 0)
        {
            return true;
        }
        if (this->write_position < 0)
        {
            this->write_position = -1;
            return false;
        }
        if (this->app_mode)
        {
            this->write_position = this->file_size;
        }
        while (true)
        {
            if (this->write_position < this->head_position or this->write_position >= this->head_position.operator+(this->buffer_size))
            {
                // 写指针在缓冲区的有效范围之外
                // 刷新缓冲区
                this->refresh_buffer();
            }
            if (this->write_position + size <= this->head_position.operator+(this->buffer_size))
            {
                // 要写的内容能全部塞进缓冲区
                for (std::size_t count = 0; count < size; count++)
                {
                    this->buffer[count + this->write_position - this->head_position] = write_buffer[count];
                    this->valid[count + this->write_position - this->head_position] = true;
                }
                this->write_position = this->write_position + size;
                if (this->app_mode)
                {
                    this->file_size = this->write_position;
                }
                return true;
            }
            else
            {
                // 缓冲区装不下要写入的内容
                // 先把缓冲区塞满
                auto to_write_size = this->buffer_size - (this->write_position - this->head_position);
                for (std::size_t count = 0; count < to_write_size; count++)
                {
                    this->buffer[count + this->write_position - this->head_position] = write_buffer[count];
                    this->valid[count + this->write_position - this->head_position] = true;
                }
                this->write_position = this->write_position.operator+(to_write_size);
                size = size - to_write_size;
                if (size <= 0)
                {
                    if (this->app_mode)
                    {
                        this->file_size = this->write_position;
                    }
                    return true;
                }
                write_buffer = &(write_buffer[to_write_size]);
            }
        }

        return true;
    }

    writter & writter::operator<<(const char * data)
    {
        const std::string temp = data;
        return (*this << temp);
    }

    writter & writter::operator<<(const std::string & data)
    {
        this->write(data.c_str(), data.size());
        return *this;
    }

    void writter::resize_buffer(const std::size_t size)
    {
        std::size_t used_size = 0;
        for (std::size_t count = 0; count < this->buffer_size; count++)
        {
            if (this->valid[count])
            {
                used_size = count + 1;
            }
        }
        char * new_buffer = new char[size];
        std::vector<bool> new_valid;
        if (size < used_size)
        {
            // 新缓冲区的长度 < 旧缓冲区记录的有效数据的长度
            // 把塞不下的这部分数据写入文件
            std::size_t to_write_size = used_size - size;
            auto TEMP_valid_itor = this->valid.cbegin() + size;
            this->valid_writter(&(this->buffer[size]), TEMP_valid_itor, to_write_size, this->head_position.operator+(size));
        }
        if (size < this->buffer_size)
        {
            for (std::size_t count = 0; count < size; count++)
            {
                new_buffer[count] = this->buffer[count];
                new_valid.push_back(this->valid[count]);
            }
        }
        else
        {
            for (std::size_t count = 0; count < this->buffer_size; count++)
            {
                new_buffer[count] = this->buffer[count];
                new_valid.push_back(this->valid[count]);
            }
            for (std::size_t count = this->buffer_size; count < size; count++)
            {
                new_buffer[count] = 0;
                new_valid.push_back(false);
            }
        }
        this->buffer_size = size;
        delete[] this->buffer;
        this->buffer = new_buffer;
        this->valid = new_valid;
    }

    void writter::refresh_buffer()
    {
        auto TEMP_valid_itor = this->valid.cbegin();
        // 把缓冲区内的数据写入文件，然后清空缓冲区
        this->valid_writter(this->buffer, TEMP_valid_itor, this->buffer_size, this->head_position);
        for (std::size_t count = 0; count < this->buffer_size; count++)
        {
            this->valid[count] = false;
        }
        // 把缓冲区的头定位到写指针的位置
        this->head_position = this->write_position;
    }

    void writter::auto_resizer(const uintmax_t size_guess)
    {
        if (size_guess <= 1000000)
        {
            this->resize_buffer(size_guess);
        }
        else
        {
            // 如果文件大小超过缓冲区的100倍，就令缓冲区为原来的10倍
            // 循环此过程，直到文件大小不超过缓冲区的100倍，或者缓冲区长为1000000000
            size_t size1 = 100 * this->buffer_size;
            size_t size2 = 10 * this->buffer_size;
            while (size_guess > size1)
            {
                size1 = 10 * size1;
                size2 = 10 * size2;
            }
            if (size2 <= 1000000000)
            {
                this->resize_buffer(size2);
            }
            else
            {
                this->resize_buffer(1000000000);
            }
        }
    }
    /*
        void writter::valid_writter(const char *write_buffer, const bool *checker, const std::size_t length, std::streamoff _off, std::ios_base::seekdir _way)
        {
            this->file.seekp(_off, _way);
            std::size_t begin = 0;
            long long int end = -1;
            long long int last = -1;
            for (std::size_t count = 0; count < length; count++)
            {
                if (checker[count])
                {
                    last = count;
                }
            }
            // 现在last是最后一个有效内容的位置
            if (last == -1)
            {
                return;
            }
            while (end < last)
            {

                for (std::size_t count = end + 1; count < length; count++)
                {
                    if (checker[count])
                    {
                        begin = count;
                        this->file.seekp(count - end - 1, std::ios::cur);
                        break;
                    }
                }
                // 现在begin是下一块有效内容的起始位置

                end = begin;
                bool last_end = true;
                for (std::size_t count = begin + 1; count < length; count++)
                {

                    if (not checker[count])
                    {
                        end = count - 1;
                        last_end = false;
                        break;
                    }
                }
                if (last_end)
                {
                    end = length - 1;
                }
                // 现在end是begin开始的这一块有效内容的结束位置

                if (this->file.write(&(write_buffer[begin]), end - begin + 1))
                {
                }
                else
                {

                    throw OKps::error(u8"文件写入失败", false, false, true, OKps::error::error_type::IO_file);
                }
            }
        }
    */
    void writter::valid_writter(const char * write_buffer, const std::vector<bool>::const_iterator & checker, const std::size_t length, std::streampos _pos)
    {
        this->file.seekp(_pos);
        std::size_t begin = 0;
        long long int end = -1;
        long long int last = -1;
        for (std::size_t count = 0; count < length; count++)
        {
            if (checker[count])
            {
                last = count;
            }
        }
        // 现在last是最后一个有效内容的位置
        if (last == -1)
        {
            return;
        }
        while (end < last)
        {

            for (std::size_t count = end + 1; count < length; count++)
            {
                if (checker[count])
                {
                    begin = count;
                    this->file.seekp(count - end - 1, std::ios::cur);
                    break;
                }
            }
            // 现在begin是下一块有效内容的起始位置

            end = begin;
            bool last_end = true;
            for (std::size_t count = begin + 1; count < length; count++)
            {

                if (not checker[count])
                {
                    end = count - 1;
                    last_end = false;
                    break;
                }
            }
            if (last_end)
            {
                end = length - 1;
            }
            // 现在end是begin开始的这一块有效内容的结束位置

            if (this->file.write(&(write_buffer[begin]), end - begin + 1))
            {
            }
            else
            {

                throw OKps::error(u8"文件写入失败", OKps::error::error_type::IO_file);
            }
        }
    }

    sequential_writter::sequential_writter(std::size_t size)
        : buffer_size(size), buffer(new char[size]), file_size(0), write_position(0), head_position(0), app_mode(false)
    {
        // 把数据全部初始化，是为了真正占有申请的内存
        // 现代操作系统，只有在进程尝试使用内存的时候，才会真正分配给进程
        // 进程可以用malloc或者new申请一大堆内存，甚至超过系统可用内存，任务管理器里是不会显示的，因为事实确实如此
        for (std::size_t count = 0; count < this->buffer_size; count++)
        {
            this->buffer[count] = 0;
        }
    }

    sequential_writter::~sequential_writter()
    {
        if (this->is_open())
        {
            this->close();
        }

        delete[] this->buffer;
    }
    template <typename string_type>
    void sequential_writter::INNER_open(string_type _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        if (this->file.is_open())
        {

            throw OKps::error(u8"已经有一个打开的文件，禁止重复打开", OKps::error::error_type::DEBUG_logic);
        }
        else
        {
            this->route = _route;
            this->file.open(_route, _mode, _prot);
            if (std::filesystem::exists(this->route))
            {
                this->file_size = std::filesystem::file_size(this->route);
                if (resize)
                {
                    this->auto_resizer(guess_size);
                }
            }
            if (this->file.is_open())
            {
                // 根据输入的文件打开模式，调整写指针
                if (_mode == (std::ios::out | std::ios::app) or _mode == (std::ios::out | std::ios::app | std::ios::binary) or _mode == std::ios::app or _mode == (std::ios::app | std::ios::binary))
                {
                    this->write_position = 0;
                    this->app_mode = true;
                }
                else if (_mode == (std::ios::out | std::ios::ate) or _mode == (std::ios::out | std::ios::ate | std::ios::binary) or _mode == (std::ios::out | std::ios::ate | std::ios::trunc) or _mode == (std::ios::out | std::ios::ate | std::ios::trunc | std::ios::binary) or _mode == std::ios::ate or _mode == (std::ios::ate | std::ios::binary) or _mode == (std::ios::ate | std::ios::trunc) or _mode == (std::ios::ate | std::ios::trunc | std::ios::binary))
                {
                    this->write_position = this->file_size;
                    this->app_mode = false;
                }
                else if (_mode == (std::ios::out | std::ios::ate | std::ios::app) or _mode == (std::ios::out | std::ios::ate | std::ios::app | std::ios::binary) or _mode == (std::ios::ate | std::ios::app) or _mode == (std::ios::ate | std::ios::app | std::ios::binary))
                {
                    this->write_position = this->file_size;
                    this->app_mode = true;
                }
                else if (_mode == std::ios::out or _mode == (std::ios::out | std::ios::binary) or _mode == (std::ios::out | std::ios::trunc) or _mode == (std::ios::out | std::ios::trunc | std::ios::binary) or _mode == std::ios::binary or _mode == std::ios::trunc or _mode == (std::ios::trunc | std::ios::binary))
                {
                    this->write_position = 0;
                    this->app_mode = false;
                }
                else
                {
                    this->file.close();
                    throw OKps::error(u8"_mode参数不合法", OKps::error::error_type::DEBUG_bad_parameter);
                }
            }
        }
    }

    void sequential_writter::open(const std::string & _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    void sequential_writter::open(const char * _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    void sequential_writter::open(const wchar_t * _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    void sequential_writter::open(const std::wstring & _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    void sequential_writter::open(const std::filesystem::path & _route, std::ios_base::openmode _mode, int _prot, bool resize, uintmax_t guess_size)
    {
        this->INNER_open(_route, _mode, _prot, resize, guess_size);
    }

    bool sequential_writter::is_open() const noexcept
    {
        return this->file.is_open();
    }

    void sequential_writter::close()
    {
        if (this->file.is_open())
        {
            // 在关闭文件之前，清理缓冲区，把其中内容写入文件
            this->refresh_buffer();
            this->file.close();
        }
        else
        {

            throw OKps::error(u8"请求关闭文件，但文件并没有打开", OKps::error::error_type::DEBUG_logic);
        }
    }

    std::streampos sequential_writter::tellp() const noexcept
    {
        return this->write_position;
    }

    bool sequential_writter::write(const char * write_buffer, std::streamsize size)
    {
        if (size <= 0)
        {
            return true;
        }
        if (this->app_mode)
        {
            this->write_position = this->file_size;
        }
        while (true)
        {
            if (this->write_position < this->head_position or this->write_position >= this->head_position.operator+(this->buffer_size))
            {
                // 写指针在缓冲区的有效范围之外
                // 刷新缓冲区
                this->refresh_buffer();
            }
            if (this->write_position + size <= this->head_position.operator+(this->buffer_size))
            {
                // 要写的内容能全部塞进缓冲区
                for (std::size_t count = 0; count < size; count++)
                {
                    this->buffer[count + this->write_position - this->head_position] = write_buffer[count];
                }
                this->write_position = this->write_position + size;
                if (this->app_mode)
                {
                    this->file_size = this->write_position;
                }
                return true;
            }
            else
            {
                // 缓冲区装不下要写入的内容
                // 先把缓冲区塞满
                auto to_write_size = this->buffer_size - (this->write_position - this->head_position);
                for (std::size_t count = 0; count < to_write_size; count++)
                {
                    this->buffer[count + this->write_position - this->head_position] = write_buffer[count];
                }
                this->write_position = this->write_position.operator+(to_write_size);
                if (this->app_mode)
                {
                    this->file_size = this->write_position;
                }
                size = size - to_write_size;
                if (size <= 0)
                {
                    return true;
                }
                write_buffer = &(write_buffer[to_write_size]);
            }
        }
    }

    void sequential_writter::resize_buffer(std::size_t size)
    {
        auto used_size = this->write_position - this->head_position;
        char * new_buffer = new char[size];
        if (size < used_size)
        {
            // 新缓冲区的长度 < 旧缓冲区记录的有效数据的长度
            // 把塞不下的这部分数据写入文件
            std::size_t to_write_size = used_size - size;
            this->file.seekp(this->head_position.operator+(size));
            this->file.write(&(this->buffer[size]), to_write_size);
        }
        if (size < this->buffer_size)
        {
            for (std::size_t count = 0; count < size; count++)
            {
                new_buffer[count] = this->buffer[count];
            }
        }
        else
        {
            for (std::size_t count = 0; count < this->buffer_size; count++)
            {
                new_buffer[count] = this->buffer[count];
            }
            for (std::size_t count = this->buffer_size; count < size; count++)
            {
                new_buffer[count] = 0;
            }
        }
        this->buffer_size = size;
        delete[] this->buffer;
        this->buffer = new_buffer;
    }

    void sequential_writter::refresh_buffer()
    {
        // 把缓冲区内的数据写入文件，然后清空缓冲区
        this->file.seekp(this->head_position);
        if (this->file.write(this->buffer, this->write_position - this->head_position))
        {
        }
        else
        {

            throw OKps::error(u8"文件写入失败", OKps::error::error_type::IO_file);
        }
        // 把缓冲区的头定位到写指针的位置
        this->head_position = this->write_position;
    }

    void sequential_writter::auto_resizer(uintmax_t size_guess)
    {
        if (size_guess <= 1000000)
        {
            this->resize_buffer(size_guess);
        }
        else
        {
            // 如果文件大小超过缓冲区的100倍，就令缓冲区为原来的10倍
            // 循环此过程，直到文件大小不超过缓冲区的100倍，或者缓冲区长为1000000000
            size_t size1 = 100 * this->buffer_size;
            size_t size2 = 10 * this->buffer_size;
            while (size_guess > size1)
            {
                size1 = 10 * size1;
                size2 = 10 * size2;
            }
            if (size2 <= 1000000000)
            {
                this->resize_buffer(size2);
            }
            else
            {
                this->resize_buffer(1000000000);
            }
        }
    }
}