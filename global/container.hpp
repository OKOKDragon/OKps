#pragma once

#include <vector>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <forward_list>
#include <stack>

#include ".\base.hpp"

namespace OKps::container
{
    /*
    标准库容器要求元素可复制，
    所以不能有 std::vector<std::unique_ptr<T>> 容器。
    此类仅用于解决该问题，禁止用于其他任何情形。

    此类内含一个mutable std::unique_ptr，但此类的复制语义会执行移动语义的功能，而移动语义则被禁用。
    这并不引发未定义行为，也不会有编译器警告，但仍是c++标准不推荐的行为。
    c++标准建议，“mutable 用于指定不影响类的外部可观察状态的成员（通常用于互斥量、记忆缓存、惰性求值和访问指令等）。”
    所以不要滥用此类，使用之前必须完全知晓自己在做什么，并编写详细完整的注释，以提醒阅读代码的人。
    */

    class copiable_unique final
    {
    private:
        mutable std::unique_ptr<base::self_copier> MEMBER_value;
    public:
        copiable_unique() = delete;
        copiable_unique(std::unique_ptr<base::self_copier> && pointer = std::unique_ptr<base::self_copier>())
            noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<base::self_copier>>);
        /*
        复制构造
        执行移动构造的功能
        */
        copiable_unique(copiable_unique const &)
            noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<base::self_copier>>);
        /*
        复制赋值
        执行移动赋值的功能
        */
        void operator =(copiable_unique const &)
            noexcept(std::is_nothrow_move_assignable_v<std::unique_ptr<base::self_copier>>);

        copiable_unique(copiable_unique &&) = delete;

        void operator =(copiable_unique &&) = delete;

        base::self_copier & value();
        base::self_copier const & value()const;

        ~copiable_unique()
            noexcept(std::is_nothrow_destructible_v<std::unique_ptr<base::self_copier>>);

        /*
        仅用作标准库容器的排序，不代表任何实际意义。
        */
        bool operator <(copiable_unique const & right)const
            noexcept(noexcept(std::declval<std::unique_ptr<base::self_copier>>().get() < std::declval<std::unique_ptr<base::self_copier>>().get()));
    };


}