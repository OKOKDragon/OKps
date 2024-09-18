#include <stdexcept>

#include ".\base.hpp"

namespace OKps::base
{
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
		if (this != (&origin))
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

	std::thread & worker::thread()noexcept
	{
		return this->MEMBER_thread;
	}
	std::thread const & worker::thread()const noexcept
	{
		return this->MEMBER_thread;
	}

	std::exception_ptr const & worker::have_error()const noexcept
	{
		return this->MEMBER_error;
	}

	void worker::raise_error(std::exception_ptr const & ep)
	{
		if (this->MEMBER_error)
		{
			throw std::logic_error("已经存储了异常，尚未得到处理");
		}
		this->MEMBER_error = ep;
	}

	void worker::release_error()noexcept(false)
	{
		if (not this->MEMBER_error)
		{
			throw std::logic_error("没有储存异常");
		}
		auto const ep = this->MEMBER_error;
		this->MEMBER_error = nullptr;
		std::rethrow_exception(ep);
	}

	bool const & move_invalid::is_valid()const noexcept
	{
		return this->MEMBER_valid;
	}

	void move_invalid::raise_invalid_error()const noexcept(false)
	{
		if (not this->MEMBER_valid)
		{
			throw std::logic_error("此对象已失效，禁止访问");
		}
	}

	move_invalid::move_invalid()noexcept
		:MEMBER_valid(true)
	{
	}

	move_invalid::move_invalid(move_invalid const & origin)noexcept
		:MEMBER_valid(origin.MEMBER_valid)
	{
	}
	void move_invalid::operator =(move_invalid const & origin)noexcept
	{
		if (this != (&origin))
		{
			this->MEMBER_valid = origin.MEMBER_valid;
		}
	}
	move_invalid::move_invalid(move_invalid && origin)noexcept
		:MEMBER_valid(origin.MEMBER_valid)
	{
		origin.MEMBER_valid = false;
	}
	void move_invalid::operator =(move_invalid && origin)noexcept
	{
		if (this != (&origin))
		{
			this->MEMBER_valid = origin.MEMBER_valid;
			origin.MEMBER_valid = false;
		}
	}

	move_invalid::~move_invalid()noexcept
	{
	}

	move_invalid & move_invalid::self()noexcept
	{
		return *this;
	}
	move_invalid const & move_invalid::self()const noexcept
	{
		return *this;
	}

	numbered::id_generator::id_generator()
		:MEMBER_seed()
		, MEMBER_engine()
		, MEMBER_generator(std::numeric_limits<std::uintmax_t>::min(), std::numeric_limits<std::uintmax_t>::max())
	{
		this->MEMBER_engine = std::make_unique<std::mt19937_64>(this->MEMBER_seed());
	}
	numbered::id_generator::~id_generator()
		noexcept(std::is_nothrow_destructible_v<std::random_device>
			and std::is_nothrow_destructible_v<std::unique_ptr<std::mt19937_64>>
			and std::is_nothrow_destructible_v<std::uniform_int_distribution<std::uintmax_t>>)
	{
	}

	std::uintmax_t numbered::id_generator::generate()
		noexcept(noexcept(std::declval<std::uniform_int_distribution<std::uintmax_t>>()(*std::declval<std::unique_ptr<std::mt19937_64>>())))
	{
		return this->MEMBER_generator(*this->MEMBER_engine);
	}


	numbered::numbered(std::uintmax_t const id)noexcept
		:MEMBER_id(id)
	{
	}

	numbered::numbered(numbered const & origin)noexcept
		:MEMBER_id(origin.MEMBER_id)
	{
	}
	void numbered::operator =(numbered const & origin)noexcept
	{
		if (this != (&origin))
		{
			this->MEMBER_id = origin.MEMBER_id;
		}
	}

	numbered::~numbered()noexcept
	{
	}

	std::uintmax_t & numbered::id() noexcept
	{
		return this->MEMBER_id;
	}
	std::uintmax_t const & numbered::id()const noexcept
	{
		return this->MEMBER_id;
	}
	numbered & numbered::self()noexcept
	{
		return *this;
	}
	numbered const & numbered::self()const noexcept
	{
		return *this;
	}


	named::named(std::string const & name)
		noexcept(std::is_nothrow_copy_constructible<std::string>::value)
		:MEMBER_name(name)
	{
	}
	named::named(std::string && name)
		noexcept(std::is_nothrow_move_constructible<std::string>::value)
		:MEMBER_name(std::move(name))
	{
	}

	named::named(named const & origin)
		noexcept(std::is_nothrow_copy_constructible<std::string>::value)
		:MEMBER_name(origin.MEMBER_name)
	{
	}
	named::named(named && origin)
		noexcept(std::is_nothrow_move_constructible<std::string>::value)
		:MEMBER_name(std::move(origin.MEMBER_name))
	{
	}
	named::~named()
		noexcept(std::is_nothrow_destructible<std::string>::value)
	{
	}

	std::string const & named::name()const noexcept
	{
		return this->MEMBER_name;
	}

	std::string & named::name()noexcept
	{
		return this->MEMBER_name;
	}


	named & named::self()noexcept
	{
		return *this;
	}
	named const & named::self()const noexcept
	{
		return *this;
	}

	void named::operator =(named const & origin)
		noexcept(std::is_nothrow_copy_assignable<std::string>::value)
	{
		if (this != (&origin))
		{
			this->MEMBER_name = origin.MEMBER_name;
		}
	}

	void named::operator =(named && origin)
		noexcept(std::is_nothrow_move_assignable<std::string>::value)
	{
		if (this != (&origin))
		{
			this->MEMBER_name = std::move(origin.MEMBER_name);
		}
	}

	parameter::parameter()noexcept
	{
	}
	parameter::parameter(parameter const &)noexcept
	{
	}
	parameter::~parameter()noexcept
	{
	}
	parameter::parameter(parameter &&)noexcept
	{
	}
	void parameter::operator =(parameter &&)noexcept
	{
	}
	void parameter::operator =(parameter const &)noexcept
	{
	}

	parameter & parameter::self()noexcept
	{
		return *this;
	}
	parameter const & parameter::self()const noexcept
	{
		return *this;
	}

	handler::handler()noexcept
	{
	}
	handler::handler(handler const &)noexcept
	{
	}
	handler::~handler()noexcept
	{
	}
	handler::handler(handler &&)noexcept
	{
	}
	void handler::operator =(handler &&)noexcept
	{
	}
	void handler::operator =(handler const &)noexcept
	{
	}

	handler & handler::self()noexcept
	{
		return *this;
	}
	handler const & handler::self()const noexcept
	{
		return *this;
	}
	copier::copier()noexcept
	{
	}
	copier::copier(copier const &)noexcept
	{
	}
	copier::~copier()noexcept
	{
	}
	copier::copier(copier &&)noexcept
	{
	}
	void copier::operator =(copier &&)noexcept
	{
	}
	void copier::operator =(copier const &)noexcept
	{
	}
	copier & copier::self()noexcept
	{
		return *this;
	}
	copier const & copier::self()const noexcept
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
		noexcept(noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker(std::make_shared<marker_type>(this))
	{
	}
	marked::~marked()
		noexcept(std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>)
	{
	}
	marked::marked(marked const &)
		noexcept(noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker(std::make_shared<marker_type>(this))
	{
	}
	marked::marked(marked &&)
		noexcept(noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker(std::make_shared<marker_type>(this))
	{
	}
	marked & marked::self()noexcept
	{
		return *this;
	}
	marked const & marked::self()const noexcept
	{
		return *this;
	}

	void marked::operator =(marked const &) noexcept
	{
	}

	void marked::operator =(marked &&) noexcept
	{
	}
	reference::reference(std::shared_ptr<marked::marker_type> const & m)
		noexcept(noexcept(std::weak_ptr<marked::marker_type>(std::declval<std::shared_ptr<marked::marker_type> const &>())))
		:MEMBER_marker(m)
	{
	}
	reference::~reference()
		noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marked::marker_type>>)
	{
	}
	reference::reference(reference const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marked::marker_type>>)
		:MEMBER_marker(origin.MEMBER_marker)
	{
	}
	void reference::operator =(reference const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marked::marker_type>>)
	{
		if (this != (&origin))
		{
			this->MEMBER_marker = origin.MEMBER_marker;
		}
	}
	reference::reference(reference && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::weak_ptr<marked::marker_type>>)
		:MEMBER_marker(std::move(origin.MEMBER_marker))
	{
	}
	void reference::operator =(reference && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::weak_ptr<marked::marker_type>>)
	{
		if (this != (&origin))
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
		noexcept(noexcept(std::declval<std::weak_ptr<marked::marker_type>>().expired()))
	{
		return (not this->MEMBER_marker.expired());
	}
	marked & reference::get()
	{
		return this->MEMBER_marker.lock()->owner();
	}
	marked const & reference::get()const
	{
		return this->MEMBER_marker.lock()->owner();
	}

	memory_recorded::memory_recorded()noexcept
	{
	}
	memory_recorded::~memory_recorded()noexcept
	{
	}

	void * memory_recorded::operator new(std::size_t const size)
	{
		void * block = ::operator new(size);
		memory_recorded::MEMBER_recorder.add(block, size);
		return block;
	}
	void memory_recorded::operator delete(void * const block, std::size_t const size)noexcept
	{
		memory_recorded::MEMBER_recorder.erase(block);
		::operator delete(block, size);
	}
	void * memory_recorded::operator new[](std::size_t const size)
	{
		void * block = ::operator new[](size);
		memory_recorded::MEMBER_recorder.add(block, size);
		return block;
	}
	void memory_recorded::operator delete[](void * const block, std::size_t const size)noexcept
	{
		memory_recorded::MEMBER_recorder.erase(block);
		::operator delete[](block, size);
	}
	memory_recorded::recorder memory_recorded::MEMBER_recorder;
	memory_recorded::memory_recorded(memory_recorded const &)noexcept
	{
	}
	void memory_recorded::operator =(memory_recorded const &)noexcept
	{
	}
	memory_recorded::memory_recorded(memory_recorded &&)noexcept
	{
	}
	void memory_recorded::operator =(memory_recorded &&)noexcept
	{
	}
	memory_recorded & memory_recorded::self()noexcept
	{
		return *this;
	}
	memory_recorded const & memory_recorded::self()const noexcept
	{
		return *this;
	}
	std::map<void *, std::size_t> const & memory_recorded::record()noexcept
	{
		return memory_recorded::MEMBER_recorder.get();
	}
	memory_recorded::recorder::recorder()
		noexcept(std::is_nothrow_default_constructible_v<std::map<memory_recorded *, std::size_t>>
		and std::is_nothrow_default_constructible_v<std::mutex>)
		:MEMBER_lock()
		, MEMBER_recorder()
	{
	}
	memory_recorded::recorder::~recorder()
		noexcept(std::is_nothrow_destructible_v<std::map<memory_recorded *, std::size_t>>
		and std::is_nothrow_destructible_v<std::mutex>)
	{
	}

	void memory_recorded::recorder::add(void * const block, std::size_t const size)
	{
		this->MEMBER_lock.lock();
		this->MEMBER_recorder.insert(std::make_pair(block, size));
		this->MEMBER_lock.unlock();
	}
	void memory_recorded::recorder::erase(void * const block)
	{
		this->MEMBER_lock.lock();
		this->MEMBER_recorder.erase(block);
		this->MEMBER_lock.unlock();
	}
	std::map<void *, std::size_t> const & memory_recorded::recorder::get()const noexcept
	{
		return this->MEMBER_recorder;
	}

	void worker::execute()
	{
		this->MEMBER_thread = std::thread(&worker::work, std::addressof(this->self()));
	}
	void worker::join()
	{
		if (this->MEMBER_thread.joinable())
		{
			this->MEMBER_thread.join();
		}
	}
}