//
// detail/thread_group.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_THREAD_GROUP_HPP
#define ASIO_DETAIL_THREAD_GROUP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/memory.hpp"
#include "asio/detail/thread.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace detail {

template <typename Allocator>
class thread_group
{
public:
  // Constructor initialises an empty thread group.
  explicit thread_group(const Allocator& a)
    : allocator_(a),
      first_(0)
  {
  }

  // Destructor joins any remaining threads in the group.
  ~thread_group()
  {
    join();
  }

  // Create a new thread in the group.
  template <typename Function>
  void create_thread(Function f)
  {
    first_ = allocate_object<item>(allocator_, allocator_, f, first_);
  }

  // Create new threads in the group.
  template <typename Function>
  void create_threads(Function f, std::size_t num_threads)
  {
    for (std::size_t i = 0; i < num_threads; ++i)
      create_thread(f);
  }

  // Wait for all threads in the group to exit.
  void join()
  {
    while (first_)
    {
      first_->thread_.join();
      item* tmp = first_;
      first_ = first_->next_;
      deallocate_object(allocator_, tmp);
    }
  }

  // Test whether the group is empty.
  bool empty() const
  {
    return first_ == 0;
  }

private:
  // Structure used to track a single thread in the group.
  struct item
  {
    template <typename Function>
    explicit item(const Allocator& a, Function f, item* next)
      : thread_(std::allocator_arg, a, f),
        next_(next)
    {
    }

    asio::detail::thread thread_;
    item* next_;
  };

  // The allocator to be used to create items in the group.
  Allocator allocator_;

  // The first thread in the group.
  item* first_;
};

} // namespace detail
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_DETAIL_THREAD_GROUP_HPP
