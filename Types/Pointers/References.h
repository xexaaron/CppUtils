#pragma once
#include <memory>
#include <functional>

namespace Utility {
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ...Args>
    constexpr Ref<T> CreateRef(Args&& ...args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

