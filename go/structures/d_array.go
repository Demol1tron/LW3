package structures

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"strings"
)

type DynamicArray struct {
	data     []string
	size     int
	capacity int
}

func NewDynamicArray(initialCapacity int) *DynamicArray {
	if initialCapacity <= 0 {
		initialCapacity = 10
	}
	return &DynamicArray{
		data:     make([]string, initialCapacity),
		size:     0,
		capacity: initialCapacity,
	}
}

func (da *DynamicArray) resizeCapacity() {
	if da.size >= da.capacity {
		newCapacity := da.capacity * 2
		if da.capacity == 0 {
			newCapacity = 1
		}
		newData := make([]string, newCapacity)
		copy(newData, da.data)
		da.data = newData
		da.capacity = newCapacity
	}
}

func (da *DynamicArray) PushBack(value string) {
	da.resizeCapacity()
	da.data[da.size] = value
	da.size++
}

func (da *DynamicArray) PushFront(value string) {
	da.resizeCapacity()
	// Сдвигаем элементы вправо
	for i := da.size; i > 0; i-- {
		da.data[i] = da.data[i-1]
	}
	da.data[0] = value
	da.size++
}

func (da *DynamicArray) InsertAt(index int, value string) {
	if index < 0 || index > da.size {
		fmt.Fprintf(os.Stderr, "Error: Index out of range in InsertAt\n")
		return
	}
	da.resizeCapacity()
	for i := da.size; i > index; i-- {
		da.data[i] = da.data[i-1]
	}
	da.data[index] = value
	da.size++
}

func (da *DynamicArray) Get(index int) string {
	if index < 0 || index >= da.size {
		return ""
	}
	return da.data[index]
}

func (da *DynamicArray) Set(index int, value string) {
	if index < 0 || index >= da.size {
		return
	}
	da.data[index] = value
}

func (da *DynamicArray) RemoveAt(index int) {
	if index < 0 || index >= da.size {
		return
	}
	for i := index; i < da.size-1; i++ {
		da.data[i] = da.data[i+1]
	}
	da.size--
}

func (da *DynamicArray) GetSize() int {
	return da.size
}

// Сериализация
func (da *DynamicArray) SaveToText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	fmt.Fprintln(file, da.size)
	for i := 0; i < da.size; i++ {
		fmt.Fprintln(file, da.data[i])
	}
	return nil
}

func (da *DynamicArray) LoadFromText(filename string) error {
	content, err := os.ReadFile(filename)
	if err != nil {
		return err
	}
	lines := strings.Split(string(content), "\n")
	if len(lines) == 0 {
		return nil
	}

	// Сброс
	da.size = 0
	da.capacity = 10
	da.data = make([]string, da.capacity)

	// Первая строка - размер, пропускаем (или используем для аллокации)
	for i := 1; i < len(lines); i++ {
		if lines[i] != "" {
			da.PushBack(lines[i])
		}
	}
	return nil
}

func (da *DynamicArray) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	// Пишем размер массива (int32)
	if err := binary.Write(file, binary.LittleEndian, int32(da.size)); err != nil {
		return err
	}

	for i := 0; i < da.size; i++ {
		strBytes := []byte(da.data[i])
		length := int32(len(strBytes))
		// Пишем длину строки
		if err := binary.Write(file, binary.LittleEndian, length); err != nil {
			return err
		}
		// Пишем саму строку
		if _, err := file.Write(strBytes); err != nil {
			return err
		}
	}
	return nil
}

func (da *DynamicArray) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	var size int32
	if err := binary.Read(file, binary.LittleEndian, &size); err != nil {
		return err
	}

	da.size = 0
	da.capacity = int(size) * 2
	if da.capacity == 0 {
		da.capacity = 10
	}
	da.data = make([]string, da.capacity)

	for i := 0; i < int(size); i++ {
		var length int32
		if err := binary.Read(file, binary.LittleEndian, &length); err != nil {
			return err
		}
		buf := make([]byte, length)
		if _, err := io.ReadFull(file, buf); err != nil {
			return err
		}
		da.PushBack(string(buf))
	}
	return nil
}
