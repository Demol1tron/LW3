#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include "d_array.h"
#include "s_list.h"
#include "d_list.h"
#include "stack.h"
#include "queue.h"
#include "hash_table.h"
#include "complete_bin_tree.h"

using namespace std;

static void BM_DynamicArray_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        DynamicArray arr;
        for (int i = 0; i < state.range(0); ++i) {
            arr.pushBack("data");
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_DynamicArray_PushBack)->Range(8, 8<<10)->Complexity();

static void BM_SinglyList_PushFront(benchmark::State& state) {
    for (auto _ : state) {
        SinglyList list;
        for (int i = 0; i < state.range(0); ++i) {
            list.pushFront("data");
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_SinglyList_PushFront)->Range(8, 8<<10)->Complexity();

static void BM_DoublyList_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        DoublyList list;
        for (int i = 0; i < state.range(0); ++i) {
            list.pushBack("data");
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_DoublyList_PushBack)->Range(8, 8<<10)->Complexity();

static void BM_Stack_Push(benchmark::State& state) {
    for (auto _ : state) {
        Stack st;
        for (int i = 0; i < state.range(0); ++i) {
            st.push("data");
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_Stack_Push)->Range(8, 8<<10)->Complexity();

static void BM_Queue_Push(benchmark::State& state) {
    for (auto _ : state) {
        Queue q;
        for (int i = 0; i < state.range(0); ++i) {
            q.push("data");
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_Queue_Push)->Range(8, 8<<10)->Complexity();

static void BM_HashTable_Put(benchmark::State& state) {
    for (auto _ : state) {
        HashTable ht(state.range(0) * 2);
        for (int i = 0; i < state.range(0); ++i) {
            ht.put("key" + to_string(i), "val");
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_HashTable_Put)->Range(8, 2<<10)->Complexity();

static void BM_CBTree_Insert(benchmark::State& state) {
    for (auto _ : state) {
        CompleteBinaryTree tree;
        for (int i = 0; i < state.range(0); ++i) {
            tree.insert(i);
        }
    }
    state.SetComplexityN(state.range(0));
}

// Диапазон меньше, так как O(N^2)
BENCHMARK(BM_CBTree_Insert)->Range(8, 512)->Complexity();

BENCHMARK_MAIN();