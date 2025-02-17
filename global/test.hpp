#pragma once

#include <map>
#include <filesystem>
#include <set>
#include <chrono>

#include ".\base.hpp"

/*
用于测试此库部分功能的函数和类
*/
namespace OKps::test
{
    void enable_self_copy();
    /*
    简单测试 OKps::storage<OKps::field>
    */
    void storage_field();
    /*
    此函数工作时，将working_path临时设为程序的工作目录，并在该目录下进行一切文件操作。
    working_path必须是此前不存在的目录，且在此函数结束时会删除它。
    */
    void key(std::filesystem::path const & working_path);
    /*
    测试 OKps::base::marked 和 OKps::base::reference 是否正常工作
    */
    void marked_reference();
    /*
    测试 OKps::order_matcher 是否正常工作。
    */
    void test_order_matcher();

    /*
    测试 OKps::message 是否正常工作。
    */
    void test_message();

    class worker_impl final : public base::worker
    {
    private:
        using base_type = base::worker;
        void work()noexcept override;

        std::string MEMBER_name;
    public:
        worker_impl(std::string const & name)noexcept;

        worker_impl(worker_impl && origin)noexcept;

        ~worker_impl()noexcept override;

        worker_impl & self()noexcept override;

        worker_impl const & self()const noexcept override;

        std::string const & name()const noexcept;

    };

    /*
    测试 OKps::base::worker 及其派生类 worker_impl 是否正常工作。
    */
    void worker_base();

    //测试 OKps::base::handler
    void test_handler();

}