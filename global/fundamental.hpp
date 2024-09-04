#pragma once

#include <type_traits>

#include ".\base.hpp"
#include ".\bits.hpp"

/*
���ļ�����һЩc++�����������͵İ�װ�࣬�Է�ֹ���������λ����ʱ��δ������Ϊ��

���ļ�Ҫ����c++20��׼�±��룬��Ϊc++20��׼�涨�������������Ͷ�Ҫ�ò�����ʽ��ʾ���Ӷ��淶�������������͵�ȡֵ��Χ��λ�������Ϊ��

���ļ��ṩ�������������������������ǿ�ƶ�δ������Ϊ����飬��Ӱ���������ܡ�
���а�װ��ĳ�ʼֵ����0��

�涨������ left �� right ֮������������㣬����������� left ��ͬ��right ���ȱ�ת������ left ��ͬ�����͡�
�����ת�����͹����У����� right ��ֵ�޷��� left �����ͱ�ʾ�����׳��쳣��

left �� right ���бȽϣ�Ҳ���� left ������Ϊ׼��right ���ȱ�ת������ left ��ͬ�����͡�

��c++��׼��ͬ���ǣ�Ϊ��ͳһ������������Ϊ�����ļ��ṩ�İ�װ����κ��������Ϊ���󣬶�c++��׼���޷�����������������ȡģ���㡣

c++��׼�涨��char ������ signed char �� unsigned char ֮һ������ͬ�ķ����ԡ���С�Ͷ��룬�����ǵ��������͡�
���ļ�רע�����㣬��ֻ�ṩ signed char �� unsigned char �İ�װ�࣬�������ǵ��� 1 �ֽڴ�С���������ͣ����ṩ char �İ�װ�ࡣ
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
	���� value_type ������ different_value_type ����ת��ʱ�Ƿ��п��������
	���ֵΪtrue���ʾ���������������Ӧ��ת��������������Ϊ noexcept(true)��
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
		static inline constexpr std::size_t const size = sizeof(value_type);
		static inline constexpr std::size_t const bit_length = size * bit_per_byte;
		static inline constexpr value_type max = std::numeric_limits<value_type>::max();
		static inline constexpr value_type min = std::numeric_limits<value_type>::min();

		integer(value_type const value = static_cast<value_type>(0))noexcept;
		integer(integer const&)noexcept;
		integer(integer&&) = delete;
		~integer()noexcept;

		void operator =(integer const&)noexcept;

		bool operator ==(integer const&)const noexcept;
		bool operator !=(integer const&)const noexcept;
		bool operator >=(integer const&)const noexcept;
		bool operator <=(integer const&)const noexcept;
		bool operator >(integer const&)const noexcept;
		bool operator <(integer const&)const noexcept;

		value_type& value()noexcept;
		value_type const& value()const noexcept;

		integer operator +(integer const&)const;
		integer operator -(integer const&)const;
		integer operator *(integer const&)const;
		integer operator /(integer const&)const;
		integer operator %(integer const&)const;
		void operator +=(integer const&);
		void operator -=(integer const&);
		void operator *=(integer const&);
		void operator /=(integer const&);
		void operator %=(integer const&);
		void operator ++();
		void operator --();
		integer operator -()const;
		integer operator ~()const noexcept;
		integer operator &(integer const&)const noexcept;
		integer operator |(integer const&)const noexcept;
		integer operator ^(integer const&)const noexcept;
		void operator &=(integer const&)noexcept;
		void operator |=(integer const&)noexcept;
		void operator ^=(integer const&)noexcept;
		integer operator <<(integer<std::size_t> const&)const;
		integer operator >>(integer<std::size_t> const&)const;
		void operator <<=(integer<std::size_t> const&);
		void operator >>=(integer<std::size_t> const&);

		template<different_arithmetic_integer<value_type> target_type>
		operator integer<target_type>()const
			noexcept(safe_convertible<value_type, target_type>);

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