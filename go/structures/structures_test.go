package structures

import (
	"fmt"
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

// Удаление временных файлов
func cleanup(filename string) {
	os.Remove(filename)
}

// DYNAMIC ARRAY TESTS
func TestDynamicArray_Extended(t *testing.T) {
	t.Run("Edge Cases & Logic", func(t *testing.T) {
		arr := NewDynamicArray(1) // Маленький размер для проверки resize

		arr.PushFront("B")
		arr.PushFront("A") // Тут сработает resize
		assert.Equal(t, 2, arr.GetSize())
		assert.Equal(t, "A", arr.Get(0))
		assert.Equal(t, "B", arr.Get(1))

		arr.InsertAt(0, "Start") // Start, A, B
		arr.InsertAt(2, "Mid")   // Start, A, Mid, B
		arr.InsertAt(4, "End")   // Start, A, Mid, B, End

		assert.Equal(t, "Start", arr.Get(0))
		assert.Equal(t, "Mid", arr.Get(2))
		assert.Equal(t, "End", arr.Get(4))

		// Ошибки индексов
		arr.InsertAt(-1, "Err")
		arr.InsertAt(100, "Err")
		assert.Equal(t, 5, arr.GetSize()) // Размер не изменился

		arr.Set(0, "NewStart")
		assert.Equal(t, "NewStart", arr.Get(0))
		arr.Set(100, "Fail") // Ничего не упадет
		assert.Equal(t, "", arr.Get(100))

		arr.RemoveAt(2) // Удаляем Mid -> Start, A, B, End
		assert.Equal(t, "B", arr.Get(2))

		arr.RemoveAt(-1)
		arr.RemoveAt(100)
		assert.Equal(t, 4, arr.GetSize())
	})

	t.Run("Persistence (Text & Binary)", func(t *testing.T) {
		arr := NewDynamicArray(10)
		arr.PushBack("Data1")
		arr.PushBack("Data2")

		txtFile := "da_test.txt"
		defer cleanup(txtFile)
		assert.NoError(t, arr.SaveToText(txtFile))

		arr2 := NewDynamicArray(0)
		assert.NoError(t, arr2.LoadFromText(txtFile))
		assert.Equal(t, arr.GetSize(), arr2.GetSize())
		assert.Equal(t, arr.Get(0), arr2.Get(0))

		binFile := "da_test.bin"
		defer cleanup(binFile)
		assert.NoError(t, arr.SaveToBinary(binFile))

		arr3 := NewDynamicArray(0)
		assert.NoError(t, arr3.LoadFromBinary(binFile))
		assert.Equal(t, "Data1", arr3.Get(0))
	})
}

// STACK TESTS
func TestStack_Extended(t *testing.T) {
	t.Run("Logic", func(t *testing.T) {
		s := NewStack(1)
		assert.Equal(t, "", s.Pop()) // Проврка с пустотой
		assert.Equal(t, "", s.Peek())

		s.Push("A")
		s.Push("B") // Проверка ресайза
		assert.Equal(t, 2, s.GetSize())
		assert.Equal(t, "B", s.Peek())
	})

	t.Run("Persistence", func(t *testing.T) {
		s := NewStack(10)
		s.Push("SaveMe")

		binFile := "stack.bin"
		defer cleanup(binFile)

		assert.NoError(t, s.SaveToBinary(binFile))

		s2 := NewStack(1)
		assert.NoError(t, s2.LoadFromBinary(binFile))
		assert.Equal(t, "SaveMe", s2.Pop())
	})
}

// QUEUE TESTS
func TestQueue_Extended(t *testing.T) {
	t.Run("Circular Buffer Logic", func(t *testing.T) {
		q := NewQueue(3) // data[3]
		q.Push("1")
		q.Push("2")
		q.Push("3")

		assert.Equal(t, "1", q.Pop()) // Теперь front сдвинулся
		q.Push("4")                   // Должно встать в начало массива (цикличность)

		// Теперь переполнение и RESIZE
		q.Push("5") // Должен произойти resize и упорядочивание

		assert.Equal(t, 4, q.size)
		assert.Equal(t, "2", q.Pop())
		assert.Equal(t, "3", q.Pop())
		assert.Equal(t, "4", q.Pop())
		assert.Equal(t, "5", q.Pop())
		assert.Equal(t, "", q.Pop())
	})

	t.Run("Persistence", func(t *testing.T) {
		q := NewQueue(5)
		q.Push("QData")

		f := "queue.bin"
		defer cleanup(f)

		q.SaveToBinary(f)
		q2 := NewQueue(1)
		q2.LoadFromBinary(f)
		assert.Equal(t, "QData", q2.Pop())
	})
}

// SINGLY LIST TESTS
func TestSinglyList_Extended(t *testing.T) {
	t.Run("Complex Operations", func(t *testing.T) {
		sl := NewSinglyList()
		sl.PushFront("A")
		sl.PushBack("C")

		assert.True(t, sl.Search("A"))
		assert.False(t, sl.Search("Z"))

		sl.Clear()
		assert.Equal(t, 0, sl.GetSize())
		assert.Equal(t, "", sl.GetHead())

		// Проверка с пустотой
		sl.PopFront()
	})

	t.Run("Persistence", func(t *testing.T) {
		sl := NewSinglyList()
		sl.PushBack("Node1")
		sl.PushBack("Node2")

		f := "slist.bin"
		defer cleanup(f)

		sl.SaveToBinary(f)
		loaded := NewSinglyList()
		loaded.LoadFromBinary(f)

		assert.Equal(t, 2, loaded.GetSize())
		assert.Equal(t, "Node1", loaded.GetHead())
	})
}

// DOUBLY LIST TESTS
func TestDoublyList_Extended(t *testing.T) {
	t.Run("Push/Pop Logic", func(t *testing.T) {
		dl := NewDoublyList()
		dl.PopBack() // Проверка с пустотой
		dl.PopFront()

		dl.PushFront("A")
		assert.Equal(t, "A", dl.GetTail())

		dl.PushBack("B") // A <-> B
		assert.Equal(t, "B", dl.GetTail())

		dl.PopFront() // Удалили A
		assert.Equal(t, "B", dl.GetTail())

		dl.PopBack() // Удалили B -> пусто
		assert.Equal(t, 0, dl.GetSize())
		assert.Equal(t, "", dl.GetTail())
	})

	t.Run("Persistence", func(t *testing.T) {
		dl := NewDoublyList()
		dl.PushFront("D1")

		f := "dlist.bin"
		defer cleanup(f)

		dl.SaveToBinary(f)
		l2 := NewDoublyList()
		l2.LoadFromBinary(f)
		assert.Equal(t, "D1", l2.GetTail())
	})
}

// HASH TABLE TESTS
func TestHashTable_Extended(t *testing.T) {
	t.Run("Collisions & Logic", func(t *testing.T) {
		ht := NewHashTable(1) // Емкость 1 гарантирует коллизии
		ht.Put("A", "ValA")
		ht.Put("B", "ValB")

		assert.Equal(t, "ValA", ht.Get("A"))
		assert.Equal(t, "ValB", ht.Get("B"))
		assert.Equal(t, 2, ht.size)

		ht.Put("A", "NewValA")
		assert.Equal(t, "NewValA", ht.Get("A"))

		ht.Remove("A")
		assert.Equal(t, "", ht.Get("A"))
		assert.Equal(t, "ValB", ht.Get("B"))

		ht.Remove("Missing")
	})

	t.Run("Persistence", func(t *testing.T) {
		ht := NewHashTable(10)
		ht.Put("Key", "Value")

		f := "hash.bin"
		defer cleanup(f)

		ht.SaveToBinary(f)
		ht2 := NewHashTable(5)
		ht2.LoadFromBinary(f)

		assert.Equal(t, "Value", ht2.Get("Key"))
	})
}

// COMPLETE BINARY TREE TESTS
func TestTree_Extended(t *testing.T) {
	t.Run("Completeness & Search", func(t *testing.T) {
		tree := NewCompleteBinaryTree()
		assert.True(t, tree.IsComplete()) // Empty is complete

		tree.Insert(1)
		tree.Insert(2)
		tree.Insert(3)
		assert.True(t, tree.IsComplete())

		assert.True(t, tree.Find(3))
		assert.False(t, tree.Find(4))
	})

	t.Run("Persistence", func(t *testing.T) {
		tree := NewCompleteBinaryTree()
		tree.Insert(100)
		tree.Insert(200)

		f := "tree.bin"
		defer cleanup(f)

		tree.SaveToBinary(f)
		t2 := NewCompleteBinaryTree()
		t2.LoadFromBinary(f)

		assert.True(t, t2.Find(100))
		assert.True(t, t2.Find(200))
	})
}

// BENCHMARKS
func BenchmarkFill_1000(b *testing.B) {
	b.Run("DynamicArray", func(b *testing.B) {
		for i := 0; i < b.N; i++ {
			arr := NewDynamicArray(10)
			for j := 0; j < 1000; j++ {
				arr.PushBack("data")
			}
		}
	})
	b.Run("SinglyList", func(b *testing.B) {
		for i := 0; i < b.N; i++ {
			list := NewSinglyList()
			for j := 0; j < 1000; j++ {
				list.PushBack("data")
			}
		}
	})
	b.Run("HashTable", func(b *testing.B) {
		for i := 0; i < b.N; i++ {
			ht := NewHashTable(2000)
			for j := 0; j < 1000; j++ {
				ht.Put(fmt.Sprintf("key%d", j), "val")
			}
		}
	})
}
