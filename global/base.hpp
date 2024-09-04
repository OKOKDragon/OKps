#pragma once

#include <thread>
#include <exception>
#include <type_traits>
#include <string>
#include <random>
#include <memory>

/*
���ļ��������ռ� OKps::base �������ͨ�õĻ���
���඼��self����������ֵ�ǵ���������ã���{return *this;}
����� == �� != ���������صĽ���ǱȽ����������Ƿ���ͬһ�����󣬼��Ƚ���������ȡ��ַ�Ľ���Ƿ���ͬ
*/

namespace OKps::base
{
	/*
	�ջ���
	*/
	class blank
	{
	protected:
		blank()noexcept;
		blank(blank const&)noexcept;
		virtual ~blank()noexcept;
		blank(blank &&)noexcept;
		virtual void operator =(blank&&)noexcept;
		virtual void operator =(blank const&)noexcept;
		virtual bool operator ==(blank const&)const noexcept;
		virtual bool operator !=(blank const&)const noexcept;
		virtual blank& self()noexcept;
		virtual blank const& self()const noexcept;

	};
	/*
	�˻�����һ��bool ���ͱ�־��true���������Ч��false���������Ч
	����Ĭ�Ϲ���ʱΪ��Ч���󣬱��ƶ�����ʶΪ��Ч����
	*/
	class move_invalid
	{
	private:
		bool MEMBER_valid;
	protected:
		bool const& is_valid()const noexcept;
		void raise_invalid_error()const noexcept(false);
		move_invalid()noexcept;
		move_invalid(move_invalid const&)noexcept;
		virtual void operator =(move_invalid const&)noexcept;
		move_invalid(move_invalid&&)noexcept;
		virtual void operator =(move_invalid &&)noexcept;
		virtual ~move_invalid()noexcept;
		virtual bool operator ==(move_invalid const&)const noexcept;
		virtual bool operator !=(move_invalid const&)const noexcept;
		virtual move_invalid& self()noexcept;
		virtual move_invalid const& self()const noexcept;

	};
	/*
	����һ���޷���������Ϊ��ŵĻ���
	*/
	class numbered
	{
	private:
		class id_generator final
		{
		private:
			std::random_device MEMBER_seed;
			std::unique_ptr<std::mt19937_64> MEMBER_engine;
			std::uniform_int_distribution<std::uintmax_t> MEMBER_generator;
		public:
			id_generator();
			~id_generator()
				noexcept(std::is_nothrow_destructible_v<std::random_device>
					and std::is_nothrow_destructible_v<std::unique_ptr<std::mt19937_64>>
					and std::is_nothrow_destructible_v<std::uniform_int_distribution<std::uintmax_t>>);
			id_generator(id_generator const&) = delete;
			id_generator(id_generator&&) = delete;
			void operator =(id_generator const&) = delete;
			void operator =(id_generator&&) = delete;

			std::uintmax_t generate()
				noexcept(noexcept(std::declval<std::uniform_int_distribution<std::uintmax_t>>()(*std::declval<std::unique_ptr<std::mt19937_64>>())));
		};
		static id_generator MEMBER_id_generator;
	protected:
		static std::uintmax_t generate_random_id()
			noexcept(noexcept(std::declval<id_generator>().generate()));
	private:
		std::uintmax_t MEMBER_id;
	protected:
		numbered(std::uintmax_t const id=0)noexcept;
		numbered(numbered const&)noexcept;
		virtual void operator =(numbered const&)noexcept;
		numbered(numbered&&)noexcept = delete;
		virtual void operator =(numbered&&)noexcept = delete;
		virtual ~numbered()noexcept;
		virtual bool operator ==(numbered const&)const noexcept;
		virtual bool operator !=(numbered const&)const noexcept;
		std::uintmax_t& id()noexcept;
		std::uintmax_t const& id()const noexcept;
		virtual numbered& self()noexcept;
		virtual numbered const& self()const noexcept;
	};
	/*
	����һ���ַ�����Ϊ��ʶ�Ļ���
	*/
	class named
	{
	private:
		std::string MEMBER_name;
	protected:
		named(std::string const& )
			noexcept(std::is_nothrow_copy_constructible<std::string>::value);
		named(std::string && = std::string())
			noexcept(std::is_nothrow_move_constructible<std::string>::value);
		named(named const&)
			noexcept(std::is_nothrow_copy_constructible<std::string>::value);
		virtual void operator =(named const&)
			noexcept(std::is_nothrow_copy_assignable<std::string>::value);
		named(named&&)
			noexcept(std::is_nothrow_move_constructible<std::string>::value);
		virtual void operator =(named &&)
			noexcept(std::is_nothrow_move_assignable<std::string>::value);
		virtual ~named()
			noexcept(std::is_nothrow_destructible<std::string>::value);
		std::string const& name()const noexcept;
		std::string& name()noexcept;
		virtual bool operator ==(named const&)const noexcept;
		virtual bool operator !=(named const&)const noexcept;
		virtual named& self()noexcept;
		virtual named const& self()const noexcept;
	};
	/*
	��������
	�Ϳջ���һ������ר������������Ĳ���
	*/
	class parameter
	{
	protected:
		parameter()noexcept;
		parameter(parameter const&)noexcept;
		virtual ~parameter()noexcept;
		parameter(parameter&&)noexcept;
		virtual void operator =(parameter&&)noexcept;
		virtual void operator =(parameter const&)noexcept;
		virtual bool operator ==(parameter const&)const noexcept;
		virtual bool operator !=(parameter const&)const noexcept;
		virtual parameter& self()noexcept;
		virtual parameter const& self()const noexcept;
	};
	/*
	����������
	���������ص������ı�׼���ͣ������� lambda ���ģ�����ʽ
	*/
	class handler
	{
	protected:
		handler()noexcept;
		handler(handler const&)noexcept;
		virtual ~handler()noexcept;
		handler(handler&&)noexcept;
		virtual void operator =(handler&&)noexcept;
		virtual void operator =(handler const&)noexcept;
		virtual bool operator ==(handler const&)const noexcept;
		virtual bool operator !=(handler const&)const noexcept;
		virtual handler& self()noexcept;
		virtual handler const& self()const noexcept;
		virtual void handle(std::unique_ptr<parameter> const&) = 0;
	};
	/*
	�̹߳����߻���

	�������һ���̣߳����̵߳��̺߳�����work������work�����Ǵ��麯��������������ʵ��
	work������ֹ�׳��쳣�������Ҫʹ���쳣����ʹ�ô˻����ṩ�ĺ��������쳣�洢�ڻ�����
	*/
	class worker
	{
	private:
		std::thread MEMBER_thread;
		std::exception_ptr MEMBER_error;
	protected:
		worker()
			noexcept(std::is_nothrow_default_constructible<std::thread>::value
				and std::is_nothrow_default_constructible<std::exception_ptr>::value);
		worker(worker const&) = delete;
		virtual void operator =(worker const&) = delete;
		worker(worker&&)
			noexcept(std::is_nothrow_move_constructible<std::thread>::value
				and std::is_nothrow_copy_constructible<std::exception_ptr>::value);
		virtual void operator =(worker&&)
			noexcept(std::is_nothrow_move_assignable<std::thread>::value
				and std::is_nothrow_copy_assignable<std::exception_ptr>::value);
		virtual ~worker()
			noexcept(std::is_nothrow_destructible<std::thread>::value
				and std::is_nothrow_destructible<std::exception_ptr>::value);
		virtual worker& self()noexcept;
		virtual worker const& self()const noexcept;
		virtual void work()noexcept = 0;
		std::thread& thread()noexcept;
		std::thread const& thread()const noexcept;
		std::exception_ptr const& have_error()const noexcept;
		/*
		���쳣�� std::exception_ptr ����ʽ�洢�ڻ�����
		*/
		void raise_error(std::exception_ptr const&);
		/*
		���洢�ڻ����е��쳣�׳�
		*/
		[[noreturn]] void release_error()noexcept(false);
    	virtual bool operator ==(worker const&)const noexcept;
		virtual bool operator !=(worker const&)const noexcept;
	};
}