#pragma once

#include <string>

/*
此文件定义异常格式
*/

namespace OKps
{

	class error
	{

	public:
		/*
		错误提示
		展示给人的可阅读信息
		*/
		const std::string hint;

		/*
		一般来说，异常应该既输出到控制台，以让用户实时了解程序运行状态，又输出到日志文件，以作记录
		当然，这2个模块的工作都需要内存
		但是，如果异常发生在其中的1个模块，那么相应的异常处理功能可能无法运行；如果有内存异常，可能任何异常处理功能都不能运行
		所以首先要确定的就是这3个模块的状态
		*/

		/*
		异常的具体类型
		DEBUG_ 开头的的错误类型，是绝对不应该在正式版本中出现的，应该在调试时全部解决的
		它们通常是数组越界、参数不合法等比较简单的错误
		*/
		enum class error_type
		{

			/*
			堆内存分配失败
			仅用于自定义new操作符
			*/
			MEMORY_heap,
			/*
			c++语言内置异常
			例如内置new操作符
			*/
			CPP_build_in,

			/*
			越界访问
			*/
			DEBUG_overstep,
			/*
			编译期可以检查的错误参数
			*/
			DEBUG_bad_parameter,
			/*
			运行时的错误参数
			*/
			RUNTIME_bad_parameter,
			/*
			逻辑错误
			例如，让一个没有打开文件的文件对象关闭文件
			*/
			DEBUG_logic,
			/*
			对空指针解引用
			或访问空对象、无效对象
			或访问野指针、悬垂引用、空引用
			*/
			DEBUG_invalid_object,
			/*
			与真实文件交互时失败
			例如在fastIO模块中刷新缓冲区时
			*/
			IO_file,
			// 操作系统提供的接口抛出异常
			OS,
			/*
			线程问题
			*/
			DEBUG_thread,
			// 未知类型异常
			unknown
		};

		// 异常的具体类型
		const error_type type;

		/*
		禁止用new构造本类，如果有内存相关异常，此时再申请堆内存，无疑对程序和整个系统都是雪上加霜
		我们只能寄希望于异常发生的时候，进程的栈内存还能读取
		*/
		void * operator new(size_t size) = delete;
		void operator delete(void * block) noexcept = delete;
		void * operator new[](size_t size) = delete;
		void operator delete[](void * block) noexcept = delete;
		void operator delete(void *, void *) noexcept = delete;
		void operator delete[](void *, void *) noexcept = delete;
		void * operator new(std::size_t count, void * ptr) = delete;
		void * operator new[](std::size_t count, void * ptr) = delete;

		error() = delete;

		// information(const ::std::string &hint, const bool memory, const bool console, const bool fastIO, const error_type type);

		error(const std::string & hint, const error_type type);

		error(const error & origin);
		error(error && origin) noexcept;
	};

	/*
	包含外部异常的OKps异常类型
	例如，当某个标准库容器发生越界访问异常，标准库会抛出其自定义异常
	则用此类接收该异常，此类会将该异常和OKps异常信息封装到一起
	*/
	template <typename error_type> /*外部异常类型*/
	class bind_error final
	{
	public:
		const error OKps_error_info;
		const error_type external_error;

		void * operator new(size_t size) = delete;
		void operator delete(void * block) noexcept = delete;
		void * operator new[](size_t size) = delete;
		void operator delete[](void * block) noexcept = delete;
		void operator delete(void *, void *) noexcept = delete;
		void operator delete[](void *, void *) noexcept = delete;
		void * operator new(std::size_t count, void * ptr) = delete;
		void * operator new[](std::size_t count, void * ptr) = delete;

		bind_error() = delete;
		bind_error(const error & OKps_error_info, const error_type & external_error);
		bind_error(const bind_error & orgin);
		bind_error(bind_error &&) = delete;
	};

	template <typename error_type>
	bind_error<error_type>::bind_error(const error & OKps_error_info, const error_type & external_error)
		: OKps_error_info(OKps_error_info), external_error(external_error)
	{
	}
	template <typename error_type>
	bind_error<error_type>::bind_error(const bind_error & orgin)
		: OKps_error_info(orgin.OKps_error_info), external_error(orgin.external_error)
	{
	}

	/*
	class handler final
	{
	private:
		//保存一些通用的错误信息
		//在发生严重错误，例如已经无法申请堆内存，无法进行文件读写时，还能尝试用此处保存的信息，输出错误日志
		//所以在main函数一开始，就应该初始化1个本类的对象，如果初始化失败，程序应该直接退出

		const information& the_error;

	public:

		handler() = delete;

		handler(const handler&) = delete;

		handler(const information& the_error);

		//返回错误代码，该错误代码最终由main函数返回，也就是说，捕获到异常后，catch代码块中最终应有 return handler_object.handle_error(); ，其中handler_object是handler对象
		//只有异常类型为内部调试异常时，才返回-1，因为调试器经常使用-1作为返回值
		//-2代表写入日志文件失败，注意写入日志文件使用的是标准库fstream
		//内存错误返回-3
		//c++内置异常返回2
		//内部调试异常返回-4
		//fastIO异常返回3
		//操作系统接口异常返回-5
		//未知异常返回1
		int handle_error();

		//处理未知异常
		int handle_unknown_error();

		//处理标准库异常
		int handle_std_error();

	private:
		void check_head(std::ofstream& logger);
	};
	*/
}
