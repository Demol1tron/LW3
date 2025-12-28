#pragma once

#include <fstream>
#include <iostream>
#include <string>

struct DNode {
    std::string data;
    DNode* next;
    DNode* prev;
    DNode(std::string val) : data(val), next(nullptr), prev(nullptr) {}
};

class DoublyList {
    DNode* head;
    DNode* tail;
    int size;

   public:
    DoublyList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyList() { clear(); }

    void clear() {
        while (head) {
            DNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    // В голову
    void pushFront(const std::string& val) {
        DNode* newNode = new DNode(val);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    // В хвост
    void pushBack(const std::string& val) {
        DNode* newNode = new DNode(val);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    // После значения
    void insertAfter(const std::string& target, const std::string& val) {
        DNode* current = head;
        while (current) {
            if (current->data == target) {
                DNode* newNode = new DNode(val);
                newNode->next = current->next;
                newNode->prev = current;

                if (current->next) {
                    current->next->prev = newNode;
                } else {
                    tail = newNode;  // Был хвост
                }
                current->next = newNode;
                size++;
                return;
            }
            current = current->next;
        }
    }

    // До значения
    void insertBefore(const std::string& target, const std::string& val) {
        DNode* current = head;
        while (current) {
            if (current->data == target) {
                DNode* newNode = new DNode(val);
                newNode->prev = current->prev;
                newNode->next = current;

                if (current->prev) {
                    current->prev->next = newNode;
                } else {
                    head = newNode;  // Был головой
                }
                current->prev = newNode;
                size++;
                return;
            }
            current = current->next;
        }
    }

    // Удалить с головы
    void popFront() {
        if (!head) return;
        DNode* temp = head;
        head = head->next;
        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;
        delete temp;
        size--;
    }

    // Удалить с хвоста
    void popBack() {
        if (!tail) return;
        DNode* temp = tail;
        tail = tail->prev;
        if (tail)
            tail->next = nullptr;
        else
            head = nullptr;
        delete temp;
        size--;
    }

    // Удалить по значению
    void removeByValue(const std::string& val) {
        DNode* current = head;
        while (current) {
            if (current->data == val) {
                if (current->prev)
                    current->prev->next = current->next;
                else
                    head = current->next;  // Это была голова

                if (current->next)
                    current->next->prev = current->prev;
                else
                    tail = current->prev;  // Это был хвост

                delete current;
                size--;
                return;
            }
            current = current->next;
        }
    }

    // Поиск
    bool search(const std::string& val) const {
        DNode* current = head;
        while (current) {
            if (current->data == val) return true;
            current = current->next;
        }
        return false;
    }

    std::string getTail() const { return tail ? tail->data : ""; }
    int getSize() const { return size; }

    // Печать вперед
    void printForward() const {
        DNode* curr = head;
        while (curr) {
            std::cout << curr->data << " <-> ";
            curr = curr->next;
        }
        std::cout << "NULL" << std::endl;
    }

    // Печать назад
    void printBackward() const {
        DNode* curr = tail;
        while (curr) {
            std::cout << curr->data << " <-> ";
            curr = curr->prev;
        }
        std::cout << "NULL" << std::endl;
    }

    void saveToText(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return;

        DNode* curr = head;
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

        file.write(reinterpret_cast<char*>(&size), sizeof(size));
        DNode* curr = head;
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