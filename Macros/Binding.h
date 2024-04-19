#pragma once

#include <functional>
#include <type_traits>
#include <utility>

#define BIND_MEMBER_FN(fn) [this](auto&&... args) { return this->fn(std::forward<decltype(args)>(args)...); }

#define BIND_FN(fn) [](auto&&... args) { return fn(std::forward<decltype(args)>(args)...); }