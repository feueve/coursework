#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <memory>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstdio>

template<typename T>
class CircularBuffer {
public:
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using size_type = std::size_t;

    // Конструкторы
    explicit CircularBuffer(size_type capacity);
    CircularBuffer(size_type capacity, const_reference value);
    CircularBuffer(std::initializer_list<T> init);

    // Правило пяти
    CircularBuffer(const CircularBuffer& other);
    CircularBuffer(CircularBuffer&& other) noexcept;
    CircularBuffer& operator=(const CircularBuffer& other);
    CircularBuffer& operator=(CircularBuffer&& other) noexcept;
    ~CircularBuffer();

    // Доступ к элементам
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    reference operator[](size_type index);
    const_reference operator[](size_type index) const;

    // Состояние
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] bool full() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] size_type capacity() const noexcept;

    // Модификаторы
    void push(const_reference value);
    void push(T&& value);
    template<typename... Args>
    void emplace(Args&&... args);
    void pop();
    void clear() noexcept;
    void resize(size_type new_capacity);

    // Файловые операции
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToTextFile(const std::string& filename) const;
    void loadFromTextFile(const std::string& filename);

    // Итераторы
    class iterator;
    class const_iterator;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

private:
    std::unique_ptr<T[]> buffer_;
    size_type capacity_;
    size_type head_;
    size_type tail_;
    size_type size_;

    size_type next_index(size_type index) const noexcept;
    void advance_head() noexcept;
    void advance_tail() noexcept;
};

// Реализация методов

template<typename T>
CircularBuffer<T>::CircularBuffer(size_type capacity)
        : buffer_(std::make_unique<T[]>(capacity))
        , capacity_(capacity)
        , head_(0)
        , tail_(0)
        , size_(0) {
    if (capacity == 0) {
        throw std::invalid_argument("Capacity must be greater than 0");
    }
}

template<typename T>
CircularBuffer<T>::CircularBuffer(size_type capacity, const_reference value)
        : CircularBuffer(capacity) {
    for (size_type i = 0; i < capacity; ++i) {
        push(value);
    }
}

template<typename T>
CircularBuffer<T>::CircularBuffer(std::initializer_list<T> init)
        : CircularBuffer(init.size()) {
    for (const auto& item : init) {
        push(item);
    }
}

template<typename T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer& other)
        : buffer_(std::make_unique<T[]>(other.capacity_))
        , capacity_(other.capacity_)
        , head_(other.head_)
        , tail_(other.tail_)
        , size_(other.size_) {

    for (size_type i = 0; i < capacity_; ++i) {
        buffer_[i] = other.buffer_[i];
    }
}

template<typename T>
CircularBuffer<T>::CircularBuffer(CircularBuffer&& other) noexcept
        : buffer_(std::move(other.buffer_))
        , capacity_(other.capacity_)
        , head_(other.head_)
        , tail_(other.tail_)
        , size_(other.size_) {

    other.capacity_ = 0;
    other.head_ = 0;
    other.tail_ = 0;
    other.size_ = 0;
    other.buffer_.reset();
}

template<typename T>
CircularBuffer<T>& CircularBuffer<T>::operator=(const CircularBuffer& other) {
    if (this != &other) {
        auto temp = std::make_unique<T[]>(other.capacity_);

        for (size_type i = 0; i < other.capacity_; ++i) {
            temp[i] = other.buffer_[i];
        }

        buffer_ = std::move(temp);
        capacity_ = other.capacity_;
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
    }
    return *this;
}

template<typename T>
CircularBuffer<T>& CircularBuffer<T>::operator=(CircularBuffer&& other) noexcept {
    if (this != &other) {
        buffer_ = std::move(other.buffer_);
        capacity_ = other.capacity_;
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;

        other.capacity_ = 0;
        other.head_ = 0;
        other.tail_ = 0;
        other.size_ = 0;
        other.buffer_.reset();
    }
    return *this;
}

template<typename T>
CircularBuffer<T>::~CircularBuffer() = default;

template<typename T>
typename CircularBuffer<T>::size_type
CircularBuffer<T>::next_index(size_type index) const noexcept {
    return (index + 1) % capacity_;
}

template<typename T>
void CircularBuffer<T>::advance_head() noexcept {
    if (full()) {
        tail_ = next_index(tail_);
    } else {
        ++size_;
    }
    head_ = next_index(head_);
}

template<typename T>
void CircularBuffer<T>::advance_tail() noexcept {
    if (!empty()) {
        tail_ = next_index(tail_);
        --size_;
    }
}

template<typename T>
typename CircularBuffer<T>::reference CircularBuffer<T>::front() {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer_[tail_];
}

template<typename T>
typename CircularBuffer<T>::const_reference CircularBuffer<T>::front() const {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer_[tail_];
}

template<typename T>
typename CircularBuffer<T>::reference CircularBuffer<T>::back() {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer_[(head_ == 0 ? capacity_ : head_) - 1];
}

template<typename T>
typename CircularBuffer<T>::const_reference CircularBuffer<T>::back() const {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer_[(head_ == 0 ? capacity_ : head_) - 1];
}

template<typename T>
typename CircularBuffer<T>::reference CircularBuffer<T>::operator[](size_type index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return buffer_[(tail_ + index) % capacity_];
}

template<typename T>
typename CircularBuffer<T>::const_reference
CircularBuffer<T>::operator[](size_type index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return buffer_[(tail_ + index) % capacity_];
}

template<typename T>
bool CircularBuffer<T>::empty() const noexcept {
    return size_ == 0;
}

template<typename T>
bool CircularBuffer<T>::full() const noexcept {
    return size_ == capacity_;
}

template<typename T>
typename CircularBuffer<T>::size_type CircularBuffer<T>::size() const noexcept {
    return size_;
}

template<typename T>
typename CircularBuffer<T>::size_type CircularBuffer<T>::capacity() const noexcept {
    return capacity_;
}

template<typename T>
void CircularBuffer<T>::push(const_reference value) {
    buffer_[head_] = value;
    advance_head();
}

template<typename T>
void CircularBuffer<T>::push(T&& value) {
    buffer_[head_] = std::move(value);
    advance_head();
}

template<typename T>
template<typename... Args>
void CircularBuffer<T>::emplace(Args&&... args) {
    buffer_[head_] = T(std::forward<Args>(args)...);
    advance_head();
}

template<typename T>
void CircularBuffer<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    advance_tail();
}

template<typename T>
void CircularBuffer<T>::clear() noexcept {
    head_ = 0;
    tail_ = 0;
    size_ = 0;
}

template<typename T>
void CircularBuffer<T>::resize(size_type new_capacity) {
    if (new_capacity == 0) {
        throw std::invalid_argument("Capacity must be greater than 0");
    }

    if (new_capacity == capacity_) {
        return;
    }

    auto new_buffer = std::make_unique<T[]>(new_capacity);
    size_type elements_to_copy = std::min(size_, new_capacity);

    for (size_type i = 0; i < elements_to_copy; ++i) {
        new_buffer[i] = std::move((*this)[i]);
    }

    buffer_ = std::move(new_buffer);
    capacity_ = new_capacity;
    head_ = elements_to_copy;
    tail_ = 0;
    size_ = elements_to_copy;
}

// Файловые операции

template<typename T>
void CircularBuffer<T>::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file.write(reinterpret_cast<const char*>(&capacity_), sizeof(capacity_));
    file.write(reinterpret_cast<const char*>(&size_), sizeof(size_));
    file.write(reinterpret_cast<const char*>(&head_), sizeof(head_));
    file.write(reinterpret_cast<const char*>(&tail_), sizeof(tail_));

    for (size_type i = 0; i < size_; ++i) {
        const T& element = buffer_[(tail_ + i) % capacity_];
        file.write(reinterpret_cast<const char*>(&element), sizeof(T));
    }
}

template<typename T>
void CircularBuffer<T>::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    size_type new_capacity, new_size, new_head, new_tail;
    file.read(reinterpret_cast<char*>(&new_capacity), sizeof(new_capacity));
    file.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    file.read(reinterpret_cast<char*>(&new_head), sizeof(new_head));
    file.read(reinterpret_cast<char*>(&new_tail), sizeof(new_tail));

    if (new_capacity != capacity_) {
        buffer_ = std::make_unique<T[]>(new_capacity);
        capacity_ = new_capacity;
    }

    head_ = new_head;
    tail_ = new_tail;
    size_ = new_size;

    for (size_type i = 0; i < size_; ++i) {
        T element;
        file.read(reinterpret_cast<char*>(&element), sizeof(T));
        buffer_[(tail_ + i) % capacity_] = element;
    }

    if (!file) {
        throw std::runtime_error("Error reading from file: " + filename);
    }
}

template<typename T>
void CircularBuffer<T>::saveToTextFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file << capacity_ << " " << size_ << " " << head_ << " " << tail_ << "\n";
    for (size_type i = 0; i < size_; ++i) {
        file << buffer_[(tail_ + i) % capacity_] << " ";
    }
    file << "\n";
}

template<typename T>
void CircularBuffer<T>::loadFromTextFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    size_type new_capacity, new_size, new_head, new_tail;
    file >> new_capacity >> new_size >> new_head >> new_tail;

    if (new_capacity != capacity_) {
        buffer_ = std::make_unique<T[]>(new_capacity);
        capacity_ = new_capacity;
    }

    head_ = new_head;
    tail_ = new_tail;
    size_ = new_size;

    for (size_type i = 0; i < size_; ++i) {
        file >> buffer_[(tail_ + i) % capacity_];
    }

    if (!file) {
        throw std::runtime_error("Error reading from file: " + filename);
    }
}

// Реализация итераторов
template<typename T>
class CircularBuffer<T>::iterator {
public:
    using pointer = T*;
    using reference = T&;

    iterator(CircularBuffer* buffer, size_type pos)
            : buffer_(buffer), pos_(pos) {}

    reference operator*() const {
        return (*buffer_)[pos_];
    }

    pointer operator->() const {
        return &(*buffer_)[pos_];
    }

    iterator& operator++() {
        ++pos_;
        return *this;
    }

    iterator operator++(int) {
        iterator temp = *this;
        ++pos_;
        return temp;
    }

    bool operator==(const iterator& other) const {
        return buffer_ == other.buffer_ && pos_ == other.pos_;
    }

    bool operator!=(const iterator& other) const {
        return !(*this == other);
    }

private:
    CircularBuffer* buffer_;
    size_type pos_;
};

template<typename T>
class CircularBuffer<T>::const_iterator {
public:
    using pointer = const T*;
    using reference = const T&;

    const_iterator(const CircularBuffer* buffer, size_type pos)
            : buffer_(buffer), pos_(pos) {}

    reference operator*() const {
        return (*buffer_)[pos_];
    }

    pointer operator->() const {
        return &(*buffer_)[pos_];
    }

    const_iterator& operator++() {
        ++pos_;
        return *this;
    }

    const_iterator operator++(int) {
        const_iterator temp = *this;
        ++pos_;
        return temp;
    }

    bool operator==(const const_iterator& other) const {
        return buffer_ == other.buffer_ && pos_ == other.pos_;
    }

    bool operator!=(const const_iterator& other) const {
        return !(*this == other);
    }

private:
    const CircularBuffer* buffer_;
    size_type pos_;
};

template<typename T>
typename CircularBuffer<T>::iterator CircularBuffer<T>::begin() noexcept {
    return iterator(this, 0);
}

template<typename T>
typename CircularBuffer<T>::const_iterator CircularBuffer<T>::begin() const noexcept {
    return const_iterator(this, 0);
}

template<typename T>
typename CircularBuffer<T>::const_iterator CircularBuffer<T>::cbegin() const noexcept {
    return begin();
}

template<typename T>
typename CircularBuffer<T>::iterator CircularBuffer<T>::end() noexcept {
    return iterator(this, size_);
}

template<typename T>
typename CircularBuffer<T>::const_iterator CircularBuffer<T>::end() const noexcept {
    return const_iterator(this, size_);
}

template<typename T>
typename CircularBuffer<T>::const_iterator CircularBuffer<T>::cend() const noexcept {
    return end();
}

#endif