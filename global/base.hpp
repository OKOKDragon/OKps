#pragma once

#include ".\cpp_std_lib.hpp"

namespace OKps
{

    /*
    基类，用于统一提供delete和new操作符
    一定要用public继承此基类

    不要在派生类中自行声明new、delete，会覆盖基类的new、delete，
    因为它们是静态函数，没法通过virtual、override关键字从语言层面显式地说明你正在重写它们

    c++强制规定释放器delete不能抛出异常。
    如果在operator delete的函数体内抛出异常，可以通过编译，但异常不会被catch捕捉。
    也就是说在delete操作符中抛出异常，无法被处理，所以程序必然异常退出。

    如果内存分配出现异常，我遵循的思想是不能做任何处理，应该立即结束程序。
    因为现代操作系统已经有非常成熟安全的内存管理，申请内存时内存不会立刻分配给进程，而是使用时才分配。如果物理内存已满，会在硬盘上分配虚拟内存，除非用户禁止系统这样做。
    一旦出现无法分配内存的情况，很可能是整个系统都面临严重故障，此时程序要是还在处理异常，只能增加额外风险。
    立即退出程序才能保护机器，保护数据，保护用户。
    */
    class base
    {
    private:
        static inline std::map<void *, std::size_t> MEMBER_pool;
        static inline ::std::atomic_bool MEMBER_lock = false;

    public:
        static void * operator new(const std::size_t size);

        static void operator delete(void * block) noexcept;

        static void * operator new[](const std::size_t size);

        static void operator delete[](void * block) noexcept;

        static void operator delete(void *, void *) noexcept = delete;
        static void operator delete[](void *, void *) noexcept = delete;
        static void * operator new(std::size_t, void *) = delete;
        static void * operator new[](std::size_t, void *) = delete;

        /*
        禁用了带std::nothrow的分配器，因为本类希望内存分配失败时抛异常
        */
        static void operator delete(void *, const std::nothrow_t &) noexcept = delete;
        static void operator delete[](void *, const std::nothrow_t &) noexcept = delete;
        static void operator delete(void *, std::align_val_t, const std::nothrow_t &) noexcept = delete;
        static void operator delete[](void *, std::align_val_t, const std::nothrow_t &) noexcept = delete;
        static void * operator new(std::size_t, const std::nothrow_t &) = delete;
        static void * operator new[](std::size_t, const std::nothrow_t &) = delete;
        static void * operator new(std::size_t, std::align_val_t, const std::nothrow_t &) = delete;
        static void * operator new[](std::size_t, std::align_val_t, const std::nothrow_t &) = delete;

    private:

        static void pool_output(void * block) noexcept;
        template <void * (*allocator)(std::size_t)>
        static void * INNER_proxy_new(const std::size_t size);

    protected:
        base() noexcept;

        base(const base &) noexcept;
        base(base &&) noexcept;
        void operator=(const base &) noexcept;
        void operator=(base &&) noexcept;

        virtual ~base() noexcept;
    };

    /*
    基类，用于禁止所有new和delete操作符
    注意，c++其实不可能禁止一个类在堆上分配，即使将构造函数全部设为私有。
    所以本类只是在本库的编写中使用，而无法限制用户。
    */
    class base_no_allocate
    {
    protected:
        base_no_allocate() noexcept;

        virtual ~base_no_allocate() noexcept;

        base_no_allocate(const base_no_allocate &) noexcept;
        base_no_allocate(base_no_allocate &&) noexcept;
        void operator=(const base_no_allocate &) noexcept;
        void operator=(base_no_allocate &&) noexcept;

    public:
        static void * operator new(std::size_t) = delete;
        static void * operator new[](std::size_t) = delete;

        static void operator delete(void *) = delete;
        static void operator delete[](void *) = delete;

        static void operator delete(void *, void *) noexcept = delete;
        static void operator delete[](void *, void *) noexcept = delete;

        static void operator delete(void *, std::size_t) noexcept = delete;
        static void operator delete[](void *, std::size_t) noexcept = delete;

        static void operator delete(void *, const std::nothrow_t &) noexcept = delete;
        static void operator delete[](void *, const std::nothrow_t &) noexcept = delete;

        static void operator delete(void *, std::align_val_t) noexcept = delete;
        static void operator delete[](void *, std::align_val_t) noexcept = delete;

        static void operator delete(void *, std::align_val_t, const std::nothrow_t &) noexcept = delete;
        static void operator delete[](void *, std::align_val_t, const std::nothrow_t &) noexcept = delete;
        static void operator delete(void *, size_t, std::align_val_t) noexcept = delete;
        static void operator delete[](void *, size_t, std::align_val_t) noexcept = delete;

        static void * operator new(std::size_t, void *) = delete;
        static void * operator new[](std::size_t, void *) = delete;

        static void * operator new(std::size_t, const std::nothrow_t &) = delete;
        static void * operator new[](std::size_t, const std::nothrow_t &) = delete;

        static void * operator new(std::size_t, std::align_val_t) = delete;
        static void * operator new[](std::size_t, std::align_val_t) = delete;

        static void * operator new(std::size_t, std::align_val_t, const std::nothrow_t &) = delete;
        static void * operator new[](std::size_t, std::align_val_t, const std::nothrow_t &) = delete;
    };

}
