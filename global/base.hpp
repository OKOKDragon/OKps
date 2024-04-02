#pragma once

#include <new>
#include <map>
#include <atomic>

namespace OKps
{
    /*
    基类，用于提供delete和new操作符
    一定要用public继承此基类
    出于调试的目的，new和delete应该检查输入，不允许delete空指针，不允许分配 0字节
    虽然现代C和C++完全允许这样做，而不引发异常，甚至很多程序员已经不认为delete空指针是不规范的行为，
    但检查这些仍有助于程序员进行调试，让程序员发现意外的情况。
    至少我有因为检查了delete空指针而定位bug的经历。

    c++规定，分配器new只能抛出std::bad_alloc类型的异常，而释放器delete不能抛出异常
    虽然在函数体中抛出自定义类型的异常，可以通过编译，但不会被catch块捕捉到。
    也就是说在new和delete操作符中抛出自定义的异常类型，无法被处理（除非自定义异常类型继承了std::bad_alloc），所以程序必然异常退出。
    但是这正中我的下怀，如果内存分配出现异常，我遵循的思想是不能做任何处理，应该立即结束程序。
    因为现代操作系统已经有非常成熟安全的内存管理，一旦出现无法分配内存的情况，很可能整个机器都面临严重故障，
    程序要是还在处理异常，只能增加额外风险。
    立即退出程序才能保护机器，保护数据，保护用户。
    */
    class base
    {
    private:
        static inline std::map<void *, size_t> MEMBER_pool;
        static inline ::std::atomic_bool MEMBER_lock = false;

    public:
        void * operator new(const size_t size);

        void operator delete(void * block) noexcept;

        void * operator new[](const size_t size);

        void operator delete[](void * block) noexcept;

        void operator delete(void *, void *) noexcept = delete;
        void operator delete[](void *, void *) noexcept = delete;
        void * operator new(size_t, void *) = delete;
        void * operator new[](size_t, void *) = delete;

        /*
        禁用了带std::nothrow的分配器，因为本类希望内存分配失败时抛异常
        */
        void operator delete(void *, const std::nothrow_t &) noexcept = delete;
        void operator delete[](void *, const std::nothrow_t &) noexcept = delete;
        void operator delete(void *, std::align_val_t, const std::nothrow_t &) noexcept = delete;
        void operator delete[](void *, std::align_val_t, const std::nothrow_t &) noexcept = delete;
        void * operator new(size_t, const std::nothrow_t &) = delete;
        void * operator new[](size_t, const std::nothrow_t &) = delete;
        void * operator new(size_t, std::align_val_t, const std::nothrow_t &) = delete;
        void * operator new[](size_t, std::align_val_t, const std::nothrow_t &) = delete;

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
    */
    class basic_no_heap
    {
    protected:
        basic_no_heap() noexcept;

        ~basic_no_heap() noexcept;

        basic_no_heap(const basic_no_heap &) noexcept;
        basic_no_heap(basic_no_heap &&) noexcept;
        void operator=(const basic_no_heap &) noexcept;
        void operator=(basic_no_heap &&) noexcept;

    public:
        void * operator new(size_t) = delete;
        void * operator new[](size_t) = delete;

        void operator delete(void *) = delete;
        void operator delete[](void *) = delete;

        void operator delete(void *, void *) noexcept = delete;
        void operator delete[](void *, void *) noexcept = delete;

        void operator delete(void *, size_t) noexcept = delete;
        void operator delete[](void *, size_t) noexcept = delete;

        void operator delete(void *, const std::nothrow_t &) noexcept = delete;
        void operator delete[](void *, const std::nothrow_t &) noexcept = delete;

        void operator delete(void *, std::align_val_t) noexcept = delete;
        void operator delete[](void *, std::align_val_t) noexcept = delete;

        void operator delete(void *, std::align_val_t, const std::nothrow_t &) noexcept = delete;
        void operator delete[](void *, std::align_val_t, const std::nothrow_t &) noexcept = delete;
        void operator delete(void *, size_t, std::align_val_t) noexcept = delete;
        void operator delete[](void *, size_t, std::align_val_t) noexcept = delete;

        void * operator new(std::size_t, void *) = delete;
        void * operator new[](std::size_t, void *) = delete;

        void * operator new(size_t, const std::nothrow_t &) = delete;
        void * operator new[](size_t, const std::nothrow_t &) = delete;

        void * operator new(size_t, std::align_val_t) = delete;
        void * operator new[](size_t, std::align_val_t) = delete;

        void * operator new(size_t, std::align_val_t, const std::nothrow_t &) = delete;
        void * operator new[](size_t, std::align_val_t, const std::nothrow_t &) = delete;
    };

}
