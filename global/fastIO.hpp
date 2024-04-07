#pragma once

#include ".\base.hpp"
#include ".\locker.hpp"

/*
通过增加一个缓冲区，一般大小为64 KB，略微提高文件读写速度
提升非常小，用处不大
*/
namespace OKps::file
{
	/*
	文件管理
	一个文件不能同时存在于reader和writter中，因为写的行为可能导致读的文件指针失效
	一个文件可以同时被多个reader读，但不可以同时被多个writter写
	当然这只能限定使用本库打开的文件，如果用户用标准库和本库打开了同一个文件，需要用户自行管理
	*/
	class manager :public OKps::base
	{
	protected:
		enum class state
		{
			reading,
			writting
		};
	private:
		static inline OKps::spin_locker MEMBER_lock;
		static inline std::map<std::filesystem::path, std::pair<state, std::size_t>> MEMBER_pool;
	protected:
		manager();
		virtual ~manager();
		manager(manager const &) = delete;
		void operator=(manager const &) = delete;
		manager(manager &&) = delete;
		void operator=(manager &&) = delete;
		static void INNER_check(std::filesystem::path const & file, state const op);
		static void INNER_erase(std::filesystem::path const & file);
	};
	// 文件读取器
	// 为了使得调用者无感，std::ifstream中的成员方法都被本类封装成同名方法
	class reader : public manager
	{
		static_assert(sizeof(std::uintmax_t) == sizeof(std::size_t), "本类期望uintmax_t和size_t类型大小相等");
	private:
		std::ifstream file;			 // 文件
		std::filesystem::path route; // 文件路径
		std::size_t buffer_size;	 // 缓冲区大小，默认为10000000，但应该允许调用者自定义

		// 缓冲区有效内容的长度
		// 因为缓冲区有可能写不满，所以有必要保存有效内容的长度
		std::size_t end_position;

		/*
		缓冲区
		使用裸指针是因为标准库的文件读写函数只接受裸指针作为参数
		缓冲区会被初始化为0，这是为了方便模仿标准库的行为
		用std::ofstream打开一个文件，不使用std::ios::app参数
		调用seekp函数，将写指针指向任意一个非负的位置n，n超过了目前文件的大小，从此处调用write函数写入文件
		那么，在此位置n之前的，没有写入的位置，会被写入0
		*/
		std::unique_ptr<char[]> buffer;

		// 文件大小
		// 由于本类是只读打开，故文件大小不会变
		std::uintmax_t file_size;

		// 记录文件的读指针
		// 如果本类想要表现得行为与标准库的文件流一致，那么我们必须维护这个信息
		// 这是调用者请求的读操作的指针，而不是我们实际从文件读的指针
		// 本类要完全隐藏缓冲区的实现，而像标准库一样行为
		// 初始化为0
		std::streampos read_position;

		// 缓冲区的头对应的文件指针的位置
		std::streampos head_position;

	public:
		// 在机械硬盘上初步测试，64kb缓冲区大小是综合最优解
		reader(std::size_t size = 64 * 1024);

		reader(const reader &) = delete;
		void operator=(const reader &) = delete;
		reader(reader &&) = delete;
		void operator=(reader &&) = delete;

		// 如果析构时文件没有关闭，将自动关闭
		// 这可能会引发未知问题
		~reader();

		void open(const std::string & _route, std::ios_base::openmode _mode = std::ios_base::in, int _prot = std::ios_base::_Default_open_prot, bool resize = false);
		void open(const char * _route, std::ios_base::openmode _mode = std::ios_base::in, int _prot = std::ios_base::_Default_open_prot, bool resize = false);
		void open(const std::wstring & _route, std::ios_base::openmode _mode = std::ios_base::in, int _prot = std::ios_base::_Default_open_prot, bool resize = false);
		void open(const wchar_t * _route, std::ios_base::openmode _mode = std::ios_base::in, int _prot = std::ios_base::_Default_open_prot, bool resize = false);
		void open(const std::filesystem::path & _route, std::ios_base::openmode _mode = std::ios_base::in, int _prot = std::ios_base::_Default_open_prot, bool resize = false);

	private:
		template <typename string_type>
		void INNER_open(string_type _route, std::ios_base::openmode _mode, int _prot, bool resize);

	public:
		bool is_open() const noexcept;

		// 关闭文件
		void close();

		std::streampos tellg() const;

		// void clear(std::ios_base::iostate _state = 0, bool _reraise = false);

		void seekg(std::streampos _pos);
		void seekg(std::streamoff _off, std::ios_base::seekdir _way);

		// 读文件
		// 注意，和标准库的接口一样，我们不检查缓存read_buffer的大小
		// 为了模仿标准库的行为，read失败会将读指针read_position改为-1，但不需要调用clear来重置
		bool read(char * read_buffer, std::streamsize size);
		bool read(std::unique_ptr<char[]> const & read_buffer, std::streamsize size);
		bool read(std::shared_ptr<char[]> const & read_buffer, std::streamsize size);

	private:
		// 从读指针read_position开始读文件，刷新缓冲区
		void refresh_buffer();

		// 根据文件大小，自动调整缓冲区大小
		void auto_resizer();

	public:
		// 返回文件大小
		std::uintmax_t size() const;

		// 改变缓冲区大小
		// 原来的数据按顺序复制到新缓冲区，多余的丢弃
		void resize_buffer(std::size_t new_size);
	};

	// 文件写入器
	// 为了使得调用者尽量无感，std::ifstream中的成员方法都被本类封装成同名方法
	class writter : public manager
	{
		static_assert(sizeof(std::uintmax_t) == sizeof(std::size_t), "本类期望uintmax_t和size_t类型大小相等");
	private:
		std::ofstream file;			 // 文件
		std::filesystem::path route; // 文件路径

		std::size_t buffer_size; // 缓冲区大小，默认为10000000，但应该允许调用者自定义

		// 与缓冲区同样长度的数组，保存每一位是否有效，全部初始化为false
		// 无效的数组不能写入文件
		// 这是为了随机读写
		// 对于写缓冲，如果要支持seek操作，比如调用者申请在10和2000处各写入一条数据，他们只会被写入缓冲区
		// 直到下次更新缓冲区的时候，10到2000之间的缓冲区不能与文件交互，因为文件对应位置的数据没有被调用者申请改动过，必须保持原样
		// 所以，再加一个bool数组，标记缓冲区的每一个下标是有效还是无效，无效的不能写入真实文件
		std::vector<bool> valid;

		// 缓冲区
		// 缓冲区会被初始化为0，这是为了方便模仿标准库的行为
		// 用std::ofstream打开一个文件，不使用std::ios::app参数
		// 调用seekp函数，将写指针指向任意一个非负的位置n，n超过了目前文件的大小，从此处调用write函数写入文件
		// 那么，在此位置n之前的，没有写入的位置，会被写入0
		std::unique_ptr<char[]> buffer;

		// 文件大小
		// 注意，这是像调用者虚拟的文件大小
		// 它是跟随调用者申请的写入操作而实时更新的
		// 这样，才能在std::ios::app参数下，向调用者返回正确的虚拟写指针位置
		// 真实文件的大小，本类不需要维护
		std::uintmax_t file_size;

		// 记录文件的写指针
		// 初始化为0
		std::streampos write_position;

		// 缓冲区的头对应的文件指针的位置
		std::streampos head_position;

		// 打开文件时是否输入了std::ios::app参数
		// 表示追加模式，在该模式下，文件打开后，写指针为0
		// 调用tellp和seekp函数的结果都和其他模式相同
		// 但是一旦调用write，写指针自动回到文件末尾，将数据追加写入文件末尾，不论之前调用seekp对写指针作了何种操作
		bool app_mode;

	public:
		writter(std::size_t size = 64 * 1024);

		writter(const writter &) = delete;
		void operator=(const writter &) = delete;
		writter(writter &&) = delete;
		void operator=(writter &&) = delete;
		// 如果析构时文件没有关闭，将自动关闭
		// 这可能会引发未知问题
		~writter();

		void open(const std::string & _route, std::ios_base::openmode _mode = std::ios_base::out, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);
		void open(const char * _route, std::ios_base::openmode _mode = std::ios_base::out, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);
		void open(const std::wstring & _route, std::ios_base::openmode _mode = std::ios_base::out, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);
		void open(const wchar_t * _route, std::ios_base::openmode _mode = std::ios_base::out, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);
		void open(const std::filesystem::path & _route, std::ios_base::openmode _mode = std::ios_base::out, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);

	private:
		template <typename string_type>
		void INNER_open(string_type _route, std::ios_base::openmode _mode = std::ios_base::in, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);

	public:
		bool is_open() const noexcept;

		// 关闭文件
		void close();

		std::streampos tellp() const;

		// void clear(std::ios_base::iostate _state = 0, bool _reraise = false);

		void seekp(std::streampos _pos);
		void seekp(std::streamoff _off, std::ios_base::seekdir _way);

		// 写文件
		bool write(const char * write_buffer, std::streamsize size);
		bool write(std::string const & content);
		bool write(std::unique_ptr<char const []> const & write_buffer, std::streamsize size);
		bool write(std::shared_ptr<char const []> const & write_buffer, std::streamsize size);

		/*流操作符*/
		template <typename number_type> /*数字类型，也就是std::to_string接受的类型*/
		writter & operator<<(const number_type & data);

		/*流操作符*/
		writter & operator<<(const char * data);

		writter & operator<<(const std::string & data);

		// 修改缓冲区大小
		// 注意，这个行为不改变write_position指针
		// write_position是向调用者虚拟的文件指针
		// 虽然向调用者开放了修改缓冲区大小的权限，但有关缓冲区和文件实际如何交互的细节，不应暴露给调用者
		void resize_buffer(const std::streamoff size);

	private:
		// 从写指针position开始写文件，刷新缓冲区
		void refresh_buffer();

	public:
		// 自动调整缓冲区大小
		void auto_resizer(const std::uintmax_t size_guess);

	private:
		// 将字符缓冲区write_buffer的内容写入文件，使用检查器checker来检查write_buffer内容的有效性，length参数是它们的长度
		//_pos参数用于真实文件对象file的seekp操作
		void valid_writter(const char * write_buffer, const std::vector<bool>::const_iterator & checker, const std::size_t length, std::streampos _pos);

		// 将字符缓冲区write_buffer的内容写入文件，使用检查器checker来检查write_buffer内容的有效性，length参数是它们的长度
		//_off和_way参数用于真实文件对象file的seekp操作
		// void valid_writter(const char *write_buffer, const bool *checker, const std::size_t length, std::streamoff _off, std::ios_base::seekdir _way);
	};

	template <typename number_type>
	writter & writter::operator<<(const number_type & data)
	{
		auto content = std::to_string(data); /*要写的字符串*/
		this->write(content.c_str(), content.size());
		return *this;
	}

	// 顺序文件写入器
	// 禁止seek操作，每次写入都是从文件尾开始顺序写入，即强制加上std::ios::app参数
	class end_writter : public manager
	{
		static_assert(sizeof(std::uintmax_t) == sizeof(std::size_t), "本类期望uintmax_t和size_t类型大小相等");
	private:
		std::ofstream file;			 // 文件
		std::filesystem::path route; // 文件路径

		std::size_t buffer_size; // 缓冲区大小，默认为10000000，但应该允许调用者自定义

		// 缓冲区
		// 缓冲区会被初始化为0，这是为了方便模仿标准库的行为
		// 用std::ofstream打开一个文件，不使用std::ios::app参数
		// 调用seekp函数，将写指针指向任意一个非负的位置n，n超过了目前文件的大小，从此处调用write函数写入文件
		// 那么，在此位置n之前的，没有写入的位置，会被写入0
		std::unique_ptr<char[]> buffer;

		// 文件大小
		// 注意，这是像调用者虚拟的文件大小
		// 它是跟随调用者申请的写入操作而实时更新的
		// 这样，才能在std::ios::app参数下，向调用者返回正确的虚拟写指针位置
		// 真实文件的大小，本类不需要维护
		std::uintmax_t file_size;

		// 记录文件的写指针
		// 初始化为0
		std::streampos write_position;

		// 缓冲区的头对应的文件指针的位置
		std::streampos head_position;

		// 我们不可能预知调用者要写入多少数据，所以也无从着手自动调整缓冲区大小的工作
		// 缓冲区大小的管理只能交给调用者
		// bool auto_resize;

		// 打开文件时是否输入了std::ios::app参数
		// 表示追加模式，在该模式下，文件打开后，写指针为0
		// 调用tellp和seekp函数的结果都和其他模式相同
		// 但是一旦调用write，写指针自动回到文件末尾，将数据追加写入文件末尾，不论之前调用seekp对写指针作了何种操作
		bool app_mode;

	public:
		end_writter(std::size_t size = 64 * 1024);

		end_writter(const end_writter &) = delete;
		void operator=(const end_writter &) = delete;
		end_writter(end_writter &&) = delete;
		void operator=(end_writter &&) = delete;

		// 如果析构时文件没有关闭，将自动关闭
		// 这可能会引发未知问题
		~end_writter();

		void open(const std::string & _route, std::ios_base::openmode _mode = std::ios_base::out | std::ios_base::app, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);
		void open(const char * _route, std::ios_base::openmode _mode = std::ios_base::out | std::ios_base::app, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);
		void open(const std::wstring & _route, std::ios_base::openmode _mode = std::ios_base::out | std::ios_base::app, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);
		void open(const wchar_t * _route, std::ios_base::openmode _mode = std::ios_base::out | std::ios_base::app, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);
		void open(const std::filesystem::path & _route, std::ios_base::openmode _mode = std::ios_base::out | std::ios_base::app, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);

	private:
		template <typename string_type>
		void INNER_open(string_type _route, std::ios_base::openmode _mode = std::ios_base::in, int _prot = std::ios_base::_Default_open_prot, bool resize = false, std::uintmax_t guess_size = 0);

	public:
		bool is_open() const noexcept;

		// 关闭文件
		void close();

		std::streampos tellp() const;

		// void clear(std::ios_base::iostate _state = 0, bool _reraise = false);

		// 写文件
		bool write(const char * write_buffer, std::streamsize size);
		bool write(std::string const & content);
		bool write(std::unique_ptr<char const []> const & write_buffer, std::streamsize size);
		bool write(std::shared_ptr<char const []> const & write_buffer, std::streamsize size);

		// 修改缓冲区大小
		// 注意，这个行为不改变write_position指针
		// write_position是向调用者虚拟的文件指针
		// 虽然向调用者开放了修改缓冲区大小的权限，但有关缓冲区和文件实际如何交互的细节，不应暴露给调用者
		void resize_buffer(std::streamoff const size);

	private:
		// 从写指针write_position开始写文件，刷新缓冲区
		void refresh_buffer();

	public:
		// 自动调整缓冲区大小
		void auto_resizer(std::uintmax_t size_guess);
	};
}
