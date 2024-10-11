#pragma once

namespace OKps
{
	/*
	空类
	它的赋值运算符什么都不做
	*/
	class blank final
	{
	public:
		blank()noexcept;
		blank(blank const &)noexcept;
		blank(blank &&)noexcept;
		~blank()noexcept;
		void operator =(blank const &)noexcept;
		void operator =(blank &&)noexcept;
		bool operator <(blank const & right)const noexcept;

	};


}