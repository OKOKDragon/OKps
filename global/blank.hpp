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
	};
}