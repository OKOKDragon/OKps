#include ".\error.hpp"
// #include "control.h"
#include <fstream>
#include <filesystem>

/*
OKps::error::information::information(const std::string &hint, const bool memory, const bool console, const bool fastIO, const OKps::error::information::error_type type)
	: hint(OKps::global::string(hint)), memory_fail(memory), console_fail(console), fastIO_fail(fastIO), type(type)
{
}
*/
namespace OKps
{
	error::error(const error & origin)
		: hint(origin.hint), type(origin.type)
	{
	}

	error::error(error && origin) noexcept
		: hint(std::move(origin.hint)), type(origin.type)
	{
	}
	error::error(const std::string & hint, const error::error_type type)
		: hint(hint), type(type)
	{
	}
}

/*
OKps::error::handler::handler(const information& the_error)
	:the_error(the_error)
{

}

int OKps::error::handler::handle_error()
{
	std::ofstream logger;
	logger.open(".\\日志\\错误日志.txt", std::ios::out | std::ios::app);

	if (not logger.is_open())
	{

		//无法打开日志文件
		return -2;
	}

	this->check_head(logger);

	int error_code = 0;

	logger << u8"{\n〔原始错误信息〕" << this->the_error.hint.read() << "\n";

	if (this->the_error.memory_fail)
		//发生堆内存错误
		//总是返回-3
	{

		error_code = -3;
		int error_count = 1;
		logger << u8"〔错误1〕堆内存分配失败，这基本上是由于操作系统或者硬件发生严重问题。在这种情况下，程序唯一能做的就是立刻中断，防止给整个系统雪上加霜。请先排查错误，再尝试重新运行程序，否则可能导致严重的数据丢失和系统崩溃。\n";

		if (this->the_error.console_fail)
		{
			error_count++;
			logger << u8"〔错误" << error_count << u8"〕控制台模块崩溃。\n";
		}

		//无论发生什么错误，都尝试把错误信息输出到控制台。如果失败，则向日志文件添加相关信息
		try
		{
			OKps::console::control.AddLog(u8"程序运行时异常 ││ 堆内存分配失败\n程序运行时异常 ││ 程序中止\n");
		}
		catch (const OKps::error::information& console_output_fail)
		{
			error_count++;
			logger << u8"　　{\n　　〔错误" << error_count << u8"〕在尝试把错误信息输出到控制台时，控制台模块崩溃。但是，因为发生内存错误，程序会立即中止，所以这些控制台输出本来也没有机会被用户看到。开发者不必特别关注此崩溃，因为它是内存错误引起的。\n"
				u8"　　〔原始错误信息〕" << console_output_fail.hint.read() << u8"\n　　}\n";
		}
		catch (...)
		{
			error_count++;
			logger << u8"〔错误" << error_count << u8"〕在尝试把错误信息输出到控制台时，控制台模块崩溃。这个崩溃来自未知的异常类型。所以，请用户报告程序崩溃时的详细情况。请开发者重视此报告。\n";
		}

		if (this->the_error.fastIO_fail)
		{
			error_count++;
			logger << u8"〔错误" << error_count << u8"〕文件读写模块崩溃。开发者不必特别关注此崩溃，因为它是内存错误引起的。\n";
		}
	}
	else
	{

		int error_count = 1;

		switch (this->the_error.type)
		{
		case OKps::error::information::error_type::heap_memory:
			error_code = -3;
			logger << u8"〔错误1〕堆内存分配失败，这基本上是由于操作系统或者硬件发生严重问题。在这种情况下，程序唯一能做的就是立刻中断，防止给整个系统雪上加霜。请先排查错误，再尝试重新运行程序，否则可能导致严重的数据丢失和系统崩溃。\n";
			break;
		case OKps::error::information::error_type::cpp_build_in:
			error_code = 2;
			logger << u8"〔错误1〕发生c++语言内置异常。请开发者查看错误信息，以判断此错误的位置和严重程度；注意检查开发环境和运行环境。\n";
			break;
		case OKps::error::information::error_type::internal_debug:
			error_code = -4;
			logger << u8"〔错误1〕发生内部调试异常。这种异常是不应该出现在对外发布版本的。请详细报告异常发生时的情况。请开发者重视并优先处理此报告，它通常是低级错误。\n";
			break;
		case OKps::error::information::error_type::fileIO:
			error_code = 3;
			logger << u8"〔错误1〕文件读写模块发生异常。请开发者查看错误信息。\n";
			break;
		case OKps::error::information::error_type::OS:
			error_code = -5;
			logger << u8"〔错误1〕操作系统接口抛出异常。请开发者查看错误信息，确定问题是否由本程序引发。\n";
			break;
		case OKps::error::information::error_type::unknown:
			error_code = 1;
			logger << u8"〔错误1〕发生未知类型异常。请开发者重视，查看错误信息，确定问题的性质、位置和严重性。\n";
			break;
		default:
			break;
		}

		if (this->the_error.console_fail)
		{
			error_count++;
			logger << u8"〔错误" << error_count << u8"〕控制台模块崩溃。\n";
		}

		//无论发生什么错误，都尝试把错误信息输出到控制台。如果失败，则向日志文件添加相关信息
		try
		{
			OKps::console::control.AddLog(u8"程序运行时异常 ││ %s\n", this->the_error.hint.read().c_str());
		}
		catch (const OKps::error::information& console_output_fail)
		{
			error_count++;
			logger << u8"　　{\n　　〔错误" << error_count << u8"〕在尝试把错误信息输出到控制台时，控制台模块崩溃。\n"
				u8"　　〔错误" << error_count << u8" ││ 原始错误信息〕" << console_output_fail.hint.read() << "\n　　}\n";
		}
		catch (...)
		{
			error_count++;
			logger << u8"〔错误" << error_count << u8"〕在尝试把错误信息输出到控制台时，控制台模块崩溃。这个崩溃来自未知的异常类型。所以，请用户报告程序崩溃时的详细情况。请开发者重视此报告。\n";
		}

		if (this->the_error.fastIO_fail)
		{
			error_count++;
			logger << u8"〔错误" << error_count << u8"〕文件读写模块崩溃。\n";
		}
	}
	logger << u8"}\n";
	logger.close();
	return error_code;
}

int OKps::error::handler::handle_unknown_error()
{
	std::ofstream logger;
	logger.open(".\\日志\\错误日志.txt", std::ios::out | std::ios::app);
	bool logger_open = logger.is_open();
	if (not logger.is_open())
	{
		//无法打开日志文件
		return -2;
	}
	this->check_head(logger);
	logger << u8"{\n〔错误1〕发生未知的异常。请用户报告程序崩溃时的详细情况。请开发者重视此报告。\n}\n";
	logger.close();
	return 1;
}

int OKps::error::handler::handle_std_error()
{
	std::ofstream logger;
	logger.open(".\\日志\\错误日志.txt", std::ios::out | std::ios::app);
	bool logger_open = logger.is_open();
	if (not logger.is_open())
	{
		//无法打开日志文件
		return -2;
	}
	this->check_head(logger);
	logger << u8"{\n〔原始错误信息〕" << this->the_error.hint.read() << "\n"
		u8"〔错误1〕捕获到标准库发生的异常。请用户报告程序崩溃时的详细情况。请开发者重视此报告。\n}\n";
	logger.close();
	return 2;
}

void OKps::error::handler::check_head(std::ofstream& logger)
{
	logger.seekp(0, std::ios::end);
	if (logger.tellp() == 0)
	{
		logger << u8"本日志文件使用utf8编码。这是大部分文本编辑器默认的编码，所以一般来说不会出现乱码。如果使用utf8编码打开此文件，仍然出现乱码，请报告给开发者。\n";
	}
}
*/