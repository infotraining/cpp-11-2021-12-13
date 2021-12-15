#include "catch.hpp"
#include <iostream>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

class Data
{
    std::string name_;
    int* data_;
    size_t size_;

public:
    using iterator = int*;
    using const_iterator = const int*;

    Data(const std::string& name, std::initializer_list<int> list)
        : name_ {name} // copy
        , data_ {new int[list.size()]}
        , size_ {list.size()}
    {
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }

#if _cplusplus > 201102L
    Data(std::string&& name, std::initializer_list<int> list)
        : name_ {std::move(name)} // move
        , data_ {new int[list.size()]}
        , size_ {list.size()}
    {
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }
#endif

    Data(const Data& other)
        : name_(other.name_)
        , data_ {new int[other.size_]}
        , size_(other.size_)
    {
        std::copy(other.begin(), other.end(), data_);

        std::cout << "Data(" << name_ << ": cc)\n";
    }

    Data& operator=(const Data& other)
    {
        Data temp(other);
        swap(temp);

        std::cout << "Data=(" << name_ << ": cc)\n";

        return *this;
    }

    /////////////////////////////////////////////////
    // move constructor
    Data(Data&& source) noexcept
        : name_ {std::move(source.name_)} // ok
        , data_ {source.data_} // ok
        , size_ {source.size_} // ok
    {
        source.data_ = nullptr;  // ok
        source.size_ = 0; // ok

        std::cout << "Data(" << name_ << ": mv)\n";
    }

    /////////////////////////////////////////////////
    // move assignment
    Data& operator=(Data&& other)
    {
        if (this != &other)
        {
            Data temp(std::move(other));
            swap(temp);

            std::cout << "Data=(" << name_ << ": mv)\n";
        }
        return *this;
    }

    ~Data() noexcept
    {
        delete[] data_;
    }

    void swap(Data& other)
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    iterator begin() noexcept
    {
        return data_;
    }

    iterator end() noexcept
    {
        return data_ + size_;
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }

    void print(const std::string& prefix)
    {
        std::cout << prefix << ": " << name_ << " - [ ";
        for (auto it = begin(); it != end(); ++it)
            std::cout << *it << " ";
        std::cout << "]/n";
    }
};

namespace ModernCpp
{
    class Data
    {
        std::string name_;
        std::vector<int> data_;

    public:
        using iterator = std::vector<int>::iterator;
        using const_iterator = std::vector<int>::const_iterator;

        Data(std::string name, std::initializer_list<int> list)
            : name_ {std::move(name)}
            , data_ {list}
        {
            std::cout << "Data(" << name_ << ")\n";
        }

        void swap(Data& other)
        {
            name_.swap(other.name_);
            std::swap(data_, other.data_);
        }

        iterator begin()
        {
            return data_.begin();
        }

        iterator end()
        {
            return data_.end();
        }

        const_iterator begin() const
        {
            return data_.begin();
        }

        const_iterator end() const
        {
            return data_.end();
        }
    };
}

namespace LegacyCode
{
    Data* create_data_set()
    {
        return new Data {"data-set-one", {54, 6, 34, 235, 64356, 235, 23}};
    }
}

Data create_data_set()
{
    std::string name = "data-set-one";
    Data ds {name, {54, 6, 34, 235, 64356, 235, 23}};

    return ds;
}

template <typename TContainer>
void print(const std::string& prefix, const TContainer& container)
{
    std::cout << prefix << " - [ ";
    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

TEST_CASE("3---")
{
    std::cout << "\n--------------------------\n\n";
}

TEST_CASE("Data & move semantics")
{
    Data ds1 {"ds1", {1, 2, 3, 4, 5}};

    Data backup = ds1; // copy
    print("backup", backup);

    Data target = std::move(ds1); // move
    print("target", target);
}

namespace RuleOfFive
{

    struct DataRow
    {
        Data row1;
        Data row2;

        DataRow(const DataRow&) = default;
        DataRow& operator=(const DataRow&) = default;
        DataRow(DataRow&&) = default;
        DataRow& operator=(DataRow&&) = default;
        ~DataRow() = default;

        void print() const
        {
            ::print("row1: ", row1);
            ::print("row2: ", row2);
        }
    };
}

namespace RuleOfZero
{
    struct DataRow
    {
        Data row1;
        Data row2;

        void print() const
        {
            ::print("row1: ", row1);
            ::print("row2: ", row2);
        }
    };
}

TEST_CASE("DataRow")
{
    using namespace RuleOfFive;

    DataRow row {Data {"one", {1, 2, 3}}, Data {"two", {4, 5, 6}}};

    DataRow backup = row;
    DataRow target = std::move(row); // copy
}

struct Type
{
    int value;

    void swap(Type& other) noexcept
    {
        int temp = value;
        value = other.value;
        other.value = temp;
    }
};

void foo(Type& a, Type& b) noexcept(noexcept(a.swap(b)))
{
    a.swap(b);
}

TEST_CASE("noexcept")
{
    std::cout << "\n--------------------noexcept\n";

    std::vector<Data> vec;

    vec.push_back(Data {"ds1", {1, 2, 3}});

    std::cout << "---\n";
    vec.push_back(Data {"ds2", {1, 2, 3}});

    std::cout << "---\n";
    vec.push_back(Data {"ds3", {1, 2, 3}});

    std::cout << "---\n";
    vec.push_back(Data {"ds4", {1, 2, 3}});
}