#pragma once

#include <source_location>
#include <exception>

namespace OKps
{
	class error;

	template<typename any_type>
	concept error_exception_type = not(std::is_same_v<any_type, error>
		or std::is_same_v<any_type, error &&>
		or std::is_same_v<any_type, error const &>
		or std::is_same_v<any_type, error &>
		or std::is_same_v<any_type, error const &&>
		or std::is_same_v<any_type, std::exception_ptr>
		or std::is_same_v<any_type, std::exception_ptr const &>
		or std::is_same_v<any_type, std::exception_ptr &&>
		or std::is_same_v<any_type, std::exception_ptr &>
		or std::is_same_v<any_type, std::exception_ptr const &&>);

	/*
	极简错误日志
	包含异常和源码信息

	不继承std::exception的原因是，此类不是异常本身，而是异常的包装器
	*/
	class error final
	{
	private:
		std::source_location MEMBER_location;
		std::exception_ptr MEMBER_error;
	public:
		void operator =(error && origin)
			noexcept(std::is_nothrow_move_assignable_v<std::exception_ptr>
			and std::is_nothrow_move_assignable_v<std::source_location>);

		error(error && origin)
			noexcept(std::is_nothrow_move_constructible_v<std::exception_ptr>
			and std::is_nothrow_move_constructible_v<std::source_location>);

		error(std::exception_ptr && error_holder, std::source_location && location = std::source_location::current())
			noexcept(std::is_nothrow_move_constructible_v<std::exception_ptr>
			and std::is_nothrow_move_constructible_v<std::source_location>);

		error(std::exception_ptr const & error_holder, std::source_location && location = std::source_location::current())
			noexcept(std::is_nothrow_copy_constructible_v<std::exception_ptr>
			and std::is_nothrow_move_constructible_v<std::source_location>);

		error(std::exception_ptr && error_holder, std::source_location const & location)
			noexcept(std::is_nothrow_move_constructible_v<std::exception_ptr>
			and std::is_nothrow_copy_constructible_v<std::source_location>);

		error(std::exception_ptr const & error_holder, std::source_location const & location)
			noexcept(std::is_nothrow_copy_constructible_v<std::exception_ptr>
			and std::is_nothrow_copy_constructible_v<std::source_location>);

		error(error const & origin)
			noexcept(std::is_nothrow_copy_constructible_v<std::source_location>
			and std::is_nothrow_copy_constructible_v<std::exception_ptr>);

		void operator =(error const & origin)
			noexcept(std::is_nothrow_copy_assignable_v<std::exception_ptr>
			and std::is_nothrow_copy_assignable_v<std::source_location>);

		~error()
			noexcept(std::is_nothrow_destructible_v<std::source_location>
				and std::is_nothrow_destructible_v<std::exception_ptr>);
		//获取日志对应的代码源文件位置
		std::source_location const & location()const noexcept;

		//是否存储了异常
		bool have_error()const noexcept;

		/*
		如果存储了异常，则抛出该异常
		*/
		void release_error() noexcept(false);

		//比较两个对象的地址，用于有序容器的排序
		bool operator <(error const & right)const noexcept;

		template<error_exception_type exception_type>
		error(exception_type const & error_object, std::source_location && location = std::source_location::current())
			noexcept(noexcept(std::make_exception_ptr(std::declval<exception_type const &>()))
		and noexcept(error(std::declval<std::exception_ptr &&>(), std::declval<std::source_location &&>())))
			:error(std::make_exception_ptr(error_object), std::move(location))
		{
		}

		template<error_exception_type exception_type>
		error(exception_type && error_object, std::source_location && location = std::source_location::current())
			noexcept(noexcept(std::make_exception_ptr(std::declval<exception_type &&>()))
		and noexcept(error(std::declval<std::exception_ptr &&>(), std::declval<std::source_location &&>())))
			:error(std::make_exception_ptr(std::move(error_object)), std::move(location))
		{
		}

		template<error_exception_type exception_type>
		error(exception_type const & error_object, std::source_location const & location)
			noexcept(noexcept(std::make_exception_ptr(std::declval<exception_type const &>()))
		and noexcept(error(std::declval<std::exception_ptr &&>(), std::declval<std::source_location const &>())))
			:error(std::make_exception_ptr(error_object), location)
		{
		}

		template<error_exception_type exception_type>
		error(exception_type && error_object, std::source_location const & location)
			noexcept(noexcept(std::make_exception_ptr(std::declval<exception_type &&>()))
		and noexcept(error(std::declval<std::exception_ptr &&>(), std::declval<std::source_location const &>())))
			:error(std::make_exception_ptr(std::move(error_object)), location)
		{
		}
	};

}