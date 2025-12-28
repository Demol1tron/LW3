#pragma once

#include <fstream>
#include <iostream>
#include <string>

class DynamicArray {
    std::string* data;
    int size;
    int capacity;

    void resizeCapacity() {
        if (size >= capacity) {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            std::string* newData = new std::string[newCapacity];
            for (int i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
    }

   public:
    DynamicArray(int initialCapacity = 10) {
        capacity = initialCapacity;
        size = 0;
        data = new std::string[capacity];
    }

    ~DynamicArray() { delete[] data; }

    // Добавление в конец
    void pushBack(const std::string& value) {
        resizeCapacity();
        data[size++] = value;
    }

    // Добавление в начало
    void pushFront(const std::string& value) {
        resizeCapacity();
        for (int i = size; i > 0; i--) {
            data[i] = data[i - 1];
        }
        data[0] = value;
        size++;
    }

    // Добавление по индексу
    void insertAt(int index, const std::string& value) {
        if (index < 0 || index > size) {
            std::cerr << "Error: Index out of range in insertAt" << std::endl;
            return;
        }
        resizeCapacity();
        for (int i = size; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        size++;
    }

    // Удаление с конца
    void popBack() {
        if (size > 0) size--;
    }

    // Удаление с начала
    void popFront() {
        if (size == 0) return;
        for (int i = 0; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    // Удаление по индексу
    void removeAt(int index) {
        if (index < 0 || index >= size) {
            std::cerr << "Error: Index out of range in removeAt" << std::endl;
            return;
        }
        for (int i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    // Поиск индекса по значению
    int find(const std::string& value) const {
        for (int i = 0; i < size; ++i) {
            if (data[i] == value) return i;
        }
        return -1;
    }

    // Получение элемента
    std::string get(int index) const {
        if (index < 0 || index >= size) {
            std::cerr << "Error: Index out of range in get" << std::endl;
            return "";  // Возвращаем пустую строку при ошибке
        }
        return data[index];
    }

    // Замена элемента
    void set(int index, const std::string& value) {
        if (index < 0 || index >= size) {
            std::cerr << "Error: Index out of range in set" << std::endl;
            return;
        }
        data[index] = value;
    }

    int getSize() const { return size; }

    void print() const {
        std::cout << "[ ";
        for (int i = 0; i < size; i++) {
            std::cout << data[i] << (i < size - 1 ? ", " : "");
        }
        std::cout << " ]" << std::endl;
    }

    // Сериализация (текст)
    void saveToText(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing" << std::endl;
            return;
        }
        file << size << std::endl;
        for (int i = 0; i < size; i++) {
            file << data[i] << std::endl;
        }
        file.close();
    }

    // Десериализация
    void loadFromText(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading" << std::endl;
            return;
        }
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
            pushBack(val);
        }
        file.close();
    }

    // Сериализация (бинарная)
    void saveToBinary(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open binary file for writing" << std::endl;
            return;
        }
        file.write(reinterpret_cast<char*>(&size), sizeof(size));
        for (int i = 0; i < size; ++i) {
            size_t len = data[i].size();
            file.write(reinterpret_cast<char*>(&len), sizeof(len));
            file.write(data[i].c_str(), len);
        }
        file.close();
    }

    // Десериализация
    void loadFromBinary(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open binary file for reading" << std::endl;
            return;
        }
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
            pushBack(std::string(buf));
            delete[] buf;
        }
        file.close();
    }
};