#pragma once

#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

struct HashNode {
    std::string key;
    std::string value;
};

class HashTable {
    std::vector<std::list<HashNode>> table;
    int capacity;
    int size;

    size_t hashFunction(const std::string& key) const {
        size_t hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % capacity;
        }
        return hash;
    }

   public:
    HashTable(int cap = 10) : capacity(cap), size(0) { table.resize(capacity); }

    void put(const std::string& key, const std::string& value) {
        size_t index = hashFunction(key);
        for (auto& node : table[index]) {
            if (node.key == key) {
                node.value = value;
                return;
            }
        }
        table[index].push_back({key, value});
        size++;
    }

    std::string get(const std::string& key) const {
        size_t index = hashFunction(key);
        for (const auto& node : table[index]) {
            if (node.key == key) {
                return node.value;
            }
        }
        return "";
    }

    void remove(const std::string& key) {
        size_t index = hashFunction(key);
        auto& chain = table[index];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->key == key) {
                chain.erase(it);
                size--;
                return;
            }
        }
    }

    int getSize() const { return size; }

    // Текстовая сериализация
    void saveToText(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return;

        file << size << std::endl;
        for (const auto& chain : table) {
            for (const auto& node : chain) {
                file << node.key << " " << node.value << std::endl;
            }
        }
        file.close();
    }

    void loadFromText(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        int newSize;
        file >> newSize;

        // Очищаем таблицу перед загрузкой
        for (auto& chain : table) chain.clear();
        size = 0;

        std::string k, v;
        for (int i = 0; i < newSize; i++) {
            file >> k >> v;
            put(k, v);
        }
        file.close();
    }

    // Бинарная сериализация
    void saveToBinary(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        // Записываем общее количество элементов
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // Проходим по всем цепочкам и сохраняем пары
        for (const auto& chain : table) {
            for (const auto& node : chain) {
                // Сохраняем ключ: размер + данные
                size_t keyLen = node.key.size();
                file.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
                file.write(node.key.c_str(), keyLen);

                // Сохраняем значение: размер + данные
                size_t valLen = node.value.size();
                file.write(reinterpret_cast<const char*>(&valLen), sizeof(valLen));
                file.write(node.value.c_str(), valLen);
            }
        }
        file.close();
    }

    void loadFromBinary(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));

        // Очищаем текущую таблицу
        for (auto& chain : table) chain.clear();
        size = 0;

        for (int i = 0; i < count; ++i) {
            // Читаем ключ
            size_t keyLen;
            file.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
            char* keyBuf = new char[keyLen + 1];
            file.read(keyBuf, keyLen);
            keyBuf[keyLen] = '\0';
            std::string key(keyBuf);
            delete[] keyBuf;

            // Читаем значение
            size_t valLen;
            file.read(reinterpret_cast<char*>(&valLen), sizeof(valLen));
            char* valBuf = new char[valLen + 1];
            file.read(valBuf, valLen);
            valBuf[valLen] = '\0';
            std::string val(valBuf);
            delete[] valBuf;

            // put сам вычислит хеш и найдет нужную корзину
            put(key, val);
        }
        file.close();
    }
};