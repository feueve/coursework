#include "circular_buffer.hpp"
#include <iostream>
#include <string>

void demonstrate_basic_operations() {
    std::cout << "=== Basic Operations Demo ===\n";

    CircularBuffer<int> buffer(5);
    std::cout << "Capacity: " << buffer.capacity() << std::endl;

    for (int i = 1; i <= 7; ++i) {
        buffer.push(i);
        std::cout << "Push " << i
                  << ", Size: " << buffer.size()
                  << ", Full: " << (buffer.full() ? "true" : "false")
                  << std::endl;
    }

    std::cout << "Buffer contents: ";
    while (!buffer.empty()) {
        std::cout << buffer.front() << " ";
        buffer.pop();
    }
    std::cout << std::endl;
}

void demonstrate_iterators() {
    std::cout << "\n=== Iterator Demo ===\n";

    CircularBuffer<std::string> str_buffer(3);
    str_buffer.push("Hello");
    str_buffer.push("World");
    str_buffer.push("!");

    std::cout << "String buffer: ";
    for (const auto& str : str_buffer) {
        std::cout << str << " ";
    }
    std::cout << std::endl;
}

void demonstrate_emplace() {
    std::cout << "\n=== Emplace Demo ===\n";

    CircularBuffer<std::pair<int, std::string>> pair_buffer(2);
    pair_buffer.emplace(1, "One");
    pair_buffer.emplace(2, "Two");

    std::cout << "Pair buffer: ";
    while (!pair_buffer.empty()) {
        auto& p = pair_buffer.front();
        std::cout << "{" << p.first << ", " << p.second << "} ";
        pair_buffer.pop();
    }
    std::cout << std::endl;
}

void demonstrate_random_access() {
    std::cout << "\n=== Random Access Demo ===\n";

    CircularBuffer<int> buffer(5);
    for (int i = 0; i < 5; ++i) {
        buffer.push(i * 10);
    }

    std::cout << "Using operator[]: ";
    for (size_t i = 0; i < buffer.size(); ++i) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
}

void demonstrate_file_operations() {
    std::cout << "\n=== File Operations Demo ===\n";

    CircularBuffer<int> buffer(4);
    buffer.push(100);
    buffer.push(200);
    buffer.push(300);
    buffer.push(400);

    std::cout << "Original buffer: ";
    for (int val : buffer) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Binary format
    buffer.saveToFile("buffer_data.bin");
    std::cout << "Saved to binary file: buffer_data.bin\n";

    CircularBuffer<int> loaded_binary(1);
    loaded_binary.loadFromFile("buffer_data.bin");

    std::cout << "Loaded from binary: ";
    for (int val : loaded_binary) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Text format
    buffer.saveToTextFile("buffer_data.txt");
    std::cout << "Saved to text file: buffer_data.txt\n";

    CircularBuffer<int> loaded_text(1);
    loaded_text.loadFromTextFile("buffer_data.txt");

    std::cout << "Loaded from text: ";
    for (int val : loaded_text) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Cleanup
    std::remove("buffer_data.bin");
    std::remove("buffer_data.txt");
    std::cout << "Temporary files removed\n";
}

void demonstrate_copy_move() {
    std::cout << "\n=== Copy/Move Operations Demo ===\n";

    CircularBuffer<int> original(3);
    original.push(1);
    original.push(2);
    original.push(3);

    // Copy constructor
    CircularBuffer<int> copied = original;
    std::cout << "Copied buffer size: " << copied.size() << std::endl;

    // Move constructor
    CircularBuffer<int> moved = std::move(original);
    std::cout << "Moved buffer size: " << moved.size() << std::endl;
    std::cout << "Original buffer size after move: " << original.size() << std::endl;
}

void demonstrate_resize() {
    std::cout << "\n=== Resize Demo ===\n";

    CircularBuffer<int> buffer(3);
    buffer.push(10);
    buffer.push(20);

    std::cout << "Before resize - Capacity: " << buffer.capacity()
              << ", Size: " << buffer.size() << std::endl;

    buffer.resize(5);
    std::cout << "After resize to 5 - Capacity: " << buffer.capacity()
              << ", Size: " << buffer.size() << std::endl;

    buffer.resize(2);
    std::cout << "After resize to 2 - Capacity: " << buffer.capacity()
              << ", Size: " << buffer.size() << std::endl;
}

int main() {
    try {
        demonstrate_basic_operations();
        demonstrate_iterators();
        demonstrate_emplace();
        demonstrate_random_access();
        demonstrate_copy_move();
        demonstrate_resize();
        demonstrate_file_operations();

        std::cout << "\n=== All demonstrations completed successfully! ===\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}