#pragma once

#include <type_traits>
#include <set>
#include <memory>

namespace OKps
{
    /*
    采用邻接表实现的有方向图容器
    */
    class graph final
    {
    public:
        /*
        数据类型基类
        */
        class data_type
        {
            friend class graph;
        protected:
            data_type()noexcept;
            data_type(data_type const &)noexcept;
            data_type(data_type &&)noexcept;
        public:
            virtual ~data_type()noexcept;
        protected:
            virtual void operator =(data_type const &)noexcept;
            virtual void operator =(data_type &&)noexcept;
            virtual data_type & self()noexcept;
            virtual data_type const & self()const noexcept;
            /*
            比较两个对象是否是同一个对象
            即比较 this 和 &right 是否相等
            */
            virtual bool operator ==(data_type const & right)const noexcept;
            virtual bool operator !=(data_type const & right)const noexcept;
            /*
            要求子类实现此函数，其功能为复制子类对象自身，并要求包装到std::unique_ptr<data_type>中返回。
            此函数会在图容器的复制构造函数中被使用。
            如果子类实现的此函数无法正确复制子类对象，产生的任何副作用，图容器不负责。
            */
            virtual std::unique_ptr<data_type> self_copy()const = 0;
        };

    private:
        class marker_type final
        {
        public:
            marker_type()noexcept;
            marker_type(marker_type const &) = delete;
            marker_type(marker_type &&) = delete;
            ~marker_type()noexcept;
            void operator =(marker_type &&) = delete;
            void operator =(marker_type const &) = delete;
        };
        class node final
        {
        private:
            graph * MEMBER_owner;
            std::unique_ptr<data_type> MEMBER_data;
            /*
            此节点指向的节点的列表，即邻接表
            */
            std::set<node *> MEMBER_next;
            std::shared_ptr<marker_type> MEMBER_marker;
        public:
            node(graph *, std::unique_ptr<data_type> && data)
                noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<data_type>>
                and std::is_nothrow_default_constructible_v<std::set<node *>>
                and noexcept(std::make_shared<marker_type>()));
            node(node && origin) = delete;
            node(node const & origin) = delete;
            ~node()
                noexcept(std::is_nothrow_destructible_v<std::unique_ptr<data_type>>
                and std::is_nothrow_destructible_v<std::set<node *>>
                and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>);
            void operator =(node const & origin) = delete;
            void operator =(node &&) = delete;
            std::unique_ptr<data_type> const & data()noexcept;
            data_type const * data()const noexcept;
            std::set<node *> const & next()const noexcept;
            std::set<node *> & next()noexcept;
            std::shared_ptr<marker_type> const & marker()const noexcept;
            void change_owner(graph * const)noexcept;
            graph * owner()noexcept;
            graph const * owner()const noexcept;
        };

        std::set<node *> MEMBER_graph;
    public:
        /*
        迭代器可以理解为对图中节点的引用。
        此迭代器的概念和标准库的迭代器相似，即以空迭代器作为尾迭代器，空迭代器可以和其他迭代器比较，但不能访问。
        */
        class iterator final
        {
            friend class graph;
        private:
            node * MEMBER_node;
            std::weak_ptr<marker_type> MEMBER_marker;
            iterator(node *)
                noexcept(noexcept(std::weak_ptr<marker_type>(std::declval<std::shared_ptr<marker_type> const &>())));

            node * operator *()const;
        public:
            iterator(iterator const & origin)
                noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>);
            void operator =(iterator const & origin)
                noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>);
            iterator(iterator &&) = delete;
            void operator =(iterator &&) = delete;
            ~iterator()
                noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>);
            void operator ++();
            void operator --();

            /*
            比较两个迭代器是否引用同一个节点，但不考虑它们引用的节点是否已经失效。
            */
            bool operator ==(iterator const & right)const noexcept;
            bool operator !=(iterator const & right)const noexcept;
            /*
            比较两个迭代器的地址，仅用于std::less。

            图中的节点之间没有定义偏序关系，所以此函数不是在比较节点之间的关系。
            */
            bool operator <(iterator const & right)const noexcept;
            /*
            添加一条指向关系，从此节点指向节点 target
            如果此节点和节点 target 是同一个节点，且它们都有效，则什么都不发生
            */
            void add(iterator const & target);
            /*
            判断迭代器是否有效
            */
            bool is_valid()const noexcept;
            /*
            删除从此节点指向节点 target 的指向关系
            */
            void erase(iterator const & target);
            std::unique_ptr<data_type> const & data();
            data_type const * data()const;
            graph * owner();
            graph const * owner()const;
            std::set<iterator> next()const;
        };
        graph()
            noexcept(std::is_nothrow_default_constructible_v<std::set<node *>>);
        ~graph()
            noexcept(std::is_nothrow_destructible_v<std::set<node *>>);
        graph(graph const & origin);
        void operator =(graph const & origin);
        graph(graph &&)
            noexcept(std::is_nothrow_move_constructible_v<std::set<node *>>);
        void operator =(graph &&)
            noexcept(std::is_nothrow_move_assignable_v<std::set<node *>>);
        /*
        向图中添加节点
        */
        iterator add(std::unique_ptr<data_type> && data);
        /*
        删除图中的节点
        */
        void erase(iterator const &);
        void clear();

        std::unique_ptr<iterator const> begin()const;
        std::unique_ptr<iterator const> end()const;
        iterator begin();
        iterator end();
    };
}