#pragma once

namespace OKps
{
	/*
	空类
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
		比较两个对象是否是同一个对象
		即比较 this 和 &right 是否相同
		*/
		bool operator ==(blank const & right)const noexcept;
		bool operator !=(blank const & right)const noexcept;
	};
}