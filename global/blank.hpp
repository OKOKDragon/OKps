#pragma once

namespace OKps
{
	/*
	����
	*/
	class blank final
	{
	public:
		blank()noexcept;
		blank(blank const&)noexcept;
		blank(blank&&)noexcept;
		~blank()noexcept;
		void operator =(blank const&)noexcept;
		void operator =(blank&&)noexcept;
		/*
		�Ƚ����������Ƿ���ͬһ������
		���Ƚ� this �� &right �Ƿ���ͬ
		*/
		bool operator ==(blank const& right)const noexcept;
		bool operator !=(blank const& right)const noexcept;
	};
}