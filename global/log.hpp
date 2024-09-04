#pragma once

#include <source_location>
#include <string>

namespace OKps
{
	/*
	���������־
	��1������λ����Ϣ��1���ַ�����ʾ��Ϣ���
	*/
	class log final
	{
	private:
		std::source_location MEMBER_location;
		std::string MEMBER_hint;
		bool MEMBER_valid;
	public:
		log(std::string const& hint, std::source_location const& location = std::source_location::current());
		log(log const& origin);
		log(log&& origin)
			noexcept(std::is_nothrow_move_constructible_v<std::source_location>
				and std::is_nothrow_move_constructible_v<std::string>);
		~log()
			noexcept(std::is_nothrow_destructible_v<std::string>
				and std::is_nothrow_destructible_v<std::source_location>);
		std::string const& hint()const;
		std::source_location const& location()const;
	};
}