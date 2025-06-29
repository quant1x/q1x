# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]

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


[Unreleased]: https://gitee.com/quant1x/api.git/compare/v0.0.5...HEAD
[0.0.5]: https://gitee.com/quant1x/api.git/compare/v0.0.4...v0.0.5
[0.0.4]: https://gitee.com/quant1x/api.git/compare/v0.0.3...v0.0.4
[0.0.3]: https://gitee.com/quant1x/api.git/compare/v0.0.2...v0.0.3
[0.0.2]: https://gitee.com/quant1x/api.git/compare/v0.0.1...v0.0.2

[0.0.1]: https://gitee.com/quant1x/api.git/releases/tag/v0.0.1
