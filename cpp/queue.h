#pragma once

#include <fstream>
#include <iostream>
#include <string>

class Queue {
    std::string* data;
    int front;
    int rear;
    int size;
    int capacity;

   public:
    Queue(int cap = 10) {
        capacity = cap;
        data = new std::string[capacity];
        front = 0;
        rear = -1;
        size = 0;
    }

    ~Queue() { delete[] data; }

    void resize() {
        int newCap = capacity * 2;
        std::string* newData = new std::string[newCap];
        for (int i = 0; i < size; i++) {
            newData[i] = data[(front + i) % capacity];
        }
        delete[] data;
        data = newData;
        capacity = newCap;
        front = 0;
        rear = size - 1;
    }

    void push(const std::string& val) {
        if (size == capacity) resize();
        rear = (rear + 1) % capacity;
        data[rear] = val;
        size++;
    }

    std::string pop() {
        if (size == 0) return "";
        std::string val = data[front];
        front = (front + 1) % capacity;
        size--;
        return val;
    }

    std::string peek() const {
        if (size == 0) return "";
        return data[front];
    }

    int getSize() const { return size; }

    // Сериализация (сохраняет в логическом порядке)
    void saveToText(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return;

        file << size << std::endl;
        for (int i = 0; i < size; i++) {
            file << data[(front + i) % capacity] << std::endl;
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
        front = 0;
        rear = -1;
        size = 0;

        for (int i = 0; i < newSize; i++) {
            std::string val;
            getline(file, val);
            push(val);
        }
        file.close();
    }

    // Сериализация
    void saveToBinary(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        file.write(reinterpret_cast<char*>(&size), sizeof(size));
        for (int i = 0; i < size; i++) {
            std::string val = data[(front + i) % capacity];
            size_t len = val.size();
            file.write(reinterpret_cast<char*>(&len), sizeof(len));
            file.write(val.c_str(), len);
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
        front = 0;
        rear = -1;
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