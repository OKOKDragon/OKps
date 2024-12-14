
#include <stdexcept>

#include ".\base.hpp"

namespace OKps::base
{
	template<bool safe_destructible>
	void marked<safe_destructible>::marker_type::change_owner(marked * const o)noexcept
	{
		this->MEMBER_owner = o;
	}
	blank * blank::operator &()noexcept
	{
		return this;
	}
	blank const * blank::operator &()const noexcept
	{
		return this;
	}

	handler<true> * handler<true>::operator &()noexcept
	{
		return this;
	}
	handler<true> const * handler<true>::operator &()const noexcept
	{
		return this;
	}
	handler<false> * handler<false>::operator &()noexcept
	{
		return this;
	}
	handler<false> const * handler<false>::operator &()const noexcept
	{
		return this;
	}
	template<bool safe_destructible>
	marked<safe_destructible> * marked<safe_destructible>::operator &()noexcept
	{
		return this;
	}
	template<bool safe_destructible>
	marked<safe_destructible> const * marked<safe_destructible>::operator &()const noexcept
	{
		return this;
	}
	template<bool object_safe_destructible, bool safe_destructible>
	reference<object_safe_destructible, safe_destructible> * reference<object_safe_destructible, safe_destructible>::operator &()noexcept
	{
		return this;
	}
	template<bool object_safe_destructible, bool safe_destructible>
	reference<object_safe_destructible, safe_destructible> const * reference<object_safe_destructible, safe_destructible>::operator &()const noexcept
	{
		return this;
	}
	self_copier * self_copier::operator &()noexcept
	{
		return this;
	}
	self_copier const * self_copier::operator &()const noexcept
	{
		return this;
	}
	worker * worker::operator &()noexcept
	{
		return this;
	}
	worker const * worker::operator &()const noexcept
	{
		return this;
	}
	bool worker::operator <(worker const & right)const noexcept
	{
		return (&(*this)) < (&right);
	}
	bool self_copier::operator <(self_copier const & right)const noexcept
	{
		return (&(*this)) < (&right);
	}
	template<bool object_safe_destructible, bool safe_destructible>
	bool reference<object_safe_destructible, safe_destructible>::operator <(reference const & right)const noexcept
	{
		return (&(*this)) < (&right);
	}
	template<bool safe_destructible>
	bool marked<safe_destructible>::operator <(marked const & right)const noexcept
	{
		return (&(*this)) < (&right);
	}
	bool handler<false>::operator <(handler const & right)const noexcept
	{
		return (&(*this)) < (&right);
	}
	bool handler<true>::operator <(handler const & right)const noexcept
	{
		return (&(*this)) < (&right);
	}
	bool blank::operator <(blank const & right)const noexcept
	{
		return (&(*this)) < (&right);
	}
	blank::blank()noexcept
	{
	}
	blank::blank(blank const &)noexcept
	{
	}
	blank::~blank()noexcept
	{
	}
	blank::blank(blank &&)noexcept
	{
	}
	void blank::operator =(blank &&)noexcept
	{
	}
	void blank::operator =(blank const &)noexcept
	{
	}

	blank & blank::self()noexcept
	{
		return *this;
	}
	blank const & blank::self()const noexcept
	{
		return *this;
	}

	worker::worker()
		noexcept(std::is_nothrow_default_constructible<std::thread>::value
			and std::is_nothrow_default_constructible<std::exception_ptr>::value)
	{
	}

	worker::worker(worker && origin)
		noexcept(std::is_nothrow_move_constructible<std::thread>::value
			and std::is_nothrow_copy_constructible<std::exception_ptr>::value)
		:MEMBER_thread(std::move(origin.MEMBER_thread))
		, MEMBER_error(origin.MEMBER_error)
	{
		origin.MEMBER_error = nullptr;
	}
	void worker::operator =(worker && origin)
		noexcept(std::is_nothrow_move_assignable<std::thread>::value
			and std::is_nothrow_copy_assignable<std::exception_ptr>::value)
	{
		if ((&(*this)) != (&origin))
		{
			this->MEMBER_thread = std::move(origin.MEMBER_thread);
			this->MEMBER_error = origin.MEMBER_error;
			origin.MEMBER_error = nullptr;
		}
	}
	worker::~worker()
		noexcept(std::is_nothrow_destructible<std::thread>::value
			and std::is_nothrow_destructible<std::exception_ptr>::value)
	{
	}
	worker & worker::self()noexcept
	{
		return *this;
	}
	worker const & worker::self()const noexcept
	{
		return *this;
	}

	std::exception_ptr const & worker::have_error()const noexcept
	{
		return this->MEMBER_error;
	}

	void worker::raise_error(std::exception_ptr const & ep)
	{
		if (this->MEMBER_error)
		{
			throw std::logic_error("有待处理的异常，无法引入新异常");
		}
		this->MEMBER_error = ep;
	}

	void worker::release_error()noexcept(false)
	{
		if (not this->MEMBER_error)
		{
			/*
			如果异常指针为空，则将其抛出产生未定义行为
			故作此判断
			*/
			throw std::logic_error("没有储存异常");
		}
		auto const ep = this->MEMBER_error;
		this->MEMBER_error = nullptr;
		std::rethrow_exception(ep);
	}

	handler<false>::handler()noexcept
	{
	}
	handler<false>::handler(handler const & origin)noexcept
	{
	}
	handler<false>::~handler()noexcept
	{
	}
	handler<false>::handler(handler && origin)noexcept
	{
	}
	void handler<false>::operator =(handler && origin)noexcept
	{
	}
	void handler<false>::operator =(handler const & origin)noexcept
	{
	}

	handler<false> & handler<false>::self()noexcept
	{
		return *this;
	}
	handler<false> const & handler<false>::self()const noexcept
	{
		return *this;
	}
	handler<true> & handler<true>::self()noexcept
	{
		return *this;
	}
	handler<true> const & handler<true>::self()const noexcept
	{
		return *this;
	}
	self_copier::self_copier()noexcept
	{
	}
	self_copier::self_copier(self_copier const & origin)noexcept
	{
	}
	self_copier::~self_copier()noexcept
	{
	}
	self_copier::self_copier(self_copier && origin)noexcept
	{
	}
	void self_copier::operator =(self_copier && origin)noexcept
	{
	}
	void self_copier::operator =(self_copier const & origin)noexcept
	{
	}
	self_copier & self_copier::self()noexcept
	{
		return *this;
	}
	self_copier const & self_copier::self()const noexcept
	{
		return *this;
	}
	template<bool safe_destructible>
	marked<safe_destructible>::marker_type::marker_type(marked * m)noexcept
		:MEMBER_owner(m)
	{
	}
	template<bool safe_destructible>
	marked<safe_destructible>::marker_type::~marker_type()noexcept
	{
	}
	template<bool safe_destructible>
	marked<safe_destructible> & marked<safe_destructible>::marker_type::owner()noexcept
	{
		return *(this->MEMBER_owner);
	}
	template<bool safe_destructible>
	marked<safe_destructible> const & marked<safe_destructible>::marker_type::owner()const noexcept
	{
		return *(this->MEMBER_owner);
	}
	template<bool safe_destructible>
	std::shared_ptr<typename marked<safe_destructible>::marker_type> const & marked<safe_destructible>::marker()const noexcept
	{
		return this->MEMBER_marker;
	}
	template<bool safe_destructible>
	marked<safe_destructible>::marked()
		noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
			and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker()
	{
		this->MEMBER_marker = std::make_shared<marker_type>(this);
	}
	template<bool safe_destructible>
	marked<safe_destructible>::~marked()
		noexcept(std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>
			and safe_destructible)
	{
	}
	template<bool safe_destructible>
	marked<safe_destructible>::marked(marked const & origin)
		noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
			and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker()
	{
		this->MEMBER_marker = std::make_shared<marker_type>(this);
	}
	template<bool safe_destructible>
	marked<safe_destructible>::marked(marked && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::shared_ptr<marker_type>>
			and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		: MEMBER_marker(std::move(origin.MEMBER_marker))
	{
		this->MEMBER_marker->change_owner(this);
		origin.MEMBER_marker = std::make_shared<marker_type>(std::addressof(origin));
	}
	template<bool safe_destructible>
	marked<safe_destructible> & marked<safe_destructible>::self()noexcept
	{
		return *this;
	}
	template<bool safe_destructible>
	marked<safe_destructible> const & marked<safe_destructible>::self()const noexcept
	{
		return *this;
	}
	template<bool safe_destructible>
	void marked<safe_destructible>::operator =(marked const & origin) noexcept
	{
	}
	template<bool safe_destructible>
	void marked<safe_destructible>::operator =(marked && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::shared_ptr<marker_type>>
		and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
	{
		if (this != std::addressof(origin))
		{
			this->MEMBER_marker = std::move(origin.MEMBER_marker);
			this->MEMBER_marker->change_owner(this);
			origin.MEMBER_marker = std::make_shared<marker_type>(std::addressof(origin));
		}
	}
	template<bool object_safe_destructible, bool safe_destructible>
	reference<object_safe_destructible, safe_destructible>::reference(marked<object_safe_destructible> const & m)
		noexcept(noexcept(std::weak_ptr<typename marked<object_safe_destructible>::marker_type>(std::declval<std::shared_ptr<typename marked<object_safe_destructible>::marker_type> const &>())))
		:MEMBER_marker(m.marker())
	{
	}
	template<bool object_safe_destructible, bool safe_destructible>
	reference<object_safe_destructible, safe_destructible>::~reference()
		noexcept(std::is_nothrow_destructible_v<std::weak_ptr<typename marked<object_safe_destructible>::marker_type>>
			and safe_destructible)
	{
	}
	template<bool object_safe_destructible, bool safe_destructible>
	reference<object_safe_destructible, safe_destructible>::reference(reference const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<typename marked<object_safe_destructible>::marker_type>>)
		: MEMBER_marker(origin.MEMBER_marker)
	{
	}
	template<bool object_safe_destructible, bool safe_destructible>
	void reference<object_safe_destructible, safe_destructible>::operator =(reference const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<typename marked<object_safe_destructible>::marker_type>>)
	{
		if ((&(*this)) != (&origin))
		{
			this->MEMBER_marker = origin.MEMBER_marker;
		}
	}
	template<bool object_safe_destructible, bool safe_destructible>
	reference<object_safe_destructible, safe_destructible>::reference(reference && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::weak_ptr<typename marked<object_safe_destructible>::marker_type>>)
		: MEMBER_marker(std::move(origin.MEMBER_marker))
	{
	}
	template<bool object_safe_destructible, bool safe_destructible>
	void reference<object_safe_destructible, safe_destructible>::operator =(reference && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::weak_ptr<typename marked<object_safe_destructible>::marker_type>>)
	{
		if ((&(*this)) != (&origin))
		{
			this->MEMBER_marker = std::move(origin.MEMBER_marker);
		}
	}
	template<bool object_safe_destructible, bool safe_destructible>
	reference<object_safe_destructible, safe_destructible> & reference<object_safe_destructible, safe_destructible>::self()noexcept
	{
		return *this;
	}
	template<bool object_safe_destructible, bool safe_destructible>
	reference<object_safe_destructible, safe_destructible> const & reference<object_safe_destructible, safe_destructible>::self()const noexcept
	{
		return *this;
	}
	template<bool object_safe_destructible, bool safe_destructible>
	bool reference<object_safe_destructible, safe_destructible>::is_valid()const
		noexcept(noexcept(not std::declval<std::weak_ptr<typename marked<object_safe_destructible>::marker_type>>().expired()))
	{
		return (not this->MEMBER_marker.expired());
	}
	template<bool object_safe_destructible, bool safe_destructible>
	marked<object_safe_destructible> & reference<object_safe_destructible, safe_destructible>::get()
	{
		if (this->MEMBER_marker.expired())
		{
			throw std::logic_error("引用已失效，禁止解引用");
		}
		return this->MEMBER_marker.lock()->owner();
	}
	template<bool object_safe_destructible, bool safe_destructible>
	marked<object_safe_destructible> const & reference<object_safe_destructible, safe_destructible>::get()const
	{
		if (this->MEMBER_marker.expired())
		{
			throw std::logic_error("引用已失效，禁止解引用");
		}
		return this->MEMBER_marker.lock()->owner();
	}

	void worker::execute()
	{
		this->MEMBER_thread = std::thread(&worker::work, &(*this));

	}
	void worker::join()
	{
		if (this->MEMBER_thread.joinable())
		{
			this->MEMBER_thread.join();
		}
	}
	std::thread::id worker::id()const
		noexcept(noexcept(std::declval<std::thread>().get_id()))
	{
		return this->MEMBER_thread.get_id();
	}

	handler<true>::handler()
		noexcept(std::is_nothrow_default_constructible<std::exception_ptr>::value)
		:MEMBER_error_holder()
	{
	}
	handler<true>::handler(handler && origin)
		noexcept(std::is_nothrow_copy_constructible<std::exception_ptr>::value
		and std::is_nothrow_default_constructible<std::exception_ptr>::value
		and std::is_nothrow_copy_assignable<std::exception_ptr>::value)
		:MEMBER_error_holder(origin.MEMBER_error_holder)
	{
		origin.MEMBER_error_holder = std::exception_ptr();
	}
	void handler<true>::operator =(handler && origin)
		noexcept(std::is_nothrow_copy_assignable<std::exception_ptr>::value
		and std::is_nothrow_default_constructible<std::exception_ptr>::value)
	{
		if ((&(*this)) != (&origin))
		{
			this->MEMBER_error_holder = origin.MEMBER_error_holder;
			origin.MEMBER_error_holder = std::exception_ptr();
		}
	}

	handler<true>::~handler()
		noexcept(std::is_nothrow_destructible<std::exception_ptr>::value)
	{
	}
	std::exception_ptr const & handler<true>::have_error()const noexcept
	{
		return this->MEMBER_error_holder;
	}
	void handler<true>::raise_error(std::exception_ptr const & error)
	{
		if (this->MEMBER_error_holder)
		{
			throw std::logic_error("已经存储了一个异常，尚未得到处理");
		}
		this->MEMBER_error_holder = error;
	}

	void handler<true>::release_error()noexcept(false)
	{
		if (not this->MEMBER_error_holder)
		{
			throw std::logic_error("没有存储异常");
		}
		auto const p = this->MEMBER_error_holder;
		this->MEMBER_error_holder = std::exception_ptr();
		std::rethrow_exception(p);
	}
}