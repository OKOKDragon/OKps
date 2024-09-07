#pragma once

#include <type_traits>
#include <set>
#include <map>
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
        public:
            data_type()noexcept;
            data_type(data_type const &)noexcept;
            data_type(data_type &&)noexcept;
            virtual ~data_type()noexcept;
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
            node(node const &) = delete;
            ~node()
                noexcept(std::is_nothrow_destructible_v<std::unique_ptr<data_type>>
                and std::is_nothrow_destructible_v<std::set<node *>>
                and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>);
            void operator =(node const &) = delete;
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

            bool operator ==(iterator const & right)const noexcept;
            bool operator !=(iterator const & right)const noexcept;
            /*
            比较两个迭代器的地址
            仅用于std::less
            */
            bool operator <(iterator const & right)const noexcept;
            /*
            添加一条指向关系，从此节点指向节点 target
            如果此节点和节点 target 是同一个节点，且它们都有效，则什么都不发生
            */
            void add(iterator const & target);
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
        graph(graph const &) = delete;
        void operator =(graph const &) = delete;
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

        std::unique_ptr<iterator const> begin()const;
        std::unique_ptr<iterator const> end()const;
        iterator begin();
        iterator end();
    };
}