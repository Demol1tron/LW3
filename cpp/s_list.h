#pragma once

#include <fstream>
#include <iostream>
#include <string>

struct Node {
    std::string data;
    Node* next;
    Node(std::string val) : data(val), next(nullptr) {}
};

class SinglyList {
    Node* head;
    Node* tail;
    int size;

   public:
    SinglyList() : head(nullptr), tail(nullptr), size(0) {}

    ~SinglyList() { clear(); }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    // Добавление в голову
    void pushFront(const std::string& val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        if (!tail) tail = head;
        size++;
    }

    // Добавление в хвост
    void pushBack(const std::string& val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    // ПОСЛЕ
    void insertAfter(const std::string& target, const std::string& val) {
        Node* current = head;
        while (current) {
            if (current->data == target) {
                Node* newNode = new Node(val);
                newNode->next = current->next;
                current->next = newNode;
                if (current == tail) tail = newNode;  // Обновляем хвост
                size++;
                return;
            }
            current = current->next;
        }
    }

    // ДО
    void insertBefore(const std::string& target, const std::string& val) {
        if (!head) return;

        if (head->data == target) {
            pushFront(val);
            return;
        }

        Node* current = head;
        while (current->next) {
            if (current->next->data == target) {
                Node* newNode = new Node(val);
                newNode->next = current->next;
                current->next = newNode;
                size++;
                return;
            }
            current = current->next;
        }
    }

    // Удаление с головы
    void popFront() {
        if (!head) return;
        Node* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        size--;
    }

    // Удаление с хвоста
    void popBack() {
        if (!head) return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size--;
    }

    // Удаление по значению
    void removeByValue(const std::string& val) {
        if (!head) return;

        if (head->data == val) {
            popFront();
            return;
        }

        Node* current = head;
        while (current->next) {
            if (current->next->data == val) {
                Node* temp = current->next;
                current->next = temp->next;
                if (temp == tail) tail = current;  // Обновляем хвост
                delete temp;
                size--;
                return;
            }
            current = current->next;
        }
    }

    // Поиск (Read)
    bool search(const std::string& val) const {
        Node* current = head;
        while (current) {
            if (current->data == val) return true;
            current = current->next;
        }
        return false;
    }

    std::string getHead() const { return head ? head->data : ""; }
    int getSize() const { return size; }

    void print() const {
        Node* curr = head;
        while (curr) {
            std::cout << curr->data << " -> ";
            curr = curr->next;
        }
        std::cout << "NULL" << std::endl;
    }

    void saveToText(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return;

        Node* curr = head;
        while (curr) {
            file << curr->data << std::endl;
            curr = curr->next;
        }
        file.close();
    }

    void loadFromText(const std::string& filename) {
        clear();
        std::ifstream file(filename);
        if (!file.is_open()) return;
        std::string line;
        while (getline(file, line)) {
            pushBack(line);
        }
        file.close();
    }

    void saveToBinary(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        int s = size;
        file.write(reinterpret_cast<char*>(&s), sizeof(s));
        Node* curr = head;
        while (curr) {
            size_t len = curr->data.size();
            file.write(reinterpret_cast<char*>(&len), sizeof(len));
            file.write(curr->data.c_str(), len);
            curr = curr->next;
        }
        file.close();
    }

    void loadFromBinary(const std::string& filename) {
        clear();
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        int s;
        file.read(reinterpret_cast<char*>(&s), sizeof(s));
        for (int i = 0; i < s; ++i) {
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