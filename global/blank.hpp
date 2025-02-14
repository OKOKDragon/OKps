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
		比较两个对象的地址
		用于标准库容器的排序
		*/
		bool operator <(blank const & right)const noexcept;

	};

	class never final
	{
	private:
		never() noexcept;
	public:
		never(never const &) = delete;
		never(never &&) = delete;
		void operator =(never const &) = delete;
		void operator =(never &&) = delete;
	private:
		~never() noexcept;
	public:
		static never const object;
	};
}