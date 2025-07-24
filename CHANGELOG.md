# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]

## [0.1.20] - 2025-07-24
### Changed
- 修订数据类型长度不匹配的问题

## [0.1.19] - 2025-07-24
### Changed
- msvc编译修复类型长度不匹配的问题
- 适配msvc
- 删除1号策略
- update changelog

## [0.1.18] - 2025-07-23
### Changed
- 新增历史分笔成交记录的proto3协议
- 屏蔽matplotplusplus绘图工具
- 第一版日历js转c++解码
- 优化部分代码
- 新增http测试, 占位
- update changelog

## [0.1.17] - 2025-07-21
### Changed
- 默认静态库
- 明确当前编译的选项, 去掉表达式
- catch2支持unicode
- 从cmake主配置文件剥离protobuf
- 数据集各模块简要注释
- 财报数据无result字段时, 日志输出主要参数
- 新增f10相关的三个测试单元
- 特征数据加载日线、分笔成交数据为空时不抛异常输出告警级日志
- 拆分基础数据类型设定到datasets/base.h
- 微调部分代码格式
- 拆分筹码分布为基础数据
- 外部接口输出告警级日志明确模块信息
- 财报接口输出告警级日志明确模块信息
- 调整特征的告警信息模块名称为类名
- 新增chips读取protobuf缓存
- 新增matplotplusplus绘图库, windows下效果不好
- 新增double比较的模板函数,支持不同精度
- 新增波浪测试代码
- xsimd组件收敛于q1x/std/simd统一管理
- 静态分析模式增加波浪
- 更新xsimd库代码(bde4df7394ccc920aeafc6689aa2d28d8ab7e518)
- 更新xtensor库v0.27.0代码(c7ae1a6aae85bed9f6bf912da78c70b3a7f49900)
- 调整基础的波峰检测函数名, 增加前缀basic
- 新增数据点结构
- 优化wave代码结构
- 新增线性回归和波浪的测试代码
- 线增加基础函数, 平行线, 点到线距离, 对称平行线以及水平方向的镜像线
- 调整点的结构体名
- 删除K线的protobuf协议
- 调整chips协议文本
- 剔除全局变量前的static, 只保留inline
- 迁移go版本筹码峰, 日线三角形计算方法
- 测试输出数据日期及代码
- 调整输出文本格式
- 更新boost::pfr(5034bf55fb6bd1efa7a14955bbb7171c2b3491fc)
- 更新asio(v1.34.2)
- 适配asio新版本1.34.2, 拆分探测服务器延时功能
- 新增工程Release编译下的瘦身设置
- 拆分探测服务器延时功能函数, 定义和实现分离
- 基础数据增加历史分笔成交数据
- update命令行增加--end参数, 表示结束日期, --start以缓存日期为基准, 需要取start的上一个交易日, 以确保start日期的特征可以缓存
- 修复msvc编译整型溢出告警的问题
- 补充日线数据的加载方式
- 命名空间前留空白行
- 基础版本的检测波峰波谷函数改成模板函数
- 临时变量引用vector元素
- 强制vector尺寸为int类型
- 强制vector尺寸为int类型
- 屏蔽实验性质的cmake检测代码
- 优化chips测试代码
- 校验命令行传入的start和end两个参数
- 注明matplotplusplus在msvc编译无法通过, gcc编译通过, 但是无法完整的绘制图形
- update changelog

## [0.1.16] - 2025-07-12
### Changed
- 调整mingw下原backward-cpp cmake配置不准确的问题
- update changelog

## [0.1.15] - 2025-07-12
### Changed
- 调整macosx下的backward设置
- 补充fmt版本号
- update changelog

## [0.1.14] - 2025-07-12
### Changed
- 从cmake主配置文件剥离backward-cpp,network,threadpool
- llvm-clang崩溃输出详细的调用栈不能带-Ox优化选项, 这样的话, 虽然可以输出调用栈但是优势不明显, 可以自由选择用msvc还是clang
- update changelog

## [0.1.13] - 2025-07-11
### Changed
- 删除从runtime查找第三方库的设置
- 从cmake主配置文件剥离spdlog
- 修正头文件宏
- 从cmake主配置文件剥离fmt
- 调整zlib顺序
- 从cmake主配置文件剥离backward-cpp,iconv,asio,openssl
- 从cmake主配置文件剥离simd
- update changelog

## [0.1.12] - 2025-07-11
### Changed
- 新增q2x应用程序, 调整插件路径为q1x/engine
- update changelog

## [0.1.11] - 2025-07-11
### Changed
- 调整iconv头文件的搜索路径优先级
- 屏蔽mingw的backward宏定义
- update changelog

## [0.1.10] - 2025-07-11
### Changed
- 补充README.md
- q1x库发布cmake配置删除spdlog,fmt,date
- 删除废弃的msvc显示命令行的设置
- 明确指定windows下MSVC编译时禁用backward的POSIX选项
- 删除废弃的backward初始化代码, 由内部封装的部分实现
- 测试Clang异常崩溃调用栈, 可以输出调用栈, 但是不精准, 只能定位到crash_function, 不能定位到*a=1这一行
- 明确在windows环境下统一/MT, 静态多线程编译
- 去掉多余的-lpthread
- debug版本库增加后缀d, windows系统增加-win
- update changelog

## [0.1.9] - 2025-07-10
### Changed
- cmake project函数前新增执行cmake策略
- 收录HowardHinnant date v3.0.4
- 收录spdlog v1.15.3
- 收录fmt v11.2.0
- vcpkg 删除spdlog,fmt,date
- 屏蔽废弃的代码
- 新增测试交易日的文件修改时间戳
- 微调注释
- 删除废弃的代码
- http请求中过期时间改用毫秒数, 去掉对chrono的依赖
- 修复clang对此处std::move用法影响性能的问题
- 新增测试交易日的文件修改时间戳
- 调整对文件时间戳的获取方法
- 优化clang在cmake的编译选项
- 计划用变量缓存编译选项, 方便管理
- update changelog

## [0.1.8] - 2025-07-10
### Changed
- 修复引用错误了已经从顶部移除的对象top_task
- 调整编译选项
- update changelog

## [0.1.7] - 2025-07-09
### Changed
- 删除冗余的日志
- update changelog

## [0.1.6] - 2025-07-09
### Changed
- 修复由于过渡优化出现的top task对象无效的bug
- history调用f10必须日期对齐，使用cache日期, 即下一个交易日
- update changelog

## [0.1.5] - 2025-07-09
### Changed
- datafrmae组件统一头文件预编译宏前缀为QUANT1X
- update changelog

## [0.1.4] - 2025-07-09
### Changed
- 暂时保留加载配置文件时输出的配置文件路径
- update changelog

## [0.1.3] - 2025-07-09
### Changed
- 屏蔽指定标准库的设置，mingw不支持
- update changelog

## [0.1.2] - 2025-07-09
### Changed
- 拆分编译选项, 可向下游项目传播编译参数
- 优化cmake编译选项
- 将构建信息的文件安装到q1x
- update changelog

## [0.1.1] - 2025-07-09
### Changed
- 非windows环境, 从本地runtime链接lib
- 调整非MSVC编译环境参数
- 强制检测是否Debug
- 输出编译器选项参数
- 尝试查询编译选项
- 调整查看链接库的参数
- 新增查看所有链接库的方法
- 非msvc的debug增加-g参数
- 调整部分代码
- 调整部分函数
- 优化价格笼子字段
- update changelog

## [0.1.0] - 2025-07-08
### Changed
- 调整默认安装路径
- 新增clangd配置文件
- 删除忽略静态库的git配置
- 添加protobuf v3.12.11
- 添加数据协议
- 添加资源文件
- 新增依赖库列表
- 调整包含头文件的书写方式
- update changelog

## [0.0.6] - 2025-07-07
### Changed
- 兼容msvc
- 调整包路径, 输出quant1x::q1x工具库
- 更新全部组件
- 调整cmake测试宏
- 新增崩溃测试应用
- 新增安全的strerror
- 补充缺少的头文件
- update changelog

## [0.0.5] - 2025-06-29
### Changed
- 强制MSVC使用静态编译
- 静态分析修复部分潜在问题
- 将vcpkg临时目录vcpkg_installed加入.gitignore强制MSVC使用静态编译
- 将vcpkg临时目录vcpkg_installed加入.gitignore强制MSVC使用静态编译
- Merge branch '0.0.x' of https://gitee.com/quant1x/api into 0.0.x
- 修订MSVC不支持0.0/0.0返回nan的问题
- 增加网络net组件
- 增加nan和±Inf常量
- 新增dataframe组件
- 增加formula组件
- 新增formula测试代码
- 在类模板内部，构造函数、析构函数、拷贝/移动构造和赋值运算符都不能写模板参数列表（即不能写 <T>），只需用类名本身。C++20 对这点要求更严格，GCC/Clang 会给出警告甚至错误。
- 梳理头文件列表
- 测试用例增加终端对utf-8的支持
- 统一引用头文件的书写格式
- 构建cmake find_package机制
- 封装通过git tag检查版本的cmake工具函数
- 修订部分函数
- update changelog

## [0.0.4] - 2025-06-26
### Changed
- 调整cmake代码归类
- 新增README.md
- 暴露终端字符集函数
- update changelog

## [0.0.3] - 2025-06-26
### Changed
- 从quant1x项目中迁移基础库api
- update changelog

## [0.0.2] - 2025-06-25
### Changed
- 规范代码格式
- update changelog

## [0.0.1] - 2025-06-25
### Changed
- git仓库忽略vscode、vs2022和clion开发工具的目录
- Visual Studio指定文件的字符集和换行符
- 构建一个最简单的v0.0.1版本的工具库项目组织架构


[Unreleased]: https://gitee.com/quant1x/q1x.git/compare/v0.1.20...HEAD
[0.1.20]: https://gitee.com/quant1x/q1x.git/compare/v0.1.19...v0.1.20
[0.1.19]: https://gitee.com/quant1x/q1x.git/compare/v0.1.18...v0.1.19
[0.1.18]: https://gitee.com/quant1x/q1x.git/compare/v0.1.17...v0.1.18
[0.1.17]: https://gitee.com/quant1x/q1x.git/compare/v0.1.16...v0.1.17
[0.1.16]: https://gitee.com/quant1x/q1x.git/compare/v0.1.15...v0.1.16
[0.1.15]: https://gitee.com/quant1x/q1x.git/compare/v0.1.14...v0.1.15
[0.1.14]: https://gitee.com/quant1x/q1x.git/compare/v0.1.13...v0.1.14
[0.1.13]: https://gitee.com/quant1x/q1x.git/compare/v0.1.12...v0.1.13
[0.1.12]: https://gitee.com/quant1x/q1x.git/compare/v0.1.11...v0.1.12
[0.1.11]: https://gitee.com/quant1x/q1x.git/compare/v0.1.10...v0.1.11
[0.1.10]: https://gitee.com/quant1x/q1x.git/compare/v0.1.9...v0.1.10
[0.1.9]: https://gitee.com/quant1x/q1x.git/compare/v0.1.8...v0.1.9
[0.1.8]: https://gitee.com/quant1x/q1x.git/compare/v0.1.7...v0.1.8
[0.1.7]: https://gitee.com/quant1x/q1x.git/compare/v0.1.6...v0.1.7
[0.1.6]: https://gitee.com/quant1x/q1x.git/compare/v0.1.5...v0.1.6
[0.1.5]: https://gitee.com/quant1x/q1x.git/compare/v0.1.4...v0.1.5
[0.1.4]: https://gitee.com/quant1x/q1x.git/compare/v0.1.3...v0.1.4
[0.1.3]: https://gitee.com/quant1x/q1x.git/compare/v0.1.2...v0.1.3
[0.1.2]: https://gitee.com/quant1x/q1x.git/compare/v0.1.1...v0.1.2
[0.1.1]: https://gitee.com/quant1x/q1x.git/compare/v0.1.0...v0.1.1
[0.1.0]: https://gitee.com/quant1x/q1x.git/compare/v0.0.6...v0.1.0
[0.0.6]: https://gitee.com/quant1x/q1x.git/compare/v0.0.5...v0.0.6
[0.0.5]: https://gitee.com/quant1x/q1x.git/compare/v0.0.4...v0.0.5
[0.0.4]: https://gitee.com/quant1x/q1x.git/compare/v0.0.3...v0.0.4
[0.0.3]: https://gitee.com/quant1x/q1x.git/compare/v0.0.2...v0.0.3
[0.0.2]: https://gitee.com/quant1x/q1x.git/compare/v0.0.1...v0.0.2

[0.0.1]: https://gitee.com/quant1x/q1x.git/releases/tag/v0.0.1
