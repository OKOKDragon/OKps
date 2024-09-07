#pragma once

#include <vector>
#include <memory>

namespace OKps
{

    /*
    基本二叉树

    使用new delete手动管理内存，原因是智能指针递归调用析构函数可能会造成栈溢出
    */
    class binary_tree
    {
    public:
        /*
        要保存在树内的数据的基类
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
            ~marker_type()noexcept;
            marker_type(marker_type const &) = delete;
            marker_type(marker_type &&) = delete;
            void operator =(marker_type const &) = delete;
            void operator =(marker_type &&) = delete;
        };

        using PTR_data = std::unique_ptr<data_type>;
    // 二叉树节点
        class node final
        {
        private:

            // 数据
            PTR_data MEMBER_data;
            // 左孩子
            node * MEMBER_left;
            // 右孩子
            node * MEMBER_right;
            // 父亲
            node * MEMBER_parent;
            //所在的树
            binary_tree * MEMBER_tree;

            std::shared_ptr<marker_type const> const MEMBER_marker;
        public:
            /*
            以root为根节点，析构树
            允许输入空指针
            不允许输入空节点
            */
            static void clear_tree(node * const root);

            node() = delete;

            //构造一个节点，其数据为空
            node(binary_tree * const tree,
                node * const parent = nullptr,
                node * const left = nullptr,
                node * const right = nullptr);

            //构造一个节点，其数据为data
            node(binary_tree * const tree,
                PTR_data && data,
                node * const parent = nullptr,
                node * const left = nullptr,
                node * const right = nullptr);
            node(const node &) = delete;
            void operator =(const node &) = delete;
            /*
            移动节点
            移动后，原节点将被置空
            */
            node(node && origin) = delete;

            void operator =(node && origin) = delete;
            /*
            析构此节点，释放数据，改变家庭关系
            要求此节点是叶子节点
            */
            ~node()
                noexcept(false);
            operator bool()const noexcept;
        private:
            //检查此节点是否无效，若无效则抛出异常
            void ERROR_invalid()const noexcept(false);
        public:
            // 返回数据内容
            data_type const * data() const;

            data_type * data();

            //将数据置空
            void clear();

            // 返回父亲
            node const * parent() const;
            // 返回父亲
            node * parent();

            // 初始化左孩子
            // 只有左孩子为空时可以使用
            void left_birth(PTR_data && data);

            //构造左孩子，但数据为空
            void left_birth();

            // 返回左孩子
            node const * left() const;
            // 返回左孩子
            node * left();

            // 清除左子树
            void left_clear();

            // 初始化右孩子
            // 只有右孩子为空时可以使用
            void right_birth(PTR_data && data);

            void right_birth();

            // 返回右孩子
            node * right();
            // 返回右孩子
            node const * right() const;

            // 清除右子树
            void right_clear();

            // 判断此节点是否为叶子
            bool is_leaf() const;

            /*
            返回此节点的深度
            规定根节点的深度为1
            */
            std::size_t depth() const;

            // 返回所在树的根节点
            node const * root() const;
            // 返回所在树的根节点
            node * root();

            /*
            判断输入的节点n是否是本节点的祖先
            如果输入空节点，返回false
            如果输入的节点与本节点相同，返回false
            */
            bool is_ancestor(node const * const n)const;

            //交换本节点的两个子树
            void swap_child();

            // 将此节点作为根节点，从原树中拆分出子树
            // 返回新树
            // 当此节点已经是树的根节点时，禁止使用
            binary_tree split_tree();

            // 将输入的树作为此节点的左孩子，与此树合并
            // 只有此节点无左孩子时能使用
            void combine_left(binary_tree && subtree);

            // 将输入的树作为此节点的右孩子，与此树合并
            // 只有此节点无右孩子时能使用
            void combine_right(binary_tree && subtree);

            //交换此节点和节点right的数据
            void swap_data(node * const right);

            /*
            任意两个节点作为子树的根节点，交换这2棵子树的位置
            这是说，仅交换输入的2个节点与其父亲的关系，所有后代关系不变
            输入不能为空指针
            输入的2个节点之间也不能有祖先关系
            */
            static void swap_tree(node * const left, node * const right);

            std::shared_ptr<marker_type const> const & marker()const noexcept;

            binary_tree * tree()noexcept;
            binary_tree const * tree()const noexcept;

            void change_tree(binary_tree * const);
        };

        // 根节点
        node * MEMBER_root;

    public:
        // 初始化为空树
        binary_tree()noexcept;

        // 将现存数据填入根节点，初始化树
        binary_tree(PTR_data && data);

        /*
        c++没有动态反射
        由于用基类data_type的指针存储其子类对象，此复制构造函数无法在运行期获取子类对象的实际类型，故无法正确复制子类对象
        所以此二叉树无法复制构造
        */
        binary_tree(const binary_tree & origin) = delete;
        void operator =(const binary_tree & origin) = delete;
        binary_tree(binary_tree && origin)noexcept(false);
        void operator =(binary_tree && origin)noexcept(false);
        ~binary_tree()noexcept;

    public:
        // 初始化根节点
        // 只有在根节点为空时才能使用
        void root_birth(PTR_data && data);

        void root_birth();


        // 删除所有节点
        void clear() noexcept;

        /*
        迭代器
        只允许通过迭代器访问节点
        */
        class iterator
        {
            friend class binary_tree;
        private:
            node * MEMBER_node;
            std::weak_ptr<marker_type const> MEMBER_marker;

            void ERROR_invalid()const noexcept(false);

            iterator(node * const origin = nullptr)
                noexcept(noexcept(std::weak_ptr<marker_type const>(std::declval<std::shared_ptr<marker_type const> const &>())));

        public:
            ~iterator()
                noexcept(std::is_nothrow_destructible<std::weak_ptr<marker_type const>>::value);

            iterator(iterator const & origin)
                noexcept(std::is_nothrow_copy_constructible<std::weak_ptr<marker_type const>>::value);

            void operator =(iterator const & origin)
                noexcept(std::is_nothrow_copy_assignable<std::weak_ptr<marker_type const>>::value);

            iterator(iterator && origin)
                noexcept(std::is_nothrow_move_constructible<std::weak_ptr<marker_type const>>::value);

            void operator =(iterator && origin)
                noexcept(std::is_nothrow_move_assignable<std::weak_ptr<marker_type const>>::value);

            /*
            比较两个迭代器所引用的是否是同一个节点
            若节点失效则抛出异常
            */
            bool operator==(iterator const & right)const;

            bool operator!=(iterator const & right)const;

            /*
            如果节点保有的数据为空则抛异常
            数据为空的意思不是数据为0，也不是数据为data_type类型的默认值
            数据为空的意思是该节点没有初始化其数据，即该节点保存的指向data_type类型数据的指针是空指针

            禁止delete或free此函数返回的指针
            */
            data_type * data();

            data_type const * data()const;

            iterator left();

            std::unique_ptr<iterator const> left()const;

            iterator right();

            std::unique_ptr<iterator const> right()const;

            iterator parent();

            std::unique_ptr<iterator const> parent()const;

            iterator root();

            std::unique_ptr<iterator const> root()const;

            void right_birth();

            void right_birth(PTR_data && data);

            void right_clear();

            void left_birth();

            void left_birth(PTR_data && data);

            void left_clear();

            bool is_leaf()const;

            std::size_t depth()const;

            bool is_ancestor(iterator const & n)const;

            void swap_child();

            /*
            注意，迭代器的成员函数都不会改变当前迭代器指向的节点
            例如此函数将当前节点作为根节点，拆分出子树
            虽然构造了一个新的二叉树对象，而且改变了原来二叉树的构造
            但当前迭代器依然指向原来所指向的节点，即构造的新二叉树的根节点
            */
            binary_tree split_tree();

            void combine_left(binary_tree && subtree);

            void combine_right(binary_tree && subtree);

            /*
            检查当前节点是否有效
            */
            operator bool()const
                noexcept(noexcept(std::declval<iterator const *>()->MEMBER_marker.expired()));

            //交换此节点和节点right的数据
            void swap_data(iterator & right);

            /*
            任意两个节点作为子树的根节点，交换这2棵子树的位置
            这是说，仅交换输入的2个节点与其父亲的关系，所有后代关系不变
            输入不能为空指针
            输入的2个节点之间也不能有祖先关系
            */
            static void swap_tree(iterator & left, iterator & right);

        private:
            node * operator ->();
            node const * operator ->()const;
        };


        // 作为遍历函数的返回值
        // 因为1次遍历就可以获取树的所有信息
        class tree_information final
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

            TYPE_list MEMBER_list;

            tree_information(const std::size_t node, const std::size_t leaf, const std::size_t depth, TYPE_list && list)
                noexcept(std::is_nothrow_move_constructible<TYPE_list>::value);

        public:
            tree_information() = delete;
            tree_information(tree_information const & origin)
                noexcept(std::is_nothrow_copy_constructible<TYPE_list>::value);

            void operator=(tree_information const & origin)
                noexcept(std::is_nothrow_copy_assignable<TYPE_list>::value);

            tree_information(tree_information && origin)
                noexcept(std::is_nothrow_move_constructible<TYPE_list>::value);

            void operator=(tree_information && origin)
                noexcept(std::is_nothrow_move_assignable<TYPE_list>::value);

            ~tree_information()
                noexcept(std::is_nothrow_destructible<TYPE_list>::value);

            // 节点数
            std::size_t node()const noexcept;

            // 叶子节点数
            std::size_t leaf()const noexcept;

            // 树的深度
            std::size_t depth()const noexcept;

            //将指向树的节点的迭代器，按遍历顺序保存在std::vector中
            TYPE_list & list()noexcept;

            TYPE_list const & list()const noexcept;

        };
    private:
        /*
        先序遍历
        如果输入空节点，则返回空列表
        */
        static tree_information front_traversal(node * const root);

    public:
        std::unique_ptr<tree_information const> front_traversal()const;

        tree_information front_traversal();

    private:

        // 中序遍历
        static tree_information middle_traversal(node * const root);

    public:
        std::unique_ptr<tree_information const> middle_traversal()const;

        tree_information middle_traversal();

    private:

        // 后序遍历
        static tree_information behind_traversal(node * const root);

    public:
        tree_information behind_traversal();

        std::unique_ptr<tree_information const> behind_traversal()const;

    private:
    // 按层次遍历，从上到下，从左到右
    // 禁止输入空节点
        static tree_information level_traversal(node * const root);

    public:
        tree_information level_traversal();

        std::unique_ptr<tree_information const> level_traversal()const;

    private:

        void INNER_change_owner_tree(binary_tree * const owner);

        //以node为子树的根节点，将这棵子树的所有节点的MEMBER_tree改为owner
        static void INNER_change_owner_tree(node * const node, binary_tree * const owner);

    public:
        /*
        根据数据，寻找节点
        要求data_type具有 == 操作符
        如果找不到，返回空迭代器
        */
        iterator find(data_type const & data);

        std::unique_ptr<iterator const> find(data_type const & data)const;

        /*
        如果目前还没有根节点，则返回空迭代器
        注意空迭代器是无法使用的
        */
        iterator root()noexcept;

        std::unique_ptr<iterator const> root()const noexcept;

        static std::unique_ptr<tree_information const> level_traversal(iterator const & root);

        static tree_information level_traversal(iterator & root);

        static tree_information behind_traversal(iterator & root);

        static std::unique_ptr<tree_information const> behind_traversal(iterator const & root);

        static std::unique_ptr<tree_information const> middle_traversal(iterator const & root);

        static tree_information middle_traversal(iterator & root);

        static std::unique_ptr<tree_information const> front_traversal(iterator const & root);

        static tree_information front_traversal(iterator & root);

    };

}
