#include <stdexcept>
#include <typeinfo>

#include ".\fundamental.hpp"

namespace OKps::base
{
	template<arithmetic_integer value_type>
	integer<value_type>::integer(value_type const value)noexcept
		:MEMBER_value(value)
	{}
	template<arithmetic_integer value_type>
	integer<value_type>::integer(integer const& origin)noexcept
		:MEMBER_value(origin.MEMBER_value)
	{}
	template<arithmetic_integer value_type>
	integer<value_type>::~integer()noexcept
	{}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator =(integer const& origin)noexcept
	{
		this->MEMBER_value = origin.MEMBER_value;
	}

	template<arithmetic_integer value_type>
	bool integer<value_type>::operator ==(integer const& right)const noexcept
	{
		return this->MEMBER_value == right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator !=(integer const& right)const noexcept
	{
		return this->MEMBER_value != right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator >=(integer const& right)const noexcept
	{
		return this->MEMBER_value >= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator <=(integer const& right)const noexcept
	{
		return this->MEMBER_value <= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator >(integer const& right)const noexcept
	{
		return this->MEMBER_value > right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator <(integer const& right)const noexcept
	{
		return this->MEMBER_value < right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	value_type& integer<value_type>::value()noexcept
	{
		return this->MEMBER_value;
	}
	template<arithmetic_integer value_type>
	value_type const& integer<value_type>::value()const noexcept
	{
		return this->MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator +(integer const& right)const
	{
		if constexpr (std::is_signed_v<value_type>)
		{
			if (right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (integer::max - right.MEMBER_value < this->MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ļӷ� "
						+ std::to_string(this->MEMBER_value)
						+ " + "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::overflow_error(hint);
				}
			}
			else
			{
				if (integer::min - right.MEMBER_value > this->MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ļӷ� "
						+ std::to_string(this->MEMBER_value)
						+ " + "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::underflow_error(hint);
				}
			}
		}
		else
		{
			if (integer::max - this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ļӷ� "
					+ std::to_string(this->MEMBER_value)
					+ " + "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::overflow_error(hint);
			}		
		}
		return integer(this->MEMBER_value + right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator +=(integer const& right)
	{

		if constexpr (std::is_signed_v<value_type>)
		{
			if (right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (integer::max - right.MEMBER_value < this->MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ļӷ� "
						+ std::to_string(this->MEMBER_value)
						+ " + "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::overflow_error(hint);
				}
			}
			else
			{
				if (integer::min - right.MEMBER_value > this->MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ļӷ� "
						+ std::to_string(this->MEMBER_value)
						+ " + "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::underflow_error(hint);
				}
			}
		}
		else
		{
			if (integer::max - this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ļӷ� "
					+ std::to_string(this->MEMBER_value)
					+ " + "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::overflow_error(hint);
			}
		}
		this->MEMBER_value += right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator ++()
	{

		if (integer::max - static_cast<value_type>(1) < this->MEMBER_value)
		{
			std::string const hint = std::string("���� ")
				+ typeid(value_type).name()
				+ " ��ֵ "
				+ std::to_string(this->MEMBER_value)
				+ " �����������";
			throw std::overflow_error(hint);
		}	
		++(this->MEMBER_value);
	}
	
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator -(integer const& right)const
	{

		if constexpr (std::is_signed_v<value_type>)
		{ 
			if (right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (integer::min + right.MEMBER_value > this->MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ļ��� "
						+ std::to_string(this->MEMBER_value)
						+ " - "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::underflow_error(hint);
				}
			}
			else
			{
				if (this->MEMBER_value > integer::max + right.MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ļ��� "
						+ std::to_string(this->MEMBER_value)
						+ " - "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::overflow_error(hint);
				}
			}
		}
		else
		{
			if (this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ļ��� "
					+ std::to_string(this->MEMBER_value)
					+ " - "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::underflow_error(hint);
			}
		}
		return integer(this->MEMBER_value - right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator -=(integer const& right)
	{

		if constexpr (std::is_signed_v<value_type>)
		{
			if (right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (integer::min + right.MEMBER_value > this->MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ļ��� "
						+ std::to_string(this->MEMBER_value)
						+ " - "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::underflow_error(hint);
				}
			}
			else
			{
				if (this->MEMBER_value > integer::max + right.MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ļ��� "
						+ std::to_string(this->MEMBER_value)
						+ " - "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::overflow_error(hint);
				}
			}
		}
		else
		{
			if (this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ļ��� "
					+ std::to_string(this->MEMBER_value)
					+ " - "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::underflow_error(hint);
			}
		}
		this->MEMBER_value -= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator --()
	{

		if (integer::min + static_cast<value_type>(1) > this->MEMBER_value)
		{
			std::string const hint = std::string("���� ")
				+ typeid(value_type).name()
				+ " ��ֵ "
				+ std::to_string(this->MEMBER_value)
				+ " �Լ��������";
			throw std::underflow_error(hint);
		}
		--(this->MEMBER_value);
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator *(integer const& right)const
	{

		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value >= static_cast<value_type>(0) and right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (this->MEMBER_value > integer::max / right.MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ĳ˷� "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::overflow_error(hint);
				}
			}
			else if (this->MEMBER_value >= static_cast<value_type>(0) and right.MEMBER_value < static_cast<value_type>(0))
			{
				if (right.MEMBER_value < integer::min / this->MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ĳ˷� "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::underflow_error(hint);
				}
			}
			else if (this->MEMBER_value < static_cast<value_type>(0) and right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (this->MEMBER_value < integer::min / right.MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ĳ˷� "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::underflow_error(hint);
				}
			}
			else
			{
				if (this->MEMBER_value < integer::max / right.MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ĳ˷� "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::overflow_error(hint);
				}
			}
		}
		else
		{
			if (integer::max / this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ĳ˷� "
					+ std::to_string(this->MEMBER_value)
					+ " * "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::overflow_error(hint);
			}
		}
		return integer(this->MEMBER_value * right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator *=(integer const& right)
	{

		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value >= static_cast<value_type>(0) and right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (this->MEMBER_value > integer::max / right.MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ĳ˷� "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::overflow_error(hint);
				}
			}
			else if (this->MEMBER_value >= static_cast<value_type>(0) and right.MEMBER_value < static_cast<value_type>(0))
			{
				if (right.MEMBER_value < integer::min / this->MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ĳ˷� "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::underflow_error(hint);
				}
			}
			else if (this->MEMBER_value < static_cast<value_type>(0) and right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (this->MEMBER_value < integer::min / right.MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ĳ˷� "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::underflow_error(hint);
				}
			}
			else
			{
				if (this->MEMBER_value < integer::max / right.MEMBER_value)
				{
					std::string const hint = std::string("���� ")
						+ typeid(value_type).name()
						+ " �ĳ˷� "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " ���";
					throw std::overflow_error(hint);
				}
			}
		}
		else
		{
			if (integer::max / this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ĳ˷� "
					+ std::to_string(this->MEMBER_value)
					+ " * "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::overflow_error(hint);
			}
		}
		this->MEMBER_value *= right.MEMBER_value;
	}

	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator /(integer const& right)const
	{

		if (right.MEMBER_value == static_cast<value_type>(0))
		{
			throw std::logic_error("��������Ϊ0");
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min and right.MEMBER_value == static_cast<value_type>(-1))
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ĳ��� "
					+ std::to_string(this->MEMBER_value)
					+ " / "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::overflow_error(hint);
			}
		}
		return integer(this->MEMBER_value / right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator /=(integer const& right)
	{

		if (right.MEMBER_value == static_cast<value_type>(0))
		{
			throw std::logic_error("��������Ϊ0");
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min and right.MEMBER_value == static_cast<value_type>(-1))
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ĳ��� "
					+ std::to_string(this->MEMBER_value)
					+ " / "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::overflow_error(hint);
			}
		}
		this->MEMBER_value /= right.MEMBER_value;
	}

	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator %(integer const& right)const
	{

		if (right.MEMBER_value == static_cast<value_type>(0))
		{
			throw std::logic_error("��������Ϊ0");
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min and right.MEMBER_value == static_cast<value_type>(-1))
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ĳ��� "
					+ std::to_string(this->MEMBER_value)
					+ " / "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::overflow_error(hint);
			}
		}
		return integer(this->MEMBER_value % right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator %=(integer const& right)
	{

		if (right.MEMBER_value == static_cast<value_type>(0))
		{
			throw std::logic_error("��������Ϊ0");
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min and right.MEMBER_value == static_cast<value_type>(-1))
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " �ĳ��� "
					+ std::to_string(this->MEMBER_value)
					+ " / "
					+ std::to_string(right.MEMBER_value)
					+ " ���";
				throw std::overflow_error(hint);
			}
		}
		this->MEMBER_value %= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator -()const
	{

		if constexpr(std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min)
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " ��ֵ "
					+ std::to_string(this->MEMBER_value)
					+ " ȡ�෴���Ľ�����";
				throw std::overflow_error(hint);
			}
			return integer(-(this->MEMBER_value));
		}
		else
		{
			if (this->MEMBER_value != static_cast<value_type>(0))
			{
				std::string const hint = std::string("���� ")
					+ typeid(value_type).name()
					+ " ��ֵ "
					+ std::to_string(this->MEMBER_value)
					+ " ȡ�෴���Ľ�����";
				throw std::underflow_error(hint);
			}
			return integer();
		}
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator ~()const noexcept
	{
		return integer(~(this->MEMBER_value));
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator &(integer const& right)const noexcept
	{
		return integer(this->MEMBER_value & right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator &=(integer const& right)noexcept
	{
		this->MEMBER_value &= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator |(integer const& right)const noexcept
	{
		return integer(this->MEMBER_value | right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator |=(integer const& right)noexcept
	{
		this->MEMBER_value |= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator ^(integer const& right)const noexcept
	{
		return integer(this->MEMBER_value ^ right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator ^=(integer const& right)noexcept
	{
		this->MEMBER_value ^= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator <<(integer<std::size_t> const& shift)const
	{

		if (shift.value() >= integer::bit_length)
		{
			std::string const hint = std::string("���� ")
				+ typeid(value_type).name()
				+ " ������ "
				+ std::to_string(shift.value())
				+ " λ�����λ�������Ϸ���Χ";
			throw std::logic_error(hint);
		}
		return integer(this->MEMBER_value << shift.value());
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator <<=(integer<std::size_t> const& shift)
	{

		if (shift.value() >= integer::bit_length)
		{
			std::string const hint = std::string("���� ")
				+ typeid(value_type).name()
				+ " ������ "
				+ std::to_string(shift.value())
				+ " λ�����λ�������Ϸ���Χ";
			throw std::logic_error(hint);
		}
		this->MEMBER_value <<= shift.value();
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator >>(integer<std::size_t> const& shift)const
	{

		if (shift.value() >= integer::bit_length)
		{
			std::string const hint = std::string("���� ")
				+ typeid(value_type).name()
				+ " ������ "
				+ std::to_string(shift.value())
				+ " λ�����λ�������Ϸ���Χ";
			throw std::logic_error(hint);
		}
		return integer(this->MEMBER_value >> shift.value());
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator >>=(integer<std::size_t> const& shift)
	{

		if (shift.value() >= integer::bit_length)
		{
			std::string const hint = std::string("���� ")
				+ typeid(value_type).name()
				+ " ������ "
				+ std::to_string(shift.value())
				+ " λ�����λ�������Ϸ���Χ";
			throw std::logic_error(hint);
		}
		this->MEMBER_value >>= shift.value();
	}
	template<arithmetic_integer value_type>
	template<different_arithmetic_integer<value_type> target_type>
	integer<value_type>::operator integer<target_type>()const
		noexcept(safe_convertible<value_type, target_type>)
	{

		if constexpr (std::is_signed_v<value_type> and std::is_signed_v<target_type>)
			//��ǰ���ͺ�Ŀ�����Ͷ��з���
		{
			if constexpr (sizeof(value_type) <= sizeof(target_type))
				//Ŀ�����ͱȵ�ǰ���ʹ�
			{
				//����������
			}
			else
				//Ŀ�����ͱȵ�ǰ����С
			{
				if (this->MEMBER_value > static_cast<value_type>(integer<target_type>::max))
				{
					std::string const hint = std::string("������ ")
						+ typeid(value_type).name()
						+ " ��ֵ "
						+ std::to_string(this->MEMBER_value)
						+ " ת�������� "
						+ typeid(target_type).name()
						+ " ʱ�������";
					throw std::overflow_error(hint);
				}
				else if (this->MEMBER_value < static_cast<value_type>(integer<target_type>::min))
				{
					std::string const hint = std::string("������ ")
						+ typeid(value_type).name()
						+ " ��ֵ "
						+ std::to_string(this->MEMBER_value)
						+ " ת�������� "
						+ typeid(target_type).name()
						+ " ʱ�������";
					throw std::underflow_error(hint);
				}
			}
		}
		else if constexpr (std::is_unsigned_v<value_type> and std::is_unsigned_v<target_type>)
			//��ǰ���ͺ�Ŀ�����Ͷ��޷���
		{
			if constexpr (sizeof(value_type) <= sizeof(target_type))
				//Ŀ�����ͱȵ�ǰ���ʹ�
			{
				//����������
			}
			else
				//Ŀ�����ͱȵ�ǰ����С
			{
				if (this->MEMBER_value > static_cast<value_type>(integer<target_type>::max))
				{
					std::string const hint = std::string("������ ")
						+ typeid(value_type).name()
						+ " ��ֵ "
						+ std::to_string(this->MEMBER_value)
						+ " ת�������� "
						+ typeid(target_type).name()
						+ " ʱ�������";
					throw std::overflow_error(hint);
				}
			}
		}
		else if constexpr (std::is_signed_v<value_type> and std::is_unsigned_v<target_type>)
			//�з����������޷�������ת��
		{
			if (this->MEMBER_value < static_cast<value_type>(0))
			{
				std::string const hint = std::string("������ ")
					+ typeid(value_type).name()
					+ " ��ֵ "
					+ std::to_string(this->MEMBER_value)
					+ " ת�������� "
					+ typeid(target_type).name()
					+ " ʱ�������";
				throw std::underflow_error(hint);
			}
			if constexpr (integer::bit_length <= integer<target_type>::bit_length + static_cast<std::size_t>(1))
				//��ǰ���͵����ֵ <= Ŀ�����͵����ֵ
			{
				//����������
			}
			else
			{
				if (this->MEMBER_value > static_cast<value_type>(integer<target_type>::max))
				{
					std::string const hint = std::string("������ ")
						+ typeid(value_type).name()
						+ " ��ֵ "
						+ std::to_string(this->MEMBER_value)
						+ " ת�������� "
						+ typeid(target_type).name()
						+ " ʱ�������";
					throw std::overflow_error(hint);
				}
			}
		}
		else
			//�޷����������з�������ת��
		{
			if constexpr (integer<target_type>::bit_length >= integer::bit_length + static_cast<std::size_t>(1))
				//��ǰ���͵����ֵ <= Ŀ�����͵����ֵ
			{
				//����������
			}
			else
			{
				if (this->MEMBER_value > static_cast<value_type>(integer<target_type>::max))
				{
					std::string const hint = std::string("������ ")
						+ typeid(value_type).name()
						+ " ��ֵ "
						+ std::to_string(this->MEMBER_value)
						+ " ת�������� "
						+ typeid(target_type).name()
						+ " ʱ�������";
					throw std::overflow_error(hint);
				}
			}
		}
		return integer<target_type>(static_cast<target_type>(this->MEMBER_value));
	}

}
