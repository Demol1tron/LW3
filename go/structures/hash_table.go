package structures

import (
	"encoding/binary"
	"io"
	"os"
)

type HashNode struct {
	Key   string
	Value string
}

type HashTable struct {
	table    [][]HashNode
	capacity int
	size     int
}

func NewHashTable(cap int) *HashTable {
	if cap <= 0 {
		cap = 10
	}
	return &HashTable{
		table:    make([][]HashNode, cap),
		capacity: cap,
		size:     0,
	}
}

func (ht *HashTable) hashFunction(key string) int {
	hash := 0
	for _, c := range key {
		hash = (hash*31 + int(c)) % ht.capacity
	}
	return hash
}

func (ht *HashTable) Put(key, value string) {
	index := ht.hashFunction(key)
	// Ищем обновление
	for i, node := range ht.table[index] {
		if node.Key == key {
			ht.table[index][i].Value = value
			return
		}
	}
	// Добавляем новое (метод цепочек)
	ht.table[index] = append(ht.table[index], HashNode{Key: key, Value: value})
	ht.size++
}

func (ht *HashTable) Get(key string) string {
	index := ht.hashFunction(key)
	for _, node := range ht.table[index] {
		if node.Key == key {
			return node.Value
		}
	}
	return ""
}

func (ht *HashTable) Remove(key string) {
	index := ht.hashFunction(key)
	chain := ht.table[index]
	for i, node := range chain {
		if node.Key == key {
			// Удаление из слайса
			ht.table[index] = append(chain[:i], chain[i+1:]...)
			ht.size--
			return
		}
	}
}

func (ht *HashTable) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(ht.size))
	for _, chain := range ht.table {
		for _, node := range chain {
			// Key
			kb := []byte(node.Key)
			binary.Write(file, binary.LittleEndian, int32(len(kb)))
			file.Write(kb)
			// Value
			vb := []byte(node.Value)
			binary.Write(file, binary.LittleEndian, int32(len(vb)))
			file.Write(vb)
		}
	}
	return nil
}

func (ht *HashTable) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	// Очистка
	ht.table = make([][]HashNode, ht.capacity)
	ht.size = 0

	var count int32
	binary.Read(file, binary.LittleEndian, &count)

	for i := 0; i < int(count); i++ {
		var l int32
		// Key
		binary.Read(file, binary.LittleEndian, &l)
		kb := make([]byte, l)
		io.ReadFull(file, kb)
		// Value
		binary.Read(file, binary.LittleEndian, &l)
		vb := make([]byte, l)
		io.ReadFull(file, vb)

		ht.Put(string(kb), string(vb))
	}
	return nil
}
