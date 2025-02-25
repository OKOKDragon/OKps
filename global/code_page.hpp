/*
仅在面向windows系统时需要包含此文件。
*/

#include <string>

namespace OKps
{
    /*
    此类记录和管理windows终端的代码页
    当程序初始化时，此类将windows终端的代码页设置为utf-8编码，并记录下修改之前的代码页，以备不时之需。
    而linux终端默认为utf-8编码，故不需要此类。
    */
    class code_page final
    {
    private:
        class implement;
        static implement MEMBER_implement;
    public:
        /*
        content是从main函数输入的命令行参数
        此函数将其转换为utf-8编码
        */
        static std::string default_input_to_UTF_8(std::string const & content);
        code_page() = delete;
        ~code_page() = delete;
        code_page(code_page const &) = delete;
        code_page(code_page &&) = delete;
        void operator =(code_page const &) = delete;
        void operator =(code_page &&) = delete;
        static std::string UTF_16_to_UTF_8(std::wstring const & content);
        /*
        获取当前进程相应的可执行文件所在的目录
        utf-8编码
        */
        static std::string get_process_directory();
    };

}