#include <catch2/catch_test_macros.hpp>

#include "d_array.h"
#include "s_list.h"
#include "d_list.h"
#include "stack.h"
#include "queue.h"
#include "hash_table.h"
#include "complete_bin_tree.h"

using namespace std;

TEST_CASE("DynamicArray operations", "[d_array]") {
    DynamicArray arr;
    
    SECTION("PushBack and Get") {
        arr.pushBack("A");
        arr.pushBack("B");
        REQUIRE(arr.getSize() == 2);
        REQUIRE(arr.get(0) == "A");
        REQUIRE(arr.get(1) == "B");
    }

    SECTION("Insert and Remove") {
        arr.pushBack("A");
        arr.pushBack("C");
        arr.insertAt(1, "B"); // A, B, C
        REQUIRE(arr.get(1) == "B");
        
        arr.removeAt(1); // A, C
        REQUIRE(arr.get(1) == "C");
        REQUIRE(arr.getSize() == 2);
    }
    
    SECTION("Out of bounds") {
        REQUIRE(arr.get(10) == "");
    }
}

TEST_CASE("SinglyList operations", "[s_list]") {
    SinglyList list;
    
    SECTION("PushFront and PopFront") {
        list.pushFront("A");
        list.pushFront("B"); // B -> A
        REQUIRE(list.getHead() == "B");
        
        list.popFront();
        REQUIRE(list.getHead() == "A");
    }
    
    SECTION("Search") {
        list.pushBack("FindMe");
        REQUIRE(list.search("FindMe") == true);
        REQUIRE(list.search("Missing") == false);
    }
}

TEST_CASE("DoublyList operations", "[d_list]") {
    DoublyList list;
    
    SECTION("PushBack and Tail check") {
        list.pushBack("A");
        list.pushBack("B"); // A <-> B
        REQUIRE(list.getTail() == "B");
        REQUIRE(list.getSize() == 2);
    }
    
    SECTION("PushFront and PopBack") {
        list.pushFront("Start");
        list.popBack();
        REQUIRE(list.getSize() == 0);
    }
}

TEST_CASE("Stack operations", "[stack]") {
    Stack s;
    
    SECTION("LIFO behavior") {
        s.push("1");
        s.push("2");
        REQUIRE(s.peek() == "2");
        REQUIRE(s.pop() == "2");
        REQUIRE(s.pop() == "1");
        REQUIRE(s.getSize() == 0);
    }
    
    SECTION("Empty pop") {
        REQUIRE(s.pop() == "");
    }
}

TEST_CASE("Queue operations", "[queue]") {
    Queue q;
    
    SECTION("FIFO behavior") {
        q.push("1");
        q.push("2");
        REQUIRE(q.peek() == "1");
        REQUIRE(q.pop() == "1");
        REQUIRE(q.pop() == "2");
    }
    
    SECTION("Circular buffer resize") {
        // Создаем маленькую очередь и переполняем её
        Queue smallQ(2);
        smallQ.push("A");
        smallQ.push("B");
        smallQ.push("C"); // Тут должен быть resize
        REQUIRE(smallQ.getSize() == 3);
        REQUIRE(smallQ.pop() == "A");
    }
}

TEST_CASE("HashTable operations", "[hash_table]") {
    HashTable ht;
    
    SECTION("Put and Get") {
        ht.put("user", "admin");
        ht.put("id", "123");
        
        REQUIRE(ht.get("user") == "admin");
        REQUIRE(ht.get("id") == "123");
        REQUIRE(ht.get("missing") == "");
    }
    
    SECTION("Remove") {
        ht.put("temp", "data");
        ht.remove("temp");
        REQUIRE(ht.get("temp") == "");
    }
    
    SECTION("Update value") {
        ht.put("key", "val1");
        ht.put("key", "val2"); // Обновление
        REQUIRE(ht.get("key") == "val2");
    }
}

TEST_CASE("CompleteBinaryTree operations", "[tree]") {
    CompleteBinaryTree tree;
    
    SECTION("Insert and Find") {
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        
        REQUIRE(tree.find(20) == true);
        REQUIRE(tree.find(99) == false);
    }
    
    SECTION("Completeness check") {
        REQUIRE(tree.isComplete() == true); // Пустое дерево полное
        tree.insert(1);
        REQUIRE(tree.isComplete() == true);
    }
    
    SECTION("Persistence (Save/Load)") {
        tree.insert(100);
        tree.saveToText("catch_tree.txt");
        
        CompleteBinaryTree loaded;
        loaded.loadFromText("catch_tree.txt");
        REQUIRE(loaded.find(100) == true);
    }
}