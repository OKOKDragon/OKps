#include <stdexcept>
#include ".\graph.hpp"

namespace OKps
{
    graph::data_type::data_type()noexcept
    {
    }
    graph::data_type::data_type(data_type const &)noexcept
    {
    }
    graph::data_type::data_type(data_type &&)noexcept
    {
    }
    graph::data_type::~data_type()noexcept
    {
    }
    void graph::data_type::operator =(data_type const &)noexcept
    {
    }
    void graph::data_type::operator =(data_type &&)noexcept
    {
    }
    graph::data_type & graph::data_type::self()noexcept
    {
        return *this;
    }
    graph::data_type const & graph::data_type::self()const noexcept
    {
        return *this;
    }
    bool graph::data_type::operator ==(data_type const & right)const noexcept
    {
        return this == (&right);
    }
    bool graph::data_type::operator !=(data_type const & right)const noexcept
    {
        return this == (&right);
    }
    graph::node::node(graph * master, std::unique_ptr<data_type> && data)
        noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<data_type>>
        and std::is_nothrow_default_constructible_v<std::set<node *>>
        and noexcept(std::make_shared<marker_type>()))
        :MEMBER_data(std::move(data))
        , MEMBER_marker(std::make_shared<marker_type>())
        , MEMBER_owner(master)
    {
    }

    graph::node::~node()
        noexcept(std::is_nothrow_destructible_v<std::unique_ptr<data_type>>
        and std::is_nothrow_destructible_v<std::set<node *>>
        and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>)
    {
    }

    std::unique_ptr<graph::data_type> const & graph::node::data()noexcept
    {
        return this->MEMBER_data;
    }
    graph::data_type const * graph::node::data()const noexcept
    {
        return this->MEMBER_data.get();
    }
    std::set<graph::node *> const & graph::node::next()const noexcept
    {
        return this->MEMBER_next;
    }
    std::set<graph::node *> & graph::node::next()noexcept
    {
        return this->MEMBER_next;
    }
    graph::graph()
        noexcept(std::is_nothrow_default_constructible_v<std::set<node *>>)
    {
    }
    graph::~graph()
        noexcept(std::is_nothrow_destructible_v<std::set<node *>>)
    {
        for (auto f = this->MEMBER_graph.begin();f != this->MEMBER_graph.end();f++)
        {
            delete (*f);
        }
    }
    void graph::node::change_owner(graph * const master)noexcept
    {
        this->MEMBER_owner = master;
    }
    graph const * graph::node::owner()const noexcept
    {
        return this->MEMBER_owner;
    }
    graph::graph(graph && origin)
        noexcept(std::is_nothrow_move_constructible_v<std::set<node *>>)
        :MEMBER_graph(std::move(origin.MEMBER_graph))
    {
        for (auto finder = this->MEMBER_graph.begin();finder != this->MEMBER_graph.end();finder++)
        {
            (*finder)->change_owner(this);
        }
    }
    void graph::operator =(graph && origin)
        noexcept(std::is_nothrow_move_assignable_v<std::set<node *>>)
    {
        this->MEMBER_graph = std::move(origin.MEMBER_graph);
        for (auto finder = this->MEMBER_graph.begin();finder != this->MEMBER_graph.end();finder++)
        {
            (*finder)->change_owner(this);
        }
    }
    std::shared_ptr<graph::marker_type> const & graph::node::marker()const noexcept
    {
        return this->MEMBER_marker;
    }
    graph::marker_type::marker_type()noexcept
    {
    }

    graph::marker_type::~marker_type()noexcept
    {
    }
    graph * graph::node::owner()noexcept
    {
        return this->MEMBER_owner;
    }
    graph::iterator::iterator(node * n)
        noexcept(noexcept(std::weak_ptr<marker_type>(std::declval<std::shared_ptr<marker_type> const &>())))
        :MEMBER_node(n)
        , MEMBER_marker()
    {
        if (this->MEMBER_node)
        {
            this->MEMBER_marker = this->MEMBER_node->marker();
        }
    }

    graph::iterator::iterator(iterator const & origin)
        noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>)
        : MEMBER_node(origin.MEMBER_node)
        , MEMBER_marker(origin.MEMBER_marker)
    {
    }
    void graph::iterator::operator =(iterator const & origin)
        noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>)
    {
        this->MEMBER_node = origin.MEMBER_node;
        this->MEMBER_marker = origin.MEMBER_marker;
    }
    bool graph::iterator::operator ==(iterator const & right)const noexcept
    {
        if (this->MEMBER_marker.expired() and right.MEMBER_marker.expired())
        {
            return true;
        }
        else if (this->MEMBER_marker.expired() or right.MEMBER_marker.expired())
        {
            return false;
        }
        return this->MEMBER_node == right.MEMBER_node;
    }
    bool graph::iterator::operator !=(iterator const & right)const noexcept
    {
        return not((*this) == right);
    }
    graph::iterator::~iterator()
        noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>)
    {
    }
    void graph::iterator::operator ++()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }

        auto finder = this->MEMBER_node->owner()->MEMBER_graph.begin();

        while (finder != this->MEMBER_node->owner()->MEMBER_graph.end())
        {
            if (this->MEMBER_node == (*finder))
            {
                ++finder;
                if (finder != this->MEMBER_node->owner()->MEMBER_graph.end())
                {
                    this->MEMBER_node = (*finder);
                    this->MEMBER_marker = this->MEMBER_node->marker();
                }
                else
                {
                    this->MEMBER_node = nullptr;
                    this->MEMBER_marker.reset();
                }
                break;
            }
            ++finder;
        }

    }

    graph::node * graph::iterator::operator * ()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node;
    }
    graph::iterator graph::add(std::unique_ptr<data_type> && data)
    {
        auto n = new node(this, std::move(data));
        this->MEMBER_graph.insert(n);
        return iterator(n);
    }

    void graph::erase(iterator const & i)
    {
        for (auto finder = this->MEMBER_graph.begin();finder != this->MEMBER_graph.end();finder++)
        {
            for (auto p = (*finder)->next().begin();p != (*finder)->next().end();p++)
            {
                if ((*p) == (*i))
                {
                    (*finder)->next().erase(p);
                    break;
                }
            }
        }
        auto f = this->MEMBER_graph.find(*i);
        if (f != this->MEMBER_graph.end())
        {
            this->MEMBER_graph.erase(f);
        }
    }

    void graph::iterator::add(iterator const & target)
    {
        if (this->MEMBER_marker.expired() or target.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (this->MEMBER_node->owner() != target.MEMBER_node->owner())
        {
            throw std::logic_error("两个节点不属于同一个图，无法操作");
        }
        if (this->MEMBER_node == target.MEMBER_node)
        {
            return;
        }
        auto t = this->MEMBER_node->next().find(target.MEMBER_node);
        if (t == this->MEMBER_node->next().end())
        {
            this->MEMBER_node->next().insert(target.MEMBER_node);
        }
    }
    /*
    删除从节点start指向节点target的指向关系
    */
    void graph::iterator::erase(iterator const & target)
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (target.MEMBER_marker.expired())
        {
            return;
        }
        if (this->MEMBER_node->owner() != target.MEMBER_node->owner())
        {
            return;
        }

        auto t = this->MEMBER_node->next().find(target.MEMBER_node);
        if (t != this->MEMBER_node->next().end())
        {
            this->MEMBER_node->next().erase(t);
        }
    }
    std::unique_ptr<graph::data_type> const & graph::iterator::data()
    {
        return this->MEMBER_node->data();
    }
    graph::data_type const * graph::iterator::data()const
    {
        return this->MEMBER_node->data().get();
    }
    bool graph::iterator::operator <(iterator const & right)const noexcept
    {
        return this < (&right);
    }
    graph * graph::iterator::owner()
    {
        return this->MEMBER_node->owner();
    }
    graph const * graph::iterator::owner()const
    {
        return this->MEMBER_node->owner();
    }
    std::set<graph::iterator> graph::iterator::next()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        std::set<iterator> result;
        for (auto f = this->MEMBER_node->next().begin();f != this->MEMBER_node->next().end();f++)
        {
            result.insert(static_cast<iterator const &>(iterator(*f)));
        }
        return result;
    }
    std::unique_ptr<graph::iterator const> graph::begin()const
    {
        auto b = this->MEMBER_graph.begin();
        iterator result = iterator(nullptr);
        if (b != this->MEMBER_graph.end())
        {
            result = static_cast<iterator const &>(iterator(*b));
        }
        return std::make_unique<iterator const>(result);
    }
    std::unique_ptr<graph::iterator const> graph::end()const
    {
        iterator result = iterator(nullptr);
        return std::make_unique<iterator const>(result);

    }
    graph::iterator graph::begin()
    {
        auto b = this->MEMBER_graph.begin();
        iterator result = iterator(nullptr);
        if (b != this->MEMBER_graph.end())
        {
            result = static_cast<iterator const &>(iterator(*b));
        }
        return static_cast<iterator const &>(result);
    }

    graph::iterator graph::end()
    {
        iterator const result = iterator(nullptr);
        return result;
    }

}
