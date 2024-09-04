﻿#pragma once

/*
此静态库的导出头文件
包含此头文件，就能使用此库的所有功能

既然使用c++，我希望不要有莫名其妙的限制，尤其是很多程序员痛恨的异常和重载。
看看c标准库，没有重载导致语义相同的函数不得不写很多个，比如字符串和算术类型的转换，不得不对 long 、int、long long等分别都用不同的函数名，为了缩短函数名，又要用极简的缩写命名。
就我的体验而言，这样的函数我用起来并不舒适。

异常更是被很多人视为洪水猛兽，他们认为只应该用错误码。
他们可能没搞清异常的用途。
c++标准库中的异常，要么是来自外部的、不可控的严重错误（内存分配失败），
要么是检测到未定义行为（std::any检测到错误的类型转换、std::vector的at函数检测到下标访问越界），
要么是传递系统底层的错误(std::system_error)。
而像因权限不足无法访问文件，std::cin和std::cout发生的可恢复性的输入输出错误，标准库是用错误码的形式。
所以，异常是用来报告严重错误的，这些错误要么是不可控的，要么就是写出了在语言层面就绝对不应该写出来的代码（如下标越界），
不论哪种，都是不可恢复的错误。
异常是用来报告“不可恢复的错误”的！异常不是用来报告用户输入不合法、http连接失败这类业务逻辑错误的。
发生异常的时候，程序就应该崩溃。之所以抛出异常，只是为了让程序记录崩溃日志或者执行一些清理代码，
不是为了让程序强行处理它不该处理的错误，并继续带病运行下去的！
对于这样的错误，就是应该使用异常，而不是任何其他机制。
对于可恢复性的业务逻辑错误，再去考虑用错误码或其他机制。
*/

//#include ".\stl.hpp"

#include ".\blank.hpp"
#include ".\base.hpp"
#include ".\string.hpp"
#include ".\tree.hpp"
#include ".\lock.hpp"
#include ".\fundamental.hpp"
#include ".\timer.hpp"
#include ".\cipher.hpp"
#include ".\message.hpp"
#include ".\storage.hpp"
#include ".\matcher.hpp"
#include ".\log.hpp"