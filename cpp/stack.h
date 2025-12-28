#pragma once

#include <fstream>
#include <iostream>
#include <string>

class Stack {
    std::string* data;
    int size;
    int capacity;

    void resize() {
        int newCapacity = (capacity == 0) ? 1 : capacity * 2;
        std::string* newData = new std::string[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

   public:
    Stack(int initialCapacity = 10) {
        capacity = initialCapacity;
        size = 0;
        data = new std::string[capacity];
    }

    ~Stack() { delete[] data; }

    void push(const std::string& value) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = value;
    }

    std::string pop() {
        if (size == 0) return "";
        std::string val = data[size - 1];
        size--;
        return val;
    }

    std::string peek() const {
        if (size == 0) return "";
        return data[size - 1];
    }

    int getSize() const { return size; }

    void saveToText(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return;

        file << size << std::endl;
        for (int i = 0; i < size; i++) {
            file << data[i] << std::endl;
        }
        file.close();
    }

    void loadFromText(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        int newSize;
        file >> newSize;
        std::string dummy;
        getline(file, dummy);

        delete[] data;
        capacity = (newSize > 0) ? newSize * 2 : 10;
        data = new std::string[capacity];
        size = 0;

        for (int i = 0; i < newSize; i++) {
            std::string val;
            getline(file, val);
            push(val);
        }
        file.close();
    }

    void saveToBinary(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        file.write(reinterpret_cast<char*>(&size), sizeof(size));
        for (int i = 0; i < size; ++i) {
            size_t len = data[i].size();
            file.write(reinterpret_cast<char*>(&len), sizeof(len));
            file.write(data[i].c_str(), len);
        }
        file.close();
    }

    void loadFromBinary(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        int newSize;
        file.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));

        delete[] data;
        capacity = (newSize > 0) ? newSize * 2 : 10;
        data = new std::string[capacity];
        size = 0;

        for (int i = 0; i < newSize; ++i) {
            size_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            char* buf = new char[len + 1];
            file.read(buf, len);
            buf[len] = '\0';
            push(std::string(buf));
            delete[] buf;
        }
        file.close();
    }
};