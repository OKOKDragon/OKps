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
	此类是std::streampos的代理类，表示文件流中的绝对位置。

	c++标准规定了std::streampos和std::streamoff足以表示操作系统支持的最大文件大小。
	标准库文件流的无格式输入输出函数 read 和 write 接受 std::streamsize 类型参数，表示输入或输出的字符数，但并未规定 std::streamsize 也足以表示最大文件大小。
	故从std::streamoff转换到std::streamsize可能溢出。
	另一方面，std::filesystem 命名空间中的函数却以 std::uintmax_t 类型表示文件大小，但 std::uintmax_t 是无符号整数，而std::streamoff规定是有符号整数，故当一个负的std::streamoff值向std::uintmax_t转换时，也会发生溢出。
	使用标准库的文件流操作文件，便存在上述多种理论上的溢出风险。此类则在转换时进行检查，若有溢出则抛出异常。
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
			noexcept(safe_convertible::value<stream_position, value_type>);

		template<typename value_type>
		operator value_type()const
			noexcept(safe_convertible::value<value_type, stream_position>);

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
		noexcept(safe_convertible::value<stream_position, long long>);
	template
		stream_position::stream_position(unsigned long long const)
		noexcept(safe_convertible::value<stream_position, unsigned long long>);
	template
		stream_position::stream_position(long const)
		noexcept(safe_convertible::value<stream_position, long>);
	template
		stream_position::stream_position(unsigned long const)
		noexcept(safe_convertible::value<stream_position, unsigned long>);
	template
		stream_position::stream_position(unsigned int const)
		noexcept(safe_convertible::value<stream_position, unsigned int>);
	template
		stream_position::stream_position(int const)
		noexcept(safe_convertible::value<stream_position, int>);

	template
		stream_position::operator long long()const
		noexcept(safe_convertible::value<long long, stream_position>);
	template
		stream_position::operator unsigned long long()const
		noexcept(safe_convertible::value<unsigned long long, stream_position>);
	template
		stream_position::operator long()const
		noexcept(safe_convertible::value<long, stream_position>);
	template
		stream_position::operator unsigned long()const
		noexcept(safe_convertible::value<unsigned long, stream_position>);
	template
		stream_position::operator int()const
		noexcept(safe_convertible::value<int, stream_position>);
	template
		stream_position::operator unsigned int()const
		noexcept(safe_convertible::value<unsigned int, stream_position>);

	template<typename stream_type>
	class stream_proxy;

	/*
	std::fstream的代理类
	此类仅保存std::fstream的引用，若引用的原对象失效，此类不负责检查。

	调用 std::fstream 的 read，write，seekp，seekg函数，如果失败则抛出异常
	*/
	template<>
	class stream_proxy<std::fstream> final
	{
	private:
		std::fstream * MEMBER_storage;
		std::filesystem::path const * MEMBER_path;

		//检查文件流的可用性
		void INNER_check();
	public:
		stream_proxy(std::fstream &, std::filesystem::path const &)noexcept;
		~stream_proxy()noexcept;
		stream_proxy(stream_proxy const &)noexcept;
		stream_proxy(stream_proxy &&) = delete;
		void operator =(stream_proxy &&) = delete;
		void operator =(stream_proxy const &) noexcept;

		void read(char * const, std::streamsize const);
		void write(char const * const, std::streamsize const);
		std::string read(std::streamsize const);
		void write(std::string const &, std::streamsize const);
		void seekp(std::streampos const &);
		void seekg(std::streampos const &);
		void seekp(std::streamoff const, std::ios_base::seekdir const &);
		void seekg(std::streamoff const, std::ios_base::seekdir const &);
		std::fstream & file_stream()noexcept;
		std::fstream const & file_stream()const noexcept;
		std::filesystem::path const & file_path()const noexcept;

	};

	/*
	目录结构树
	*/
	class directory_tree final
	{
	private:
		using path_type = std::filesystem::path;
	public:
		class path_judgement
		{
		public:
			path_judgement()noexcept;
			/*
			此析构函数本身不会抛出异常
			但是为了允许派生类的析构函数抛出异常，故没有声明noexcept
			*/
			virtual ~path_judgement();
			path_judgement(path_judgement const &) = delete;
			path_judgement(path_judgement &&) = delete;
			void operator =(path_judgement const &) = delete;
			void operator =(path_judgement &&) = delete;
			//返回对自身的引用
			virtual path_judgement & self()noexcept;
			//返回对自身的引用
			virtual path_judgement const & self()const noexcept;

			//判断一个路径是否满足一定条件
			virtual bool judge(path_type const &) = 0;
		};
		/*
		目录结构树的节点
		每个节点都对应文件系统中的一个路径
		如果该路径是一个目录，则该节点还包含此目录下的所有子目录和文件对应的节点，但不包括子目录中的内容。
		*/
		class node final
		{
			friend class directory_tree;
		private:
			path_type MEMBER_path;
			std::vector<std::size_t> MEMBER_content;
			directory_tree const * MEMBER_owner;
			node(path_type const & path, directory_tree const * const owner)
				noexcept(std::is_nothrow_copy_constructible_v<path_type>
				and std::is_nothrow_default_constructible_v<std::vector<std::size_t>>);
			node(path_type && path, directory_tree const * const owner)
				noexcept(std::is_nothrow_move_constructible_v<path_type>
				and std::is_nothrow_default_constructible_v<std::vector<std::size_t>>);
			void add_content(std::size_t const content)
				noexcept(noexcept(std::declval<std::vector<std::size_t> &>().push_back(std::declval<std::size_t const >())));
		public:
			//返回此节点对应的路径
			path_type const & path()const noexcept;
			operator path_type const & ()const noexcept;
			//如果此节点是一个目录，返回该目录下的所有条目；否则，返回一个空列表。
			std::vector<node const *> content()const;
			//如果此节点是一个目录，返回该目录下符合judgement判断函数的所有条目；否则，返回一个空列表。
			std::vector<node const *> content(path_judgement & judgement)const;
			~node()
				noexcept(std::is_nothrow_destructible_v<path_type>
				and std::is_nothrow_destructible_v<std::vector<std::size_t>>);
			node(node &&)
				noexcept(std::is_nothrow_move_constructible_v<path_type>
				and std::is_nothrow_move_constructible_v<std::vector<std::size_t>>);
			void operator =(node &&)
				noexcept(std::is_nothrow_move_assignable_v<path_type>
				and std::is_nothrow_move_assignable_v<std::vector<std::size_t>>);
			node(node const &)
				noexcept(std::is_nothrow_copy_constructible_v<path_type>
				and std::is_nothrow_copy_constructible_v<std::vector<std::size_t>>);
			void operator =(node const &)
				noexcept(std::is_nothrow_copy_assignable_v<path_type>
				and std::is_nothrow_copy_assignable_v<std::vector<std::size_t>>);
		};
	private:
		std::vector<node> MEMBER_storage;
		std::vector<node> & storage()noexcept;
		std::vector<node> const & storage()const noexcept;
	public:
		//返回根节点
		node const & root()const;
		//路径root是一个既存目录，此类以它为根节点，建立目录结构树
		directory_tree(path_type const & root);
		//路径root是一个既存目录，此类以它为根节点，建立目录结构树
		directory_tree(path_type && root);
		~directory_tree()
			noexcept(std::is_nothrow_destructible_v<std::vector<node>>);
		directory_tree(directory_tree const &)
			noexcept(std::is_nothrow_copy_constructible_v<std::vector<node>>);
		void operator =(directory_tree const &)
			noexcept(std::is_nothrow_copy_assignable_v<std::vector<node>>);
		directory_tree(directory_tree &&)
			noexcept(std::is_nothrow_move_constructible_v<std::vector<node>>);
		void operator =(directory_tree &&)
			noexcept(std::is_nothrow_move_assignable_v<std::vector<node>>);
		/*
		刷新
		相当于使用 (*this)=directory_tree(this->root_path())
		如果根节点所映射的目录已经不存在，则清空当前对象，并抛出异常。
		*/
		void refresh();
	private:
		//构造空树
		directory_tree()
			noexcept(std::is_nothrow_default_constructible_v<std::vector<node>>);
		//FLAG_refresh控制抛出的异常类型
		directory_tree(path_type && root, bool const FLAG_refresh);
		//FLAG_refresh控制抛出的异常类型
		directory_tree(path_type const & root, bool const FLAG_refresh);

		class refresh_failure final
		{
		private:
			std::runtime_error MEMBER_error;
		public:
			refresh_failure(std::string const &)
				noexcept(noexcept(std::runtime_error(std::declval<std::string const &>())));
			~refresh_failure()
				noexcept(std::is_nothrow_destructible_v<std::runtime_error>);
			refresh_failure(refresh_failure const &)
				noexcept(std::is_nothrow_copy_constructible_v<std::runtime_error>);
			void operator =(refresh_failure const &)
				noexcept(std::is_nothrow_copy_assignable_v<std::runtime_error>);
			std::runtime_error release()const
				noexcept(std::is_nothrow_copy_constructible_v<std::runtime_error>);
			refresh_failure(refresh_failure &&) = delete;
			void operator =(refresh_failure &&) = delete;
		};
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
			and noexcept(std::declval<std::string &>().resize(std::declval<std::vector<std::byte> const &>().size())));

		//原始2进制数据
		std::string const & raw_string()const noexcept;
		std::string & raw_string()noexcept;
		//field_stream格式2进制数据
		std::string field_string()const
			noexcept(std::is_nothrow_default_constructible_v<std::string>
and noexcept(std::declval<std::string>().resize(std::declval<std::string const &>().size())));
			//原始2进制数据
		std::vector<std::byte> raw_data()const
			noexcept(noexcept(value_cast<std::vector<std::byte>>(std::declval<std::string const &>())));
			//field_stream格式2进制数据
		std::vector<std::byte> field_data()const
			noexcept(std::is_nothrow_default_constructible_v<std::vector<std::byte>>
and noexcept(std::declval<std::vector<std::byte>>().resize(std::declval<std::string const &>().size())));
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

		/*
		parse函数的逆操作
		将多个字段按顺序连接成一长串2进制字节流
		*/
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
	class storage<field> final
		:public base::marked
	{
	private:
		using base_type = base::marked;

		static stream_position const MEMBER_block_size;

		bool MEMBER_temporary_file;

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
		class reference final
			:public base::reference
		{
			friend class storage;
		private:
			using base_type = base::reference;

			std::size_t MEMBER_position;

			reference(storage const & marker, std::size_t const position)
				noexcept(noexcept(base_type(std::declval<storage<field>::base_type const &>())));

		public:
			reference() = delete;

			//读取数据
			operator field()const;
			//写入数据
			void operator =(field const &);

			~reference()noexcept override;
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

		};
		storage() = delete;
		reference operator [](std::size_t const position)
			noexcept(noexcept(reference(std::declval<storage &>(), std::declval<std::size_t const>())));
		field operator [](std::size_t const position)const;
		/*
		如果输入的路径是此前不存在的合法文件路径，此构造函数会创建文件，若创建失败则抛出异常；并且在析构函数中会删除该文件，即将该文件当作临时文件。
		如果输入的路径是此前存在的文件，则析构函数不会删除文件。
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
		and std::is_nothrow_destructible_v<std::vector<field_info>>
		and noexcept(std::declval<std::fstream &>().close())
		and noexcept(std::filesystem::remove(std::declval<std::filesystem::path &>())))
			override;
	private:
		storage & self()noexcept override;
		storage const & self()const noexcept override;
	public:

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
