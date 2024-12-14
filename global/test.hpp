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
#ifdef MACRO_OKps_enable_global_memory_record
    /*
    每间隔一定时间 dur，向文件 rec中记录此刻程序的动态内存分配情况。
    detail 参数控制是否详细记录，即记录每一个内存块的地址和大小，如果为 false 则只记录总内存块数量和总大小。
    */
    void real_time_memory_track(std::filesystem::path const & rec, std::chrono::seconds const & dur = std::chrono::seconds(1), bool const detail = false);
#endif
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

    /*
    测试 OKps::string::requirement

    要求用户从 std::cin 原封不动地输入字符串 expected，并使用 OKps::string::get_line 函数接收输入，
    使用 OKps::string::requirement 函数对象检查输入是否正确。
    */
    void string_requirement(std::string const & expected = "噫吁嚱，危乎高哉！蜀道之难，难于上青天！");

}