#pragma once

#include <fstream>
#include <iostream>
#include <queue>
#include <string>

struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

class CompleteBinaryTree {
    TreeNode* root;
    int size;

    void deleteTree(TreeNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

   public:
    CompleteBinaryTree() : root(nullptr), size(0) {}

    ~CompleteBinaryTree() { deleteTree(root); }

    void insert(int value) {
        TreeNode* newNode = new TreeNode(value);
        if (!root) {
            root = newNode;
            size++;
            return;
        }

        std::queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();

            if (!curr->left) {
                curr->left = newNode;
                size++;
                return;
            } else {
                q.push(curr->left);
            }

            if (!curr->right) {
                curr->right = newNode;
                size++;
                return;
            } else {
                q.push(curr->right);
            }
        }
    }

    bool find(int value) const {
        if (!root) return false;
        std::queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();
            if (curr->data == value) return true;
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        return false;
    }

    bool isComplete() const {
        if (!root) return true;
        std::queue<TreeNode*> q;
        q.push(root);
        bool flag = false;
        while (!q.empty()) {
            TreeNode* temp = q.front();
            q.pop();
            if (temp->left) {
                if (flag) return false;
                q.push(temp->left);
            } else {
                flag = true;
            }
            if (temp->right) {
                if (flag) return false;
                q.push(temp->right);
            } else {
                flag = true;
            }
        }
        return true;
    }

    // Сохранение (обход в ширину)
    void saveToText(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return;

        if (!root) {
            file.close();
            return;
        }

        std::queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* temp = q.front();
            q.pop();
            file << temp->data << std::endl;
            if (temp->left) q.push(temp->left);
            if (temp->right) q.push(temp->right);
        }
        file.close();
    }

    // Загрузка из текста
    void loadFromText(const std::string& filename) {
        deleteTree(root);
        root = nullptr;
        size = 0;

        std::ifstream file(filename);
        if (!file.is_open()) return;

        int val;
        while (file >> val) {
            insert(val);
        }
        file.close();
    }

    // Бинарная сериализция
    void saveToBinary(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        // Сначала записываем количество элементов, чтобы при чтении знать сколько считывать
        file.write(reinterpret_cast<char*>(&size), sizeof(size));

        if (!root) {
            file.close();
            return;
        }

        std::queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* temp = q.front();
            q.pop();

            file.write(reinterpret_cast<char*>(&(temp->data)), sizeof(int));

            if (temp->left) q.push(temp->left);
            if (temp->right) q.push(temp->right);
        }
        file.close();
    }

    void loadFromBinary(const std::string& filename) {
        deleteTree(root);
        root = nullptr;
        size = 0;  // Сбрасываем размер, он увеличится внутри insert

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        int count;
        // Читаем сколько всего было узлов
        file.read(reinterpret_cast<char*>(&count), sizeof(count));

        for (int i = 0; i < count; ++i) {
            int val;
            file.read(reinterpret_cast<char*>(&val), sizeof(val));
            insert(val);  // insert корректно распределит узлы по дереву
        }
        file.close();
    }
};
