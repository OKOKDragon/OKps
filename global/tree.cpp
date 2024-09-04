#include <stack>
#include <queue>
#include <stdexcept>

#include ".\tree.hpp"

namespace OKps
{
    binary_tree::data_type::data_type()noexcept
    {}
    binary_tree::data_type::data_type(data_type const&)noexcept
    {
    }
    binary_tree::data_type::data_type(data_type&&)noexcept
    {}
    binary_tree::data_type::~data_type()noexcept
    {}
    void binary_tree::data_type::operator =(data_type const&)noexcept
    {}
    void binary_tree::data_type::operator =(data_type&&)noexcept
    {}
    binary_tree::data_type& binary_tree::data_type::self()noexcept
    {
        return *this;
    }
    binary_tree::data_type const& binary_tree::data_type::self()const noexcept
    {
        return *this;
    }

    binary_tree::node::node(binary_tree* const tree,
        node* const parent,
        node* const left,
        node* const right)
        :MEMBER_tree(tree),
        MEMBER_left(left),
        MEMBER_right(right),
        MEMBER_parent(parent),
        MEMBER_marker(std::make_shared<marker_type const>())
    {
    }

    binary_tree::node::node(binary_tree* const tree,
        PTR_data&& data,
        node* const parent,
        node* const left,
        node* const right)
        : MEMBER_tree(tree),
        MEMBER_data(std::move(data)),
        MEMBER_left(left),
        MEMBER_right(right),
        MEMBER_parent(parent)
        ,MEMBER_marker(std::make_shared<marker_type const>())
    {
    }

    binary_tree::node::~node()
        noexcept(false)
    {
        //��ʵ�֣�������������Ϊֻ��������������������������������������
        if (this->is_leaf())
        {
            if (this->MEMBER_parent)
            {
                if (this->MEMBER_parent->MEMBER_left == this)
                {
                    this->MEMBER_parent->MEMBER_left = nullptr;
                }
                else
                {
                    this->MEMBER_parent->MEMBER_right = nullptr;
                }
            }
        }
        else
        {
            throw std::logic_error("�˽ڵ㲻��Ҷ�ӽڵ㣬�޷�����");
        }

    }
    binary_tree::node::operator bool()const noexcept
    {
        if (not this->MEMBER_tree)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    void binary_tree::node::ERROR_invalid()const noexcept(false)
    {
        if (not (*this))
        {
            throw std::logic_error("�˽ڵ�Ϊ�գ���ֹ����");
        }
    }
    binary_tree::data_type const * binary_tree::node::data() const
    {
        this->ERROR_invalid();
        return this->MEMBER_data.get();
    }
    binary_tree::data_type * binary_tree::node::data()
    {
        this->ERROR_invalid();
        return this->MEMBER_data.get();
    }
    void binary_tree::node::clear()
    {
        this->ERROR_invalid();
        this->MEMBER_data.reset();
    }
    binary_tree::node const* binary_tree::node::parent() const
    {
        this->ERROR_invalid();
        return this->MEMBER_parent;
    }
    void binary_tree::node::left_birth(PTR_data&& data)
    {
        this->ERROR_invalid();
        if (this->MEMBER_left)
        {
            throw std::logic_error("�˽ڵ��Ѿ������ӣ���ֹ�ظ���ʼ����");
        }
        else
        {
            // new�½ڵ㣬����˽ڵ���Ϊ����
            this->MEMBER_left = new node(this->MEMBER_tree, std::move(data), this);
        }
    }
    void binary_tree::node::left_birth()
    {
        this->ERROR_invalid();
        if (this->MEMBER_left)
        {
            throw std::logic_error("�˽ڵ��Ѿ������ӣ���ֹ�ظ���ʼ����");
        }
        else
        {
            // new�½ڵ㣬����˽ڵ���Ϊ����
            this->MEMBER_left = new node(this->MEMBER_tree, this);
        }
    }
    binary_tree::node const* binary_tree::node::left() const
    {
        this->ERROR_invalid();
        return this->MEMBER_left;
    }
    void binary_tree::node::clear_tree(node* const root)
    {
        if(not root)
        {
            return;
        }
        root->ERROR_invalid();
        auto node_list = binary_tree::behind_traversal(root);
        for (std::size_t count = 0; count < node_list.list().size(); count++)
        {
            delete (node_list.list())[count].MEMBER_node;
        }
    }
    void binary_tree::node::left_clear()
    {
        this->ERROR_invalid();
        node::clear_tree(this->MEMBER_left);
    }
    binary_tree::node* binary_tree::node::left()
    {
        this->ERROR_invalid();
        return this->MEMBER_left;
    }
    binary_tree::node* binary_tree::node::parent() 
    {
        this->ERROR_invalid();
        return this->MEMBER_parent;
    }
    void binary_tree::node::right_birth(PTR_data&& data)
    {
        this->ERROR_invalid();
        if (this->MEMBER_right)
        {
            throw std::logic_error("�˽ڵ��Ѿ����Һ��ӣ���ֹ�ظ���ʼ����");
        }
        else
        {
            // new�½ڵ㣬����˽ڵ���Ϊ����
            this->MEMBER_right = new node(this->MEMBER_tree, std::move(data), this);
        }
    }

    void binary_tree::node::right_birth()
    {
        this->ERROR_invalid();
        if (this->MEMBER_right)
        {
            throw std::logic_error("�˽ڵ��Ѿ����Һ��ӣ���ֹ�ظ���ʼ����");
        }
        else
        {
            // new�½ڵ㣬����˽ڵ���Ϊ����
            this->MEMBER_right = new node(this->MEMBER_tree, this);
        }
    }
    binary_tree::node const* binary_tree::node::right() const
    {
        this->ERROR_invalid();
        return this->MEMBER_right;
    }
    binary_tree::node* binary_tree::node::right()
    {
        this->ERROR_invalid();
        return this->MEMBER_right;
    }
    void binary_tree::node::right_clear()
    {
        this->ERROR_invalid();
        node::clear_tree(this->MEMBER_right);
    }
    bool binary_tree::node::is_leaf() const
    {
        this->ERROR_invalid();
        if ((not this->MEMBER_left) and (not this->MEMBER_right))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    std::size_t binary_tree::node::depth() const
    {
        this->ERROR_invalid();
        std::size_t result = 1;
        auto temp = this->MEMBER_parent;
        while (temp)
        {
            result++;
            temp = temp->MEMBER_parent;
        }
        return result;
    }
    binary_tree::node const* binary_tree::node::root() const
    {
        this->ERROR_invalid();
        return this->MEMBER_tree->MEMBER_root;
    }
    binary_tree::node * binary_tree::node::root()
    {
        this->ERROR_invalid();
        return this->MEMBER_tree->MEMBER_root;
    }
    bool binary_tree::node::is_ancestor(node const* const n)const
    {
        this->ERROR_invalid();
        if (not n)
        {
            return false;
        }
        n->ERROR_invalid();
        if (n->MEMBER_tree != this->MEMBER_tree)
            //����ͬһ�����ϣ���Ȼ�������ȹ�ϵ
        {
            return false;
        }
        auto TEMP_itor = this;//������

        if (TEMP_itor == n)
        {
            return false;
        }
        //node�ͱ��ڵ���ͬһ�����ϣ�������ͬһ���ڵ�
        while (true)
        {
            auto const TEMP_parent = TEMP_itor->MEMBER_parent;
            if (TEMP_parent)
            {
                if (TEMP_parent == n)
                {
                    return true;
                }
                else
                {
                    TEMP_itor = TEMP_parent;
                }
            }
            else
                //��ǰ�������Ǹ��ڵ�
            {
                return false;
            }
        }
    }
    void binary_tree::node::swap_child() 
    {
        this->ERROR_invalid();
        auto const temp = this->MEMBER_left;
        this->MEMBER_left = this->MEMBER_right;
        this->MEMBER_right = temp;
    }
    binary_tree binary_tree::node::split_tree()
    {
        this->ERROR_invalid();
        if (not this->MEMBER_parent)
        {
            throw std::logic_error("�˽ڵ��Ѿ��Ǹ��ڵ㣬�޷���ֳ�����");
        }
        else
        {
            // new�����������ڵ��Ϊ�˽ڵ�
            auto result = binary_tree();
            result.MEMBER_root = this;

            // �޸Ĵ˽ڵ�ԭ���׵Ĺ�ϵ
            if (this->MEMBER_parent->MEMBER_left == this)
            {
                this->MEMBER_parent->MEMBER_left = nullptr;
            }
            else
            {
                this->MEMBER_parent->MEMBER_right = nullptr;
            }
            this->MEMBER_parent = nullptr;

            result.INNER_change_owner_tree(&result);
            return result;
        }
    }
    void binary_tree::node::combine_left(binary_tree&& subtree)
    {
        this->ERROR_invalid();
        if (this->MEMBER_left)
        {
            throw std::logic_error("�˽ڵ��Ѿ������ӣ��޷��ϲ�����");
        }
        else
        {
            subtree.INNER_change_owner_tree(this->MEMBER_tree);
            // �ϲ����ṹ
            this->MEMBER_left = subtree.MEMBER_root;
            subtree.MEMBER_root = nullptr;
        }
    }
    void binary_tree::node::swap_tree(node* const left, node* const right)
    {
        if ((not left) or (not right))
        {
            throw std::invalid_argument("��ֹ�����ָ��");
            return;
        }
        left->ERROR_invalid();
        right->ERROR_invalid();
        if (left == right)
            //����������ڵ���ͬ
        {
            return;
        }
        auto TEMP_node1_parent = left->MEMBER_parent;//node1ԭ���ĸ���
        auto TEMP_node2_parent = right->MEMBER_parent;//node2ԭ���ĸ���

        auto TEMP_node1_tree = left->MEMBER_tree;//node1ԭ������
        auto TEMP_node2_tree = right->MEMBER_tree;//node2ԭ������
        if ((not TEMP_node1_parent) and (not TEMP_node2_parent))
            /*
            �������2�����ڵ�
            �������ǲ���ͬһ���ڵ㣬�ʲ���ͬһ����
            */
        {
            TEMP_node1_tree->INNER_change_owner_tree(TEMP_node2_tree);//node1ԭ�����µ����нڵ��Ϊnode2ԭ����������
            TEMP_node2_tree->INNER_change_owner_tree(TEMP_node1_tree);
            TEMP_node1_tree->MEMBER_root = right;
            TEMP_node2_tree->MEMBER_root = left;
            return;
        }
        if (left->is_ancestor(right)/*node2��node1������*/ or right->is_ancestor(left)/*node1��node2������*/)
        {
            throw std::invalid_argument("��ֹ����Ľڵ�֮��������ȹ�ϵ");
            return;
        }
        if (TEMP_node1_tree != TEMP_node2_tree)
            //node1��node2����ͬһ������
        {
            binary_tree::INNER_change_owner_tree(left, TEMP_node2_tree);
            binary_tree::INNER_change_owner_tree(right, TEMP_node1_tree);
        }

        right->MEMBER_parent = TEMP_node1_parent;
        left->MEMBER_parent = TEMP_node2_parent;
        if (TEMP_node2_parent)
        {
            if (TEMP_node2_parent->MEMBER_left == right)
                //node2������
            {
                TEMP_node2_parent->MEMBER_left = left;
            }
            else
                //node2���Һ���
            {
                TEMP_node2_parent->MEMBER_right = left;
            }

        }
        else
            //node2�Ǹ��ڵ�
        {
            TEMP_node2_tree->MEMBER_root = left;
        }
        if (TEMP_node1_parent)
        {
            if (TEMP_node1_parent->MEMBER_left == left)
                //node1������
            {
                TEMP_node1_parent->MEMBER_left = right;
            }
            else
                //node1���Һ���
            {
                TEMP_node1_parent->MEMBER_right = right;
            }

        }
        else
            //node1�Ǹ��ڵ�
        {
            TEMP_node1_tree->MEMBER_root = right;
        }

    }
    void binary_tree::node::combine_right(binary_tree&& subtree)
    {
        this->ERROR_invalid();
        if (this->MEMBER_right)
        {
            throw std::logic_error("�˽ڵ��Ѿ����Һ��ӣ��޷��ϲ�����");
        }
        else
        {
            subtree.INNER_change_owner_tree(this->MEMBER_tree);
            // �ϲ����ṹ
            this->MEMBER_right = subtree.MEMBER_root;
            subtree.MEMBER_root = nullptr;
        }
    }
    void binary_tree::node::swap_data(node* const right)
    {
        this->ERROR_invalid();
        if (not right)
        {
            throw std::invalid_argument("��ֹ�����ָ��");
        }
        right->ERROR_invalid();
        this->MEMBER_data.swap(right->MEMBER_data);
    }
    std::shared_ptr<binary_tree::marker_type const> const& binary_tree::node::marker()const noexcept
    {
        return this->MEMBER_marker;
    }
    binary_tree::binary_tree()noexcept
        :MEMBER_root(nullptr)
    {
    }
    binary_tree::binary_tree(PTR_data&& data)
        : MEMBER_root(new node(this, std::move(data)))
    {
    }
    binary_tree::binary_tree(binary_tree&& origin)
        :MEMBER_root(origin.MEMBER_root)
    {
        origin.MEMBER_root = nullptr;
        this->INNER_change_owner_tree(this);
    }
    binary_tree::~binary_tree()noexcept
    {
        node::clear_tree(this->MEMBER_root);
    }
    void binary_tree::root_birth(PTR_data&& data)
    {
        if (this->MEMBER_root)
        {
            throw std::logic_error("���ڵ㲻�գ���ֹ�ظ���ʼ����");
        }
        else
        {
            this->MEMBER_root = new node(this, std::move(data));
        }
    }
    void binary_tree::root_birth()
    {
        if (this->MEMBER_root)
        {
            throw std::logic_error("���ڵ㲻�գ���ֹ�ظ���ʼ����");
        }
        else
        {
            this->MEMBER_root = new node(this);
        }
    }
    void binary_tree::clear() noexcept
    {
        node::clear_tree(this->MEMBER_root);
        this->MEMBER_root = nullptr;
    }
    binary_tree::marker_type::marker_type()noexcept
    {}
    binary_tree::marker_type::~marker_type()noexcept
    {}
    binary_tree::iterator::iterator(node* const origin )
        noexcept(noexcept(std::weak_ptr<marker_type const>(std::declval<std::shared_ptr<marker_type const> const&>())))
        :MEMBER_node(origin)
    {
        if(origin)
        {
            this->MEMBER_marker = origin->marker();
        }
    }
    binary_tree::iterator::~iterator()
        noexcept(std::is_nothrow_destructible<std::weak_ptr<marker_type const>>::value)
    {
    }
    binary_tree::iterator::iterator(iterator const& origin)
        noexcept(std::is_nothrow_copy_constructible<std::weak_ptr<marker_type const>>::value)
        :MEMBER_node(origin.MEMBER_node)
        ,MEMBER_marker(origin.MEMBER_marker)
    {
    }
    void binary_tree::iterator::ERROR_invalid()const noexcept(false)
    {
        if (not (*this))
        {
            throw std::logic_error("�˵��������õĽڵ���ʧЧ");
        }
    }
    void binary_tree::iterator::operator=(iterator const& origin)
        noexcept(std::is_nothrow_copy_assignable<std::weak_ptr<marker_type const>>::value)
    {
        if (this == &origin)
        {
            return;
        }
        this->MEMBER_node = origin.MEMBER_node;
        this->MEMBER_marker = origin.MEMBER_marker;
    }
    binary_tree::iterator::iterator(iterator&& origin)
        noexcept(std::is_nothrow_move_constructible<std::weak_ptr<marker_type const>>::value)
        :MEMBER_node(origin.MEMBER_node)
        ,MEMBER_marker(std::move(origin.MEMBER_marker))
    {
        origin.MEMBER_node = nullptr;
    }
    void binary_tree::iterator::operator=(iterator&& origin)
        noexcept(std::is_nothrow_move_assignable<std::weak_ptr<marker_type const>>::value)
    {
        if (this == &origin)
        {
            return;
        }
        this->MEMBER_node = origin.MEMBER_node;
        origin.MEMBER_node = nullptr;
    }
    bool binary_tree::iterator::operator==(iterator const& right)const
    {
        this->ERROR_invalid();
        right.ERROR_invalid();
        if (this->MEMBER_node == right.MEMBER_node)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool binary_tree::iterator::operator!=(iterator const& right)const
    {
        return not((*this) == right);
    }
    binary_tree::data_type* binary_tree::iterator::data()
    {
        this->ERROR_invalid();
        return this->MEMBER_node->data();
    }
    binary_tree::data_type const* binary_tree::iterator::data()const
    {
        this->ERROR_invalid();
        return this->MEMBER_node->data();
    }
    binary_tree::iterator binary_tree::iterator::left()
    {
        this->ERROR_invalid();
        return iterator(this->MEMBER_node->left());
    }
    std::unique_ptr<binary_tree::iterator const> binary_tree::iterator::left()const
    {
        this->ERROR_invalid();
        auto result = iterator(this->MEMBER_node->left());
        return std::make_unique<iterator const>(std::move(result));
    }
    binary_tree::iterator binary_tree::iterator::right()
    {
        this->ERROR_invalid();
        return iterator(this->MEMBER_node->right());
    }
    std::unique_ptr<binary_tree::iterator const> binary_tree::iterator::right()const
    {
        this->ERROR_invalid();
        auto result = iterator(this->MEMBER_node->right());
        return std::make_unique<iterator const>(std::move(result));
    }
    binary_tree::iterator binary_tree::iterator::parent()
    {
        this->ERROR_invalid();
        return iterator(this->MEMBER_node->parent());
    }
    std::unique_ptr<binary_tree::iterator const> binary_tree::iterator::parent()const
    {
        this->ERROR_invalid();
        auto result = iterator(this->MEMBER_node->parent());
        return std::make_unique<iterator const>(std::move(result));
    }
    binary_tree::iterator binary_tree::iterator::root()
    {
        this->ERROR_invalid();
        return iterator(this->MEMBER_node->root());
    }
    std::unique_ptr<binary_tree::iterator const> binary_tree::iterator::root()const
    {
        this->ERROR_invalid();
        auto result = iterator(this->MEMBER_node->root());
        return std::make_unique<iterator const>(std::move(result));
    }
    void binary_tree::iterator::right_birth()
    {
        this->ERROR_invalid();
        this->MEMBER_node->right_birth();
    }
    void binary_tree::iterator::right_birth(PTR_data&& data)
    {
        this->ERROR_invalid();
        this->MEMBER_node->right_birth(std::move(data));
    }
    void binary_tree::iterator::right_clear()
    {
        this->ERROR_invalid();
        this->MEMBER_node->right_clear();
    }
    void binary_tree::iterator::left_birth()
    {
        this->ERROR_invalid();
        this->MEMBER_node->left_birth();
    }
    void binary_tree::iterator::left_birth(PTR_data&& data)
    {
        this->ERROR_invalid();
        this->MEMBER_node->left_birth(std::move(data));
    }
    void binary_tree::iterator::left_clear()
    {
        this->ERROR_invalid();
        this->MEMBER_node->left_clear();
    }
    bool binary_tree::iterator::is_leaf()const
    {
        this->ERROR_invalid();
        return this->MEMBER_node->is_leaf();
    }
    std::size_t binary_tree::iterator::depth()const
    {
        this->ERROR_invalid();
        return this->MEMBER_node->depth();
    }
    bool binary_tree::iterator::is_ancestor(iterator const& n)const
    {
        this->ERROR_invalid();
        n.ERROR_invalid();
        return this->MEMBER_node->is_ancestor(n.MEMBER_node);
    }
    void binary_tree::iterator::swap_child()
    {
        this->ERROR_invalid();
        this->MEMBER_node->swap_child();
    }
    binary_tree binary_tree::iterator::split_tree()
    {
        this->ERROR_invalid();
        return this->MEMBER_node->split_tree();
    }
    void binary_tree::iterator::combine_left(binary_tree&& subtree)
    {
        this->ERROR_invalid();
        this->MEMBER_node->combine_left(std::move(subtree));
    }
    void binary_tree::iterator::combine_right(binary_tree&& subtree)
    {
        this->ERROR_invalid();
        this->MEMBER_node->combine_right(std::move(subtree));
    }
    binary_tree::iterator::operator bool()const 
        noexcept(noexcept(std::declval<iterator const*>()->MEMBER_marker.expired()))
    {
        if (not this->MEMBER_node or this->MEMBER_marker.expired())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    void binary_tree::iterator::swap_data(iterator& right)
    {
        this->ERROR_invalid();
        right.ERROR_invalid();
        this->MEMBER_node->swap_data(right.MEMBER_node);
    }
    void binary_tree::iterator::swap_tree(iterator& left, iterator& right)
    {
        left.ERROR_invalid();
        right.ERROR_invalid();
        node::swap_tree(left.MEMBER_node, right.MEMBER_node);
    }
    binary_tree::tree_information::tree_information(const std::size_t node, const std::size_t leaf, const std::size_t depth, TYPE_list&& list)
        noexcept(std::is_nothrow_move_constructible<TYPE_list>::value)
        : MEMBER_node(node), MEMBER_leaf(leaf), MEMBER_depth(depth), MEMBER_list(std::move(list))
    {
    }
    binary_tree::tree_information::tree_information(const tree_information& origin)
        noexcept(std::is_nothrow_copy_constructible<TYPE_list>::value)
        :MEMBER_node(origin.MEMBER_node),
        MEMBER_leaf(origin.MEMBER_leaf),
        MEMBER_depth(origin.MEMBER_depth),
        MEMBER_list(origin.MEMBER_list)
    {
    }
    void binary_tree::tree_information::operator=(const tree_information& origin)
        noexcept(std::is_nothrow_copy_assignable<TYPE_list>::value)
    {
        if (this == &origin)
        {
            return;
        }
        this->MEMBER_node = origin.MEMBER_node;
        this->MEMBER_leaf = origin.MEMBER_leaf;
        this->MEMBER_depth = origin.MEMBER_depth;
        this->MEMBER_list = origin.MEMBER_list;
    }

    binary_tree::tree_information::tree_information(tree_information&& origin)
        noexcept(std::is_nothrow_move_constructible<TYPE_list>::value)
        : MEMBER_node(origin.MEMBER_node),
        MEMBER_leaf(origin.MEMBER_leaf),
        MEMBER_depth(origin.MEMBER_depth),
        MEMBER_list(std::move(origin.MEMBER_list))
    {

    }

    void binary_tree::tree_information::operator=(tree_information&& origin)
        noexcept(std::is_nothrow_move_assignable<TYPE_list>::value)
    {
        if (this == &origin)
        {
            return;
        }
        this->MEMBER_node = origin.MEMBER_node;
        origin.MEMBER_node = 0;
        this->MEMBER_leaf = origin.MEMBER_leaf;
        origin.MEMBER_leaf = 0;
        this->MEMBER_depth = origin.MEMBER_depth;
        origin.MEMBER_depth = 0;
        this->MEMBER_list = std::move(origin.MEMBER_list);
    }

    binary_tree::tree_information::~tree_information()
        noexcept(std::is_nothrow_destructible<TYPE_list>::value)
    {
    }
    std::size_t binary_tree::tree_information::node()const noexcept
    {
        return this->MEMBER_node;
    }
    std::size_t binary_tree::tree_information::leaf()const noexcept
    {
        return this->MEMBER_leaf;
    }
    std::size_t binary_tree::tree_information::depth()const noexcept
    {
        return this->MEMBER_depth;
    }
    binary_tree::tree_information::TYPE_list& binary_tree::tree_information::list()noexcept
    {
        return this->MEMBER_list;
    }
    binary_tree::tree_information::TYPE_list const& binary_tree::tree_information::list()const noexcept
    {
        return this->MEMBER_list;
    }
    binary_tree::tree_information binary_tree::front_traversal(node* const root)
    {

        auto result = typename tree_information::TYPE_list();
        if (not root)
            // ���������ؿ�����
        {
            return tree_information(0, 0, 0, std::move(result));
        }

        // ������

        // �������������±�
        std::size_t count = 0;
        std::size_t leaf = 0;
        std::size_t depth = 1;
        // ������
        auto _root = root;

        // ����ջ
        std::stack<node*> assistant;
        while (true)
        {
            while (_root)
                // ����������
            {
                
                // ������ָ��Ľڵ�д����
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
                    // ���������Һ���
                {
                    // �Һ�����ջ
                    assistant.push(_root->right());
                }
                // ��������Ϊ�Լ�������
                _root = _root->left();
            }
            if (assistant.empty())
                // ����ջ��
            {
                break;
            }
            // ����ջ������ջ����Ϊ��һ��ѭ�������

            _root = assistant.top();
            assistant.pop();
        }
        return tree_information(count, leaf, depth, std::move(result));

    }

    std::unique_ptr<binary_tree::tree_information const> binary_tree::middle_traversal()const
    {
        return std::make_unique<tree_information const>(binary_tree::middle_traversal(this->MEMBER_root));
    }
    std::unique_ptr<binary_tree::tree_information const> binary_tree::front_traversal()const
    {
        return std::make_unique<tree_information const>(binary_tree::front_traversal(this->MEMBER_root));
    }
    binary_tree::tree_information binary_tree::front_traversal()
    {
        return binary_tree::front_traversal(this->MEMBER_root);
    }
    binary_tree::tree_information binary_tree::middle_traversal(node* const root)
    {

        auto result = typename tree_information::TYPE_list();

        if (not root)
            // ���������ؿ�����
        {
            return tree_information(0, 0, 0, std::move(result));
        }

        // ������
        // �������������±�
        std::size_t count = 0;
        std::size_t leaf = 0;
        std::size_t depth = 0;
        // ������
        auto _root = root;
        // ����ջ
        std::stack<node*> assistant;
        while (true)
        {
            if (_root)
            {
                // ��������ջ
                assistant.push(_root);
                // ����������
                _root = _root->left();
            }
            else if (not assistant.empty())
            {
                // ��δ���ʵ���������Ƚڵ��ջ
                _root = assistant.top();
                assistant.pop();
                // ���ʸýڵ�
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
                // ������������
                _root = _root->right();
            }
            else
                // �������
            {
                break;
            }
        }
        return tree_information(count, leaf, depth, std::move(result));
    }
    binary_tree::tree_information binary_tree::middle_traversal()
    {
        return binary_tree::middle_traversal(this->MEMBER_root);
    }

    binary_tree::tree_information binary_tree::behind_traversal(node* const root)
    {

        auto result = typename tree_information::TYPE_list();

        if (not root)
            // ���������ؿ�����
        {
            return tree_information(0, 0, 0, std::move(result));
        }

        // ������
        // �������������±�
        std::size_t count = 0;
        std::size_t leaf = 0;
        std::size_t depth = 0;
        // ������
        auto _root = root;
        // ����ջ
        std::stack<node*> assistant;

        assistant.push(_root);

        while (not assistant.empty())
        {
            if (assistant.top() != _root->parent())
            {

                while (auto temp = assistant.top()) // �Զ����£�������鵱ǰ�ڵ㣨��ջ����
                {
                    if (temp->left())
                    { // �������������
                        if (temp->right())
                        {
                            assistant.push(temp->right());
                        }                                   // �����Һ��ӣ�������ջ
                        assistant.push(temp->left()); // Ȼ���ת������
                    }
                    else // �޷����������
                    {
                        assistant.push(temp->right());
                    }
                }
                assistant.pop(); // ����֮ǰ������ջ���Ŀսڵ�
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

    binary_tree::tree_information binary_tree::behind_traversal()
    {
        return binary_tree::behind_traversal(this->MEMBER_root);
    }
    std::unique_ptr<binary_tree::tree_information const> binary_tree::behind_traversal()const
    {
        return std::make_unique<tree_information const>(binary_tree::behind_traversal(this->MEMBER_root));
    }
    binary_tree::tree_information binary_tree::level_traversal(node* const root)
    {

        auto result = typename tree_information::TYPE_list();

        if (not root)
            // ���������ؿ�����
        {
            return tree_information(0, 0, 0, std::move(result));
        }

        // ������
        // �������������±�
        std::size_t count = 0;
        std::size_t leaf = 0;
        std::size_t depth = 0;
        // ��������
        std::queue<node*> assistant;
        // ���ڵ����
        assistant.push(root);
        while (not assistant.empty())
        {
            // ȡ����ͷ�ڵ㣬����
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
                // �������
                assistant.push(temp->left());
            }
            if (temp->right())
            {
                // �Һ������
                assistant.push(temp->right());
            }
        }
        return tree_information(count, leaf, depth, std::move(result));
    }
    binary_tree::tree_information binary_tree::level_traversal()
    {
        return binary_tree::level_traversal(this->MEMBER_root);
    }
    std::unique_ptr<binary_tree::tree_information const> binary_tree::level_traversal()const
    {
        return std::make_unique<tree_information const>(binary_tree::level_traversal(this->MEMBER_root));
    }
    void binary_tree::INNER_change_owner_tree(binary_tree* const owner)
    {
        auto temp = this->behind_traversal();
        for (std::size_t counter = 0; counter < temp.MEMBER_list.size(); counter++)
        {
            temp.MEMBER_list[counter]->change_tree(owner);
        }
    }
    bool binary_tree::data_type::operator ==(data_type const& right)const noexcept
    {
        return this == &right;
    }
    bool binary_tree::data_type::operator !=(data_type const& right)const noexcept
    {
        return not(*this == right);
    }
    void binary_tree::INNER_change_owner_tree(node* const node, binary_tree* const owner)
    {
        auto temp = binary_tree::behind_traversal(node);
        for (std::size_t counter = 0; counter < temp.MEMBER_list.size(); counter++)
        {
            temp.MEMBER_list[counter]->change_tree(owner);
        }
    }
    void binary_tree::node::change_tree(binary_tree* const t)
    {
        if (not t)
        {
            throw std::invalid_argument("��ֹ�����ָ��");
        }
        this->MEMBER_tree = t;
    }
    binary_tree::node* binary_tree::iterator::operator ->()
    {
        this->ERROR_invalid();
        return this->MEMBER_node;
    }
    binary_tree::node const* binary_tree::iterator::operator ->()const
    {
        this->ERROR_invalid();
        return this->MEMBER_node;
    }
    binary_tree* binary_tree::node::tree()noexcept
    {
        return this->MEMBER_tree;
    }
    binary_tree const* binary_tree::node::tree()const noexcept
    {
        return this->MEMBER_tree;
    }
    binary_tree::iterator binary_tree::find(data_type const& data)
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
    std::unique_ptr<binary_tree::iterator const> binary_tree::find(data_type const& data)const
    {
        auto temp = this->behind_traversal();
        for (std::size_t count = 0; count < temp->MEMBER_list.size(); count++)
        {
            auto finder = temp->MEMBER_list[count]->data();
            if (finder and ((*finder) == data))
            {
                auto result = iterator(temp->MEMBER_list[count]);
                return std::make_unique<iterator const>(std::move(result));
            }
        }
        auto result = iterator();
        return std::make_unique<iterator const>(std::move(result));
    }
    binary_tree::iterator binary_tree::root()noexcept
    {
        return iterator(this->MEMBER_root);
    }

    std::unique_ptr<binary_tree::iterator const> binary_tree::root()const noexcept
    {
        auto result= iterator(this->MEMBER_root);
        return std::make_unique<iterator const>(std::move(result));
    }

    binary_tree::tree_information binary_tree::level_traversal(iterator& root)
    {
        root.ERROR_invalid();
        return binary_tree::level_traversal(root.MEMBER_node);
    }
    std::unique_ptr<binary_tree::tree_information const> binary_tree::level_traversal(iterator const& root)
    {
        root.ERROR_invalid();
        return std::make_unique<tree_information const>(binary_tree::level_traversal(root.MEMBER_node));
    }
    binary_tree::tree_information binary_tree::behind_traversal(iterator& root)
    {
        root.ERROR_invalid();
        return binary_tree::behind_traversal(root.MEMBER_node);
    }
    std::unique_ptr<binary_tree::tree_information const> binary_tree::behind_traversal(iterator const& root)
    {
        root.ERROR_invalid();
        return std::make_unique<tree_information const>(binary_tree::behind_traversal(root.MEMBER_node));
    }
    std::unique_ptr<binary_tree::tree_information const> binary_tree::middle_traversal(iterator const& root)
    {
        root.ERROR_invalid();
        return std::make_unique<tree_information const>(binary_tree::middle_traversal(root.MEMBER_node));
    }
    binary_tree::tree_information binary_tree::middle_traversal(iterator& root)
    {
        root.ERROR_invalid();
        return binary_tree::middle_traversal(root.MEMBER_node);
    }
    std::unique_ptr<binary_tree::tree_information const> binary_tree::front_traversal(iterator const& root)
    {
        root.ERROR_invalid();
        return std::make_unique<tree_information const>(binary_tree::front_traversal(root.MEMBER_node));
    }
    binary_tree::tree_information binary_tree::front_traversal(iterator& root)
    {
        root.ERROR_invalid();
        return binary_tree::front_traversal(root.MEMBER_node);
    }
}
