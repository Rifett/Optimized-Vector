#ifndef EPC_VECTOR_H
#define EPC_VECTOR_H

#include <cstdlib>
#include <algorithm>
#include <memory>

//This is my final implementation
//Hope you will enjoy reading it 
// :>

namespace epc
{
    template <typename T, size_t N>
    class vector
    {
    private:
        T *data_;
        size_t capacity_;
        size_t size_;
        alignas(T) std::byte buffer_[N * sizeof(T)];

    public:
        vector() noexcept : data_(reinterpret_cast<T*>(buffer_)), capacity_(N), size_(0) { };

        vector(const vector &other) : vector() {
            this->copyData(other);
            capacity_ = other.capacity_;
            size_ = other.size_;
        };
        vector &operator=(const vector &other) {
            if (&other == this) return *this;
            vector tmpCopy(other);
            this->swap(tmpCopy);
            return *this;
        };

        vector(vector &&other) : vector() {
            this->swap(other);
        };
        vector& operator=(vector &&other) {
            if (&other == this) return *this;
            vector tmpCopy(std::move(other));
            this->swap(tmpCopy);
            return *this;
        };

        ~vector() {
            std::destroy_n(data_, size_);
            if (!this->isShort()) {
                operator delete(data_);
            }
        };

        T *data() {
            return data_;
        };
        const T *data() const {
            return data_;
        };

        T &operator[](size_t index) {
            return data_[index];
        };
        const T &operator[](size_t index) const {
            return data_[index];
        };

        void push_back(const T &newElement) {
            if (capacity_ == size_)
                this->reserve(capacity_ * 2);
            new (data_ + size_) T(newElement);
            ++size_;
        };
        void push_back(T &&newElement) {
            if (capacity_ == size_)
                this->reserve(capacity_ * 2);
            new (data_ + size_) T(std::move(newElement));
            ++size_;
        };

        template <typename... Ts>
        void emplace_back(Ts &&...args) {
            if (capacity_ == size_)
                this->reserve(capacity_ * 2);
            new (data_ + size_) T( std::forward<Ts>(args)... );
            ++size_;
        };

        void pop_back() {
            data_[--size_].~T();
        };

        void clear() {
            std::destroy_n(data_, size_);
            size_ = 0;
        };

        void reserve(size_t newCapacity) {
            if (newCapacity <= capacity_) return;
            copyData(newCapacity);
            capacity_ = newCapacity;
        };

        size_t capacity() const {
            return capacity_;
        };

        size_t size() const {
            return size_;
        };

        void swap(vector &other) {
            if (this->isShort()) {
                if (other.isShort())
                    this->swapShortWithShort(other);
                else
                    this->swapShortWithLong(other);
            } else {
                if (other.isShort())
                    other.swapShortWithLong(*this);
                else
                    this->swapLongWithLong(other);
            }
        };

    private:
        bool isShort() const {
            return capacity_ == N;
        }

        void copyData(const vector &from) {
            if ( from.isShort() ) {
                copyFromShort(from);
            } else {
                copyFromLong(from);
            }
        }
        void copyFromShort(const vector &from) {
            std::uninitialized_copy_n(from.data_, from.size_, data_);
        }
        void copyFromLong(const vector &from) {
            data_ = static_cast<T*> (operator new(sizeof(T) * from.capacity_));
            try {
                std::uninitialized_copy_n(from.data_, from.size_, data_);
            } catch (...) {
                operator delete(data_);
                data_ = reinterpret_cast<T*> (buffer_);
                throw;
            }
        }

        void copyData(size_t newCapacity) {
            T *newData = static_cast<T*> (operator new (sizeof(T) * newCapacity));
            try {
                std::uninitialized_move_n(data_, size_, newData);
            } catch (...) {
                operator delete (newData);
                throw;
            }
            std::destroy_n(data_, size_);
            if (!this->isShort())
                operator delete(data_);
            data_ = newData;
        }

        void swapShortWithShort(vector &other) {
            vector* smaller, *bigger;
            if (this->size_ > other.size_) {
                smaller = &other; bigger = this;
            } else {
                smaller = this; bigger = &other;
            }
            std::swap_ranges(smaller->data_, smaller->data_ + smaller->size_, bigger->data_);
            std::uninitialized_move_n(bigger->data_ + smaller->size_, bigger->size_ - smaller->size_, smaller->data_ + smaller->size_);
            std::destroy_n(bigger->data_ + smaller->size_, bigger->size_ - smaller->size_);
            std::swap(this->size_, other.size_);
        }
        void swapShortWithLong(vector &other) {
            std::uninitialized_move_n(this->data_, this->size_, reinterpret_cast<T*> (other.buffer_));
            std::destroy_n(this->data_, this->size_);
            this->data_ = other.data_;
            other.data_ = reinterpret_cast<T*>(other.buffer_);
            std::swap(this->size_, other.size_);
            std::swap(this->capacity_, other.capacity_);
        }
        void swapLongWithLong(vector &other) {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
        }
    };
}


#endif //EPC_VECTOR_H
