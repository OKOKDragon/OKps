#pragma once

//导入标准库

//c
//工具库
#include <csetjmp>//保存及跳转到执行环境的宏和函数
#include <csignal>//信号管理
#include <cstdarg>//变长实参列表
#include <cstddef>//c标准宏、typedef
#include <cstdlib>//c通用工具，包含内存分配、随机数、排序、搜索
#include <ctime>//c时间工具
//数值和极限
#include <cfloat>//浮点类型
#include <cinttypes>//格式化宏，数学与转换
#include <climits>//整数类型的极限
#include <cstdint>//定宽整数类型与其他类型的极限
//错误处理
#include <cassert>//条件编译宏
#include <cerrno>//含有最后一次错误号的宏
//字符串
#include <cctype>//确定窄字符类别的函数
#include <cstring>//窄字符串处理函数
#include <cuchar>//unicode字符转换函数
#include <cwchar>//宽字符串、多字节字符串处理函数
#include <cwctype>//确定宽字符类别的函数
//数学库
#include <cfenv>//浮点环境访问函数
#include <cmath>//数学函数
//本地化
#include <clocale>
//输入输出
#include <cstdio>

//c++11及更低版本
//工具库
#include <bitset>
#include <chrono>//c++时间工具
#include <functional>
#include <initializer_list>
#include <tuple>
#include <type_traits>//编译时类型信息
#include <typeindex>
#include <typeinfo>//运行是类型信息
#include <utility>
//动态内存管理
#include <memory>
#include <new>
#include <scoped_allocator>//嵌套分配器
//数值极限
#include <limits>
//错误处理
#include <exception>
#include <stdexcept>//标准异常对象
#include <system_error>//依赖平台的错误码
//字符串
#include <string>
//容器
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
//迭代器
#include <iterator>
//算法
#include <algorithm>
//数值
#include <complex>
#include <numeric>
#include <random>
#include <ratio>//编译期有理数算术
#include <valarray>
//本地化
#include <locale>
/*
unicode转换
在c++17弃用，在c++26移除
*/
#include <codecvt>
//输入输出
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
//正则表达式
#include <regex>
//原子操作
#include <atomic>
//线程
#include <condition_variable>//线程等待条件
#include <future>//异步计算元件
#include <mutex>
#include <thread>

//c++14
//线程
#include <shared_mutex>

//c++17
//工具库
#include <any>
#include <optional>
#include <variant>
//动态内存管理
#include <memory_resource>//多态分配器和内存资源
//字符串
#include <charconv>
#include <string_view>
//算法
#include <execution>//并行版本算法的预定义执行策略
//文件系统
#include <filesystem>

//c++20
//概念库
#include <concepts>
//协程库
#include <coroutine>
//工具库
#include <compare>//三路运算符
#include <source_location>//获取源码位置
#include <version>//提供依赖实现的库信息
//字符串
#include <format>//格式化
//容器
#include <span>
//范围
#include <ranges>
//数值
#include <bit>//位操作
#include <numbers>//数学常数
//输入输出
#include <syncstream>
//线程
#include <latch>
#include <semaphore>//信号量
#include <stop_token>

//c++23
//工具库
//#include <expected>
//数值极限
//#include <stdfloat>//拓展浮点类型
//异常处理
//#include <stacktrace>//栈追踪
//容器
//#include <flat_map>
//#include <flat_set>
//#include <mdspan>
//范围
//#include <generator>
//输入输出
//#include <print>
//#include <spanstream>

//c++26
//工具库
//#include <debugging>
//数值
//#include <linalg>//基本线性代数算法
//本地化
//#include <text_encoding>
//线程
//#include <hazard_pointer>//风险指针
