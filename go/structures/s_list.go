package structures

import (
	"encoding/binary"
	"io"
	"os"
)

type SNode struct {
	Data string
	Next *SNode
}

type SinglyList struct {
	head *SNode
	tail *SNode
	size int
}

func NewSinglyList() *SinglyList {
	return &SinglyList{}
}

func (sl *SinglyList) Clear() {
	sl.head = nil
	sl.tail = nil
	sl.size = 0
}

func (sl *SinglyList) PushFront(val string) {
	newNode := &SNode{Data: val, Next: sl.head}
	sl.head = newNode
	if sl.tail == nil {
		sl.tail = sl.head
	}
	sl.size++
}

func (sl *SinglyList) PushBack(val string) {
	newNode := &SNode{Data: val}
	if sl.head == nil {
		sl.head = newNode
		sl.tail = newNode
	} else {
		sl.tail.Next = newNode
		sl.tail = newNode
	}
	sl.size++
}

func (sl *SinglyList) PopFront() {
	if sl.head == nil {
		return
	}
	sl.head = sl.head.Next
	if sl.head == nil {
		sl.tail = nil
	}
	sl.size--
}

func (sl *SinglyList) GetHead() string {
	if sl.head != nil {
		return sl.head.Data
	}
	return ""
}

func (sl *SinglyList) GetSize() int {
	return sl.size
}

func (sl *SinglyList) Search(val string) bool {
	curr := sl.head
	for curr != nil {
		if curr.Data == val {
			return true
		}
		curr = curr.Next
	}
	return false
}

func (sl *SinglyList) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(sl.size))
	curr := sl.head
	for curr != nil {
		b := []byte(curr.Data)
		binary.Write(file, binary.LittleEndian, int32(len(b)))
		file.Write(b)
		curr = curr.Next
	}
	return nil
}

func (sl *SinglyList) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	sl.Clear()
	var size int32
	binary.Read(file, binary.LittleEndian, &size)
	for i := 0; i < int(size); i++ {
		var l int32
		binary.Read(file, binary.LittleEndian, &l)
		buf := make([]byte, l)
		io.ReadFull(file, buf)
		sl.PushBack(string(buf))
	}
	return nil
}
