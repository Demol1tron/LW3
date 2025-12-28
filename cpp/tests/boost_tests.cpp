#define BOOST_TEST_MODULE Lab3_Tests
#include <boost/test/included/unit_test.hpp>

#include "d_array.h"
#include "s_list.h"
#include "d_list.h"
#include "stack.h"
#include "queue.h"
#include "hash_table.h"
#include "complete_bin_tree.h"

using namespace std;

BOOST_AUTO_TEST_CASE(DynamicArray_Ops) {
    DynamicArray arr;
    arr.pushBack("A");
    arr.pushBack("B");
    
    BOOST_CHECK_EQUAL(arr.getSize(), 2);
    BOOST_CHECK_EQUAL(arr.get(1), "B");
    
    arr.set(0, "Z");
    BOOST_CHECK_EQUAL(arr.get(0), "Z");

    BOOST_CHECK_EQUAL(arr.get(10), ""); // cerr
    
    // Сериализация
    arr.saveToText("boost_arr.txt");
    DynamicArray loaded;
    loaded.loadFromText("boost_arr.txt");
    BOOST_CHECK_EQUAL(loaded.getSize(), 2);
    BOOST_CHECK_EQUAL(loaded.get(0), "Z");
}

BOOST_AUTO_TEST_CASE(SinglyList_Ops) {
    SinglyList list;
    list.pushBack("1");
    list.pushBack("2");
    
    BOOST_CHECK_EQUAL(list.getSize(), 2);
    
    list.popFront();
    BOOST_CHECK_EQUAL(list.getHead(), "2");
    
    list.clear();
    BOOST_CHECK_EQUAL(list.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(DoublyList_Ops) {
    DoublyList list;
    list.pushFront("Head");
    list.pushBack("Tail");
    
    BOOST_CHECK_EQUAL(list.getTail(), "Tail");
    
    list.popBack();
    BOOST_CHECK_EQUAL(list.getTail(), "Head");
}

BOOST_AUTO_TEST_CASE(Stack_Ops) {
    Stack st;
    st.push("data");
    BOOST_CHECK_EQUAL(st.peek(), "data");
    
    string val = st.pop();
    BOOST_CHECK_EQUAL(val, "data");
    BOOST_CHECK_EQUAL(st.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(Queue_Ops) {
    Queue q;
    q.push("one");
    q.push("two");
    
    BOOST_CHECK_EQUAL(q.pop(), "one");
    BOOST_CHECK_EQUAL(q.peek(), "two");
    
    q.saveToBinary("boost_queue.bin");
    Queue loaded;
    loaded.loadFromBinary("boost_queue.bin");
    BOOST_CHECK_EQUAL(loaded.peek(), "two");
}

BOOST_AUTO_TEST_CASE(HashTable_Ops) {
    HashTable ht;
    ht.put("user", "admin");
    ht.put("pass", "1234");
    
    BOOST_CHECK_EQUAL(ht.get("user"), "admin");
    
    ht.remove("user");
    BOOST_CHECK_EQUAL(ht.get("user"), "");
}

BOOST_AUTO_TEST_CASE(Tree_Ops) {
    CompleteBinaryTree tree;
    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    
    BOOST_CHECK(tree.find(75));
    BOOST_CHECK(!tree.find(100));
    
    tree.saveToText("boost_tree.txt");
    CompleteBinaryTree loaded;
    loaded.loadFromText("boost_tree.txt");
    BOOST_CHECK(loaded.find(25));
}