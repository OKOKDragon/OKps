
#include <iostream>
#include <stdexcept>
#include <atomic>
#include <sstream>

#include ".\cipher.hpp"
#include ".\timer.hpp"
#include ".\message.hpp"
#include ".\matcher.hpp"
#include ".\storage.hpp"

#include ".\test.hpp"

namespace OKps::test
{

    void storage_field()
    {
        std::filesystem::path const path = "F:\\字段文件";
        {
            storage<field> st(path);
            integer const hd(100, integer::sign_type::positive);
            integer const sn(69, integer::sign_type::negative);
            st[2] = hd.field();
            st[1] = sn.field();

            integer const hd2(st[2]);
            integer const sn2(st[1]);

            if (hd2 == hd and sn2 == sn)
            {
                std::cout << "成功\n";
            }
            else
            {
                std::cout << "失败\n";
            }
        }
        std::filesystem::remove(path);
    }
    void test_handler()
    {
        class para_type final : public base::blank
        {
        private:
            std::string MEMBER_input_arg;
            std::string MEMBER_output_arg;
        public:
            para_type()
                :base::blank()
                , MEMBER_input_arg("输入参数")
                , MEMBER_output_arg()
            {
            }
            ~para_type()noexcept override
            {
            }
            std::string & output_arg()noexcept
            {
                return this->MEMBER_output_arg;
            }
            std::string const & input_arg()const noexcept
            {
                return this->MEMBER_input_arg;
            }
        };
        class func_type final : public base::handler<false>
        {
        public:
            func_type()noexcept
                :base::handler<false>()
            {
            }
            ~func_type()noexcept override
            {
            }

            void operator ()(base::blank & arg)override
            {
                para_type & real_type_arg = dynamic_cast<para_type &>(arg);
                std::cout << "收到参数 ││ " << real_type_arg.input_arg() << "\n";
                real_type_arg.output_arg() = "输出参数";
                std::cout << "输出结果 ││ " << real_type_arg.output_arg() << "\n";
            }
        };

        para_type p;
        func_type f;
        f(p);
    }
    void marked_reference()
    {
        base::marked * m = new base::marked;
        base::reference ref(*m);
        if (ref.is_valid())
        {
            std::cout << "引用有效\n";
            if (std::addressof(ref.get()) == m)
            {
                std::cout << "引用正确\n";
            }
            else
            {
                std::cout << "引用错误\n";
            }
        }
        else
        {
            std::cout << "引用无效\n";
        }
        delete m;
        if (ref.is_valid())
        {
            std::cout << "引用有效\n";
            if (std::addressof(ref.get()) == m)
            {
                std::cout << "引用正确\n";
            }
            else
            {
                std::cout << "引用错误\n";
            }
        }
        else
        {
            std::cout << "引用无效\n";
        }

        m = new base::marked;
        ref = base::reference(*m);
        if (ref.is_valid())
        {
            std::cout << "引用有效\n";
            if (std::addressof(ref.get()) == m)
            {
                std::cout << "引用正确\n";
            }
            else
            {
                std::cout << "引用错误\n";
            }
        }
        else
        {
            std::cout << "引用无效\n";
        }
        delete m;
        if (ref.is_valid())
        {
            std::cout << "引用有效\n";
            if (std::addressof(ref.get()) == m)
            {
                std::cout << "引用正确\n";
            }
            else
            {
                std::cout << "引用错误\n";
            }
        }
        else
        {
            std::cout << "引用无效\n";
        }
    }
    void worker_base()
    {
        {
            worker_impl t1("测试对象t1");
            worker_impl t2("测试对象t2");
        }

        worker_impl t("测试对象t");
        t.execute();
        t.join();
        std::cout << "工作线程结束\n";
        std::cout << "name = " << t.name() << "\n";
    }
    std::string const & worker_impl::name()const noexcept
    {
        return this->MEMBER_name;
    }
    worker_impl const & worker_impl::self()const noexcept
    {
        std::cout << "T::self函数const调用\n";
        return *this;
    }
    worker_impl & worker_impl::self()noexcept
    {
        std::cout << "T::self函数调用\n";
        return *this;
    }
    worker_impl::~worker_impl()noexcept
    {
    }
    worker_impl::worker_impl(worker_impl && origin)noexcept
        :base_type(std::move(origin.base_type::self()))
        , MEMBER_name(std::move(origin.MEMBER_name))
    {

    }
    worker_impl::worker_impl(std::string const & name)noexcept
        :base_type()
        , MEMBER_name(name)
    {
    }
    void worker_impl::work()noexcept
    {
        std::cout << "T::work函数调用\n"
            "name = " << this->MEMBER_name << "\n";
        this->MEMBER_name = std::string("新") + this->MEMBER_name;
    }
    void key(std::filesystem::path const & ARG_working_path)
    {

        namespace fs = std::filesystem;

        using nanosecond = std::chrono::nanoseconds;

        fs::path const working_path = fs::absolute(ARG_working_path);

        std::cout << "密钥文件和加密解密测试\n";

        if (not fs::exists(working_path))
        {
            fs::create_directory(fs::path(working_path));
        }
        else
        {
            std::string const hint = "目录 " + working_path.string() + " 已存在，无法使用";
            throw std::invalid_argument(hint);
        }
        auto original_working_dir = fs::current_path();
        if (not original_working_dir.is_absolute())
        {
            original_working_dir = fs::absolute(original_working_dir);
        }
        fs::current_path(working_path);

        std::cout << "程序当前的工作目录 ││ " << working_path.string() << "\n"
            "工作目录的可用空间 ││ " << fs::space(working_path).available << " 字节\n";

        fs::path const temp_file_path = working_path / "临时文件\\";

        if (not fs::exists(temp_file_path))
        {
            fs::create_directory(temp_file_path);
        }

        std::string origin_route;
        std::cout << "输入要加密的文件 ││ ";
        {
            string TEMP_input_str;
            TEMP_input_str.get_line(std::cin);
            origin_route = TEMP_input_str.utf_8<std::string>();
        }

        {
            time_monitor timer;
            timer.begin();
            auto key = crypt::cipher();
            auto times = timer.end<nanosecond>();
            std::cout << "生成密钥耗时" << times << "纳秒\n";
            timer.begin();
            key.save(".\\密钥");
            times = timer.end<nanosecond>();
            std::cout << "生成密钥文件耗时" << times << "纳秒\n";
            timer.begin();
            auto reader = crypt::cipher(".\\密钥");
            times = timer.end<nanosecond>();
            std::cout << "读取密钥文件耗时" << times << "纳秒\n";

            if (reader == key)
            {
                std::cout << "密钥文件生成成功\n";
            }
            else
            {
                std::cout << "密钥文件生成失败\n";
                wait_input();
                return;
            }
            timer.begin();
            key.encrypt(origin_route, ".\\加密文件", temp_file_path);
            times = timer.end<nanosecond>();
            std::cout << "加密耗时" << times << "纳秒\n";
            fs::path result_route = fs::path(std::string(".\\解密文件"));
            result_route += fs::path(origin_route).extension();
            std::cout << "生成的文件的路径 ││ " << result_route.string() << "\n";
            timer.begin();
            key.decrypt(".\\加密文件", result_route, temp_file_path);
            times = timer.end<nanosecond>();
            std::cout << "解密耗时" << times << "纳秒\n";
            fs::remove(".\\加密文件");
            timer.begin();
            bool const file_compare_result = compare(origin_route, result_route);
            times = timer.end<nanosecond>();
            std::cout << "比较文件耗时" << times << "纳秒\n";
            if (file_compare_result)
            {
                std::cout << "文件“" << fs::path(origin_route).string() << "”和文件“" << result_route.string() << "”相同，测试成功\n";
            }
            else
            {
                std::cout << "文件“" << fs::path(origin_route).string() << "”和文件“" << result_route.string() << "”不同，测试失败\n";
            }
            fs::remove(result_route);
            fs::remove(".\\密钥");
            wait_input();


        }

        fs::current_path(original_working_dir);
        fs::remove_all(working_path);
    }

    void test_order_matcher()
    {
        class para_type final : public base::blank
        {
        private:
            using base_type = base::blank;

            std::uintmax_t const MEMBER_id;
        public:
            para_type(std::uintmax_t const id)
                :base_type()
                , MEMBER_id(id)
            {
                std::cout << "参数对象 " << id << " 构造\n";
            }
            para_type(para_type const & origin)
                :base_type(origin)
                , MEMBER_id(origin.MEMBER_id)
            {
                std::cout << "参数对象 " << origin.MEMBER_id << " 复制构造\n";
            }
            para_type(para_type &&) = delete;
            ~para_type()override
            {
                std::cout << "参数对象 " << this->MEMBER_id << " 析构\n";
            }
            std::uintmax_t const & get_id()const noexcept
            {
                return this->MEMBER_id;
            }
        };
        class handler_type final :public base::handler<false>
        {
        private:
            using base_type = base::handler<false>;
        public:
            handler_type()
                :base_type()
            {
                std::cout << "处理器对象默认构造\n";
            }
            handler_type(handler_type const & origin)
                :base_type(origin)
            {
                std::cout << "处理器对象复制构造\n";
            }
            handler_type(handler_type && origin)
                :base_type(std::move(origin))
            {
                std::cout << "处理器对象移动构造\n";
            }
            ~handler_type() override
            {
                std::cout << "处理器对象析构\n";
            }
        private:
            void operator ()(base::blank & arg)override
            {
                auto & TEMP_arg = dynamic_cast<para_type &>(arg);
                std::cout << "处理函数接收参数对象 " << TEMP_arg.get_id() << "\n";
            }
        };
        auto arg1 = para_type(1);
        auto handler1 = std::make_unique<handler_type>();
        order_matcher matcher;
        matcher.regist("order1", std::move(handler1));
        matcher.find("order1");
        matcher.execute("order1", arg1);
        matcher.erase("order1");
    }

    void test_message()
    {
        class arg_type final : public base::blank
        {
        private:
            std::string MEMBER_name;
        public:
            arg_type(std::string const & name)
                :base::blank()
                , MEMBER_name(name)
            {
                std::cout << "参数 " << name << " 构造\n";
            }
            ~arg_type()noexcept override
            {
                std::cout << "参数 " << this->MEMBER_name << " 析构\n";
            }
            arg_type(arg_type const &) = delete;
            arg_type & self() noexcept
            {
                return *this;
            }
            arg_type const & self()const noexcept
            {
                return *this;
            }
            arg_type * operator &() noexcept
            {
                return this;
            }
            arg_type const * operator &() const noexcept
            {
                return this;
            }
            std::string const & name()const noexcept
            {
                return this->MEMBER_name;
            }
        };
        class tester final :public base::handler<true>
        {
        private:
            std::string MEMBER_name;
        public:
            tester(std::string const & name)
                :base::handler<true>()
                , MEMBER_name(name)
            {
                std::cout << "handler " << this->MEMBER_name << " 构造\n";
            }
            ~tester()noexcept override
            {
                std::cout << "handler " << this->MEMBER_name << " 析构\n";
            }
            tester(tester const &) = delete;
            tester(tester &&) = delete;
            void operator =(tester const &) = delete;
            void operator =(tester &&) = delete;
            void operator ()(base::blank & arg) noexcept override
            {
                try
                {
                    std::cout << "handler " << this->MEMBER_name << " 被运行，参数为 " << dynamic_cast<arg_type &>(arg).name() << "\n";
                }
                catch (...)
                {
                    this->handler::raise_error(std::current_exception());
                }
            }
            tester * operator &()noexcept override
            {
                return this;
            }
            tester const * operator &()const noexcept override
            {
                return this;
            }
            tester & self()noexcept override
            {
                return *this;
            }
            tester const & self()const noexcept override
            {
                return *this;
            }
        };
        try
        {
            namespace fs = std::filesystem;

            using nanosecond = std::chrono::nanoseconds;

            std::cout << "测试信号引发系统\n";
            message m;
            auto t = std::make_shared<tester>("测试对象");
            auto ag = std::make_shared<arg_type>("参数0");
            std::cout << "注册信号\n";
            auto sig = m.regist(t);
            std::cout << "注册信号完毕\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            m.trigger(sig, ag);
            std::cout << "引发信号指令\n";

        }
        catch (std::exception const & hinter)
        {
            std::cout << "捕捉到异常 ││ " << hinter.what() << "\n";
            wait_input();
        }
        catch (...)
        {
            std::cout << "捕捉到未知异常\n";
            wait_input();
        }
    }

    void string_requirement(std::string const & expected)
    {

        for (std::size_t i = 0;i < expected.size();++i)
        {
            if (expected[i] == '\n')
            {
                throw std::invalid_argument("要求的输入中不能有换行符，因为此测试不能读取多行输入");
            }
        }

        class judgement :public string::requirement<std::string>
        {
        private:
            using base_type = string::requirement<std::string>;
            std::string const & MEMBER_expected;
        public:
            judgement(std::string const & expected)noexcept
                :base_type()
                , MEMBER_expected(expected)
            {
            }
            ~judgement()noexcept override
            {
            }
            judgement(judgement const &) = delete;
            void operator =(judgement const &) = delete;
            judgement(judgement &&) = delete;
            void operator =(judgement &&) = delete;

            bool operator()(std::string const & value)override
            {
                return value == this->MEMBER_expected;
            }
        };

        judgement judge(expected);

        std::cout << "原封不动地输入以下字符串 ││ " << expected << "\n"
            "输入 ││ ";
        std::string input;

        if (string::get_line<std::string>(std::cin, input, judge))
        {
            std::cout << "输入正确\n";
        }
        else
        {
            std::cout << "输入错误，这一行输入已被抛弃\n";
        }
    }

}