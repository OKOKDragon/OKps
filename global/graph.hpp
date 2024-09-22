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
        数据是节点的属性
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
            要求子类实现此函数，其功能为复制子类对象自身，并要求包装到std::unique_ptr<data_type>中返回。
            此函数会在图容器的复制构造函数中被使用。
            如果子类实现的此函数无法正确复制子类对象，产生的任何副作用，图容器不负责。
            */
            virtual std::unique_ptr<data_type> self_copy()const = 0;
        };
        /*
        权重的基类
        权重是边的属性
        */
        class weight_type
        {
            friend class graph;
        protected:
            weight_type()noexcept;
            weight_type(weight_type const &)noexcept;
            weight_type(weight_type &&)noexcept;
        public:
            virtual ~weight_type()noexcept;
        protected:
            virtual void operator =(weight_type const &)noexcept;
            virtual void operator =(weight_type &&)noexcept;
            virtual weight_type & self()noexcept;
            virtual weight_type const & self()const noexcept;

            /*
            要求子类实现此函数，其功能为复制子类对象自身，并要求包装到std::unique_ptr<data_type>中返回。
            此函数会在图容器的复制构造函数中被使用。
            如果子类实现的此函数无法正确复制子类对象，产生的任何副作用，图容器不负责。
            */
            virtual std::unique_ptr<weight_type> self_copy()const = 0;

            /*
            比较两个权重的大小
            用于最短路径等算法
            */
            virtual bool operator <(weight_type const & right)const noexcept = 0;
            virtual bool operator ==(weight_type const & right)const noexcept = 0;

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
        //节点
        class node;
        //边
        class edge;

    private:
        class node final
        {
        private:
            graph * MEMBER_owner;
            std::unique_ptr<data_type> MEMBER_data;
            //以此节点为起点的边的集合
            std::set<edge *> MEMBER_next;
            //指向此节点的边的集合
            std::set<edge *>MEMBER_previous;
            std::shared_ptr<marker_type> MEMBER_marker;
        public:
            node(graph *, std::unique_ptr<data_type> && data)
                noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<data_type>>
                and std::is_nothrow_default_constructible_v<std::set<edge *>>
                and noexcept(std::make_shared<marker_type>()));
            node(node && origin) = delete;
            node(node const & origin) = delete;

            ~node()
                noexcept(std::is_nothrow_destructible_v<std::unique_ptr<data_type>>
                and std::is_nothrow_destructible_v<std::set<edge *>>
                and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>);
            void operator =(node const & origin) = delete;
            void operator =(node &&) = delete;
            std::unique_ptr<data_type> const & data()noexcept;
            data_type const * data()const noexcept;
            std::set<edge *> const & next()const noexcept;
            std::set<edge *> & next()noexcept;
            std::set<edge *> const & previous()const noexcept;
            std::set<edge *> & previous()noexcept;
            std::shared_ptr<marker_type> const & marker()const noexcept;
            void change_owner(graph *)noexcept;
            graph * owner()noexcept;
            graph const * owner()const noexcept;
        };
        class edge final
        {
        private:
            node * MEMBER_begin;//起点
            node * MEMBER_end;//终点
            std::unique_ptr<weight_type> MEMBER_weight;//权重
            graph * MEMBER_owner;
            std::shared_ptr<marker_type> MEMBER_marker;
        public:
            edge(graph *, node * begin_node, node * end_node, std::unique_ptr<weight_type> &&)
                noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<weight_type>>
                and noexcept(std::make_shared<marker_type>()));
            edge(edge && origin) = delete;
            edge(edge const & origin) = delete;
            ~edge()
                noexcept(std::is_nothrow_destructible_v<std::unique_ptr<weight_type>>
                and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>);
            void operator =(edge const & origin) = delete;
            void operator =(edge &&) = delete;
            std::unique_ptr<weight_type> const & weight()noexcept;
            weight_type const * weight()const noexcept;
            node const * begin()const noexcept;
            node * begin()noexcept;
            node const * end()const noexcept;
            node * end()noexcept;
            std::shared_ptr<marker_type> const & marker()const noexcept;
            void change_owner(graph *)noexcept;
            graph * owner()noexcept;
            graph const * owner()const noexcept;
            void change_weight(std::unique_ptr<weight_type> &&)
                noexcept(std::is_nothrow_move_assignable_v<std::unique_ptr<weight_type>>);
        };
        /*
        图中所有节点的集合
        节点的内存由图管理
        */
        std::set<node *> MEMBER_node;
        /*
        图中所有边的集合
        边的内存由图管理
        */
        std::set<edge *>MEMBER_edge;
    public:

        //可变的节点迭代器
        class mutable_node_iterator;
        //不可变的节点迭代器
        class immutable_node_iterator;
        //可变的边迭代器
        class mutable_edge_iterator;
        //不可变的边迭代器
        class immutable_edge_iterator;
        /*
        迭代器可以理解为对图中节点的引用。
        此迭代器的概念和标准库的迭代器相似，即以空迭代器作为尾迭代器，空迭代器可以和其他迭代器比较，但不能访问。
        */
        class mutable_node_iterator final
        {
            friend class graph;
        private:
            node * MEMBER_node;
            std::weak_ptr<marker_type> MEMBER_marker;
            mutable_node_iterator(node *)
                noexcept(noexcept(std::declval<std::weak_ptr<marker_type>>() = std::declval<std::shared_ptr<marker_type> const &>())
                and std::is_nothrow_default_constructible_v<std::weak_ptr<marker_type>>);

            node const * get_node()const;
            node * get_node();
        public:
            mutable_node_iterator(mutable_node_iterator const & origin)
                noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>);
            void operator =(mutable_node_iterator const & origin)
                noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>);
            mutable_node_iterator(mutable_node_iterator &&) = delete;
            void operator =(mutable_node_iterator &&) = delete;
            ~mutable_node_iterator()
                noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>);
            void operator ++();
            void operator --();
            operator immutable_node_iterator()const
                noexcept(noexcept(immutable_node_iterator(nullptr))
                and std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>);
            /*
            比较两个迭代器是否引用同一个节点，但不考虑它们引用的节点是否已经失效。
            */
            bool operator ==(mutable_node_iterator const & right)const noexcept;
            bool operator !=(mutable_node_iterator const & right)const noexcept;
            bool operator ==(immutable_node_iterator const & right)const noexcept;
            bool operator !=(immutable_node_iterator const & right)const noexcept;
            /*
            比较两个迭代器
            此函数仅用于std::less，以使得迭代器可以用std::set包装。
            此函数比较的不是迭代器所指向的图节点，图中的节点之间没有定义任何可以用来比较的偏序关系。
            */
            bool operator <(mutable_node_iterator const & right)const noexcept;
            /*
            添加一条指向关系，从此节点指向节点 target。
            如果此节点和节点 target 不属于同一个图容器，则什么都不发生，返回空迭代器。
            如果此节点和节点 target 是同一个节点，且它们都有效，则什么都不发生，返回空迭代器。
            */
            mutable_edge_iterator add(mutable_node_iterator const & target, std::unique_ptr<weight_type> &&)const;
            /*
            判断迭代器是否有效
            */
            bool is_valid()const noexcept;
            /*
            删除从此节点指向节点 target 的指向关系
            */
            void erase(mutable_node_iterator const & target);
            std::unique_ptr<data_type> const & data();
            data_type const * data()const;
            graph & owner();
            graph const & owner()const;
            /*
            以此节点为起点的所有边
            */
            std::set<immutable_edge_iterator> next()const;
            std::set<mutable_edge_iterator> next();
            /*
            找到此节点指向节点 another 的边
            如果没有则返回空迭代器
            */
            immutable_edge_iterator find_target(immutable_node_iterator const & another)const;
            mutable_edge_iterator find_target(immutable_node_iterator const & another);
            /*
            此节点的入度，即图中指向此节点的节点数
            */
            std::size_t in_degree()const;
            /*
            此节点的出度，即图中此节点指向的节点数
            */
            std::size_t out_degree()const;
            /*
            指向此节点的所有边
            */
            std::set<immutable_edge_iterator> previous()const;
            std::set<mutable_edge_iterator> previous();
            /*
            找到节点 another 指向此节点的边
            如果找不到则返回空迭代器
            */
            immutable_edge_iterator find_former(immutable_node_iterator const &)const;
            mutable_edge_iterator find_former(immutable_node_iterator const &);

        };
        class immutable_node_iterator final
        {
            friend class graph;
        private:
            node * MEMBER_node;
            std::weak_ptr<marker_type> MEMBER_marker;
            immutable_node_iterator(node *)
                noexcept(std::is_nothrow_default_constructible_v<std::weak_ptr<marker_type>>
                    and noexcept(std::declval<std::weak_ptr<marker_type>>() = std::declval<std::shared_ptr<marker_type> const &>()));

            node const * get_node()const;
            node * get_node();
        public:
            immutable_node_iterator(immutable_node_iterator const & origin)
                noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>);
            void operator =(immutable_node_iterator const & origin)
                noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>);
            immutable_node_iterator(immutable_node_iterator &&) = delete;
            void operator =(immutable_node_iterator &&) = delete;
            ~immutable_node_iterator()
                noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>);

            /*
            比较两个迭代器是否引用同一个节点，但不考虑它们引用的节点是否已经失效。
            */
            bool operator ==(mutable_node_iterator const & right)const noexcept;
            bool operator !=(mutable_node_iterator const & right)const noexcept;
            bool operator ==(immutable_node_iterator const & right)const noexcept;
            bool operator !=(immutable_node_iterator const & right)const noexcept;
            /*
            比较两个迭代器
            此函数仅用于std::less，以使得迭代器可以用std::set包装。
            此函数比较的不是迭代器所指向的图节点，图中的节点之间没有定义任何可以用来比较的偏序关系。
            */
            bool operator <(immutable_node_iterator const & right)const noexcept;

            /*
            判断迭代器是否有效
            */
            bool is_valid()const noexcept;

            data_type const * data()const;

            graph const & owner()const;
            /*
            以此节点为起点的所有边
            */
            std::set<immutable_edge_iterator> next()const;

            /*
            找到此节点指向节点 another 的边
            如果没有则返回空迭代器
            */
            immutable_edge_iterator find_target(immutable_node_iterator const & another)const;

            /*
            此节点的入度，即图中指向此节点的节点数
            */
            std::size_t in_degree()const;
            /*
            此节点的出度，即图中此节点指向的节点数
            */
            std::size_t out_degree()const;
            /*
            指向此节点的所有边
            */
            std::set<immutable_edge_iterator> previous()const;

            /*
            找到节点 another 指向此节点的边
            如果找不到则返回空迭代器
            */
            immutable_edge_iterator find_former(immutable_node_iterator const &)const;


        };

        class mutable_edge_iterator final
        {
            friend class graph;
        private:
            edge * MEMBER_edge;
            std::weak_ptr<marker_type> MEMBER_marker;
            mutable_edge_iterator(edge *)
                noexcept(noexcept(std::weak_ptr<marker_type>(std::declval<std::shared_ptr<marker_type> const &>()))
                and std::is_nothrow_default_constructible_v<std::weak_ptr<marker_type>>);

            edge const * get_edge()const;
            edge * get_edge();
        public:
            mutable_edge_iterator(mutable_edge_iterator const & origin)
                noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>);
            void operator =(mutable_edge_iterator const & origin)
                noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>);
            mutable_edge_iterator(mutable_edge_iterator &&) = delete;
            void operator =(mutable_edge_iterator &&) = delete;
            ~mutable_edge_iterator()
                noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>);
            void operator ++();
            void operator --();
            operator graph::immutable_edge_iterator()const
                noexcept(noexcept(immutable_edge_iterator(nullptr))
                and std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>);
                /*
            比较两个迭代器是否引用同一个节点，但不考虑它们引用的节点是否已经失效。
            */
            bool operator ==(mutable_edge_iterator const & right)const noexcept;
            bool operator !=(mutable_edge_iterator const & right)const noexcept;
            bool operator ==(immutable_edge_iterator const & right)const noexcept;
            bool operator !=(immutable_edge_iterator const & right)const noexcept;
            /*
            比较两个迭代器
            此函数仅用于std::less，以使得迭代器可以用std::set包装。
            此函数比较的不是迭代器所指向的图节点，图中的节点之间没有定义任何可以用来比较的偏序关系。
            */
            bool operator <(mutable_edge_iterator const & right)const noexcept;

            /*
            判断迭代器是否有效
            */
            bool is_valid()const noexcept;

            std::unique_ptr<weight_type> const & weight();
            weight_type const * weight()const;
            graph & owner();
            graph const & owner()const;

            //此边的起点
            immutable_node_iterator begin()const;
            mutable_node_iterator begin();

            //此边的终点
            immutable_node_iterator end()const;
            mutable_node_iterator end();

        };
        class immutable_edge_iterator final
        {
            friend class graph;
        private:
            edge * MEMBER_edge;
            std::weak_ptr<marker_type> MEMBER_marker;
            immutable_edge_iterator(edge *)
                noexcept(std::is_nothrow_default_constructible_v<std::weak_ptr<marker_type>>
                    and noexcept(std::declval<std::weak_ptr<marker_type>>() = std::declval<std::shared_ptr<marker_type> const &>()));

            edge const * get_edge()const;
            edge * get_edge();
        public:
            immutable_edge_iterator(immutable_edge_iterator const & origin)
                noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>);
            void operator =(immutable_edge_iterator const & origin)
                noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>);
            immutable_edge_iterator(immutable_edge_iterator &&) = delete;
            void operator =(immutable_edge_iterator &&) = delete;
            ~immutable_edge_iterator()
                noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>);

            /*
            比较两个迭代器是否引用同一个节点，但不考虑它们引用的节点是否已经失效。
            */
            bool operator ==(mutable_edge_iterator const & right)const noexcept;
            bool operator !=(mutable_edge_iterator const & right)const noexcept;
            bool operator ==(immutable_edge_iterator const & right)const noexcept;
            bool operator !=(immutable_edge_iterator const & right)const noexcept;
            /*
            比较两个迭代器
            此函数仅用于std::less，以使得迭代器可以用std::set包装。
            此函数比较的不是迭代器所指向的图节点，图中的节点之间没有定义任何可以用来比较的偏序关系。
            */
            bool operator <(immutable_edge_iterator const & right)const noexcept;

            /*
            判断迭代器是否有效
            */
            bool is_valid()const noexcept;

            weight_type const * weight()const;

            graph const & owner()const;

            //此边的起点
            immutable_node_iterator begin()const;
            //此边的终点
            immutable_node_iterator end()const;


        };
        graph()
            noexcept(std::is_nothrow_default_constructible_v<std::set<node *>>
            and std::is_nothrow_default_constructible_v<std::set<edge *>>);
        ~graph()
            noexcept(std::is_nothrow_destructible_v<std::set<node *>>
            and std::is_nothrow_destructible_v<std::set<edge *>>);
        graph(graph const & origin);
        void operator =(graph const & origin);
        graph(graph &&)
            noexcept(std::is_nothrow_move_constructible_v<std::set<node *>>
            and std::is_nothrow_move_constructible_v<std::set<edge *>>);
        void operator =(graph &&)
            noexcept(std::is_nothrow_move_assignable_v<std::set<node *>>
            and std::is_nothrow_move_assignable_v<std::set<edge *>>);
        /*
        向图中添加节点
        */
        mutable_node_iterator add(std::unique_ptr<data_type> && data);
        /*
        删除图中的节点
        包含该节点的边也会被删除
        */
        void erase(mutable_node_iterator const &);
        /*
        添加一条从 begin_node 指向 end_node 的边
        */
        mutable_edge_iterator add(mutable_node_iterator const & begin_node, mutable_node_iterator const & end_node, std::unique_ptr<weight_type> &&);
        /*
        删除边
        */
        void erase(mutable_edge_iterator const &);
        /*
        清空图容器
        */
        void clear();

        immutable_node_iterator begin_node()const
            noexcept(noexcept(std::declval<std::set<node *>>().begin() == std::declval<std::set<node *>>().end())
         and noexcept(immutable_node_iterator(nullptr)));
        immutable_node_iterator end_node()const
            noexcept(noexcept(immutable_node_iterator(nullptr)));
        mutable_node_iterator begin_node()
            noexcept(noexcept(std::declval<std::set<node *>>().begin() == std::declval<std::set<node *>>().end())
and noexcept(mutable_node_iterator(nullptr)));
        mutable_node_iterator end_node()
            noexcept(noexcept(mutable_node_iterator(nullptr)));

        immutable_edge_iterator begin_edge()const
            noexcept(noexcept(std::declval<std::set<edge *>>().begin() == std::declval<std::set<edge *>>().end())
         and noexcept(immutable_edge_iterator(nullptr)));
        immutable_edge_iterator end_edge()const
            noexcept(noexcept(immutable_edge_iterator(nullptr)));
        mutable_edge_iterator begin_edge()
            noexcept(noexcept(std::declval<std::set<edge *>>().begin() == std::declval<std::set<edge *>>().end())
and noexcept(mutable_edge_iterator(nullptr)));
        mutable_edge_iterator end_edge()
            noexcept(noexcept(mutable_edge_iterator(nullptr)));
    };
}