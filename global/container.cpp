#include ".\container.hpp"

namespace OKps::container
{
    copiable_unique::copiable_unique(std::unique_ptr<base::self_copier> && pointer)
        noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<base::self_copier>>)
        :MEMBER_value(std::move(pointer))
    {
    }

    copiable_unique::copiable_unique(copiable_unique const & origin)
        noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<base::self_copier>>)
        :MEMBER_value(std::move(origin.MEMBER_value))
    {
    }

    void copiable_unique::operator =(copiable_unique const & origin)
        noexcept(std::is_nothrow_move_assignable_v<std::unique_ptr<base::self_copier>>)
    {
        this->MEMBER_value = std::move(origin.MEMBER_value);
    }



    base::self_copier & copiable_unique::value()
    {
        if (not this->MEMBER_value)
        {
            throw std::logic_error("禁止访问空指针");
        }
        return *(this->MEMBER_value);
    }
    base::self_copier const & copiable_unique::value()const
    {
        if (not this->MEMBER_value)
        {
            throw std::logic_error("禁止访问空指针");
        }
        return *(this->MEMBER_value);
    }
    copiable_unique::~copiable_unique()
        noexcept(std::is_nothrow_destructible_v<std::unique_ptr<base::self_copier>>)
    {
    }
    bool copiable_unique::operator <(copiable_unique const & right)const
        noexcept(noexcept(std::declval<std::unique_ptr<base::self_copier>>().get() < std::declval<std::unique_ptr<base::self_copier>>().get()))
    {
        return (this->MEMBER_value.get() < right.MEMBER_value.get());
    }
}