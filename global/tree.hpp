#pragma once
#include ".\base.hpp"
#include <vector>
#include <memory>
#include <stack>
#include <queue>

namespace OKps
{

    template <typename data_type /*数据类型*/>
    /*
    基本二叉树
    使用智能指针管理内存，故分配器和释放器的选择由标准库决定
    */
    class binary_tree : public base
    {

    private:
        using REF_tree = binary_tree &;
        using CREF_tree = binary_tree const &;
        using PTR_tree = binary_tree *;
        using CPTR_tree = binary_tree const *;

    // 二叉树节点
        class node : public base
        /*
        此基类提供函数shared_from_this，作用是构造一个智能指针，和已经存在的其他智能指针共享当前对象的所有权
        它必须被public继承
        */
            , public std::enable_shared_from_this<node>
        {
            friend class binary_tree;
        private:
            using UPTR_data = std::unique_ptr<data_type>;
            using CUPTR_data = std::unique_ptr<data_type const>;
            using SPTR_data = std::shared_ptr<data_type>;
            using CSPTR_data = std::shared_ptr<data_type const>;
            using WPTR_data = std::weak_ptr<data_type>;
            using CWPTR_data = std::weak_ptr<data_type const>;
            using UPTR_node = std::unique_ptr<node>;
            using CUPTR_node = std::unique_ptr<node const>;
            using NPTR_node = node *;
            using CNPTR_node = node const *;
            using NPTRC_node = node * const;
            using WPTR_node = std::weak_ptr<node>;
            using CWPTR_node = std::weak_ptr<node const>;
            using SPTR_node = std::shared_ptr<node>;
            using CSPTR_node = std::shared_ptr<node const>;

            // 数据
            SPTR_data  MEMBER_data;
            // 左孩子
            SPTR_node MEMBER_left;
            // 右孩子
            SPTR_node MEMBER_right;
            // 父亲
            WPTR_node MEMBER_parent;


            /*
            所在的树
            */
            PTR_tree MEMBER_tree;



        public:

            node() = delete;

            //构造一个节点，其数据为空
            node(PTR_tree tree,
             SPTR_node const & parent = SPTR_node(),
              SPTR_node const & left = SPTR_node(),
               SPTR_node const & right = SPTR_node())noexcept
                :MEMBER_tree(tree),
                MEMBER_left(left),
                MEMBER_right(right),
                MEMBER_parent(parent)

            {

            }

            node(PTR_tree tree,
                data_type const & data,
                SPTR_node const & parent = SPTR_node(),
                SPTR_node const & left = SPTR_node(),
                SPTR_node const & right = SPTR_node())
                : MEMBER_tree(tree),
                MEMBER_data(std::make_shared<data_type>(data)),
                MEMBER_left(left),
                MEMBER_right(right),
                MEMBER_parent(parent)

            {


            }

            node(PTR_tree tree,
            data_type && data,
            SPTR_node const & parent = SPTR_node(),
            SPTR_node const & left = SPTR_node(),
             SPTR_node const & right = SPTR_node())
                : MEMBER_tree(tree),
                MEMBER_data(std::make_shared<data_type>(data)),
                MEMBER_left(left),
                MEMBER_right(right),
                MEMBER_parent(parent)

            {


            }
            node(const node &) = delete;
            void operator=(const node &) = delete;
            node(node && origin)noexcept
                :MEMBER_tree(origin.MEMBER_tree),
                MEMBER_data(std::move(origin.MEMBER_data)),
                MEMBER_left(std::move(origin.MEMBER_left)),
                MEMBER_right(std::move(origin.MEMBER_right)),
                MEMBER_parent(std::move(origin.MEMBER_parent))

            {

                origin.MEMBER_tree = nullptr;
            }
            void operator=(node && origin)noexcept

            {

                this->MEMBER_data = std::move(origin.MEMBER_data);
                this->MEMBER_left = std::move(origin.MEMBER_left);
                this->MEMBER_right = std::move(origin.MEMBER_right);
                this->MEMBER_parent = std::move(origin.MEMBER_parent);
                this->MEMBER_tree = origin.MEMBER_tree;
                origin.MEMBER_tree = nullptr;

            }


            ~node()noexcept
            {


            }

            // 返回数据内容
            SPTR_data  data() const noexcept
            {
                return this->MEMBER_data;
            }

            // 修改数据内容
            void change(data_type && data)
            {
                this->MEMBER_data = std::make_shared<data_type>(data);
            }
            // 修改数据内容
            void change(data_type const & data)
            {
                this->MEMBER_data = std::make_shared<data_type>(data);
            }
            //将数据置空
            void change()noexcept
            {

                this->MEMBER_data.reset();
            }

            // 返回父亲
            SPTR_node parent() const noexcept
            {
                return this->MEMBER_parent.lock();
            }


            // 初始化左孩子
            // 只有左孩子为空时可以使用
            void left_birth(data_type && data)
            {
                if (this->MEMBER_left)
                {
                    throw std::logic_error("此节点已经有左孩子，禁止重复初始化。");
                }
                else
                {
                    // new新节点，传入此节点作为父亲
                    this->MEMBER_left = std::make_shared<node>(this->MEMBER_tree, data, this->shared_from_this());


                }
            }
            void left_birth(data_type const & data)
            {
                if (this->MEMBER_left)
                {
                    throw std::logic_error("此节点已经有左孩子，禁止重复初始化。");
                }
                else
                {
                    // new新节点，传入此节点作为父亲
                    this->MEMBER_left = std::make_shared<node>(this->MEMBER_tree, data, this->shared_from_this());
                }
            }
            //构造左孩子，但数据为空
            void left_birth()
            {
                if (this->MEMBER_left)
                {
                    throw std::logic_error("此节点已经有左孩子，禁止重复初始化。");
                }
                else
                {
                    // new新节点，传入此节点作为父亲
                    this->MEMBER_left = std::make_shared<node>(this->MEMBER_tree, this->shared_from_this());
                }
            }

            // 返回左孩子
            SPTR_node left() const noexcept
            {
                return this->MEMBER_left;
            }



            // 清除左子树
            void left_clear()noexcept
            {
                if (this->MEMEBR_left)
                {

                    this->MEMBER_left.reset();
                }
            }

            // 初始化右孩子
            // 只有右孩子为空时可以使用
            void right_birth(data_type && data)
            {
                if (this->MEMBER_right)
                {
                    throw std::logic_error("此节点已经有右孩子，禁止重复初始化。");
                }
                else
                {
                    // new新节点，传入此节点作为父亲
                    this->MEMBER_right = std::make_shared<node>(this->MEMBER_tree, data, this->shared_from_this());

                }
            }
            void right_birth(data_type const & data)
            {
                if (this->MEMBER_right)
                {
                    throw std::logic_error("此节点已经有右孩子，禁止重复初始化。");
                }
                else
                {
                    // new新节点，传入此节点作为父亲
                    this->MEMBER_right = std::make_shared<node>(this->MEMBER_tree, data, this->shared_from_this());

                }
            }
            void right_birth()
            {
                if (this->MEMBER_right)
                {
                    throw std::logic_error("此节点已经有右孩子，禁止重复初始化。");
                }
                else
                {
                    // new新节点，传入此节点作为父亲
                    this->MEMBER_right = std::make_shared<node>(this->MEMBER_tree, this->shared_from_this());

                }
            }
            // 返回右孩子
            SPTR_node right() const noexcept
            {
                return this->MEMBER_right;

            }


            // 清除右子树
            void right_clear()noexcept
            {
                if (this->MEMBER_right)
                {

                    this->MEMBER_right.reset();
                }
            }

            // 判断此节点是否为叶子
            bool is_leaf() const noexcept
            {
                if ((not this->MEMBER_left) and (not this->MEMBER_right))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            // 返回此节点的深度
            std::size_t depth() const noexcept
            {
                std::size_t result = 1;
                auto temp = this->MEMBER_parent.lock();
                while (temp)
                {
                    result++;
                    temp = temp->MEMBER_parent.lock();
                }
                return result;
            }

            // 返回所在树的根节点
            SPTR_node root() const noexcept
            {
                return this->MEMBER_tree->MEMBER_root;
            }


            /*
            判断输入的节点是否是本节点的祖先
            如果输入空节点，返回false
            如果输入的节点与本节点相同，返回false
            */
            bool is_ancestor(SPTR_node const & node)const noexcept
            {
                if (not node)
                {
                    return false;
                }
                if (node->MEMBER_tree != this->MEMBER_tree)
                //不在同一棵树上，必然不是祖先关系
                {
                    return false;
                }
                auto TEMP_itor = this->shared_from_this();//迭代器

                if (TEMP_itor == node)
                {
                    return false;
                }
                //node和本节点在同一棵树上，但不是同一个节点
                while (true)
                {
                    auto TEMP_parent = TEMP_itor->MEMBER_parent.lock();
                    if (TEMP_parent)
                    {
                        if (TEMP_parent == node)
                        {
                            return true;
                        }
                        else
                        {
                            TEMP_itor = TEMP_parent;
                        }
                    }
                    else
                    //当前迭代器是根节点
                    {
                        return false;
                    }
                }
            }
            //交换本节点的两个子树
            void swap_child() noexcept
            {
                this->MEMBER_left.swap(this->MEMBER_right);
            }



            // 将此节点作为根节点，从原树中拆分出子树
            // 返回新树
            // 当此节点已经是树的根节点时，禁止使用
            binary_tree<data_type>  split_tree()
            {
                auto TEMP_parent = this->MEMBER_parent.lock();//此节点原来的父亲
                if (not TEMP_parent)
                {
                    throw std::logic_error("此节点已经是根节点，无法拆分出子树");
                }
                else
                {
                    // new空树，将根节点改为此节点
                    auto result = binary_tree<data_type>();
                    auto TEMP_self = this->shared_from_this();
                    result.MEMBER_root = TEMP_self;


                    // 修改此节点原父亲的关系
                    if (TEMP_parent->MEMBER_left == TEMP_self)
                    {
                        TEMP_parent->MEMBER_left.reset();
                    }
                    else
                    {
                        TEMP_parent->MEMBER_right.reset();
                    }
                    this->MEMBER_parent.reset();

                    result.INNER_change_owner_tree(result);
                    return result;
                }
            }

            // 将输入的树作为此节点的左孩子，与此树合并
            // 只有此节点无左孩子时能使用
            void combine_left(binary_tree<data_type> && subtree)
            {
                if (this->MEMBER_left)
                {
                    throw std::logic_error("此节点已经有左孩子，无法合并子树");
                }
                else
                {
                    subtree.INNER_change_owner_tree(this->MEMBER_tree);
                    // 合并树结构
                    this->MEMBER_left = std::move(subtree->MEMBER_root);


                }
            }

            // 将输入的树作为此节点的右孩子，与此树合并
            // 只有此节点无右孩子时能使用
            void combine_right(binary_tree<data_type> && subtree)
            {
                if (this->MEMBER_right)
                {
                    throw std::logic_error("此节点已经有右孩子，无法合并子树");
                }
                else
                {
                    subtree.INNER_change_owner_tree(this->MEMBER_tree);

                // 合并树结构
                    this->MEMBER_right = std::move(subtree->MEMBER_root);

                }
            }
        };


        // 根节点
        typename node::SPTR_node MEMBER_root;


        // 任意两个节点交换数据内容，而不改变家庭关系
        static void swap_data(typename node::SPTR_node const & node1, typename node::SPTR_node const & node2)
        {
            if (not node1 or not node2)
            // 输入空指针
            {
                throw std::invalid_argument("禁止输入空指针");
            }
            else
            {
                node1->MEMBER_data.swap(node2->MEMBER_data);

            }
        }
        /*
        任意两个节点作为子树的根节点，交换这2棵子树的位置
        这是说，仅交换输入的2个节点与其父亲的关系，所有后代关系不变
        输入不能为空指针
        输入的2个节点之间也不能有祖先关系
        */
        static void swap_tree(typename node::SPTR_node const & node1, typename node::SPTR_node const & node2)
        {
            if ((not node1) or (not node2))
            {
                throw std::invalid_argument("禁止输入空指针");
                return;
            }
            if (node1 == node2)
            //输入的两个节点相同
            {
                return;
            }
            auto TEMP_node1_parent = node1->MEMBER_parent.lock();//node1原来的父亲
            auto TEMP_node2_parent = node2->MEMBER_parent.lock();//node2原来的父亲


            auto TEMP_node1_tree = node1->MEMBER_tree;//node1原来的树
            auto TEMP_node2_tree = node2->MEMBER_tree;//node2原来的树
            if ((not TEMP_node1_parent) and (not TEMP_node2_parent))
            /*
            输入的是2个根节点
            由于它们不是同一个节点，故不在同一棵树
            */
            {

                TEMP_node1_tree->INNER_change_owner_tree(TEMP_node2_tree);//node1原来树下的所有节点改为node2原来的树所有
                TEMP_node2_tree->INNER_change_owner_tree(TEMP_node1_tree);
                TEMP_node1_tree->MEMBER_root = node2;
                TEMP_node2_tree->MEMBER_root = node1;
                return;
            }
            if (node1->is_ancestor(node2)/*node2是node1的祖先*/ or node2->is_ancestor(node1)/*node1是node2的祖先*/)
            {
                throw std::invalid_argument("禁止输入的节点之间存在祖先关系");
                return;
            }
            if (TEMP_node1_tree != TEMP_node2_tree)
            //node1和node2不在同一棵树上
            {

                binary_tree::INNER_change_owner_tree(node1, TEMP_node2_tree);
                binary_tree::INNER_change_owner_tree(node2, TEMP_node1_tree);
            }

            node2->MEMBER_parent = TEMP_node1_parent;
            node1->MEMBER_parent = TEMP_node2_parent;
            if (TEMP_node2_parent)
            {
                if (TEMP_node2_parent->MEMBER_left == node2)
                //node2是左孩子
                {
                    TEMP_node2_parent->MEMBER_left = node1;

                }
                else
                //node2是右孩子
                {
                    TEMP_node2_parent->MEMBER_right = node1;

                }

            }
            else
            //node2是根节点
            {

                TEMP_node2_tree->MEMBER_root = node1;
            }
            if (TEMP_node1_parent)
            {
                if (TEMP_node1_parent->MEMBER_left == node1)
                //node1是左孩子
                {
                    TEMP_node1_parent->MEMBER_left = node2;

                }
                else
                //node1是右孩子
                {
                    TEMP_node1_parent->MEMBER_right = node2;

                }

            }
            else
            //node1是根节点
            {

                TEMP_node1_tree->MEMBER_root = node2;
            }

        }
    public:
        // 初始化为空树
        binary_tree()noexcept
        {
        }

            // 将现存数据填入根节点，初始化树
        binary_tree(data_type && data)noexcept
            : MEMBER_root(std::make_shared<node>(this, data))
        {

        }
        binary_tree(data_type const & data)noexcept
            : MEMBER_root(std::make_shared<node>(this, data))
        {


        }

        binary_tree(const binary_tree &) = delete;
        binary_tree(binary_tree && origin)noexcept
            :MEMBER_root(std::move(origin.MEMBER_root))
        {
            this->INNER_change_owner_tree(this);
        }

        ~binary_tree()noexcept
        {

        }


    public:
        // 初始化根节点
        // 只有在根节点为空时才能使用
        void root_birth(data_type && data)
        {
            if (this->MEMBER_root)
            {
                throw std::logic_error("根节点不空，禁止重复初始化。");
            }
            else
            {

                this->MEMBER_root = std::make_shared<node>(this, data);

            }
        }
        void root_birth(data_type const & data)
        {
            if (this->MEMBER_root)
            {
                throw std::logic_error("根节点不空，禁止重复初始化。");
            }
            else
            {
                this->MEMBER_root = std::make_shared<node>(this, data);


            }
        }

        void root_birth()
        {
            if (this->MEMBER_root)
            {
                throw std::logic_error("根节点不空，禁止重复初始化。");
            }
            else
            {
                this->MEMBER_root = std::make_shared<node>(this);


            }
        }

        // 清除此树，即delete所有节点，指针置空
        void clear() noexcept
        {

            if (this->MEMBER_root)
            {

                this->MEMBER_root.reset();
            }
        }

        //迭代器
        class iterator :public base
        {
            friend class binary_tree;
        private:
            typename node::WPTR_node MEMBER_node;

            iterator(typename node::SPTR_node const & origin = typename node::SPTR_node())noexcept
                :MEMBER_node(origin)
            {
            }
        public:
            ~iterator()noexcept
            {

            }
        private:
            typename node::SPTR_node ERROR_invalid_node()const
            {
                auto temp_node = this->MEMBER_node.lock();
                if (not (temp_node))
                {
                    throw std::logic_error("此迭代器引用的是空节点，无法操作");
                }
                return temp_node;
            }
        public:
            iterator(iterator const & origin)noexcept
                :MEMBER_node(origin.MEMBER_node)
            {
            }
            void operator=(iterator const & origin)noexcept
            {
                this->MEMBER_node = origin.MEMBER_node;
            }
            iterator(iterator && origin)noexcept
                :MEMBER_node(std::move(origin.MEMBER_node))
            {
            }
            void operator=(iterator && origin)noexcept
            {
                this->MEMBER_node = std::move(origin.MEMBER_node);
            }
            bool operator==(iterator const & right)const
            {
                auto temp_me = this->ERROR_invalid_node();
                auto temp_other = right.ERROR_invalid_node();
                if (temp_me == temp_other)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            bool operator!=(iterator const & right)const
            {
                return not((*this) == right);
            }
            typename node::SPTR_data data()
            {
                auto temp_node = this->ERROR_invalid_node();
                return temp_node->data();
            }
            typename node::CSPTR_data data()const
            {
                auto temp_node = this->ERROR_invalid_node();
                return temp_node->data();
            }
            iterator left()
            {
                auto temp_node = this->ERROR_invalid_node();
                return iterator(temp_node->left());
            }
            iterator const left()const
            {
                auto temp_node = this->ERROR_invalid_node();
                return iterator(temp_node->left());
            }
            iterator right()
            {
                auto temp_node = this->ERROR_invalid_node();
                return iterator(temp_node->right());
            }
            iterator const right()const
            {
                auto temp_node = this->ERROR_invalid_node();
                return iterator(temp_node->right());
            }
            iterator parent()
            {
                auto temp_node = this->ERROR_invalid_node();
                return iterator(temp_node->parent());
            }
            iterator const parent()const
            {
                auto temp_node = this->ERROR_invalid_node();
                return iterator(temp_node->parent());
            }
            iterator root()
            {
                auto temp_node = this->ERROR_invalid_node();
                return iterator(temp_node->root());
            }
            iterator const root()const
            {
                auto temp_node = this->ERROR_invalid_node();
                return iterator(temp_node->root());
            }
            void change()
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->change();

            }
            void change(data_type && data)
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->change(data);
            }
            void change(data_type const & data)
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->change(data);
            }

            void right_birth()
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->right_birth();

            }
            void right_birth(data_type && data)
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->right_birth(data);
            }
            void right_birth(data_type const & data)
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->right_birth(data);
            }
            void right_clear()
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->right_clear();
            }
            void left_birth()
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->left_birth();

            }
            void left_birth(data_type && data)
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->left_birth(data);
            }
            void left_birth(data_type const & data)
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->left_birth(data);
            }
            void left_clear()
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->left_clear();
            }
            bool is_leaf()const
            {
                auto temp_node = this->ERROR_invalid_node();
                return temp_node->is_leaf();
            }
            std::size_t depth()const
            {
                auto temp_node = this->ERROR_invalid_node();
                return temp_node->depth();
            }
            bool is_ancestor()const
            {
                auto temp_node = this->ERROR_invalid_node();
                return temp_node->is_ancestor();
            }
            void swap_child()
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->swap_child();
            }
            /*
            注意，迭代器的成员函数都不会改变当前迭代器指向的节点
            例如此函数将当前节点作为根节点，拆分出子树
            虽然构造了一个新的二叉树对象，而且改变了原来二叉树的构造
            但当前迭代器依然指向原来所指向的节点，即构造的新二叉树的根节点
            */
            binary_tree<data_type>  split_tree()
            {
                auto temp_node = this->ERROR_invalid_node();
                return temp_node->split_tree();
            }

            void combine_left(binary_tree<data_type> && subtree)
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->combine_left(subtree);
            }

            void combine_right(binary_tree<data_type> && subtree)
            {
                auto temp_node = this->ERROR_invalid_node();
                temp_node->combine_right(subtree);
            }

            /*
            检查当前节点是否有效
            而不是检查当前节点的数据是否为空
            */
            operator bool()const noexcept
            {
                auto temp_node = this->MEMBER_node.lock();
                if (not temp_node)
                {
                    return false;
                }
                else
                {

                    return true;

                }
            }
        private:
            typename node::SPTR_node operator ->()
            {

                auto temp_node = this->ERROR_invalid_node();
                return temp_node;
            }
            typename node::CSPTR_node operator ->()const
            {

                auto temp_node = this->ERROR_invalid_node();
                return temp_node;
            }
        };


        // 作为遍历函数的返回值
        // 因为1次遍历就可以获取树的所有信息
        class tree_information final : public basic_no_heap
        {
            friend class binary_tree;
        private:
            // 节点数
            std::size_t  MEMBER_node;
           // 叶子节点数
            std::size_t  MEMBER_leaf;
           // 深度
            std::size_t  MEMBER_depth;

            using TYPE_list = std::vector<iterator>;
           // 遍历数组
           // std::vector不允许使用const类型作为模板参数，否则编译时会触发静态断言

            TYPE_list  MEMBER_list;
        public:
            tree_information(const std::size_t node, const std::size_t leaf, const std::size_t depth, TYPE_list && list) noexcept
                : MEMBER_node(node), MEMBER_leaf(leaf), MEMBER_depth(depth), MEMBER_list(std::move(list))
            {
            }

            tree_information() = delete;
            tree_information(const tree_information & origin)noexcept
                :MEMBER_node(origin.MEMBER_node),
                MEMBER_leaf(origin.MEMBER_leaf),
                MEMBER_depth(origin.MEMBER_depth),
                MEMBER_list(origin.MEMBER_list)
            {
            }
            void operator=(const tree_information & origin)noexcept
            {
                this->MEMBER_node = origin.MEMBER_node;
                this->MEMBER_leaf = origin.MEMBER_leaf;
                this->MEMBER_depth = origin.MEMBER_depth;
                this->MEMBER_list = origin.MEMBER_list;

            }
            tree_information(tree_information && origin)noexcept
                : MEMBER_node(origin.MEMBER_node),
                MEMBER_leaf(origin.MEMBER_leaf),
                MEMBER_depth(origin.MEMBER_depth),
                MEMBER_list(std::move(origin.MEMBER_list))
            {

            }
            void operator=(tree_information && origin)noexcept
            {
                this->MEMBER_node = origin.MEMBER_node;
                origin.MEMBER_node = 0;
                this->MEMBER_leaf = origin.MEMBER_leaf;
                origin.MEMBER_leaf = 0;
                this->MEMBER_depth = origin.MEMBER_depth;
                origin.MEMBER_depth = 0;
                this->MEMBER_list = std::move(origin.MEMBER_list);
            }

            ~tree_information()noexcept
            {

            }
            // 节点数
            std::size_t node()const noexcept
            {
                return this->MEMBER_node;
            }
            // 叶子节点数
            std::size_t leaf()const noexcept
            {
                return this->MEMBER_leaf;
            }
            // 树的深度
            std::size_t depth()const noexcept
            {
                return this->MEMBER_depth;
            }
            //将指向树的节点的迭代器，按遍历顺序保存在std::vector中
            TYPE_list & list()noexcept
            {
                return this->MEMBER_list;
            }
            TYPE_list const & list()const noexcept
            {
                return this->MEMBER_list;
            }
        };
    private:
        /*
        先序遍历
        如果输入空节点，则返回空列表
        */
        static tree_information front_traversal(typename  node::SPTR_node const & root)
        {

            auto result = typename tree_information::TYPE_list();
            if (not root)
            // 空树，返回空数组
            {

                return tree_information(0, 0, 0, std::move(result));
            }

            // 保存结果

            // 操作结果数组的下标
            std::size_t count = 0;
            std::size_t leaf = 0;
            std::size_t depth = 1;
             // 迭代器
            auto _root = root;

            // 辅助栈
            std::stack<typename node::SPTR_node  > assistant;
            while (true)
            {
                while (_root)
                // 迭代器不空
                {
                    // 迭代器指向的节点写入结果
                    result.push_back(iterator(_root));
                    count++;
                    if (_root->is_leaf())
                    {
                        leaf++;
                    }
                    if (_root->depth() > depth)
                    {
                        depth = _root->depth();
                    }
                    if (_root->right())
                    // 迭代器有右孩子
                    {
                        // 右孩子入栈
                        assistant.push(_root->right());
                    }
                    // 迭代器变为自己的左孩子
                    _root = _root->left();
                }
                if (assistant.empty())
                // 辅助栈空
                {
                    break;
                }
                // 辅助栈顶部出栈，作为下一个循环的起点

                _root = assistant.top();
                assistant.pop();
            }
            return tree_information(count, leaf, depth, std::move(result));

        }
    public:
        const tree_information front_traversal()const
        {
            return binary_tree::front_traversal(this->MEMBER_root);
        }
        tree_information front_traversal()
        {
            return binary_tree::front_traversal(this->MEMBER_root);
        }
    private:

    // 中序遍历
        static tree_information middle_traversal(typename node::SPTR_node const & root)
        {

            auto result = typename tree_information::TYPE_list();

            if (not root)
            // 空树，返回空数组
            {
                return tree_information(0, 0, 0, std::move(result));
            }

            // 保存结果
            // 操作结果数组的下标
            std::size_t count = 0;
            std::size_t leaf = 0;
            std::size_t depth = 0;
            // 迭代器
            auto _root = root;
            // 辅助栈
            std::stack<typename node::SPTR_node  > assistant;
            while (true)
            {
                if (_root)
                {
                    // 迭代器进栈
                    assistant.push(_root);
                    // 迭代到左孩子
                    _root = _root->left();
                }
                else if (not assistant.empty())
                {
                    // 尚未访问到的最低祖先节点出栈
                    _root = assistant.top();
                    assistant.pop();
                    // 访问该节点
                    result.push_back(iterator(_root));
                    count++;
                    if (_root->is_leaf())
                    {
                        leaf++;
                    }
                    if (_root->depth() > depth)
                    {
                        depth = _root->depth();
                    }
                    // 遍历其右子树
                    _root = _root->right();
                }
                else
                // 遍历完成
                {
                    break;
                }
            }
            return tree_information(count, leaf, depth, std::move(result));
        }
    public:
        const tree_information middle_traversal()const
        {
            return binary_tree::middle_traversal(this->MEMBER_root);
        }
        tree_information middle_traversal()
        {
            return binary_tree::middle_traversal(this->MEMBER_root);
        }
    private:

        // 后序遍历
        static tree_information behind_traversal(typename  node::SPTR_node const & root)
        {

            auto result = typename tree_information::TYPE_list();

            if (not root)
            // 空树，返回空数组
            {
                return tree_information(0, 0, 0, std::move(result));
            }

            // 保存结果
            // 操作结果数组的下标
            std::size_t count = 0;
            std::size_t leaf = 0;
            std::size_t depth = 0;
            // 迭代器
            auto _root = root;
            // 辅助栈
            std::stack<typename node::SPTR_node > assistant;

            assistant.push(_root);

            while (not assistant.empty())
            {
                if (assistant.top() != _root->parent())
                {

                    while (auto temp = assistant.top()) // 自顶而下，反复检查当前节点（即栈顶）
                    {
                        if (temp->left())
                        { // 尽可能向左迭代
                            if (temp->right())
                            {
                                assistant.push(temp->right());
                            }                                   // 若有右孩子，优先入栈
                            assistant.push(temp->left()); // 然后才转至左孩子
                        }
                        else // 无法向左才向右
                        {
                            assistant.push(temp->right());
                        }
                    }
                    assistant.pop(); // 返回之前，弹出栈顶的空节点
                }
                _root = assistant.top();
                assistant.pop();
                result.push_back(iterator(_root));
                count++;
                if (_root->is_leaf())
                {
                    leaf++;
                }
                if (_root->depth() > depth)
                {
                    depth = _root->depth();
                }
            }

            return tree_information(count, leaf, depth, std::move(result));
        }
    public:
        tree_information behind_traversal()
        {
            return binary_tree::behind_traversal(this->MEMBER_root);
        }
        const  tree_information behind_traversal()const
        {
            return binary_tree::behind_traversal(this->MEMBER_root);
        }
    private:
    // 按层次遍历，从上到下，从左到右
    // 禁止输入空节点
        static tree_information level_traversal(typename  node::SPTR_node const & root)
        {

            auto result = typename tree_information::TYPE_list();

            if (not root)
                   // 空树，返回空数组
            {
                return tree_information(0, 0, 0, std::move(result));
            }

            // 保存结果
            // 操作结果数组的下标
            std::size_t count = 0;
            std::size_t leaf = 0;
            std::size_t depth = 0;
              // 辅助队列
            std::queue<typename node::SPTR_node > assistant;
            // 根节点入队
            assistant.push(root);
            while (not assistant.empty())
            {
                // 取出队头节点，访问
                auto temp = assistant.front();
                assistant.pop();
                result.push_back(iterator(temp));
                count++;
                if (temp->is_leaf())
                {
                    leaf++;
                }
                if (temp->depth() > depth)
                {
                    depth = temp->depth();
                }

                if (temp->left())
                {
                    // 左孩子入队
                    assistant.push(temp->left());
                }
                if (temp->right())
                {
                    // 右孩子入队
                    assistant.push(temp->right());
                }
            }
            return tree_information(count, leaf, depth, std::move(result));
        }
    public:
        tree_information level_traversal()
        {
            return binary_tree::level_traversal(this->MEMBER_root);
        }
        const tree_information level_traversal()const
        {
            return binary_tree::level_traversal(this->MEMBER_root);
        }
    private:

        void INNER_change_owner_tree(PTR_tree owner)const
        {
            auto temp = this->behind_traversal();
            for (std::size_t counter = 0;counter < temp.MEMBER_list.size();counter++)
            {
                temp.MEMBER_list[counter]->MEMBER_tree = owner;
            }
        }
        void INNER_change_owner_tree(REF_tree owner)const
        {
            this->INNER_change_owner_tree(&owner);
        }
        //以node为子树的根节点，将这棵子树的所有节点的MEMBER_tree改为owner
        static void INNER_change_owner_tree(typename node::SPTR_node const & node, PTR_tree owner)
        {
            auto temp = binary_tree::behind_traversal(node);
            for (std::size_t counter = 0;counter < temp.MEMBER_list.size(); counter++)
            {
                temp.MEMBER_list[counter]->MEMBER_tree = owner;
            }
        }
        static void INNER_change_owner_tree(typename node::SPTR_node const & node, REF_tree owner)
        {
            binary_tree::INNER_change_owner_tree(node, &owner);
        }

    public:
        /*
        根据数据，寻找节点
        要求data_type具有 == 操作符
        如果找不到，返回空迭代器
        */
        iterator find(data_type const & data)
        {
            auto temp = this->behind_traversal();
            for (std::size_t count = 0;count < temp.MEMBER_list.size();count++)
            {
                auto finder = temp.MEMBER_list[count]->data();
                if (finder and ((*finder) == data))
                {
                    return temp.MEMBER_list[count];
                }
            }
            return iterator();
        }
        iterator const find(data_type const & data)const
        {
            auto temp = this->behind_traversal();
            for (std::size_t count = 0; count < temp.MEMBER_list.size(); count++)
            {
                auto finder = temp.MEMBER_list[count]->data();
                if (finder and ((*finder) == data))
                {
                    return temp.MEMBER_list[count];
                }
            }
            return iterator();
        }
        static void swap_data(iterator & node1, iterator & node2)
        {
            auto temp_node1 = node1.ERROR_invalid_node();
            auto temp_node2 = node2.ERROR_invalid_node();
            binary_tree::swap_data(temp_node1, temp_node2);
        }
        /*
        任意两个节点作为子树的根节点，交换这2棵子树的位置
        这是说，仅交换输入的2个节点与其父亲的关系，所有后代关系不变
        输入不能为空指针
        输入的2个节点之间也不能有祖先关系
        */
        static void swap_tree(iterator & node1, iterator & node2)
        {
            auto temp_node1 = node1.ERROR_invalid_node();
            auto temp_node2 = node2.ERROR_invalid_node();
            binary_tree::swap_tree(temp_node1, temp_node2);
        }
        /*
        如果目前还没有根节点，则返回空迭代器
        注意空迭代器是无法使用的
        */
        iterator root()noexcept
        {
            return iterator(this->MEMBER_root);
        }
        iterator const root()const noexcept
        {
            return iterator(this->MEMBER_root);
        }


        static const tree_information level_traversal(iterator const & root)
        {
            auto temp_root = root.ERROR_invalid_node();

            return binary_tree::level_traversal(temp_root);
        }
        static  tree_information level_traversal(iterator & root)
        {
            auto temp_root = root.ERROR_invalid_node();

            return binary_tree::level_traversal(temp_root);
        }
        static  tree_information behind_traversal(iterator & root)
        {
            auto temp_root = root.ERROR_invalid_node();

            return binary_tree::behind_traversal(temp_root);
        }
        static const tree_information behind_traversal(iterator const & root)
        {
            auto temp_root = root.ERROR_invalid_node();

            return binary_tree::behind_traversal(temp_root);
        }
        static const tree_information middle_traversal(iterator const & root)
        {
            auto temp_root = root.ERROR_invalid_node();

            return binary_tree::middle_traversal(temp_root);
        }
        static tree_information middle_traversal(iterator & root)
        {
            auto temp_root = root.ERROR_invalid_node();

            return binary_tree::middle_traversal(temp_root);
        }
        static const tree_information front_traversal(iterator const & root)
        {
            auto temp_root = root.ERROR_invalid_node();

            return binary_tree::front_traversal(temp_root);
        }
        static tree_information front_traversal(iterator & root)
        {
            auto temp_root = root.ERROR_invalid_node();

            return binary_tree::front_traversal(temp_root);
        }
    };

}
