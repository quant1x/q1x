# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]

## [0.1.4] - 2025-07-09
### Changed
- 暂时保留加载配置文件时输出的配置文件路径

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


[Unreleased]: https://gitee.com/quant1x/q1x.git/compare/v0.1.4...HEAD
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
