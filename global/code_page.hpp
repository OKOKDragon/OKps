/*
仅在面向windows系统时需要包含此文件。
*/

#include <memory>
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
        static std::unique_ptr<implement> const MEMBER_implement;
    public:
        /*
        content是以windows系统控制台默认的标准输入采用的编码方式编码的字符串
        此函数将其转换为utf-8编码
        */
        static std::string default_input_convert(std::string const & content);
        code_page() = delete;
        ~code_page() = delete;
        code_page(code_page const &) = delete;
        code_page(code_page &&) = delete;
        void operator =(code_page const &) = delete;
        void operator =(code_page &&) = delete;
    };

}