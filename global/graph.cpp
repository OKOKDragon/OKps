#include <stdexcept>
#include <map>
#include <algorithm>
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
    graph::weight_type::weight_type()noexcept
    {
    }
    graph::weight_type::weight_type(weight_type const &)noexcept
    {
    }
    graph::weight_type::weight_type(weight_type &&)noexcept
    {
    }

    graph::weight_type::~weight_type()noexcept
    {
    }

    void graph::weight_type::operator =(weight_type const &)noexcept
    {
    }
    void graph::weight_type::operator =(weight_type &&)noexcept
    {
    }
    graph::weight_type & graph::weight_type::self()noexcept
    {
        return *this;
    }
    graph::weight_type const & graph::weight_type::self()const noexcept
    {
        return *this;
    }

    graph::node::node(graph * master, std::unique_ptr<data_type> && data)
        noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<data_type>>
        and std::is_nothrow_default_constructible_v<std::set<edge *>>
        and noexcept(std::make_shared<marker_type>()))
        :MEMBER_data(std::move(data))
        , MEMBER_marker(std::make_shared<marker_type>())
        , MEMBER_owner(master)
    {
    }

    graph::node::~node()
        noexcept(std::is_nothrow_destructible_v<std::unique_ptr<data_type>>
        and std::is_nothrow_destructible_v<std::set<edge *>>
        and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>)
    {
    }
    graph::edge::edge(graph * g, node * begin_node, node * end_node, std::unique_ptr<weight_type> && w)
        noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<weight_type>>
        and noexcept(std::make_shared<marker_type>()))
        :MEMBER_marker(std::make_shared<marker_type>())
        , MEMBER_weight(std::move(w))
        , MEMBER_begin(begin_node)
        , MEMBER_end(end_node)
        , MEMBER_owner(g)
    {
    }

    graph::edge::~edge()
        noexcept(std::is_nothrow_destructible_v<std::unique_ptr<weight_type>>
        and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>)
    {

    }

    std::unique_ptr<graph::weight_type> const & graph::edge::weight()noexcept
    {
        return this->MEMBER_weight;
    }
    graph::weight_type const * graph::edge::weight()const noexcept
    {
        return this->MEMBER_weight.get();
    }
    graph::node const * graph::edge::begin()const noexcept
    {
        return this->MEMBER_begin;
    }
    graph::node * graph::edge::begin()noexcept
    {
        return this->MEMBER_begin;
    }
    graph::node const * graph::edge::end()const noexcept
    {
        return this->MEMBER_end;
    }
    graph::node * graph::edge::end()noexcept
    {
        return this->MEMBER_end;
    }
    std::shared_ptr<graph::marker_type> const & graph::edge::marker()const noexcept
    {
        return this->MEMBER_marker;
    }
    void graph::edge::change_owner(graph * g)noexcept
    {
        this->MEMBER_owner = g;
    }
    graph * graph::edge::owner()noexcept
    {
        return this->MEMBER_owner;
    }
    graph const * graph::edge::owner()const noexcept
    {
        return this->MEMBER_owner;
    }
    std::unique_ptr<graph::data_type> const & graph::node::data()noexcept
    {
        return this->MEMBER_data;
    }
    graph::data_type const * graph::node::data()const noexcept
    {
        return this->MEMBER_data.get();
    }
    std::set<graph::edge *> const & graph::node::next()const noexcept
    {
        return this->MEMBER_next;
    }
    std::set<graph::edge *> & graph::node::next()noexcept
    {
        return this->MEMBER_next;
    }
    graph::graph()
        noexcept(std::is_nothrow_default_constructible_v<std::set<node *>>)
    {
    }
    graph::~graph()
        noexcept(std::is_nothrow_destructible_v<std::set<node *>>
                    and std::is_nothrow_destructible_v<std::set<edge *>>)
    {
        for (auto f = this->MEMBER_node.begin();f != this->MEMBER_node.end();f++)
        {
            delete (*f);
        }
        for (auto f = this->MEMBER_edge.begin();f != this->MEMBER_edge.end();f++)
        {
            delete (*f);
        }
    }
    void graph::node::change_owner(graph * master)noexcept
    {
        this->MEMBER_owner = master;
    }
    graph const * graph::node::owner()const noexcept
    {
        return this->MEMBER_owner;
    }
    graph::graph(graph && origin)
        noexcept(std::is_nothrow_move_constructible_v<std::set<node *>>
        and std::is_nothrow_move_constructible_v<std::set<edge *>>)
        :MEMBER_node(std::move(origin.MEMBER_node))
        , MEMBER_edge(std::move(origin.MEMBER_edge))
    {
        for (auto finder = this->MEMBER_node.begin();finder != this->MEMBER_node.end();finder++)
        {
            (*finder)->change_owner(this);
        }
        for (auto finder = this->MEMBER_edge.begin();finder != this->MEMBER_edge.end();finder++)
        {
            (*finder)->change_owner(this);
        }
    }
    void graph::operator =(graph && origin)
        noexcept(std::is_nothrow_move_assignable_v<std::set<node *>>
        and std::is_nothrow_move_assignable_v<std::set<edge *>>)
    {
        this->MEMBER_node = std::move(origin.MEMBER_node);
        for (auto finder = this->MEMBER_node.begin();finder != this->MEMBER_node.end();finder++)
        {
            (*finder)->change_owner(this);
        }
        this->MEMBER_edge = std::move(origin.MEMBER_edge);
        for (auto finder = this->MEMBER_edge.begin();finder != this->MEMBER_edge.end();finder++)
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
    std::set<graph::edge *> const & graph::node::previous()const noexcept
    {
        return this->MEMBER_previous;
    }
    std::set<graph::edge *> & graph::node::previous()noexcept
    {
        return this->MEMBER_previous;
    }
    graph::marker_type::~marker_type()noexcept
    {
    }
    graph * graph::node::owner()noexcept
    {
        return this->MEMBER_owner;
    }
    graph::mutable_node_iterator::mutable_node_iterator(node * n)
        noexcept(noexcept(std::declval<std::weak_ptr<marker_type>>() = std::declval<std::shared_ptr<marker_type> const &>())
        and std::is_nothrow_default_constructible_v<std::weak_ptr<marker_type>>)
        :MEMBER_node(n)
        , MEMBER_marker()
    {
        if (this->MEMBER_node)
        {
            this->MEMBER_marker = this->MEMBER_node->marker();
        }
    }
    graph::immutable_edge_iterator::immutable_edge_iterator(edge * n)
        noexcept(std::is_nothrow_default_constructible_v<std::weak_ptr<marker_type>>
            and noexcept(std::declval<std::weak_ptr<marker_type>>() = std::declval<std::shared_ptr<marker_type> const &>()))
        :MEMBER_edge(n)
        , MEMBER_marker()
    {
        if (this->MEMBER_edge)
        {
            this->MEMBER_marker = n->marker();
        }
    }

    graph::edge const * graph::immutable_edge_iterator::get_edge()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_edge;
    }
    graph::edge * graph::immutable_edge_iterator::get_edge()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_edge;
    }

    graph::immutable_edge_iterator::immutable_edge_iterator(immutable_edge_iterator const & origin)
        noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>)
        :MEMBER_edge(origin.MEMBER_edge)
        , MEMBER_marker(origin.MEMBER_marker)
    {
    }
    void graph::immutable_edge_iterator::operator =(immutable_edge_iterator const & origin)
        noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>)
    {
        this->MEMBER_edge = origin.MEMBER_edge;
        this->MEMBER_marker = origin.MEMBER_marker;
    }

    graph::immutable_edge_iterator::~immutable_edge_iterator()
        noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>)
    {
    }


    bool graph::immutable_edge_iterator::operator ==(mutable_edge_iterator const & right)const noexcept
    {
        return this->MEMBER_edge == right.MEMBER_edge;
    }
    bool graph::immutable_edge_iterator::operator !=(mutable_edge_iterator const & right)const noexcept
    {
        return this->MEMBER_edge != right.MEMBER_edge;
    }
    bool graph::immutable_edge_iterator::operator ==(immutable_edge_iterator const & right)const noexcept
    {
        return this->MEMBER_edge == right.MEMBER_edge;
    }
    bool graph::immutable_edge_iterator::operator !=(immutable_edge_iterator const & right)const noexcept
    {
        return this->MEMBER_edge != right.MEMBER_edge;
    }

    bool graph::immutable_edge_iterator::operator <(immutable_edge_iterator const & right)const noexcept
    {
        return this < (&right);
    }


    bool  graph::immutable_edge_iterator::is_valid()const noexcept
    {
        return (not this->MEMBER_marker.expired());
    }

    graph::weight_type const * graph::immutable_edge_iterator::weight()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_edge->weight().get();
    }

    graph const & graph::immutable_edge_iterator::owner()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return *(this->MEMBER_edge->owner());
    }

    graph::immutable_node_iterator graph::immutable_edge_iterator::begin()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return immutable_node_iterator(this->MEMBER_edge->begin());
    }

    graph::immutable_node_iterator graph::immutable_edge_iterator::end()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return immutable_node_iterator(this->MEMBER_edge->end());
    }
    graph::mutable_edge_iterator graph::add(mutable_node_iterator const & begin_node, mutable_node_iterator const & end_node, std::unique_ptr<weight_type> && w)
    {
        return static_cast<mutable_edge_iterator const &>(begin_node.add(end_node, std::move(w)));
    }

    void graph::erase(mutable_edge_iterator const & i)
    {
        if (i.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        i.MEMBER_edge->begin()->next().erase(i.MEMBER_edge);
        i.MEMBER_edge->end()->previous().erase(i.MEMBER_edge);
        auto t = i.MEMBER_edge;
        this->MEMBER_edge.erase(t);
        delete t;
    }
    graph::mutable_node_iterator::mutable_node_iterator(mutable_node_iterator const & origin)
        noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>)
        : MEMBER_node(origin.MEMBER_node)
        , MEMBER_marker(origin.MEMBER_marker)
    {
    }
    graph::node const * graph::mutable_node_iterator::get_node()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node;
    }
    bool graph::mutable_node_iterator::operator ==(immutable_node_iterator const & right)const noexcept
    {
        return this->MEMBER_node == right.MEMBER_node;
    }
    bool graph::mutable_node_iterator::operator !=(immutable_node_iterator const & right)const noexcept
    {
        return this->MEMBER_node != right.MEMBER_node;
    }
    graph::node * graph::mutable_node_iterator::get_node()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node;
    }
    void graph::mutable_node_iterator::operator =(mutable_node_iterator const & origin)
        noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>)
    {
        this->MEMBER_node = origin.MEMBER_node;
        this->MEMBER_marker = origin.MEMBER_marker;
    }
    bool graph::mutable_node_iterator::operator ==(mutable_node_iterator const & right)const noexcept
    {
        return this->MEMBER_node == right.MEMBER_node;
    }
    bool graph::mutable_node_iterator::operator !=(mutable_node_iterator const & right)const noexcept
    {
        return this->MEMBER_node != right.MEMBER_node;
    }
    graph::mutable_node_iterator::~mutable_node_iterator()
        noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>)
    {
    }
    graph::graph(graph const & origin)
    {
        /*
        临时保存 origin 中的节点与新建图容器 this 中的节点的对应关系
        first是 origin 中的节点，second是 this 中新建的节点
        */
        std::map<node *, node *> temp_map;
        for (auto i = origin.MEMBER_node.begin();i != origin.MEMBER_node.end();i++)
        {
            auto n = new node(this, (*i)->data()->self_copy());
            temp_map.insert(std::make_pair(*i, n));
            this->MEMBER_node.insert(n);
        }
        for (auto i = origin.MEMBER_edge.begin();i != origin.MEMBER_edge.end();i++)
        {
            auto b = temp_map.find((*i)->begin());
            auto e = temp_map.find((*i)->end());
            auto n = new edge(this, b->second, e->second, (*i)->weight()->self_copy());
            this->MEMBER_edge.insert(n);
        }
    }
    void graph::clear()
    {
        for (auto i = this->MEMBER_node.begin();i != this->MEMBER_node.end();i++)
        {
            auto n = (*i);
            delete n;
        }
        this->MEMBER_node.clear();
        for (auto i = this->MEMBER_edge.begin();i != this->MEMBER_edge.end();i++)
        {
            auto n = (*i);
            delete n;
        }
        this->MEMBER_edge.clear();
    }
    void graph::operator =(graph const & origin)
    {
        if (this == (&origin))
        {
            return;
        }
        this->clear();
        /*
        临时保存 origin 中的节点与新建图容器 this 中的节点的对应关系
        first是 origin 中的节点，second是 this 中新建的节点
        */
        std::map<node *, node *> temp_map;
        for (auto i = origin.MEMBER_node.begin();i != origin.MEMBER_node.end();i++)
        {
            auto n = new node(this, (*i)->data()->self_copy());
            temp_map.insert(std::make_pair(*i, n));
            this->MEMBER_node.insert(n);
        }
        for (auto i = origin.MEMBER_edge.begin();i != origin.MEMBER_edge.end();i++)
        {
            auto b = temp_map.find((*i)->begin());
            auto e = temp_map.find((*i)->end());
            auto n = new edge(this, b->second, e->second, (*i)->weight()->self_copy());
            this->MEMBER_edge.insert(n);
        }
    }
    void graph::mutable_node_iterator::operator --()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }

        auto finder = this->MEMBER_node->owner()->MEMBER_node.begin();

        while (finder != this->MEMBER_node->owner()->MEMBER_node.end())
        {
            if (this->MEMBER_node == (*finder))
            {
                if (finder == this->MEMBER_node->owner()->MEMBER_node.begin())
                {
                    this->MEMBER_node = nullptr;
                    this->MEMBER_marker.reset();
                }
                else
                {
                    --finder;
                    this->MEMBER_node = (*finder);
                    this->MEMBER_marker = this->MEMBER_node->marker();
                }
                break;
            }
            ++finder;
        }
    }
    bool graph::mutable_node_iterator::is_valid()const noexcept
    {
        return not this->MEMBER_marker.expired();
    }
    void graph::mutable_node_iterator::operator ++()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }

        auto finder = this->MEMBER_node->owner()->MEMBER_node.begin();

        while (finder != this->MEMBER_node->owner()->MEMBER_node.end())
        {
            if (this->MEMBER_node == (*finder))
            {
                ++finder;
                if (finder != this->MEMBER_node->owner()->MEMBER_node.end())
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


    graph::mutable_node_iterator graph::add(std::unique_ptr<data_type> && data)
    {
        auto n = new node(this, std::move(data));
        this->MEMBER_node.insert(n);
        return mutable_node_iterator(n);
    }

    void graph::erase(mutable_node_iterator const & i)
    {
        if (i.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        //图中所有以节点 i 为起点的边
        std::set<edge *> temp_former;
        //图中所有以节点 i 为终点的边
        std::set<edge *>temp_target;
        for (auto f = this->MEMBER_edge.begin();f != this->MEMBER_edge.end();f++)
        {
            if ((*f)->begin() == i.MEMBER_node)
            {
                temp_former.insert(*f);
            }
            else if ((*f)->end() == i.MEMBER_node)
            {
                temp_target.insert(*f);
            }
        }
        for (auto f = temp_former.begin();f != temp_former.end();f++)
        {
            (*f)->end()->previous().erase(*f);
            delete (*f);
            this->MEMBER_edge.erase(*f);
        }
        for (auto f = temp_target.begin();f != temp_target.end();f++)
        {
            (*f)->begin()->next().erase(*f);
            delete (*f);
            this->MEMBER_edge.erase(*f);
        }

        this->MEMBER_node.erase(i.MEMBER_node);
        delete i.MEMBER_node;
    }

    graph::mutable_edge_iterator graph::mutable_node_iterator::add(mutable_node_iterator const & target, std::unique_ptr<weight_type> && w)const
    {
        if (this->MEMBER_marker.expired() or target.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (this->MEMBER_node->owner() != target.MEMBER_node->owner())
        {
            return mutable_edge_iterator(nullptr);
        }
        if (this->MEMBER_node == target.MEMBER_node)
        {
            return mutable_edge_iterator(nullptr);
        }
        for (auto f = this->MEMBER_node->next().begin();f != this->MEMBER_node->next().end();f++)
        {
            if ((*f)->begin() == this->MEMBER_node and (*f)->end() == target.MEMBER_node)
            {
                (*f)->change_weight(std::move(w));
                return mutable_edge_iterator(*f);
            }
        }
        auto r = new edge(this->MEMBER_node->owner(), this->MEMBER_node, target.MEMBER_node, std::move(w));
        this->MEMBER_node->owner()->MEMBER_edge.insert(r);
        this->MEMBER_node->next().insert(r);
        target.MEMBER_node->previous().insert(r);
        return mutable_edge_iterator(r);
    }
    void graph::edge::change_weight(std::unique_ptr<weight_type> && w)
        noexcept(std::is_nothrow_move_assignable_v<std::unique_ptr<weight_type>>)
    {
        this->MEMBER_weight = std::move(w);
    }

    void graph::mutable_node_iterator::erase(mutable_node_iterator const & target)
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

        for (auto f = this->MEMBER_node->next().begin();f != this->MEMBER_node->next().end();f++)
        {
            if ((*f)->begin() == this->MEMBER_node and (*f)->end() == target.MEMBER_node)
            {
                this->MEMBER_node->next().erase(*f);
                target.MEMBER_node->previous().erase(*f);
                auto p = (*f);
                this->MEMBER_node->owner()->MEMBER_edge.erase(p);
                delete p;
                return;
            }
        }
    }
    std::unique_ptr<graph::data_type> const & graph::mutable_node_iterator::data()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node->data();
    }
    graph::data_type const * graph::mutable_node_iterator::data()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node->data().get();
    }
    bool graph::mutable_node_iterator::operator <(mutable_node_iterator const & right)const noexcept
    {
        return this < (&right);
    }
    graph & graph::mutable_node_iterator::owner()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return *(this->MEMBER_node->owner());
    }
    graph const & graph::mutable_node_iterator::owner()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return *(this->MEMBER_node->owner());
    }
    std::set<graph::mutable_edge_iterator> graph::mutable_node_iterator::next()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        std::set<mutable_edge_iterator> result;
        for (auto f = this->MEMBER_node->next().begin();f != this->MEMBER_node->next().end();f++)
        {
            result.insert(static_cast<mutable_edge_iterator const &>(mutable_edge_iterator(*f)));
        }
        return result;
    }
    std::set<graph::immutable_edge_iterator> graph::mutable_node_iterator::next()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        std::set<immutable_edge_iterator> result;
        for (auto f = this->MEMBER_node->next().begin();f != this->MEMBER_node->next().end();f++)
        {
            result.insert(static_cast<immutable_edge_iterator const &>(immutable_edge_iterator(*f)));
        }
        return result;
    }
    graph::immutable_node_iterator graph::begin_node()const
        noexcept(noexcept(std::declval<std::set<node *>>().begin() == std::declval<std::set<node *>>().end())
        and noexcept(immutable_node_iterator(nullptr)))
    {
        auto b = this->MEMBER_node.begin();
        if (b == this->MEMBER_node.end())
        {
            return immutable_node_iterator(nullptr);
        }
        else
        {
            return immutable_node_iterator(*b);
        }
    }

    graph::immutable_node_iterator graph::end_node()const
        noexcept(noexcept(immutable_node_iterator(nullptr)))
    {
        return immutable_node_iterator(nullptr);
    }
    graph::mutable_node_iterator graph::begin_node()
        noexcept(noexcept(std::declval<std::set<node *>>().begin() == std::declval<std::set<node *>>().end())
and noexcept(mutable_node_iterator(nullptr)))
    {
        auto b = this->MEMBER_node.begin();
        if (b == this->MEMBER_node.end())
        {
            return mutable_node_iterator(nullptr);
        }
        else
        {
            return mutable_node_iterator(*b);
        }
    }
    graph::mutable_node_iterator graph::end_node()
        noexcept(noexcept(mutable_node_iterator(nullptr)))
    {
        return mutable_node_iterator(nullptr);
    }
    graph::immutable_node_iterator::immutable_node_iterator(node * n)
        noexcept(std::is_nothrow_default_constructible_v<std::weak_ptr<marker_type>>
            and noexcept(std::declval<std::weak_ptr<marker_type>>() = std::declval<std::shared_ptr<marker_type> const &>()))
        :MEMBER_node(n)
        , MEMBER_marker()
    {
        if (this->MEMBER_node)
        {
            this->MEMBER_marker = this->MEMBER_node->marker();
        }
    }

    graph::node const * graph::immutable_node_iterator::get_node()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("此迭代器已失效，禁止访问");
        }
        return this->MEMBER_node;
    }
    graph::node * graph::immutable_node_iterator::get_node()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("此迭代器已失效，禁止访问");
        }
        return this->MEMBER_node;
    }

    graph::immutable_node_iterator::immutable_node_iterator(immutable_node_iterator const & origin)
        noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>)
        :MEMBER_node(origin.MEMBER_node)
        , MEMBER_marker(origin.MEMBER_marker)
    {
    }
    void  graph::immutable_node_iterator::operator =(immutable_node_iterator const & origin)
        noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>)
    {
        this->MEMBER_node = origin.MEMBER_node;
        this->MEMBER_marker = origin.MEMBER_marker;
    }

    graph::immutable_node_iterator::~immutable_node_iterator()
        noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>)
    {
    }


    bool graph::immutable_node_iterator::operator ==(mutable_node_iterator const & right)const noexcept
    {
        return this->MEMBER_node == right.MEMBER_node;
    }
    bool graph::immutable_node_iterator::operator !=(mutable_node_iterator const & right)const noexcept
    {
        return this->MEMBER_node != right.MEMBER_node;
    }
    bool graph::immutable_node_iterator::operator ==(immutable_node_iterator const & right)const noexcept
    {
        return this->MEMBER_node == right.MEMBER_node;
    }
    bool graph::immutable_node_iterator::operator !=(immutable_node_iterator const & right)const noexcept
    {
        return this->MEMBER_node != right.MEMBER_node;
    }

    bool graph::immutable_node_iterator::operator <(immutable_node_iterator const & right)const noexcept
    {
        return this < (&right);
    }


    bool  graph::immutable_node_iterator::is_valid()const noexcept
    {
        return not this->MEMBER_marker.expired();
    }

    graph::data_type const * graph::immutable_node_iterator::data()const
    {
        return this->MEMBER_node->data().get();
    }

    graph const & graph::immutable_node_iterator::owner()const
    {
        return *(this->MEMBER_node->owner());
    }

    std::set<graph::immutable_edge_iterator> graph::immutable_node_iterator::next()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        std::set<immutable_edge_iterator> result;
        for (auto f = this->MEMBER_node->next().begin();f != this->MEMBER_node->next().end();f++)
        {
            result.insert(static_cast<immutable_edge_iterator const &>(immutable_edge_iterator(*f)));
        }
        return result;
    }

    graph::immutable_edge_iterator graph::immutable_node_iterator::find_target(immutable_node_iterator const & another)const
    {
        if (this->MEMBER_marker.expired() or another.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (this->MEMBER_node->owner() != another.MEMBER_node->owner())
        {
            return immutable_edge_iterator(nullptr);
        }
        for (auto f = this->MEMBER_node->owner()->MEMBER_edge.begin();f != this->MEMBER_node->owner()->MEMBER_edge.end();f++)
        {
            if ((*f)->begin() == this->MEMBER_node and (*f)->end() == another.MEMBER_node)
            {
                return immutable_edge_iterator(*f);
            }
        }
        return immutable_edge_iterator(nullptr);
    }


    std::size_t graph::immutable_node_iterator::in_degree()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node->previous().size();
    }

    std::size_t graph::immutable_node_iterator::out_degree()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node->next().size();
    }

    std::set<graph::immutable_edge_iterator> graph::immutable_node_iterator::previous()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        std::set<immutable_edge_iterator> result;
        for (auto i = this->MEMBER_node->previous().begin();i != this->MEMBER_node->previous().end();i++)
        {
            auto const temp = immutable_edge_iterator(*i);
            result.insert(temp);
        }
        return result;
    }


    graph::immutable_edge_iterator graph::immutable_node_iterator::find_former(immutable_node_iterator const & another)const
    {
        if (this->MEMBER_marker.expired() or another.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (this->MEMBER_node->owner() != another.MEMBER_node->owner())
        {
            return immutable_edge_iterator(nullptr);
        }
        for (auto f = this->MEMBER_node->owner()->MEMBER_edge.begin();f != this->MEMBER_node->owner()->MEMBER_edge.end();f++)
        {
            if ((*f)->begin() == another.MEMBER_node and (*f)->end() == this->MEMBER_node)
            {
                return immutable_edge_iterator(*f);
            }
        }
        return immutable_edge_iterator(nullptr);
    }

    graph::immutable_edge_iterator graph::begin_edge()const
        noexcept(noexcept(std::declval<std::set<edge *>>().begin() == std::declval<std::set<edge *>>().end())
and noexcept(immutable_edge_iterator(nullptr)))
    {
        auto b = this->MEMBER_edge.begin();
        if (b == this->MEMBER_edge.end())
        {
            return immutable_edge_iterator(nullptr);
        }
        else
        {
            return immutable_edge_iterator(*b);
        }
    }
    graph::immutable_edge_iterator graph::end_edge()const
        noexcept(noexcept(immutable_edge_iterator(nullptr)))
    {
        return immutable_edge_iterator(nullptr);
    }
    graph::mutable_edge_iterator graph::begin_edge()
        noexcept(noexcept(std::declval<std::set<edge *>>().begin() == std::declval<std::set<edge *>>().end())
and noexcept(mutable_edge_iterator(nullptr)))
    {
        auto b = this->MEMBER_edge.begin();
        if (b == this->MEMBER_edge.end())
        {
            return mutable_edge_iterator(nullptr);
        }
        else
        {
            return mutable_edge_iterator(*b);
        }
    }
    graph::mutable_edge_iterator graph::end_edge()
        noexcept(noexcept(mutable_edge_iterator(nullptr)))
    {
        return mutable_edge_iterator(nullptr);
    }
    graph::immutable_edge_iterator graph::mutable_node_iterator::find_target(immutable_node_iterator const & another)const
    {
        if (this->MEMBER_marker.expired() or another.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (this->MEMBER_node->owner() != another.MEMBER_node->owner())
        {
            return immutable_edge_iterator(nullptr);
        }
        for (auto f = this->MEMBER_node->owner()->MEMBER_edge.begin();f != this->MEMBER_node->owner()->MEMBER_edge.end();f++)
        {
            if ((*f)->begin() == this->MEMBER_node and (*f)->end() == another.MEMBER_node)
            {
                return immutable_edge_iterator(*f);
            }
        }
        return immutable_edge_iterator(nullptr);

    }
    graph::mutable_edge_iterator graph::mutable_node_iterator::find_target(immutable_node_iterator const & another)
    {
        if (this->MEMBER_marker.expired() or another.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (this->MEMBER_node->owner() != another.MEMBER_node->owner())
        {
            return mutable_edge_iterator(nullptr);
        }
        for (auto f = this->MEMBER_node->owner()->MEMBER_edge.begin();f != this->MEMBER_node->owner()->MEMBER_edge.end();f++)
        {
            if ((*f)->begin() == this->MEMBER_node and (*f)->end() == another.MEMBER_node)
            {
                return mutable_edge_iterator(*f);
            }
        }
        return mutable_edge_iterator(nullptr);
    }

    std::size_t  graph::mutable_node_iterator::in_degree()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node->previous().size();
    }

    std::size_t  graph::mutable_node_iterator::out_degree()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_node->next().size();
    }
    std::set<graph::immutable_edge_iterator> graph::mutable_node_iterator::previous()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        std::set<immutable_edge_iterator> result;
        for (auto i = this->MEMBER_node->previous().begin();i != this->MEMBER_node->previous().end();i++)
        {
            auto const temp = immutable_edge_iterator(*i);
            result.insert(temp);
        }
        return result;
    }
    std::set<graph::mutable_edge_iterator> graph::mutable_node_iterator::previous()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        std::set<mutable_edge_iterator> result;
        for (auto i = this->MEMBER_node->previous().begin();i != this->MEMBER_node->previous().end();i++)
        {
            auto const temp = mutable_edge_iterator(*i);
            result.insert(temp);
        }
        return result;

    }

    graph::immutable_edge_iterator graph::mutable_node_iterator::find_former(immutable_node_iterator const & another)const
    {
        if (this->MEMBER_marker.expired() or another.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (this->MEMBER_node->owner() != another.MEMBER_node->owner())
        {
            return immutable_edge_iterator(nullptr);
        }
        for (auto f = this->MEMBER_node->owner()->MEMBER_edge.begin();f != this->MEMBER_node->owner()->MEMBER_edge.end();f++)
        {
            if ((*f)->begin() == another.MEMBER_node and (*f)->end() == this->MEMBER_node)
            {
                return immutable_edge_iterator(*f);
            }
        }
        return immutable_edge_iterator(nullptr);
    }
    graph::mutable_edge_iterator graph::mutable_node_iterator::find_former(immutable_node_iterator const & another)
    {
        if (this->MEMBER_marker.expired() or another.MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        if (this->MEMBER_node->owner() != another.MEMBER_node->owner())
        {
            return mutable_edge_iterator(nullptr);
        }
        for (auto f = this->MEMBER_node->owner()->MEMBER_edge.begin();f != this->MEMBER_node->owner()->MEMBER_edge.end();f++)
        {
            if ((*f)->begin() == another.MEMBER_node and (*f)->end() == this->MEMBER_node)
            {
                return mutable_edge_iterator(*f);
            }
        }
        return mutable_edge_iterator(nullptr);
    }
    graph::mutable_node_iterator::operator graph::immutable_node_iterator()const
        noexcept(noexcept(immutable_node_iterator(nullptr))
and std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>)
    {
        auto r = immutable_node_iterator(nullptr);
        r.MEMBER_node = this->MEMBER_node;
        r.MEMBER_marker = this->MEMBER_marker;
        return static_cast<immutable_node_iterator const &>(r);
    }

    graph::mutable_edge_iterator::mutable_edge_iterator(edge * n)
        noexcept(noexcept(std::weak_ptr<marker_type>(std::declval<std::shared_ptr<marker_type> const &>()))
        and std::is_nothrow_default_constructible_v<std::weak_ptr<marker_type>>)
        :MEMBER_edge(n)
        , MEMBER_marker()
    {
        if (this->MEMBER_edge)
        {
            this->MEMBER_marker = this->MEMBER_edge->marker();
        }
    }

    graph::edge const * graph::mutable_edge_iterator::get_edge()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_edge;
    }
    graph::edge * graph::mutable_edge_iterator::get_edge()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_edge;
    }

    graph::mutable_edge_iterator::mutable_edge_iterator(mutable_edge_iterator const & origin)
        noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>)
        :MEMBER_edge(origin.MEMBER_edge)
        , MEMBER_marker(origin.MEMBER_marker)
    {
    }
    void graph::mutable_edge_iterator::operator =(mutable_edge_iterator const & origin)
        noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>)
    {
        this->MEMBER_edge = origin.MEMBER_edge;
        this->MEMBER_marker = origin.MEMBER_marker;
    }

    graph::mutable_edge_iterator::~mutable_edge_iterator()
        noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>)
    {
    }
    void graph::mutable_edge_iterator::operator ++()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }

        auto finder = this->MEMBER_edge->owner()->MEMBER_edge.begin();

        while (finder != this->MEMBER_edge->owner()->MEMBER_edge.end())
        {
            if (this->MEMBER_edge == (*finder))
            {
                ++finder;
                if (finder != this->MEMBER_edge->owner()->MEMBER_edge.end())
                {
                    this->MEMBER_edge = (*finder);
                    this->MEMBER_marker = this->MEMBER_edge->marker();
                }
                else
                {
                    this->MEMBER_edge = nullptr;
                    this->MEMBER_marker.reset();
                }
                break;
            }
            ++finder;
        }
    }
    void graph::mutable_edge_iterator::operator --()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }

        auto finder = this->MEMBER_edge->owner()->MEMBER_edge.begin();

        while (finder != this->MEMBER_edge->owner()->MEMBER_edge.end())
        {
            if (this->MEMBER_edge == (*finder))
            {
                if (finder == this->MEMBER_edge->owner()->MEMBER_edge.begin())
                {
                    this->MEMBER_edge = nullptr;
                    this->MEMBER_marker.reset();
                }
                else
                {
                    --finder;
                    this->MEMBER_edge = (*finder);
                    this->MEMBER_marker = this->MEMBER_edge->marker();
                }
                break;
            }
            ++finder;
        }
    }
    graph::mutable_edge_iterator::operator graph::immutable_edge_iterator()const
        noexcept(noexcept(immutable_edge_iterator(nullptr))
and std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>)
    {
        auto r = immutable_edge_iterator(nullptr);
        r.MEMBER_edge = this->MEMBER_edge;
        r.MEMBER_marker = this->MEMBER_marker;
        return static_cast<immutable_edge_iterator const &>(r);
    }

    bool  graph::mutable_edge_iterator::operator ==(mutable_edge_iterator const & right)const noexcept
    {
        return this->MEMBER_edge == right.MEMBER_edge;
    }
    bool  graph::mutable_edge_iterator::operator !=(mutable_edge_iterator const & right)const noexcept
    {
        return this->MEMBER_edge != right.MEMBER_edge;
    }
    bool  graph::mutable_edge_iterator::operator ==(immutable_edge_iterator const & right)const noexcept
    {
        return this->MEMBER_edge == right.MEMBER_edge;
    }
    bool  graph::mutable_edge_iterator::operator !=(immutable_edge_iterator const & right)const noexcept
    {
        return this->MEMBER_edge != right.MEMBER_edge;
    }

    bool graph::mutable_edge_iterator::operator <(mutable_edge_iterator const & right)const noexcept
    {
        return this < (&right);
    }


    bool graph::mutable_edge_iterator::is_valid()const noexcept
    {
        return (not this->MEMBER_marker.expired());
    }

    std::unique_ptr<graph::weight_type> const & graph::mutable_edge_iterator::weight()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_edge->weight();
    }
    graph::weight_type const * graph::mutable_edge_iterator::weight()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return this->MEMBER_edge->weight().get();
    }
    graph & graph::mutable_edge_iterator::owner()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return *(this->MEMBER_edge->owner());
    }
    graph const & graph::mutable_edge_iterator::owner()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return *(this->MEMBER_edge->owner());
    }

    graph::immutable_node_iterator  graph::mutable_edge_iterator::begin()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return immutable_node_iterator(this->MEMBER_edge->begin());
    }
    graph::mutable_node_iterator graph::mutable_edge_iterator::begin()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return mutable_node_iterator(this->MEMBER_edge->begin());
    }


    graph::immutable_node_iterator  graph::mutable_edge_iterator::end()const
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return immutable_node_iterator(this->MEMBER_edge->end());
    }
    graph::mutable_node_iterator graph::mutable_edge_iterator::end()
    {
        if (this->MEMBER_marker.expired())
        {
            throw std::logic_error("迭代器已失效，禁止访问");
        }
        return mutable_node_iterator(this->MEMBER_edge->end());
    }
}
