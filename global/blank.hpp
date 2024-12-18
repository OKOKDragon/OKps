#pragma once

namespace OKps
{
	/*
	空类
	它的赋值运算符什么都不做

	此类可以用作占位符
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
		/*
		比较两个对象的地址
		用于标准库容器的排序
		*/
		bool operator <(blank const & right)const noexcept;

	};


}