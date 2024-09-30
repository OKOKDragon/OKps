#include <stdexcept>

#include ".\base.hpp"

namespace OKps::base
{
	bool worker::operator <(worker const & right)const noexcept
	{
		return this < (&right);
	}
	bool self_copier::operator <(self_copier const & right)const noexcept
	{
		return this < (&right);
	}
	bool reference::operator <(reference const & right)const noexcept
	{
		return this < (&right);
	}
	bool marked::operator <(marked const & right)const noexcept
	{
		return this < (&right);
	}
	bool handler::operator <(handler const & right)const noexcept
	{
		return this < (&right);
	}
	bool named::operator <(named const & right)const noexcept
	{
		return this < (&right);
	}
	bool numbered::operator <(numbered const & right)const noexcept
	{
		return this < (&right);
	}
	bool move_invalid::operator <(move_invalid const & right)const noexcept
	{
		return this < (&right);
	}
	bool blank::operator <(blank const & right)const noexcept
	{
		return this < (&right);
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
	self_copier::self_copier()noexcept
	{
	}
	self_copier::self_copier(self_copier const &)noexcept
	{
	}
	self_copier::~self_copier()noexcept
	{
	}
	self_copier::self_copier(self_copier &&)noexcept
	{
	}
	void self_copier::operator =(self_copier &&)noexcept
	{
	}
	void self_copier::operator =(self_copier const &)noexcept
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
	marked::~marked()
		noexcept(std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>)
	{
	}
	marked::marked(marked const &)
		noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
			and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker()
	{
		this->MEMBER_marker = std::make_shared<marker_type>(this);
	}
	marked::marked(marked &&)
		noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
			and noexcept(std::make_shared<marker_type>(std::declval<marked *>())))
		:MEMBER_marker()
	{
		this->MEMBER_marker = std::make_shared<marker_type>(this);
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
	reference::reference(marked const & m)
		noexcept(noexcept(reference(std::declval<std::shared_ptr<marked::marker_type> const &>())))
		:reference(m.marker())
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
		noexcept(noexcept(not std::declval<std::weak_ptr<marked::marker_type>>().expired()))
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
		this->MEMBER_thread = std::thread(&worker::work, std::addressof(this->self()));

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

	pool_memory::allocator::allocator()
		noexcept(noexcept(std::declval<std::optional<std::pmr::polymorphic_allocator<std::byte>>>().emplace(std::declval<std::pmr::synchronized_pool_resource *>()))
		and noexcept(std::pmr::synchronized_pool_resource(std::pmr::pool_options(), std::pmr::new_delete_resource())))
		:MEMBER_allocator()
		, MEMBER_pool(std::pmr::pool_options(), std::pmr::new_delete_resource())
	{
		this->MEMBER_allocator.emplace(&(this->MEMBER_pool));
	}
	pool_memory::allocator::~allocator()
		noexcept(std::is_nothrow_destructible_v<std::optional<std::pmr::polymorphic_allocator<std::byte>>>
		and std::is_nothrow_destructible_v<std::pmr::synchronized_pool_resource>)
	{
	}
	std::pmr::polymorphic_allocator<std::byte> & pool_memory::allocator::get()noexcept
	{
		return this->MEMBER_allocator.value();
	}

	pool_memory::allocator pool_memory::MEMBER_allocator = pool_memory::allocator();

	void * pool_memory::operator new(std::size_t const size)
	{
		return pool_memory::MEMBER_allocator.get().allocate_bytes(size);
	}
	void * pool_memory::operator new[](std::size_t const size)
	{
		return pool_memory::MEMBER_allocator.get().allocate_bytes(size);
	}
	void pool_memory::operator delete[](void * const block, std::size_t const size)
	{
		pool_memory::MEMBER_allocator.get().deallocate_bytes(block, size);
	}
	void pool_memory::operator delete(void * const block, std::size_t const size)
	{
		pool_memory::MEMBER_allocator.get().deallocate_bytes(block, size);
	}

	pool_memory::pool_memory()noexcept
	{
	}
	pool_memory::pool_memory(pool_memory const &)noexcept
	{
	}
	pool_memory::pool_memory(pool_memory &&)noexcept
	{
	}
	pool_memory::~pool_memory()noexcept
	{
	}
	void  pool_memory::operator =(pool_memory const &)noexcept
	{
	}
	void  pool_memory::operator =(pool_memory &&)noexcept
	{
	}
	bool  pool_memory::operator <(pool_memory const & right)const noexcept
	{
		return this < (&right);
	}
	pool_memory::deleter_type::deleter_type(std::size_t const size)
		:MEMBER_size(size)
	{
	}

	pool_memory::deleter_type::deleter_type(deleter_type const & origin)noexcept
		:MEMBER_size(origin.MEMBER_size)
	{
	}

	void pool_memory::deleter_type::operator =(deleter_type const & origin)noexcept
	{
		this->MEMBER_size = origin.MEMBER_size;
	}

	pool_memory::deleter_type::~deleter_type()noexcept
	{
	}
	void pool_memory::deleter_type::operator()(void * block) const noexcept
	{
		pool_memory::MEMBER_allocator.get().deallocate_bytes(block, this->MEMBER_size, alignof(std::byte));
	}
	pool_memory::deleter_type::deleter_type(deleter_type && origin) noexcept
		:MEMBER_size(origin.MEMBER_size)
	{
		origin.MEMBER_size = 0;
	}

	void pool_memory::deleter_type::operator =(deleter_type && origin) noexcept
	{
		this->MEMBER_size = origin.MEMBER_size;
		origin.MEMBER_size = 0;
	}
	std::unique_ptr<std::byte[], pool_memory::deleter_type> pool_memory::allocate(std::size_t const size)
	{
		std::byte * block = static_cast<std::byte *>(pool_memory::MEMBER_allocator.get().allocate_bytes(size, alignof(std::byte)));
		deleter_type deleter = deleter_type(size);
		return std::unique_ptr<std::byte[], deleter_type>(block, deleter);
	}
}