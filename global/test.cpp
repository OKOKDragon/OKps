#include <iostream>
#include <stdexcept>

#include ".\cipher.hpp"
#include ".\timer.hpp"
#include ".\message.hpp"
#include ".\matcher.hpp"

#include ".\test.hpp"

namespace OKps::test
{

    void marked_reference()
    {
        base::marked * m = new base::marked;
        base::reference ref(*m);
        if (ref.is_valid())
        {
            std::cout << "引用有效\n";
            if ((&(ref.get())) == m)
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
            if ((&(ref.get())) == m)
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
            if ((&(ref.get())) == m)
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
            if ((&(ref.get())) == m)
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
            t1 < t2;
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
        class para_type final : public order_matcher::parameter
        {
        private:
            using base_type = order_matcher::parameter;

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
        class handler_type final :public order_matcher::handler
        {
        private:
            using base_type = order_matcher::handler;
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
            void handle(argument_type arg)
            {
                auto temp_arg = dynamic_cast<para_type *>(arg.get());
                std::cout << "处理函数接收参数对象 " << temp_arg->get_id() << "\n";
            }
        };
        auto arg1 = std::make_unique<para_type>(1);
        auto handler1 = std::make_unique<handler_type>();
        order_matcher matcher;
        matcher.regist("order1", std::move(handler1));
        matcher.find("order1");
        matcher.execute("order1", std::move(arg1));
        matcher.erase("order1");
    }

    void test_message()
    {
        class tester final :public message::handler
        {
        private:
            std::string MEMBER_name;
        public:
            tester(std::string const & name)
                :message::handler()
                , MEMBER_name(name)
            {
            }
            ~tester()noexcept override
            {
            }
            void handle() noexcept override
            {
                try
                {
                    std::cout << "handler " << this->MEMBER_name << " 被运行\n";
                }
                catch (...)
                {
                    this->handler::raise_exception(std::current_exception());
                }
            }
        };
        try
        {
            namespace fs = std::filesystem;

            using nanosecond = std::chrono::nanoseconds;

            std::cout << "测试信号引发系统\n";
            message m;
            auto t = std::make_unique<tester>("测试对象");
            auto sig = m.regist(std::move(t));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            m.trigger(sig);

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

}