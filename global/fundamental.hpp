#pragma once

#include <type_traits>
#include <bitset>
#include <memory>

#include ".\blank.hpp"
#include ".\bit.hpp"

/*
此文件定义一些c++内置整数类型的包装类，以防止四则运算和位运算时的未定义行为。

此文件要求在c++20标准下编译，因为c++20标准规定了内置整数类型都要用补码形式表示，从而规范了内置整数类型的取值范围和位运算的行为。

包装类提供的所有操作都会在运行期强制对未定义行为做检查，将影响程序的性能。
默认构造的包装类的初始值都是0。

规定两个包装类 left 和 right 之间进行四则运算，结果的类型与 left 相同，而 right 会先被转换成与 left 相同的类型再运算。
如果在转换类型过程中，发现 right 的值无法用 left 的类型表示，则抛出异常。

left 和 right 进行比较，也是以 left 的类型为准，right 会先被转换成与 left 相同的类型。

与c++标准不同的是，包装类对任何溢出都视为错误，而c++标准则允许无符号整数的溢出。

c++标准规定：char 类型与 signed char 和 unsigned char 二者之一具有相同的符号性、大小和对齐，但它在语法层面是单独的类型。
此文件专注于运算，故只提供 signed char 和 unsigned char 的包装类，并将它们当作 1 字节大小的整数类型；不提供 char 的包装类。
*/

namespace OKps::base
{
	template<typename value_type>
	concept arithmetic_integer = std::is_same_v<value_type, signed char>
		or std::is_same_v<value_type, unsigned char>
		or std::is_same_v<value_type, signed short>
		or std::is_same_v<value_type, unsigned short>
		or std::is_same_v<value_type, signed int>
		or std::is_same_v<value_type, unsigned int>
		or std::is_same_v<value_type, signed long>
		or std::is_same_v<value_type, unsigned long>
		or std::is_same_v<value_type, signed long long>
		or std::is_same_v<value_type, unsigned long long>;

	template<typename value_type, typename different_value_type>
	concept different_arithmetic_integer = arithmetic_integer<value_type>
		and arithmetic_integer<different_value_type>
		and (not std::is_same_v<different_value_type, value_type>);

	template<arithmetic_integer value_type, different_arithmetic_integer<value_type> target_type>
	constexpr bool const have_same_sign = ((std::is_unsigned_v<value_type> == std::is_unsigned_v<target_type>)
		and (std::is_signed_v<value_type> == std::is_signed_v<target_type>));

	template<arithmetic_integer value_type>
	class integer;

	/*
	决定 value_type 类型向 different_value_type 类型转换时是否有可能溢出。
	如果值为true则表示不可能溢出，即相应的转换函数可以声明为 noexcept(true)。
	*/
	template<arithmetic_integer value_type, different_arithmetic_integer<value_type> target_type>
	constexpr bool const safe_convertible = ((have_same_sign<value_type, target_type> and (sizeof(value_type) <= sizeof(target_type)))
		or (std::is_unsigned_v<value_type> and std::is_signed_v<target_type> and (integer<target_type>::bit_length >= integer<value_type>::bit_length + static_cast<std::size_t>(1))));


	template<arithmetic_integer value_type>
	class integer final
	{
	private:
		value_type MEMBER_value;
	public:
		//数值占用的大小，单位是字节
		static std::size_t const size;
		//当前整数类型占用的长度，单位是位
		static inline constexpr std::size_t const bit_length = sizeof(value_type) * bit_per_byte;
		//最大值
		static value_type const max;
		//最小值
		static value_type const min;

		integer(value_type const value = static_cast<value_type>(0))noexcept;
		integer(std::bitset<integer<value_type>::bit_length> const &)
			noexcept(noexcept(static_cast<bool>((std::declval<std::bitset<integer<value_type>::bit_length> const &>())[std::declval<std::size_t>()])));
		integer(integer const &)noexcept;
		integer(integer &&) = delete;
		~integer()noexcept;

		void operator =(integer const &)noexcept;
		void operator =(integer &&)noexcept = delete;

		bool operator ==(integer const &)const noexcept;
		bool operator !=(integer const &)const noexcept;
		bool operator >=(integer const &)const noexcept;
		bool operator <=(integer const &)const noexcept;
		bool operator >(integer const &)const noexcept;
		bool operator <(integer const &)const noexcept;

		value_type const & value()const noexcept;

		integer operator +(integer const &)const;
		integer operator -(integer const &)const;
		integer operator *(integer const &)const;
		integer operator /(integer const &)const;
		integer operator %(integer const &)const;
		void operator +=(integer const &);
		void operator -=(integer const &);
		void operator *=(integer const &);
		void operator /=(integer const &);
		void operator %=(integer const &);
		void operator ++();
		void operator --();
		integer operator -()const;
		integer operator ~()const noexcept;
		integer operator &(integer const &)const noexcept;
		integer operator |(integer const &)const noexcept;
		integer operator ^(integer const &)const noexcept;
		void operator &=(integer const &)noexcept;
		void operator |=(integer const &)noexcept;
		void operator ^=(integer const &)noexcept;
		integer operator <<(integer<std::size_t> const &)const;
		integer operator >>(integer<std::size_t> const &)const;
		void operator <<=(integer<std::size_t> const &);
		void operator >>=(integer<std::size_t> const &);

		template<different_arithmetic_integer<value_type> target_type>
		operator integer<target_type>()const
			noexcept(safe_convertible<value_type, target_type>);

		operator std::bitset<integer<value_type>::bit_length>()const
			noexcept(noexcept(std::bitset<integer<value_type>::bit_length>())
				and noexcept((std::declval<std::bitset<integer<value_type>::bit_length>>())[std::declval<std::size_t>()] = std::declval<bool>()));
	};

	template class integer<signed char>;
	template class integer<unsigned char>;
	template class integer<signed short>;
	template class integer<unsigned short>;
	template class integer<signed int>;
	template class integer<unsigned int>;
	template class integer<signed long>;
	template class integer<unsigned long>;
	template class integer<signed long long>;
	template class integer<unsigned long long>;

	template
		integer<signed char>::operator integer<unsigned char>()const
		noexcept(safe_convertible<signed char, unsigned char>);
	template
		integer<signed char>::operator integer<signed short>()const
		noexcept(safe_convertible<signed char, signed short>);
	template
		integer<signed char>::operator integer<unsigned short>()const
		noexcept(safe_convertible<signed char, unsigned short>);
	template
		integer<signed char>::operator integer<signed int>()const
		noexcept(safe_convertible<signed char, signed int>);
	template
		integer<signed char>::operator integer<unsigned int>()const
		noexcept(safe_convertible<signed char, unsigned int>);
	template
		integer<signed char>::operator integer<signed long>()const
		noexcept(safe_convertible<signed char, signed long>);
	template
		integer<signed char>::operator integer<unsigned long>()const
		noexcept(safe_convertible<signed char, unsigned long>);
	template
		integer<signed char>::operator integer<signed long long>()const
		noexcept(safe_convertible<signed char, signed long long>);
	template
		integer<signed char>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<signed char, unsigned long long>);

	template
		integer<unsigned char>::operator integer<signed char>()const
		noexcept(safe_convertible<unsigned char, signed char>);
	template
		integer<unsigned char>::operator integer<signed short>()const
		noexcept(safe_convertible<unsigned char, signed short>);
	template
		integer<unsigned char>::operator integer<unsigned short>()const
		noexcept(safe_convertible<unsigned char, unsigned short>);
	template
		integer<unsigned char>::operator integer<signed int>()const
		noexcept(safe_convertible<unsigned char, signed int>);
	template
		integer<unsigned char>::operator integer<unsigned int>()const
		noexcept(safe_convertible<unsigned char, unsigned int>);
	template
		integer<unsigned char>::operator integer<signed long>()const
		noexcept(safe_convertible<unsigned char, signed long>);
	template
		integer<unsigned char>::operator integer<unsigned long>()const
		noexcept(safe_convertible<unsigned char, unsigned long>);
	template
		integer<unsigned char>::operator integer<signed long long>()const
		noexcept(safe_convertible<unsigned char, signed long long>);
	template
		integer<unsigned char>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<unsigned char, unsigned long long>);

	template
		integer<signed short>::operator integer<signed char>()const
		noexcept(safe_convertible<signed short, signed char>);
	template
		integer<signed short>::operator integer<unsigned char>()const
		noexcept(safe_convertible<signed short, unsigned char>);
	template
		integer<signed short>::operator integer<unsigned short>()const
		noexcept(safe_convertible<signed short, unsigned short>);
	template
		integer<signed short>::operator integer<signed int>()const
		noexcept(safe_convertible<signed short, signed int>);
	template
		integer<signed short>::operator integer<unsigned int>()const
		noexcept(safe_convertible<signed short, unsigned int>);
	template
		integer<signed short>::operator integer<signed long>()const
		noexcept(safe_convertible<signed short, signed long>);
	template
		integer<signed short>::operator integer<unsigned long>()const
		noexcept(safe_convertible<signed short, unsigned long>);
	template
		integer<signed short>::operator integer<signed long long>()const
		noexcept(safe_convertible<signed short, signed long long>);
	template
		integer<signed short>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<signed short, unsigned long long>);

	template
		integer<unsigned short>::operator integer<signed char>()const
		noexcept(safe_convertible<unsigned short, signed char>);
	template
		integer<unsigned short>::operator integer<unsigned char>()const
		noexcept(safe_convertible<unsigned short, unsigned char>);
	template
		integer<unsigned short>::operator integer<signed short>()const
		noexcept(safe_convertible<unsigned short, signed short>);
	template
		integer<unsigned short>::operator integer<signed int>()const
		noexcept(safe_convertible<unsigned short, signed int>);
	template
		integer<unsigned short>::operator integer<unsigned int>()const
		noexcept(safe_convertible<unsigned short, unsigned int>);
	template
		integer<unsigned short>::operator integer<signed long>()const
		noexcept(safe_convertible<unsigned short, signed long>);
	template
		integer<unsigned short>::operator integer<unsigned long>()const
		noexcept(safe_convertible<unsigned short, unsigned long>);
	template
		integer<unsigned short>::operator integer<signed long long>()const
		noexcept(safe_convertible<unsigned short, signed long long>);
	template
		integer<unsigned short>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<unsigned short, unsigned long long>);

	template
		integer<signed int>::operator integer<signed char>()const
		noexcept(safe_convertible<signed int, signed char>);
	template
		integer<signed int>::operator integer<unsigned char>()const
		noexcept(safe_convertible<signed int, unsigned char>);
	template
		integer<signed int>::operator integer<signed short>()const
		noexcept(safe_convertible<signed int, signed short>);
	template
		integer<signed int>::operator integer<unsigned short>()const
		noexcept(safe_convertible<signed int, unsigned short>);
	template
		integer<signed int>::operator integer<unsigned int>()const
		noexcept(safe_convertible<signed int, unsigned int>);
	template
		integer<signed int>::operator integer<signed long>()const
		noexcept(safe_convertible<signed int, signed long>);
	template
		integer<signed int>::operator integer<unsigned long>()const
		noexcept(safe_convertible<signed int, unsigned long>);
	template
		integer<signed int>::operator integer<signed long long>()const
		noexcept(safe_convertible<signed int, signed long long>);
	template
		integer<signed int>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<signed int, unsigned long long>);

	template
		integer<unsigned int>::operator integer<signed char>()const
		noexcept(safe_convertible<unsigned int, signed char>);
	template
		integer<unsigned int>::operator integer<unsigned char>()const
		noexcept(safe_convertible<unsigned int, unsigned char>);
	template
		integer<unsigned int>::operator integer<signed short>()const
		noexcept(safe_convertible<unsigned int, signed short>);
	template
		integer<unsigned int>::operator integer<unsigned short>()const
		noexcept(safe_convertible<unsigned int, unsigned short>);
	template
		integer<unsigned int>::operator integer<signed int>()const
		noexcept(safe_convertible<unsigned int, signed int>);
	template
		integer<unsigned int>::operator integer<signed long>()const
		noexcept(safe_convertible<unsigned int, signed long>);
	template
		integer<unsigned int>::operator integer<unsigned long>()const
		noexcept(safe_convertible<unsigned int, unsigned long>);
	template
		integer<unsigned int>::operator integer<signed long long>()const
		noexcept(safe_convertible<unsigned int, signed long long>);
	template
		integer<unsigned int>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<unsigned int, unsigned long long>);

	template
		integer<signed long>::operator integer<signed char>()const
		noexcept(safe_convertible<signed long, signed char>);
	template
		integer<signed long>::operator integer<unsigned char>()const
		noexcept(safe_convertible<signed long, unsigned char>);
	template
		integer<signed long>::operator integer<signed short>()const
		noexcept(safe_convertible<signed long, signed short>);
	template
		integer<signed long>::operator integer<unsigned short>()const
		noexcept(safe_convertible<signed long, unsigned short>);
	template
		integer<signed long>::operator integer<signed int>()const
		noexcept(safe_convertible<signed long, signed int>);
	template
		integer<signed long>::operator integer<unsigned int>()const
		noexcept(safe_convertible<signed long, unsigned int>);
	template
		integer<signed long>::operator integer<unsigned long>()const
		noexcept(safe_convertible<signed long, unsigned long>);
	template
		integer<signed long>::operator integer<signed long long>()const
		noexcept(safe_convertible<signed long, signed long long>);
	template
		integer<signed long>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<signed long, unsigned long long>);

	template
		integer<unsigned long>::operator integer<signed char>()const
		noexcept(safe_convertible<unsigned long, signed char>);
	template
		integer<unsigned long>::operator integer<unsigned char>()const
		noexcept(safe_convertible<unsigned long, unsigned char>);
	template
		integer<unsigned long>::operator integer<signed short>()const
		noexcept(safe_convertible<unsigned long, signed short>);
	template
		integer<unsigned long>::operator integer<unsigned short>()const
		noexcept(safe_convertible<unsigned long, unsigned short>);
	template
		integer<unsigned long>::operator integer<signed int>()const
		noexcept(safe_convertible<unsigned long, signed int>);
	template
		integer<unsigned long>::operator integer<unsigned int>()const
		noexcept(safe_convertible<unsigned long, unsigned int>);
	template
		integer<unsigned long>::operator integer<signed long>()const
		noexcept(safe_convertible<unsigned long, signed long>);
	template
		integer<unsigned long>::operator integer<signed long long>()const
		noexcept(safe_convertible<unsigned long, signed long long>);
	template
		integer<unsigned long>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<unsigned long, unsigned long long>);

	template
		integer<signed long long>::operator integer<signed char>()const
		noexcept(safe_convertible<signed long long, signed char>);
	template
		integer<signed long long>::operator integer<unsigned char>()const
		noexcept(safe_convertible<signed long long, unsigned char>);
	template
		integer<signed long long>::operator integer<signed short>()const
		noexcept(safe_convertible<signed long long, signed short>);
	template
		integer<signed long long>::operator integer<unsigned short>()const
		noexcept(safe_convertible<signed long long, unsigned short>);
	template
		integer<signed long long>::operator integer<signed int>()const
		noexcept(safe_convertible<signed long long, signed int>);
	template
		integer<signed long long>::operator integer<unsigned int>()const
		noexcept(safe_convertible<signed long long, unsigned int>);
	template
		integer<signed long long>::operator integer<signed long>()const
		noexcept(safe_convertible<signed long long, signed long>);
	template
		integer<signed long long>::operator integer<unsigned long>()const
		noexcept(safe_convertible<signed long long, unsigned long>);
	template
		integer<signed long long>::operator integer<unsigned long long>()const
		noexcept(safe_convertible<signed long long, unsigned long long>);

	template
		integer<unsigned long long>::operator integer<signed char>()const
		noexcept(safe_convertible<unsigned long long, signed char>);
	template
		integer<unsigned long long>::operator integer<unsigned char>()const
		noexcept(safe_convertible<unsigned long long, unsigned char>);
	template
		integer<unsigned long long>::operator integer<signed short>()const
		noexcept(safe_convertible<unsigned long long, signed short>);
	template
		integer<unsigned long long>::operator integer<unsigned short>()const
		noexcept(safe_convertible<unsigned long long, unsigned short>);
	template
		integer<unsigned long long>::operator integer<signed int>()const
		noexcept(safe_convertible<unsigned long long, signed int>);
	template
		integer<unsigned long long>::operator integer<unsigned int>()const
		noexcept(safe_convertible<unsigned long long, unsigned int>);
	template
		integer<unsigned long long>::operator integer<signed long>()const
		noexcept(safe_convertible<unsigned long long, signed long>);
	template
		integer<unsigned long long>::operator integer<unsigned long>()const
		noexcept(safe_convertible<unsigned long long, unsigned long>);
	template
		integer<unsigned long long>::operator integer<signed long long>()const
		noexcept(safe_convertible<unsigned long long, signed long long>);
}