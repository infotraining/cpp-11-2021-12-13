#include "catch.hpp"
#include "gadget.hpp"
#include <memory>
#include <string>

template <typename T>
class UniquePtr
{
    T* ptr_;

public:
    UniquePtr(nullptr_t)
        : ptr_ {nullptr}
    {
    }

    UniquePtr()
        : ptr_ {nullptr}
    {
    }

    explicit UniquePtr(T* ptr)
        : ptr_ {ptr}
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& source) noexcept
        : ptr_ {source.ptr_}
    {
        source.ptr_ = nullptr;
    }

    // UniquePtr& operator=(UniquePtr&& source)
    // {
    //     if (this != &source)
    //     {
    //         delete ptr_;
            
    //         ptr_ = source.ptr_;
    //         source.ptr_ = nullptr;
    //     }

    //     return *this;
    // }

    UniquePtr& operator=(UniquePtr&& source) noexcept
    {
        if (this != &source)
        {
            UniquePtr<T> temp = std::move(source); // move constructor
            swap(temp);
        }

        return *this;
    }

    ~UniquePtr()
    {
        delete ptr_;
    }

    explicit operator bool() const
    {
        return ptr_ != nullptr;
    }

    T* get() const
    {
        return ptr_;
    }

    T* operator->() const
    {
        return ptr_;
    }

    T& operator*() const
    {
        return ptr_;
    }

    void swap(UniquePtr& other) noexcept
    {
        std::swap(ptr_, other.ptr_);
    }
};

TEST_CASE("2---")
{
    std::cout << "\n--------------------------\n\n";
}

template <typename T, typename TArg1, typename TArg2>
UniquePtr<T> MakeUnique(TArg1&& arg1, TArg2&& arg2)
{
    return UniquePtr<T>{ new T(std::forward<TArg1>(arg1), std::forward<TArg2>(arg2)) };
}

TEST_CASE("move semantics - UniquePtr")
{
    //UniquePtr<Gadget> pg1 {new Gadget {1, "ipad"}};
    UniquePtr<Gadget> pg1 = MakeUnique<Gadget>(1, "ipad");
    pg1->use();

    UniquePtr<Gadget> pg2 = std::move(pg1);
    pg2->use();

    UniquePtr<Gadget> pg3 = nullptr;
}