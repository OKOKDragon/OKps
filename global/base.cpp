
#include <stdexcept>

#include ".\base.hpp"

namespace OKps::base
{

	void marked::marker_type::change_owner(marked * const o)noexcept
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
	marked * marked::operator &()noexcept
	{
		return this;
	}
	marked const * marked::operator &()const noexcept
	{
		return this;
	}
	reference * reference::operator &()noexcept
	{
		return this;
	}
	reference const * reference::operator &()const noexcept
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

	blank::blank()noexcept
	{
	}
	blank::blank(blank const &)noexcept
	{
	}
	blank::~blank()
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
	worker::~worker()noexcept(false)
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

	bool worker::have_error()const noexcept
	{
		if (this->MEMBER_error)
		{
			return true;
		}
		else
		{
			return false;
		}
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
	handler<false>::~handler()
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
	self_copier::~self_copier()
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
	marked::marker_type::marker_type(marked * m)noexcept
		:MEMBER_owner(m)
	{
	}
	marked::marker_type::~marker_type()noexcept
	{
	}
	marked & marked::marker_type::owner()noexcept
	{
		return *(this->MEMBER_owner);
	}
	marked const & marked::marker_type::owner()const noexcept
	{
		return *(this->MEMBER_owner);
	}
	std::shared_ptr<marked::marker_type> const & marked::marker()const noexcept
	{
		return this->MEMBER_marker;
	}
	marked::marked()
		noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
			and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker()
	{
		this->MEMBER_marker = std::make_shared<marker_type>(this);
	}
	marked::~marked()noexcept(false)
	{
	}
	marked::marked(marked const & origin)
		noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
			and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker()
	{
		this->MEMBER_marker = std::make_shared<marker_type>(this);
	}
	marked::marked(marked && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::shared_ptr<marker_type>>
			and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		: MEMBER_marker(std::move(origin.MEMBER_marker))
	{
		this->MEMBER_marker->change_owner(this);
		origin.MEMBER_marker = std::make_shared<marker_type>(std::addressof(origin));
	}
	marked & marked::self()noexcept
	{
		return *this;
	}
	marked const & marked::self()const noexcept
	{
		return *this;
	}
	void marked::operator =(marked const & origin) noexcept
	{
	}
	void marked::operator =(marked && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::shared_ptr<marker_type>>
		and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
	{
		if ((&(*this)) != (&origin))
		{
			this->MEMBER_marker = std::move(origin.MEMBER_marker);
			this->MEMBER_marker->change_owner(this);
			origin.MEMBER_marker = std::make_shared<marker_type>(std::addressof(origin));
		}
	}
	reference::reference(marked const & m)
		noexcept(noexcept(std::weak_ptr<marked::marker_type>(std::declval<std::shared_ptr<marked::marker_type> const &>())))
		:MEMBER_marker(m.marker())
	{
	}
	reference::~reference()noexcept(false)
	{
	}
	reference::reference(reference const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marked::marker_type>>)
		: MEMBER_marker(origin.MEMBER_marker)
	{
	}
	void reference::operator =(reference const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marked::marker_type>>)
	{
		if ((&(*this)) != (&origin))
		{
			this->MEMBER_marker = origin.MEMBER_marker;
		}
	}
	reference::reference(reference && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::weak_ptr<marked::marker_type>>)
		: MEMBER_marker(std::move(origin.MEMBER_marker))
	{
	}
	void reference::operator =(reference && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::weak_ptr<marked::marker_type>>)
	{
		if ((&(*this)) != (&origin))
		{
			this->MEMBER_marker = std::move(origin.MEMBER_marker);
		}
	}
	reference & reference::self()noexcept
	{
		return *this;
	}
	reference const & reference::self()const noexcept
	{
		return *this;
	}
	bool reference::is_valid()const
		noexcept(noexcept(not std::declval<std::weak_ptr<marked::marker_type> const &>().expired()))
	{
		return (not this->MEMBER_marker.expired());
	}
	marked & reference::get()
	{
		if (this->MEMBER_marker.expired())
		{
			throw std::logic_error("引用已失效，禁止解引用");
		}
		return this->MEMBER_marker.lock()->owner();
	}
	marked const & reference::get()const
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
		noexcept(noexcept(std::declval<std::thread const &>().get_id()))
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

	handler<true>::~handler()noexcept(false)
	{
	}
	bool handler<true>::have_error()const noexcept
	{
		if (this->MEMBER_error_holder)
		{
			return true;
		}
		else
		{
			return false;
		}
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