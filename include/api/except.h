#pragma once
#ifndef API_EXCEPT_H
#define API_EXCEPT_H 1

#include <iostream>
#include <exception>
#include <string>
#include <stdexcept>
#include <utility>
#include <mutex>

// 自定义异常类（继承自 std::exception）
class BaseException : public std::exception {
private:
    std::string msg_;
    std::string file_;
    int line_;

public:
    BaseException(std::string msg, std::string  file, int line)
            : msg_(std::move(msg)), file_(std::move(file)), line_(line) {}

    [[nodiscard]] const char* what() const noexcept override {
        return msg_.c_str();
    }

    [[nodiscard]] std::string getFile() const { return file_; }
    [[nodiscard]] int getLine() const { return line_; }
};

// 封装 throw 的宏（自动注入文件名和行号）
#define THROW_EXCEPTION(msg) throw BaseException(msg, __FILE__, __LINE__)

// 自定义错误类别，携带特定消息
class dynamic_error_category : public std::error_category {
    std::string msg_;
public:
    explicit dynamic_error_category(std::string msg) : msg_(std::move(msg)) {}

    const char* name() const noexcept override {
        return "DynamicError";
    }

    std::string message(int ev) const override {
        return msg_;
    }

    // 可选：用于支持比较逻辑
    bool equivalent(const std::error_code& code, int condition) const noexcept override {
        return code.value() == condition;
    }
};

// 缓存池：线程安全的单例管理器
class category_cache {
    std::unordered_map<std::string, std::weak_ptr<std::error_category>> cache_;
    mutable std::mutex mtx_;

public:
    static category_cache& instance() {
        static category_cache inst;
        return inst;
    }

    std::shared_ptr<std::error_category> get_or_create(const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = cache_.find(message);
        if (it != cache_.end()) {
            if (auto sp = it->second.lock()) {
                return sp; // 命中缓存
            }
        }

        // 创建新的 category
        auto sp = std::make_shared<dynamic_error_category>(message);
        cache_[message] = sp;
        return sp;
    }
};

// 工厂函数
inline std::error_code make_error_code(int err_code, std::string message) {
    auto category = category_cache::instance().get_or_create(message);
    return {err_code, *category};
}

#endif //API_EXCEPT_H
