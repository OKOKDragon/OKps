#include <stdexcept>
#include <typeinfo>

#include ".\fundamental.hpp"

namespace OKps::base
{
	template<arithmetic_integer value_type>
	integer<value_type>::integer(value_type const value)noexcept
		:MEMBER_value(value)
	{
	}
	template<arithmetic_integer value_type>
	integer<value_type>::integer(integer const & origin)noexcept
		:MEMBER_value(origin.MEMBER_value)
	{
	}
	template<arithmetic_integer value_type>
	integer<value_type>::~integer()noexcept
	{
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator =(integer const & origin)noexcept
	{
		if (this != (&origin))
		{
			this->MEMBER_value = origin.MEMBER_value;
		}
	}

	template<arithmetic_integer value_type>
	bool integer<value_type>::operator ==(integer const & right)const noexcept
	{
		return this->MEMBER_value == right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator !=(integer const & right)const noexcept
	{
		return this->MEMBER_value != right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator >=(integer const & right)const noexcept
	{
		return this->MEMBER_value >= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator <=(integer const & right)const noexcept
	{
		return this->MEMBER_value <= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator >(integer const & right)const noexcept
	{
		return this->MEMBER_value > right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	bool integer<value_type>::operator <(integer const & right)const noexcept
	{
		return this->MEMBER_value < right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	value_type const & integer<value_type>::value()const noexcept
	{
		return this->MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator +(integer const & right)const
	{
		if constexpr (std::is_signed_v<value_type>)
		{
			if (right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (integer::max - right.MEMBER_value < this->MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的加法 "
						+ std::to_string(this->MEMBER_value)
						+ " + "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::overflow_error(hint);
				}
			}
			else
			{
				if (integer::min - right.MEMBER_value > this->MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的加法 "
						+ std::to_string(this->MEMBER_value)
						+ " + "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::underflow_error(hint);
				}
			}
		}
		else
		{
			if (integer::max - this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的加法 "
					+ std::to_string(this->MEMBER_value)
					+ " + "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
				throw std::overflow_error(hint);
			}
		}
		return integer(this->MEMBER_value + right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator +=(integer const & right)
	{

		if constexpr (std::is_signed_v<value_type>)
		{
			if (right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (integer::max - right.MEMBER_value < this->MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的加法 "
						+ std::to_string(this->MEMBER_value)
						+ " + "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::overflow_error(hint);
				}
			}
			else
			{
				if (integer::min - right.MEMBER_value > this->MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的加法 "
						+ std::to_string(this->MEMBER_value)
						+ " + "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::underflow_error(hint);
				}
			}
		}
		else
		{
			if (integer::max - this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的加法 "
					+ std::to_string(this->MEMBER_value)
					+ " + "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
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
			std::string const hint = std::string("类型 ")
				+ typeid(value_type).name()
				+ " 的值 "
				+ std::to_string(this->MEMBER_value)
				+ " 自增运算溢出";
			throw std::overflow_error(hint);
		}
		++(this->MEMBER_value);
	}

	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator -(integer const & right)const
	{

		if constexpr (std::is_signed_v<value_type>)
		{
			if (right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (integer::min + right.MEMBER_value > this->MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的减法 "
						+ std::to_string(this->MEMBER_value)
						+ " - "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::underflow_error(hint);
				}
			}
			else
			{
				if (this->MEMBER_value > integer::max + right.MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的减法 "
						+ std::to_string(this->MEMBER_value)
						+ " - "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::overflow_error(hint);
				}
			}
		}
		else
		{
			if (this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的减法 "
					+ std::to_string(this->MEMBER_value)
					+ " - "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
				throw std::underflow_error(hint);
			}
		}
		return integer(this->MEMBER_value - right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator -=(integer const & right)
	{

		if constexpr (std::is_signed_v<value_type>)
		{
			if (right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (integer::min + right.MEMBER_value > this->MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的减法 "
						+ std::to_string(this->MEMBER_value)
						+ " - "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::underflow_error(hint);
				}
			}
			else
			{
				if (this->MEMBER_value > integer::max + right.MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的减法 "
						+ std::to_string(this->MEMBER_value)
						+ " - "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::overflow_error(hint);
				}
			}
		}
		else
		{
			if (this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的减法 "
					+ std::to_string(this->MEMBER_value)
					+ " - "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
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
			std::string const hint = std::string("类型 ")
				+ typeid(value_type).name()
				+ " 的值 "
				+ std::to_string(this->MEMBER_value)
				+ " 自减运算溢出";
			throw std::underflow_error(hint);
		}
		--(this->MEMBER_value);
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator *(integer const & right)const
	{
		if (this->MEMBER_value == static_cast<value_type>(0) or right.MEMBER_value == static_cast<value_type>(0))
		{
			return integer(static_cast<value_type>(0));
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value >= static_cast<value_type>(0) and right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (this->MEMBER_value > integer::max / right.MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的乘法 "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::overflow_error(hint);
				}
			}
			else if (this->MEMBER_value >= static_cast<value_type>(0) and right.MEMBER_value < static_cast<value_type>(0))
			{
				if (right.MEMBER_value < integer::min / this->MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的乘法 "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::underflow_error(hint);
				}
			}
			else if (this->MEMBER_value < static_cast<value_type>(0) and right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (this->MEMBER_value < integer::min / right.MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的乘法 "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::underflow_error(hint);
				}
			}
			else
			{
				if (this->MEMBER_value < integer::max / right.MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的乘法 "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::overflow_error(hint);
				}
			}
		}
		else
		{
			if (integer::max / this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的乘法 "
					+ std::to_string(this->MEMBER_value)
					+ " * "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
				throw std::overflow_error(hint);
			}
		}
		return integer(this->MEMBER_value * right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator *=(integer const & right)
	{
		if (this->MEMBER_value == static_cast<value_type>(0) or right.MEMBER_value == static_cast<value_type>(0))
		{
			this->MEMBER_value = static_cast<value_type>(0);
			return;
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value >= static_cast<value_type>(0) and right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (this->MEMBER_value > integer::max / right.MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的乘法 "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::overflow_error(hint);
				}
			}
			else if (this->MEMBER_value >= static_cast<value_type>(0) and right.MEMBER_value < static_cast<value_type>(0))
			{
				if (right.MEMBER_value < integer::min / this->MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的乘法 "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::underflow_error(hint);
				}
			}
			else if (this->MEMBER_value < static_cast<value_type>(0) and right.MEMBER_value >= static_cast<value_type>(0))
			{
				if (this->MEMBER_value < integer::min / right.MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的乘法 "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::underflow_error(hint);
				}
			}
			else
			{
				if (this->MEMBER_value < integer::max / right.MEMBER_value)
				{
					std::string const hint = std::string("类型 ")
						+ typeid(value_type).name()
						+ " 的乘法 "
						+ std::to_string(this->MEMBER_value)
						+ " * "
						+ std::to_string(right.MEMBER_value)
						+ " 溢出";
					throw std::overflow_error(hint);
				}
			}
		}
		else
		{
			if (integer::max / this->MEMBER_value < right.MEMBER_value)
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的乘法 "
					+ std::to_string(this->MEMBER_value)
					+ " * "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
				throw std::overflow_error(hint);
			}
		}
		this->MEMBER_value *= right.MEMBER_value;
	}

	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator /(integer const & right)const
	{

		if (right.MEMBER_value == static_cast<value_type>(0))
		{
			throw std::logic_error("除数不能为0");
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min and right.MEMBER_value == static_cast<value_type>(-1))
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的除法 "
					+ std::to_string(this->MEMBER_value)
					+ " / "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
				throw std::overflow_error(hint);
			}
		}
		return integer(this->MEMBER_value / right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator /=(integer const & right)
	{

		if (right.MEMBER_value == static_cast<value_type>(0))
		{
			throw std::logic_error("除数不能为0");
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min and right.MEMBER_value == static_cast<value_type>(-1))
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的除法 "
					+ std::to_string(this->MEMBER_value)
					+ " / "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
				throw std::overflow_error(hint);
			}
		}
		this->MEMBER_value /= right.MEMBER_value;
	}

	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator %(integer const & right)const
	{

		if (right.MEMBER_value == static_cast<value_type>(0))
		{
			throw std::logic_error("除数不能为0");
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min and right.MEMBER_value == static_cast<value_type>(-1))
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的除法 "
					+ std::to_string(this->MEMBER_value)
					+ " / "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
				throw std::overflow_error(hint);
			}
		}
		return integer(this->MEMBER_value % right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator %=(integer const & right)
	{

		if (right.MEMBER_value == static_cast<value_type>(0))
		{
			throw std::logic_error("除数不能为0");
		}
		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min and right.MEMBER_value == static_cast<value_type>(-1))
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的除法 "
					+ std::to_string(this->MEMBER_value)
					+ " / "
					+ std::to_string(right.MEMBER_value)
					+ " 溢出";
				throw std::overflow_error(hint);
			}
		}
		this->MEMBER_value %= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator -()const
	{

		if constexpr (std::is_signed_v<value_type>)
		{
			if (this->MEMBER_value == integer::min)
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的值 "
					+ std::to_string(this->MEMBER_value)
					+ " 取相反数的结果溢出";
				throw std::overflow_error(hint);
			}
			return integer(-(this->MEMBER_value));
		}
		else
		{
			if (this->MEMBER_value != static_cast<value_type>(0))
			{
				std::string const hint = std::string("类型 ")
					+ typeid(value_type).name()
					+ " 的值 "
					+ std::to_string(this->MEMBER_value)
					+ " 取相反数的结果溢出";
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
	integer<value_type> integer<value_type>::operator &(integer const & right)const noexcept
	{
		return integer(this->MEMBER_value & right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator &=(integer const & right)noexcept
	{
		this->MEMBER_value &= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator |(integer const & right)const noexcept
	{
		return integer(this->MEMBER_value | right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator |=(integer const & right)noexcept
	{
		this->MEMBER_value |= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator ^(integer const & right)const noexcept
	{
		return integer(this->MEMBER_value ^ right.MEMBER_value);
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator ^=(integer const & right)noexcept
	{
		this->MEMBER_value ^= right.MEMBER_value;
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator <<(integer<std::size_t> const & shift)const
	{

		if (shift.value() >= integer::bit_length)
		{
			std::string const hint = std::string("类型 ")
				+ typeid(value_type).name()
				+ " 的左移 "
				+ std::to_string(shift.value())
				+ " 位运算的位数超出合法范围";
			throw std::logic_error(hint);
		}
		return integer(this->MEMBER_value << shift.value());
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator <<=(integer<std::size_t> const & shift)
	{

		if (shift.value() >= integer::bit_length)
		{
			std::string const hint = std::string("类型 ")
				+ typeid(value_type).name()
				+ " 的左移 "
				+ std::to_string(shift.value())
				+ " 位运算的位数超出合法范围";
			throw std::logic_error(hint);
		}
		this->MEMBER_value <<= shift.value();
	}
	template<arithmetic_integer value_type>
	integer<value_type> integer<value_type>::operator >>(integer<std::size_t> const & shift)const
	{

		if (shift.value() >= integer::bit_length)
		{
			std::string const hint = std::string("类型 ")
				+ typeid(value_type).name()
				+ " 的右移 "
				+ std::to_string(shift.value())
				+ " 位运算的位数超出合法范围";
			throw std::logic_error(hint);
		}
		return integer(this->MEMBER_value >> shift.value());
	}
	template<arithmetic_integer value_type>
	void integer<value_type>::operator >>=(integer<std::size_t> const & shift)
	{

		if (shift.value() >= integer::bit_length)
		{
			std::string const hint = std::string("类型 ")
				+ typeid(value_type).name()
				+ " 的右移 "
				+ std::to_string(shift.value())
				+ " 位运算的位数超出合法范围";
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
			//当前类型和目标类型都有符号
		{
			if constexpr (sizeof(value_type) <= sizeof(target_type))
				//目标类型比当前类型大
			{
				//不会产生溢出
			}
			else
				//目标类型比当前类型小
			{
				if (this->MEMBER_value > static_cast<value_type>(integer<target_type>::max))
				{
					std::string const hint = std::string("从类型 ")
						+ typeid(value_type).name()
						+ " 的值 "
						+ std::to_string(this->MEMBER_value)
						+ " 转换到类型 "
						+ typeid(target_type).name()
						+ " 时发生溢出";
					throw std::overflow_error(hint);
				}
				else if (this->MEMBER_value < static_cast<value_type>(integer<target_type>::min))
				{
					std::string const hint = std::string("从类型 ")
						+ typeid(value_type).name()
						+ " 的值 "
						+ std::to_string(this->MEMBER_value)
						+ " 转换到类型 "
						+ typeid(target_type).name()
						+ " 时发生溢出";
					throw std::underflow_error(hint);
				}
			}
		}
		else if constexpr (std::is_unsigned_v<value_type> and std::is_unsigned_v<target_type>)
			//当前类型和目标类型都无符号
		{
			if constexpr (sizeof(value_type) <= sizeof(target_type))
				//目标类型比当前类型大
			{
				//不会产生溢出
			}
			else
				//目标类型比当前类型小
			{
				if (this->MEMBER_value > static_cast<value_type>(integer<target_type>::max))
				{
					std::string const hint = std::string("从类型 ")
						+ typeid(value_type).name()
						+ " 的值 "
						+ std::to_string(this->MEMBER_value)
						+ " 转换到类型 "
						+ typeid(target_type).name()
						+ " 时发生溢出";
					throw std::overflow_error(hint);
				}
			}
		}
		else if constexpr (std::is_signed_v<value_type> and std::is_unsigned_v<target_type>)
			//有符号类型向无符号类型转换
		{
			if (this->MEMBER_value < static_cast<value_type>(0))
			{
				std::string const hint = std::string("从类型 ")
					+ typeid(value_type).name()
					+ " 的值 "
					+ std::to_string(this->MEMBER_value)
					+ " 转换到类型 "
					+ typeid(target_type).name()
					+ " 时发生溢出";
				throw std::underflow_error(hint);
			}
			if constexpr (integer::bit_length <= integer<target_type>::bit_length + static_cast<std::size_t>(1))
				//当前类型的最大值 <= 目标类型的最大值
			{
				//不会产生溢出
			}
			else
			{
				if (this->MEMBER_value > static_cast<value_type>(integer<target_type>::max))
				{
					std::string const hint = std::string("从类型 ")
						+ typeid(value_type).name()
						+ " 的值 "
						+ std::to_string(this->MEMBER_value)
						+ " 转换到类型 "
						+ typeid(target_type).name()
						+ " 时发生溢出";
					throw std::overflow_error(hint);
				}
			}
		}
		else
			//无符号类型向有符号类型转换
		{
			if constexpr (integer<target_type>::bit_length >= integer::bit_length + static_cast<std::size_t>(1))
				//当前类型的最大值 <= 目标类型的最大值
			{
				//不会产生溢出
			}
			else
			{
				if (this->MEMBER_value > static_cast<value_type>(integer<target_type>::max))
				{
					std::string const hint = std::string("从类型 ")
						+ typeid(value_type).name()
						+ " 的值 "
						+ std::to_string(this->MEMBER_value)
						+ " 转换到类型 "
						+ typeid(target_type).name()
						+ " 时发生溢出";
					throw std::overflow_error(hint);
				}
			}
		}
		return integer<target_type>(static_cast<target_type>(this->MEMBER_value));
	}

}
