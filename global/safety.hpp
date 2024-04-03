#pragma once

#include ".\aes.hpp"
#include ".\block.hpp"

namespace OKps
{

    template <typename data_type, typename enable_condition = void>
    /*
    保存一个基本类型对象，每次访问时都加密其内存
    线程不安全，如果要在不同线程中访问同一个safe_data对象，需要自行加锁
    */
    class safe_data : public base
    {
        static_assert(not std::is_reference_v<data_type>, "不能是引用。");
        static_assert(not std::is_union_v<data_type>, "不要使用原生联合体。现代c++应该使用std::variant类型，以提升安全性。");
        static_assert(not std::is_const_v<data_type>, "不能有不可变限定符。");
        static_assert(not std::is_void_v<data_type>, "不能是void类型。");
        static_assert(not std::is_array_v<data_type>, "不能是原生数组。");
        static_assert(not std::is_function_v<data_type>, "不能是函数。");
        static_assert(not std::is_null_pointer_v<data_type>, "不能是空指针。");

    private:
        mutable std::string MEMBER_data;

    public:
        safe_data(data_type input)
        {
            this->set(input);
        }
        safe_data(const safe_data & origin)
            : MEMBER_data(origin.MEMBER_data)
        {
        }
        ~safe_data()
        {
        }
        data_type get() const
        {
            basic_aes_device::random_InvCipher(this->MEMBER_data);
            block<data_type> TEMP_content = this->MEMBER_data;

            data_type result = TEMP_content.at(0);
            basic_aes_device::random_Cipher(this->MEMBER_data);

            return result;
        }
        void set(data_type input)
        {
            block<data_type> TEMP_content = block<data_type>(1);
            TEMP_content.at(0) = input;
            this->MEMBER_data = (std::string)TEMP_content;
            basic_aes_device::random_Cipher(this->MEMBER_data);
        }
    };
    template <typename data_type>
    /*
    保存一个类类型对象，每次访问时都加密其内存
    注意，如果一块内存可以通过多个对象访问，那么当其中任何一个对象被safe_data加密时，不要通过其他几个对象访问这块内存。典型的例子就是容器和迭代器。

    线程不安全，如果要在不同线程中访问同一个safe_data对象，需要自行加锁
    */
    class safe_data<data_type, typename std::enable_if<std::is_class_v<data_type>>::type> : public base
    {

        static_assert(not std::is_const_v<data_type>, "不能有不可变限定符。");
        static_assert(std::is_move_constructible_v<data_type>, "必须有移动构造函数。");
        static_assert(std::is_move_assignable_v<data_type>, "必须有移动赋值操作符。");
        // static_assert(not std::is_base_of_v<base, data_type>, "必须继承OKps::basic类。");
        /*
        本类的作用是加密data_type对象的内存
        但是管理数据的类通常只包含指向数据的指针，而不含数据本身。
        例如当data_type为采用三指针实现动态数组容器，则本类加密的是这几个指针地址，而不是它们指向的内容。
        */
        // static_assert(not std::is_class_v<data_type>, "暂时不支持对类使用safe_data。");

    private:
        mutable std::string MEMBER_data;

    public:
        safe_data(const safe_data & origin)
            : MEMBER_data(origin.MEMBER_data)
        {
        }

        safe_data(safe_data && origin)
            :MEMBER_data(std::move(origin.MEMBER_data))
        {

        }
        /*
        input对象将被移动
        调用此构造函数后不能再使用input
        */
        safe_data(data_type & input)
        {
            this->set(input);
        }
        /*
        input对象将被移动
        调用此构造函数后不能再使用input
        */
        safe_data(data_type && input)
        {
            this->set(input);
        }
        ~safe_data()
        {
        }
        data_type get() const
        {
            basic_aes_device::random_InvCipher(this->MEMBER_data);
            block<data_type> TEMP_content = this->MEMBER_data;

            data_type result = TEMP_content.at(0);
            basic_aes_device::random_Cipher(this->MEMBER_data);

            return result;
        }
        void set(data_type & input)
        {

            block<data_type> TEMP_content = block<data_type>(1);
            TEMP_content.at(0) = std::move(input);
            this->MEMBER_data = (std::string)TEMP_content;
            basic_aes_device::random_Cipher(this->MEMBER_data);
        }
        void set(data_type && input)
        {
            block<data_type> TEMP_content = block<data_type>(1);
            TEMP_content.at(0) = std::move(input);
            this->MEMBER_data = (std::string)TEMP_content;
            basic_aes_device::random_Cipher(this->MEMBER_data);
        }

    };
}
