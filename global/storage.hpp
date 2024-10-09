#pragma once

#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <set>
#include <filesystem>
#include <fstream>

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




	/*
	字段，是一串2进制数据，其结构是：
	开头是以2进制补码表示的std::size_t类型的值 l ，l的数值为有效数据的长度；
	后接 l 字节2进制有效数据。
	*/
	class field_stream final
	{
	private:
		std::unique_ptr<std::byte[]> MEMBER_data;
		std::size_t MEMBER_length;
	public:
		/*
		构造空字段
		*/
		field_stream()
			noexcept(std::is_nothrow_default_constructible_v<std::unique_ptr<std::byte[]>>);
		field_stream(std::string const & data);
		~field_stream()
			noexcept(std::is_nothrow_destructible_v<std::unique_ptr<std::byte[]>>);
		field_stream(std::vector<std::byte> const & data);
		//原始2进制数据
		std::string raw_string()const;
		//field_stream格式2进制数据
		std::string field_string()const;
		//原始2进制数据
		std::vector<std::byte> raw_data()const;
		//field_stream格式2进制数据
		std::vector<std::byte> field_data()const;
		/*
		输入的数据是由多个字段连接而成的一串2进制有效数据
		如果格式错误则抛出异常
		此函数将输入的数据按字段切分
		*/
		static std::vector<field_stream> parse(std::string const & data);
		static std::vector<field_stream> parse(std::vector<std::byte> const & data);
		field_stream(field_stream const & origin);
		void operator =(field_stream const & origin);
		/*
		移动后，原对象的数据会被清空
		*/
		field_stream(field_stream && origin)
			noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<std::byte[]>>);
		void operator =(field_stream && origin)
			noexcept(std::is_nothrow_move_assignable_v<std::unique_ptr<std::byte[]>>);
	};

	/*
	将二进制数据以字节为单位存储于文件中，但操作起来如同它是存储于内存中的一段 std::byte 数组。

	如同直接操作内存一样，不设任何安全措施，访问越界、数据竞争等问题均由使用者自行负责。
	*/
	class byte_memory final
	{
	private:
		mutable std::fstream MEMBER_storage;
		std::filesystem::path MEMBER_path;
		class marker_type final
		{
		private:
			byte_memory * MEMBER_owner;
		public:
			marker_type() = delete;
			marker_type(byte_memory * owner)noexcept;
			marker_type(marker_type const &) = delete;
			marker_type(marker_type &&) = delete;
			void operator =(marker_type const &) = delete;
			void operator =(marker_type &&) = delete;
			~marker_type()noexcept;
			byte_memory & owner()noexcept;
			byte_memory const & owner()const noexcept;

		};
		std::shared_ptr<marker_type> MEMBER_marker;

	public:
		/*
		代理类

		内部存储一个文件流位置，根据该位置从文件读写数据，如同操作内存时根据指针的值寻找数据的地址。
		*/
		class reference final
		{
			friend class byte_memory;
		private:

			std::weak_ptr<marker_type> MEMBER_marker;
			std::streampos MEMBER_position;

			reference(std::shared_ptr<marker_type> const & marker, std::streampos const & position)
				noexcept(std::is_nothrow_copy_constructible_v<std::streampos>
				and noexcept(std::weak_ptr<marker_type>(std::declval<std::shared_ptr<marker_type> const &>())));

			void raise_invalid_error()const noexcept(false);
		public:
			reference() = delete;
			bool is_valid()const
				noexcept(noexcept(std::declval<std::weak_ptr<marker_type>>().expired()));

			//读取数据
			operator std::byte()const;
			//写入数据
			void operator =(std::byte const);

			~reference()
				noexcept(std::is_nothrow_destructible_v<std::streampos>
				and std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>);
			reference(reference const &)
				noexcept(std::is_nothrow_copy_constructible_v<std::streampos>
				and std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>);
			reference(reference &&)
				noexcept(std::is_nothrow_copy_constructible_v<std::streampos>
				and std::is_nothrow_default_constructible_v<std::streampos>
				and std::is_nothrow_move_constructible_v<std::weak_ptr<marker_type>>);
			void operator =(reference const &)
				noexcept(std::is_nothrow_copy_assignable_v<std::streampos>
				and std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>);
			void operator =(reference &&)
				noexcept(std::is_nothrow_copy_assignable_v<std::streampos>
				and std::is_nothrow_default_constructible_v<std::streampos>
				and std::is_nothrow_move_assignable_v<std::weak_ptr<marker_type>>);

			/*
			文件流位置标识数据在文件中的位置，如同指针的值标识数据在内存中的地址。
			故重载取地址运算符，使得此类中存储的文件流位置可以如同指针一样操作。
			*/
			std::streampos & operator &();
			std::streampos const & operator &()const;

			/*
			用于标准库容器的排序方案
			对此类内存储的文件流位置大小进行排序
			*/
			bool operator <(reference const &)const noexcept;
		};
		byte_memory() = delete;
		reference operator [](std::streampos const & position);
		std::byte operator [](std::streampos const & position)const;
		/*
		要求输入的路径是此前不存在的合法文件路径。
		此构造函数会创建文件，若创建失败则抛出异常，如同申请内存分配。
		文件会在析构函数中被删除，如同回收内存。
		*/
		byte_memory(std::filesystem::path const & file_path);
		byte_memory(byte_memory const &) = delete;
		void operator =(byte_memory const &) = delete;
		byte_memory(byte_memory &&)
			noexcept(std::is_nothrow_move_constructible_v<std::fstream>
			and std::is_nothrow_move_constructible_v<std::filesystem::path>
			and std::is_nothrow_move_constructible_v<std::shared_ptr<marker_type>>);
		void operator =(byte_memory &&)
			noexcept(std::is_nothrow_move_assignable_v<std::fstream>
			and std::is_nothrow_move_assignable_v<std::filesystem::path>
			and std::is_nothrow_move_assignable_v<std::shared_ptr<marker_type>>);
		~byte_memory()
			noexcept(noexcept(std::declval<std::fstream>().close())
				and noexcept(std::filesystem::remove(std::declval<std::filesystem::path>()))
			and std::is_nothrow_destructible_v<std::filesystem::path>
			and std::is_nothrow_destructible_v<std::fstream>
			and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>);

		/*
		从position处，按照 field_stream 的格式读取数据。
		*/
		field_stream read(std::streampos const & position)const;
		/*
		从position处，无格式地读取2进制数据，
		将结果作为原始数据，构造 field_stream。
		*/
		field_stream read(std::streampos const & position, std::streamoff const length)const;
		/*
		参数 raw 控制写入的格式。
		若 raw 为 true 则写入 data.raw_string()，否则写入data.field_string()。
		*/
		void write(field_stream const & data, bool const raw, std::streampos const & position);
	};
}
