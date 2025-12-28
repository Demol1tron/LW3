package structures

import (
	"encoding/binary"
	"io"
	"os"
)

type Stack struct {
	data     []string
	size     int
	capacity int
}

func NewStack(initialCapacity int) *Stack {
	if initialCapacity <= 0 {
		initialCapacity = 10
	}
	return &Stack{
		data:     make([]string, initialCapacity),
		size:     0,
		capacity: initialCapacity,
	}
}

func (s *Stack) resize() {
	newCap := s.capacity * 2
	if s.capacity == 0 {
		newCap = 1
	}
	newData := make([]string, newCap)
	copy(newData, s.data)
	s.data = newData
	s.capacity = newCap
}

func (s *Stack) Push(val string) {
	if s.size >= s.capacity {
		s.resize()
	}
	s.data[s.size] = val
	s.size++
}

func (s *Stack) Pop() string {
	if s.size == 0 {
		return ""
	}
	val := s.data[s.size-1]
	s.size--
	return val
}

func (s *Stack) Peek() string {
	if s.size == 0 {
		return ""
	}
	return s.data[s.size-1]
}

func (s *Stack) GetSize() int {
	return s.size
}

// Сериализация
func (s *Stack) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	binary.Write(file, binary.LittleEndian, int32(s.size))
	for i := 0; i < s.size; i++ {
		strBytes := []byte(s.data[i])
		binary.Write(file, binary.LittleEndian, int32(len(strBytes)))
		file.Write(strBytes)
	}
	return nil
}

func (s *Stack) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	var size int32
	binary.Read(file, binary.LittleEndian, &size)
	s.size = 0
	s.capacity = int(size) * 2
	if s.capacity == 0 {
		s.capacity = 10
	}
	s.data = make([]string, s.capacity)
	for i := 0; i < int(size); i++ {
		var l int32
		binary.Read(file, binary.LittleEndian, &l)
		buf := make([]byte, l)
		io.ReadFull(file, buf)
		s.Push(string(buf))
	}
	return nil
}
