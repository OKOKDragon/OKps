#pragma once

#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <set>
#include <filesystem>
#include <fstream>

#include ".\base.hpp"
#include ".\string.hpp"

namespace OKps
{
	/*
	按字节比较两个文件的内容是否完全相同
	thread 参数表示建议使用的线程数
	*/
	bool compare(std::filesystem::path const & left, std::filesystem::path const & right, std::uintmax_t const thread = 0);
	/*
	深度遍历目录 directory，递归地获取此目录下所有文件、子目录、子目录下的文件和子目录
	result_types 参数决定此函数返回的结果中排除哪些类型的文件
	*/
	std::vector<std::filesystem::path> totally_traverse_directory(std::filesystem::path const & directory, std::set<std::filesystem::file_type> const & result_types = std::set<std::filesystem::file_type>());

	class stream_position;

	template<>
	constexpr bool const safe_convertible<long long, stream_position> = noexcept(static_cast<base::integer<long long>>(base::integer<std::streamoff>(std::declval<std::streampos const &>())));
	template<>
	constexpr bool const safe_convertible<unsigned long long, stream_position> = noexcept(static_cast<base::integer<unsigned long long>>(base::integer<std::streamoff>(std::declval<std::streampos const &>())));
	template<>
	constexpr bool const safe_convertible<long, stream_position> = noexcept(static_cast<base::integer<long>>(base::integer<std::streamoff>(std::declval<std::streampos const &>())));
	template<>
	constexpr bool const safe_convertible<unsigned long, stream_position> = noexcept(static_cast<base::integer<unsigned long>>(base::integer<std::streamoff>(std::declval<std::streampos const &>())));
	template<>
	constexpr bool const safe_convertible<int, stream_position> = noexcept(static_cast<base::integer<int>>(base::integer<std::streamoff>(std::declval<std::streampos const &>())));
	template<>
	constexpr bool const safe_convertible<unsigned int, stream_position> = noexcept(static_cast<base::integer<unsigned int>>(base::integer<std::streamoff>(std::declval<std::streampos const &>())));

	template<>
	constexpr bool const safe_convertible<stream_position, long long> = noexcept(std::streampos(static_cast<base::integer<std::streamoff>>(base::integer<long long>(std::declval<long long const &>())).value()));
	template<>
	constexpr bool const safe_convertible<stream_position, unsigned long long> = noexcept(std::streampos(static_cast<base::integer<std::streamoff>>(base::integer<unsigned long long>(std::declval<unsigned long long const &>())).value()));
	template<>
	constexpr bool const safe_convertible<stream_position, long> = noexcept(std::streampos(static_cast<base::integer<std::streamoff>>(base::integer<long>(std::declval<long const &>())).value()));
	template<>
	constexpr bool const safe_convertible<stream_position, unsigned long> = noexcept(std::streampos(static_cast<base::integer<std::streamoff>>(base::integer<unsigned long>(std::declval<unsigned long const &>())).value()));
	template<>
	constexpr bool const safe_convertible<stream_position, int> = noexcept(std::streampos(static_cast<base::integer<std::streamoff>>(base::integer<int>(std::declval<int const &>())).value()));
	template<>
	constexpr bool const safe_convertible<stream_position, unsigned int> = noexcept(std::streampos(static_cast<base::integer<std::streamoff>>(base::integer<unsigned int>(std::declval<unsigned int const &>())).value()));

	/*
	此类是std::streampos的代理类，表示文件流中的绝对位置。

	c++标准规定了std::streampos和std::streamoff足以表示操作系统支持的最大文件大小。
	标准库文件流的无格式输入输出函数 read 和 write 接受 std::streamsize 类型参数，表示输入或输出的字符数，
	但并未规定 std::streamsize 也足以表示最大文件大小。故从std::streamoff转换到std::streamsize可能溢出。
	另一方面，std::filesystem 命名空间中的函数却以 std::uintmax_t 类型表示文件大小，但 std::uintmax_t 是无符号整数，
	而std::streamoff规定是有符号整数，故它们必然是不同类型，转换时也可能发生溢出。
	使用标准库的文件流操作文件，便存在上述多种理论上的溢出风险。
	*/
	class stream_position final
	{
	private:
		std::streampos MEMBER_position;
	public:
		stream_position(std::streampos const &)
			noexcept(std::is_nothrow_copy_constructible_v<std::streampos>);

		template<typename value_type>
		stream_position(value_type const)
			noexcept(safe_convertible<stream_position, value_type>);

		template<typename value_type>
		operator value_type()const
			noexcept(safe_convertible<value_type, stream_position>);

		stream_position(stream_position const &)
			noexcept(std::is_nothrow_copy_constructible_v<std::streampos>);
		stream_position(stream_position &&) = delete;
		void operator =(stream_position const &)
			noexcept(std::is_nothrow_copy_assignable_v<std::streampos>);
		void operator =(stream_position &&) = delete;

		~stream_position()
			noexcept(std::is_nothrow_destructible_v<std::streampos>);

		std::streampos const & value()const noexcept;
		operator std::streampos const & ()const noexcept;

		stream_position operator +(stream_position const &)const
			noexcept(noexcept(stream_position((base::integer<std::streamoff>(std::declval<std::streampos const &>()) + base::integer<std::streamoff>(std::declval<std::streampos const &>())).value())));
		stream_position operator -(stream_position const &)const
			noexcept(noexcept(stream_position((base::integer<std::streamoff>(std::declval<std::streampos const &>()) - base::integer<std::streamoff>(std::declval<std::streampos const &>())).value())));
		void operator +=(stream_position const &)
			noexcept(noexcept(std::declval<std::streampos &>() = (base::integer<std::streamoff>(std::declval<std::streampos &>()) + base::integer<std::streamoff>(std::declval<std::streampos const &>())).value()));
		void operator -=(stream_position const &)
			noexcept(noexcept(std::declval<std::streampos &>() = (base::integer<std::streamoff>(std::declval<std::streampos &>()) - base::integer<std::streamoff>(std::declval<std::streampos const &>())).value()));

		bool operator ==(stream_position const &)const
			noexcept(noexcept(std::declval<std::streampos const &>() == std::declval<std::streampos const &>()));
		bool operator !=(stream_position const &)const
			noexcept(noexcept(std::declval<std::streampos const &>() != std::declval<std::streampos const &>()));
		bool operator <=(stream_position const &)const
			noexcept(noexcept(std::declval<std::streampos const &>() <= std::declval<std::streampos const &>()));
		bool operator >=(stream_position const &)const
			noexcept(noexcept(std::declval<std::streampos const &>() >= std::declval<std::streampos const &>()));
		bool operator <(stream_position const &)const
			noexcept(noexcept(std::declval<std::streampos const &>() < std::declval<std::streampos const &>()));
		bool operator >(stream_position const &)const
			noexcept(noexcept(std::declval<std::streampos const &>() > std::declval<std::streampos const &>()));
	};

	template
		stream_position::stream_position(long long const)
		noexcept(safe_convertible<stream_position, long long>);
	template
		stream_position::stream_position(unsigned long long const)
		noexcept(safe_convertible<stream_position, unsigned long long>);
	template
		stream_position::stream_position(long const)
		noexcept(safe_convertible<stream_position, long>);
	template
		stream_position::stream_position(unsigned long const)
		noexcept(safe_convertible<stream_position, unsigned long>);
	template
		stream_position::stream_position(unsigned int const)
		noexcept(safe_convertible<stream_position, unsigned int>);
	template
		stream_position::stream_position(int const)
		noexcept(safe_convertible<stream_position, int>);

	template
		stream_position::operator long long()const
		noexcept(safe_convertible<long long, stream_position>);
	template
		stream_position::operator unsigned long long()const
		noexcept(safe_convertible<unsigned long long, stream_position>);
	template
		stream_position::operator long()const
		noexcept(safe_convertible<long, stream_position>);
	template
		stream_position::operator unsigned long()const
		noexcept(safe_convertible<unsigned long, stream_position>);
	template
		stream_position::operator int()const
		noexcept(safe_convertible<int, stream_position>);
	template
		stream_position::operator unsigned int()const
		noexcept(safe_convertible<unsigned int, stream_position>);

	template<typename stream_type>
	class stream_proxy;

	/*
	std::fstream的代理类

	调用 std::fstream 的 read，write，seekp，seekg函数，如果失败则抛出异常
	*/
	template<>
	class stream_proxy<std::fstream> final
	{
	private:
		std::fstream * MEMBER_storage;
		std::filesystem::path const * MEMBER_path;
	public:
		stream_proxy(std::fstream &, std::filesystem::path const &)noexcept;
		~stream_proxy()noexcept;
		stream_proxy(stream_proxy const &)noexcept;
		stream_proxy(stream_proxy &&) = delete;
		void operator =(stream_proxy &&) = delete;
		void operator =(stream_proxy const &) noexcept;

		void read(char * const, std::streamsize const);
		void write(char const * const, std::streamsize const);
		void seekp(std::streampos const &);
		void seekg(std::streampos const &);
		void seekp(std::streamoff const, std::ios_base::seekdir const &);
		void seekg(std::streamoff const, std::ios_base::seekdir const &);

	};

	/*
	字段，是一串2进制数据，其结构是：
	开头是以2进制补码表示的std::size_t类型的值 l ，l的数值为有效数据的长度；
	后接 l 字节2进制有效数据。
	*/
	class field final
	{
	private:
		std::string MEMBER_raw_data;

	public:
		/*
		构造空字段
		*/
		field()
			noexcept(std::is_nothrow_default_constructible_v<std::string>);
		/*
		将std::string作为无格式2进制数据使用
		*/
		field(std::string const & data)
			noexcept(std::is_nothrow_copy_constructible_v<std::string>);
		field(std::string && data)
			noexcept(std::is_nothrow_move_constructible_v<std::string>);
		~field()
			noexcept(std::is_nothrow_destructible_v<std::string>);
		field(std::vector<std::byte> const & data)
			noexcept(std::is_nothrow_default_constructible_v<std::string>
			and noexcept(std::declval<std::string>().resize(std::declval<std::vector<std::byte> const &>().size())));

		//原始2进制数据
		std::string const & raw_string()const noexcept;
		std::string & raw_string()noexcept;
		//field_stream格式2进制数据
		std::string field_string()const
			noexcept(std::is_nothrow_default_constructible_v<std::string>
and noexcept(std::declval<std::string>().resize(std::declval<std::string>().size())));
			//原始2进制数据
		std::vector<std::byte> raw_data()const
			noexcept(noexcept(value_cast<std::vector<std::byte>>(std::declval<std::string const>())));
			//field_stream格式2进制数据
		std::vector<std::byte> field_data()const
			noexcept(std::is_nothrow_default_constructible_v<std::vector<std::byte>>
and noexcept(std::declval<std::vector<std::byte>>().resize(std::declval<std::string>().size())));
		/*
		输入的数据是由多个字段连接而成的一串2进制有效数据
		如果格式错误则抛出异常
		此函数将输入的数据按字段切分
		*/
		static std::vector<field> parse(std::string const & data);
		static std::vector<field> parse(std::vector<std::byte> const & data);
		field(field const & origin)
			noexcept(std::is_nothrow_copy_constructible_v<std::string>);
		void operator =(field const & origin)
			noexcept(std::is_nothrow_copy_assignable_v<std::string>);
		/*
		移动后，原对象的数据会被清空
		*/
		field(field && origin)
			noexcept(std::is_nothrow_move_constructible_v<std::string>);
		void operator =(field && origin)
			noexcept(std::is_nothrow_move_assignable_v<std::string>);

		template<typename result_type>
		static result_type connect(std::vector<field> const & data);
	};
	template<>
	std::string field::connect(std::vector<field> const & data);
	template<>
	std::vector<std::byte> field::connect(std::vector<field> const & data);

	template<typename data_type>
	class storage;

	/*
	以若干个字段连接形成的文件
	操作起来如同它是内存中的一个 std::vector<OKps::field> 字段动态数组。

	但是与内存中的 std::vector 不同，此类的只读操作也是线程不安全的。
	*/
	template<>
	class storage<field> final :public base::marked
	{
	private:
		using base_type = base::marked;

		static stream_position const MEMBER_block_size;

		/*
		将一段2进制数据 data 从位置 pos 处插入文件，原来处于位置 pos 处开始直到文件末尾的数据向后移动。
		*/
		void INNER_insert_data(std::streampos const & pos, std::string const & data);
		/*
		将begin到end位置的数据移动到从another_begin处开始的位置。
		*/
		void INNER_move_data(std::streampos const & another_begin, std::streampos const & begin, std::streampos const & end);
		/*
		删除文件位置 begin 到 end 处的数据，包括begin，不包括end。然后将从end开始，后面的数据向前移动，再缩减文件大小。
		*/
		void INNER_erase_data(std::streampos const & begin, std::streampos const & end);

		void INNER_resize_storage(std::uintmax_t const size);

		mutable std::fstream MEMBER_storage;
		std::filesystem::path MEMBER_path;

		class field_info final
		{
		private:
			std::streampos MEMBER_begin;
			std::streamoff MEMBER_length;
		public:
			std::streampos const & begin()const noexcept;
			std::streamoff const & length()const noexcept;
			std::streampos & begin() noexcept;
			std::streamoff & length() noexcept;
			/*
			构造空值

			空值是无效的，禁止使用空值。
			此空值构造仅作为默认构造函数供std::vector调用。
			*/
			field_info()
				noexcept(noexcept(std::streampos(0)));
			field_info(std::streampos const & ARG_begin, std::streamoff const ARG_length)
				noexcept(std::is_nothrow_copy_constructible_v<std::streampos>);
			~field_info()
				noexcept(std::is_nothrow_destructible_v<std::streampos>);
			field_info(field_info const &)
				noexcept(std::is_nothrow_copy_constructible_v<std::streampos>);
			void operator =(field_info const &)
				noexcept(std::is_nothrow_copy_assignable_v<std::streampos>);
			/*
			为了启用std::vector的push_back函数，此类必须可移动。
			此移动构造函数实际进行复制构造。
			*/
			field_info(field_info &&)
				noexcept(std::is_nothrow_copy_constructible_v<std::streampos>);
			/*
			为了启用std::vector的push_back函数，此类必须可移动。
			此移动赋值操作符实际进行复制赋值。
			*/
			void operator =(field_info &&)
				noexcept(std::is_nothrow_copy_assignable_v<std::streampos>);
			std::streampos field_begin()const
				noexcept(noexcept(stream_position(std::declval<std::streampos const &>()) - stream_position(std::declval<std::size_t>())));
			std::streampos end()const
				noexcept(noexcept(stream_position(std::declval<std::streampos const &>()) + stream_position(std::declval<std::streamoff const &>())));
		};

		std::vector<field_info> MEMBER_position_list;

	public:
		/*
		代理类

		内部存储一个文件流位置，根据该位置从文件读写数据，如同操作内存时根据指针的值寻找数据的地址。
		*/
		class reference final :public base::reference
		{
			friend class storage;
		private:
			using base_type = base::reference;

			std::size_t MEMBER_position;

			reference(storage const & marker, std::size_t const position)
				noexcept(noexcept(base_type(std::declval<base::marked const &>())));

		public:
			reference() = delete;

			//读取数据
			operator field()const;
			//写入数据
			void operator =(field const &);

			~reference()
				noexcept(std::is_nothrow_destructible_v<base_type>)
				override;
			reference(reference const &)
				noexcept(std::is_nothrow_copy_constructible_v<base_type>);
			//移动后，原对象 origin 无法再使用
			reference(reference && origin)
				noexcept(std::is_nothrow_move_constructible_v<base_type>);
			void operator =(reference const &)
				noexcept(std::is_nothrow_copy_assignable_v<base_type>);
			void operator =(reference &&)
				noexcept(std::is_nothrow_move_assignable_v<base_type>);

			/*
			用于标准库容器的排序方案
			对此类内存储的文件流位置大小进行排序
			*/
			bool operator <(reference const &)const noexcept;
		private:
			reference & self()noexcept override;
			reference const & self()const noexcept override;
		public:
			reference * operator &()noexcept override;
			reference const * operator &()const noexcept override;
		};
		storage() = delete;
		reference operator [](std::size_t const position)
			noexcept(noexcept(reference(std::declval<storage &>(), std::declval<std::size_t const>())));
		field operator [](std::size_t const position)const;
		/*
		如果输入的路径是此前不存在的合法文件路径，此构造函数会创建文件，若创建失败则抛出异常。
		*/
		storage(std::filesystem::path const & file_path);
		storage(storage const &) = delete;
		void operator =(storage  const &) = delete;
		storage(storage &&)
			noexcept(std::is_nothrow_move_constructible_v<std::fstream>
			and std::is_nothrow_move_constructible_v<std::filesystem::path>
			and std::is_nothrow_move_constructible_v<base_type>
			and std::is_nothrow_move_constructible_v<std::vector<field_info>>);
		void operator =(storage &&)
			noexcept(std::is_nothrow_move_assignable_v<std::fstream>
			and std::is_nothrow_move_assignable_v<std::filesystem::path>
			and std::is_nothrow_move_assignable_v<base_type>
			and std::is_nothrow_move_assignable_v<std::vector<field_info>>);
		~storage()
			noexcept(std::is_nothrow_destructible_v<std::filesystem::path>
			and std::is_nothrow_destructible_v<std::fstream>
			and std::is_nothrow_destructible_v<base_type>
			and std::is_nothrow_destructible_v<std::vector<field_info>>)
			override;
	private:
		storage & self()noexcept override;
		storage const & self()const noexcept override;
	public:
		storage * operator &()noexcept override;
		storage const * operator &()const noexcept override;

		/*
		读取文件的第 position 个字段。
		*/
		field read(std::size_t const position)const;

		/*
		以 data 覆盖文件的第 position 个字段。
		*/
		void write(field const & data, std::size_t const position);

		void resize(std::size_t const size);

		std::size_t size()const
			noexcept(noexcept(std::declval<std::vector<field_info> const &>().size()));

	};
}
