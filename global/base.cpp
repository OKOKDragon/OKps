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
	bool blank::operator ==(blank const & right)const noexcept
	{
		return this == &right;
	}
	bool blank::operator !=(blank const & right)const noexcept
	{
		return not(*this == right);
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
		if (this != &origin)
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
		if (this != &origin)
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
		if (this != &origin)
		{
			this->MEMBER_valid = origin.MEMBER_valid;
			origin.MEMBER_valid = false;
		}
	}

	move_invalid::~move_invalid()noexcept
	{
	}
	bool move_invalid::operator ==(move_invalid const & right)const noexcept
	{
		return this == &right;
	}
	bool move_invalid::operator !=(move_invalid const & right)const noexcept
	{
		return not(*this == right);
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
	std::uintmax_t numbered::generate_random_id()
		noexcept(noexcept(std::declval<id_generator>().generate()))
	{
		return numbered::MEMBER_id_generator.generate();
	}
	numbered::id_generator numbered::MEMBER_id_generator;

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
		if (this != &origin)
		{
			this->MEMBER_id = origin.MEMBER_id;
		}
	}

	numbered::~numbered()noexcept
	{
	}
	bool numbered::operator ==(numbered const & right)const noexcept
	{
		return this == &right;
	}
	bool numbered::operator !=(numbered const & right)const noexcept
	{
		return not(*this == right);
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
	bool worker::operator ==(worker const & right)const noexcept
	{
		return this == &right;
	}
	bool worker::operator !=(worker const & right)const noexcept
	{
		return not(*this == right);
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

	bool named::operator ==(named const & right)const noexcept
	{
		return this == &right;
	}
	bool named::operator !=(named const & right)const noexcept
	{
		return not(*this == right);
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
		if (this != &origin)
		{
			this->MEMBER_name = origin.MEMBER_name;
		}
	}

	void named::operator =(named && origin)
		noexcept(std::is_nothrow_move_assignable<std::string>::value)
	{
		if (this != &origin)
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
	bool parameter::operator ==(parameter const & right)const noexcept
	{
		return this == &right;
	}
	bool parameter::operator !=(parameter const & right)const noexcept
	{
		return not(*this == right);
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
	bool handler::operator ==(handler const & right)const noexcept
	{
		return this == &right;
	}
	bool handler::operator !=(handler const & right)const noexcept
	{
		return not(*this == right);
	}
	handler & handler::self()noexcept
	{
		return *this;
	}
	handler const & handler::self()const noexcept
	{
		return *this;
	}
}