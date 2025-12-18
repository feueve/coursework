#include "circular_buffer.hpp"
#include <iostream>
#include <string>
#include <cstdio>

int main() {
    try {
        std::cout << "\n";

        // Базовые операции
        std::cout << "1. Basic operations:\n";
        CircularBuffer<int> buffer(5);
        std::cout << "   Capacity: " << buffer.capacity() << std::endl;

        for (int i = 1; i <= 7; ++i) {
            buffer.push(i);
            std::cout << "   Push " << i << ", Size: " << buffer.size()
                      << ", Full: " << (buffer.full() ? "true" : "false") << std::endl;
        }

        std::cout << "\n   Buffer contents: ";
        while (!buffer.empty()) {
            std::cout << buffer.front() << " ";
            buffer.pop();
        }
        std::cout << std::endl;

        // Итераторы
        std::cout << "\n2. Advanced features:\n";
        CircularBuffer<std::string> str_buffer(3);
        str_buffer.push("Hello");
        str_buffer.push("World");
        str_buffer.push("!");

        std::cout << "   String buffer (using iterators): ";
        for (const auto& str : str_buffer) {
            std::cout << str << " ";
        }
        std::cout << std::endl;

        // Файловые операции
        std::cout << "\n3. File operations (requirement):\n";
        CircularBuffer<int> file_buffer(4);
        file_buffer.push(100);
        file_buffer.push(200);
        file_buffer.push(300);
        file_buffer.push(400);

        std::cout << "   Original buffer: ";
        for (int val : file_buffer) {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        file_buffer.saveToFile("buffer_data.bin");
        std::cout << "   Saved to binary file: buffer_data.bin\n";

        CircularBuffer<int> loaded_buffer(1);
        loaded_buffer.loadFromFile("buffer_data.bin");

        std::cout << "   Loaded from binary: ";
        for (int val : loaded_buffer) {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        file_buffer.saveToTextFile("buffer_data.txt");
        std::cout << "   Saved to text file: buffer_data.txt\n";

        // Очистка временных файлов
        std::remove("buffer_data.bin");
        std::remove("buffer_data.txt");
        std::cout << "   Temporary files removed\n";

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}