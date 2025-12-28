#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "d_array.h"
#include "s_list.h"
#include "d_list.h"
#include "stack.h"
#include "queue.h"
#include "hash_table.h"
#include "complete_bin_tree.h"

using namespace std;

TEST(DynamicArrayTest, UltimateCoverage) {
    DynamicArray arr(1); // Емкость 1
    arr.pushBack("A");
    arr.pushBack("B"); // Вызов resize() (capacity * 2)
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr.get(0), "A");

    arr.pushFront("Start"); // Start, A, B
    EXPECT_EQ(arr.get(0), "Start");

    arr.insertAt(1, "Mid"); // Start, Mid, A, B
    EXPECT_EQ(arr.get(1), "Mid");

    testing::internal::CaptureStderr();
    arr.insertAt(-1, "X");
    arr.insertAt(100, "X");
    string err = testing::internal::GetCapturedStderr();
    EXPECT_FALSE(err.empty()); // Убедились, что ошибка была напечатана

    arr.set(0, "NewStart");
    EXPECT_EQ(arr.get(0), "NewStart");
    
    // Ошибки set/get
    testing::internal::CaptureStderr();
    arr.set(-1, "X");
    arr.set(100, "X");
    EXPECT_EQ(arr.get(-1), "");   // Возвращает "" при ошибке
    EXPECT_EQ(arr.get(100), "");
    testing::internal::GetCapturedStderr(); // Очистка буфера ошибок

    EXPECT_EQ(arr.find("Mid"), 1);
    EXPECT_EQ(arr.find("Missing"), -1);

    arr.removeAt(1); // Удаляем Mid -> NewStart, A, B
    EXPECT_EQ(arr.get(1), "A");
    
    // Ошибки removeAt
    testing::internal::CaptureStderr();
    arr.removeAt(-1);
    arr.removeAt(100);
    testing::internal::GetCapturedStderr();

    arr.popFront(); // Удаляем NewStart -> A, B
    EXPECT_EQ(arr.get(0), "A");
    arr.popBack();  // Удаляем B -> A
    EXPECT_EQ(arr.getSize(), 1);
    
    arr.popBack(); // 0 элементов
    arr.popBack();
    arr.popFront();
    EXPECT_EQ(arr.getSize(), 0);

    arr.pushBack("P");
    
    testing::internal::CaptureStdout();
    arr.print();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("P"), string::npos);

    // Сериализация
    arr.saveToText("da_cov.txt");
    DynamicArray l1;
    l1.loadFromText("da_cov.txt");
    EXPECT_EQ(l1.get(0), "P");
    
    // Ошибка открытия файла (должна писать в cerr)
    testing::internal::CaptureStderr();
    l1.loadFromText("/invalid/path/missing.txt"); 
    testing::internal::GetCapturedStderr();

    arr.saveToBinary("da_cov.bin");
    DynamicArray l2;
    l2.loadFromBinary("da_cov.bin");
    EXPECT_EQ(l2.get(0), "P");
    
    testing::internal::CaptureStderr();
    l2.loadFromBinary("/invalid/path/missing.bin");
    testing::internal::GetCapturedStderr();
}

TEST(SinglyListTest, UltimateCoverage) {
    SinglyList list;

    list.popFront();
    list.popBack();
    list.removeByValue("X");
    EXPECT_EQ(list.getHead(), "");

    list.pushBack("A");
    list.popBack(); // Head и Tail должны стать nullptr
    EXPECT_EQ(list.getSize(), 0);

    list.pushFront("B");
    list.popFront(); // Head и Tail должны стать nullptr
    EXPECT_EQ(list.getSize(), 0);

    list.pushBack("A");
    list.insertAfter("A", "B"); // Добавление в хвост (обновление tail)
    EXPECT_EQ(list.getSize(), 2);
    list.insertAfter("A", "A.5"); // Добавление в середину
    // A -> A.5 -> B

    list.insertBefore("A", "Start"); // Добавление перед головой
    EXPECT_EQ(list.getHead(), "Start");
    // Start -> A -> A.5 -> B
    list.insertBefore("B", "B_Prev"); // Добавление в середине
    // ... A.5 -> B_Prev -> B
    list.insertBefore("Missing", "X"); // Не должно ничего сломать

    list.removeByValue("Start"); // Удаление головы
    EXPECT_EQ(list.getHead(), "A");
    
    list.removeByValue("B"); // Удаление хвоста (обновление tail)
    // Сейчас: A -> A.5 -> B_Prev
    
    list.removeByValue("A.5"); // Удаление из середины
    // Сейчас: A -> B_Prev
    EXPECT_EQ(list.getSize(), 2);

    list.removeByValue("Z"); // Несуществующий

    EXPECT_TRUE(list.search("A"));
    EXPECT_FALSE(list.search("Z"));

    testing::internal::CaptureStdout();
    list.print();
    string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("A"), string::npos);

    // Сериализация
    list.saveToText("sl_cov.txt");
    SinglyList l1; l1.loadFromText("sl_cov.txt");
    EXPECT_EQ(l1.getSize(), 2);
    l1.loadFromText("no.txt");

    list.saveToBinary("sl_cov.bin");
    SinglyList l2; l2.loadFromBinary("sl_cov.bin");
    EXPECT_EQ(l2.getSize(), 2);
    l2.loadFromBinary("no.bin");
}

TEST(DoublyListTest, UltimateCoverage) {
    DoublyList list;

    list.popFront();
    list.popBack();
    EXPECT_EQ(list.getTail(), "");

    list.pushBack("A");
    list.popFront(); // Очистка
    EXPECT_EQ(list.getSize(), 0);

    list.pushFront("B");
    list.popBack(); // Очистка
    EXPECT_EQ(list.getSize(), 0);

    list.pushBack("A");
    list.pushFront("Start"); // Start <-> A
    list.insertAfter("A", "End"); // Start <-> A <-> End (Tail update)
    EXPECT_EQ(list.getTail(), "End");
    list.insertAfter("Start", "Mid"); // Start <-> Mid <-> A <-> End

    list.insertBefore("Start", "NewStart"); // NewStart <-> Start ... (Head update)
    EXPECT_EQ(list.getSize(), 5);
    list.insertBefore("End", "PreEnd"); // ... A <-> PreEnd <-> End

    list.removeByValue("NewStart"); // Удаление головы
    list.removeByValue("End"); // Удаление хвоста
    list.removeByValue("Mid"); // Удаление середины
    list.removeByValue("Missing");

    testing::internal::CaptureStdout();
    list.printBackward();
    string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("Start"), string::npos);

    testing::internal::CaptureStdout();
    list.printForward();
    testing::internal::GetCapturedStdout();

    list.saveToText("dl_cov.txt");
    DoublyList l1; l1.loadFromText("dl_cov.txt");
    EXPECT_EQ(l1.getSize(), list.getSize());
    l1.loadFromText("no.txt");

    list.saveToBinary("dl_cov.bin");
    DoublyList l2; l2.loadFromBinary("dl_cov.bin");
    EXPECT_EQ(l2.getSize(), list.getSize());
    l2.loadFromBinary("no.bin");
}

TEST(StackTest, UltimateCoverage) {
    Stack st(1); // Малая емкость
    EXPECT_EQ(st.pop(), "");
    EXPECT_EQ(st.peek(), "");

    st.push("1");
    st.push("2"); // Resize
    EXPECT_EQ(st.getSize(), 2);
    EXPECT_EQ(st.pop(), "2");
    EXPECT_EQ(st.peek(), "1");

    st.saveToText("st.txt");
    Stack l1; l1.loadFromText("st.txt");
    EXPECT_EQ(l1.getSize(), 1);
    l1.loadFromText("no.txt");

    st.saveToBinary("st.bin");
    Stack l2; l2.loadFromBinary("st.bin");
    EXPECT_EQ(l2.getSize(), 1);
    l2.loadFromBinary("no.bin");
}

TEST(QueueTest, UltimateCoverage) {
    Queue q(3);
    EXPECT_EQ(q.pop(), "");
    EXPECT_EQ(q.peek(), "");

    q.push("1");
    q.push("2");
    q.push("3"); // Full
    q.push("4"); // Resize
    EXPECT_EQ(q.getSize(), 4);

    Queue qRing(3);
    qRing.push("A");
    qRing.push("B");
    qRing.pop(); // Front сдвигается
    qRing.push("C"); 
    qRing.push("D"); // Rear переходит через границу массива
    EXPECT_EQ(qRing.getSize(), 3);
    EXPECT_EQ(qRing.pop(), "B");
    EXPECT_EQ(qRing.pop(), "C");
    EXPECT_EQ(qRing.pop(), "D");

    qRing.push("X");
    qRing.saveToText("q.txt");
    Queue l1; l1.loadFromText("q.txt");
    EXPECT_EQ(l1.peek(), "X");
    l1.loadFromText("no.txt");

    qRing.saveToBinary("q.bin");
    Queue l2; l2.loadFromBinary("q.bin");
    EXPECT_EQ(l2.peek(), "X");
    l2.loadFromBinary("no.bin");
}

TEST(HashTableTest, UltimateCoverage) {
    HashTable ht(2); // Очень мало места -> будут коллизии
    
    ht.put("Key1", "Val1");
    ht.put("Key2", "Val2"); // Вероятная коллизия
    ht.put("Key3", "Val3"); // Точная коллизия
    
    EXPECT_EQ(ht.get("Key1"), "Val1");
    EXPECT_EQ(ht.get("Key3"), "Val3");
    EXPECT_EQ(ht.get("Missing"), "");

    ht.put("Key1", "NewVal");
    EXPECT_EQ(ht.get("Key1"), "NewVal");

    ht.remove("Key1");
    EXPECT_EQ(ht.get("Key1"), "");
    ht.remove("Missing"); // Безопасное удаление

    ht.saveToText("ht.txt");
    HashTable l1(5);
    l1.loadFromText("ht.txt");
    EXPECT_EQ(l1.get("Key3"), "Val3");
    l1.loadFromText("no.txt");
    
    ht.saveToBinary("ht.bin");
    HashTable l2(5);
    l2.loadFromBinary("ht.bin");
    EXPECT_EQ(l2.get("Key3"), "Val3");
    l2.loadFromBinary("no.bin");
}

TEST(TreeTest, UltimateCoverage) {
    CompleteBinaryTree tree;
    EXPECT_FALSE(tree.find(1));
    EXPECT_TRUE(tree.isComplete());

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    
    EXPECT_TRUE(tree.find(40));
    EXPECT_FALSE(tree.find(99));
    
    EXPECT_TRUE(tree.isComplete());

    // Сериализация
    tree.saveToText("tree.txt");
    
    CompleteBinaryTree loaded;
    loaded.loadFromText("tree.txt");
    EXPECT_TRUE(loaded.find(50));
    
    loaded.loadFromText("no.txt");
    
    // Binary
    tree.saveToBinary("tree.bin");
    CompleteBinaryTree binLoaded;
    binLoaded.loadFromBinary("tree.bin");
    EXPECT_TRUE(binLoaded.find(50));
    binLoaded.loadFromBinary("no.bin");

    // Сохранение пустого дерева
    CompleteBinaryTree emptyT;
    emptyT.saveToText("empty_tree.txt");
    emptyT.saveToBinary("empty_tree.bin");
}