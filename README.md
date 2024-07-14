# Optimized-Vector

## Overview

A custom vector implementation with move and emplace semantics, exception safety, and performance benchmarking.

## Features

- **Move Semantics**: Efficient resource transfer.
- **Emplace Semantics**: In-place element construction.
- **Exception Safety**: Exception handling.
- **Performance Benchmarking**: Compare with `std::vector`, `boost::container::small_vector`, and `llvm::SmallVector`.

## Usage

### Interface

```cpp
namespace epc {

template <typename T, size_t N>
class vector {
public:
    vector() noexcept;
    vector(const vector&);
    vector& operator=(const vector&);
    vector(vector&&) noexcept;
    vector& operator=(vector&&) noexcept;
    ~vector();

    T* data();
    const T* data() const;
    T& operator[](size_t);
    const T& operator[](size_t) const;

    void push_back(const T&);
    void push_back(T&&);
    template <typename... Ts>
    void emplace_back(Ts&&...);
    void pop_back();
    void clear();
    void reserve(size_t);
    size_t capacity() const;
    size_t size() const;
    void swap(vector&) noexcept;
};

} // namespace epc
```

### Generics meaning

-T means the type of objects that will be stored inside the vector

-N means the size of initially allocated buffer, this buffer is allocated statically, so there is no dynamic memory allocation overhead.

### Use cases + Benchmarking results

Can be used as a sequential container for C++ programs.
Usage is similar to any other vector implementation, however this implementation provides a better performance when small preallocated buffer is utilized. After running provided benchmarking code, the following results  were collected:

GCC Compiler Results

    test<std::vector<value_type>>
        Time: 506091 ns
        CPU: 506066 ns
        Iterations: 1330

    test<epc::vector<value_type, 8>>
        Time: 124846 ns
        CPU: 124844 ns
        Iterations: 5623

    test<boost::container::small_vector<value_type, 8>>
        Time: 150709 ns
        CPU: 150702 ns
        Iterations: 4568

    test<llvm::SmallVector<value_type, 8>>
        Time: 179861 ns
        CPU: 179859 ns
        Iterations: 3872

Clang Compiler Results

    test<std::vector<value_type>>
        Time: 449343 ns
        CPU: 449293 ns
        Iterations: 1568

    test<epc::vector<value_type, 8>>
        Time: 144092 ns
        CPU: 144091 ns
        Iterations: 4846

    test<boost::container::small_vector<value_type, 8>>
        Time: 239774 ns
        CPU: 239767 ns
        Iterations: 2919

    test<llvm::SmallVector<value_type, 8>>
        Time: 177738 ns
        CPU: 177732 ns
        Iterations: 3936







