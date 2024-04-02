#pragma once

#include <iostream>
#include ".\basic.hpp"
// #include ".\malloc.hpp"
#include <string>
#include <map>
#include <memory>
#include ".\error.hpp"

namespace OKps
{
    /*
    内存块
    可以用于将同一块内存重新解释为不同类型的数组
    例如，一个 unsigned long long 类型变量 a，在以二进制写入文件时相当于一个长度为8的 char 类型数组
    */
    template <typename data_type>
    class block : public basic
    {
    private:
        /*
        内存块指针
        */
        data_type * data;
        /*
        内存块长度
        */
        size_t MEMBER_size;

    public:
        block(const std::string & input)
            : data(nullptr), MEMBER_size(0)
        {
            if (input.size() % sizeof(data_type) != 0)
            {
                std::string TEMP_hint = std::string(u8"输入的字节流不可能是完整的") + typeid(data_type).name() + u8"数组";

                throw error(TEMP_hint, error::error_type::DEBUG_bad_parameter);
            }
            else
            {
                this->MEMBER_size = input.size() / sizeof(data_type);
                this->data = new data_type[this->MEMBER_size];
                for (std::size_t counter = 0; counter < input.size(); counter++)
                {
                    ((char *)(this->data))[counter] = input[counter];
                }
            }
        }
        block(const size_t size)
            : data(new data_type[size]), MEMBER_size(size)
        {
        }
        block(const block & origin)
            : data(new data_type[origin.MEMBER_size]), MEMBER_size(origin.MEMBER_size)
        {
            for (size_t counter = 0; counter < origin.MEMBER_size; counter++)
            {
                this->data[counter] = origin.data[counter];
            }
        }
        block(block &&) = delete;
        ~block() noexcept
        {
            delete[] this->data;
        }
        size_t size() const noexcept
        {
            return this->MEMBER_size;
        }
        /*
        如果将std::string看作字节流，而不是字符串
        则可用std::string记录任意内存
        */
        operator std::string() const
        {
            std::string result;
            for (std::size_t counter = 0; counter < sizeof(data_type) * this->MEMBER_size; counter++)
            {
                result.push_back(((char *)(this->data))[counter]);
            }
            return result;
        }
        data_type & at(const size_t position) const
        {
            if (position < this->MEMBER_size)
            {
                return this->data[position];
            }
            else
            {
                throw error(u8"访问的位置超出内存块范围", error::error_type::DEBUG_overstep);
            }
        }
        template <typename another_type>
        another_type & at(const size_t position) const
        {
            if ((position + 1) * sizeof(another_type) > (this->MEMBER_size * sizeof(data_type)))
            {

                throw error(u8"访问的位置超出内存块范围", error::error_type::DEBUG_overstep);
            }
            else
            {
                return ((another_type *)(this->data))[position];
            }
        }
        data_type & operator[](const size_t position) const
        {
            return this->at(position);
        }
        template <typename another_type>
        another_type & operator[](const size_t position) const
        {
            return this->at<another_type>(position);
        }
        data_type * operator&() const noexcept
        {
            return this->data;
        }
    };

}