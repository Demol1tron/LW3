package structures

import (
	"encoding/binary"
	"io"
	"os"
)

type DNode struct {
	Data string
	Next *DNode
	Prev *DNode
}

type DoublyList struct {
	head *DNode
	tail *DNode
	size int
}

func NewDoublyList() *DoublyList {
	return &DoublyList{}
}

func (dl *DoublyList) Clear() {
	dl.head = nil
	dl.tail = nil
	dl.size = 0
}

func (dl *DoublyList) PushFront(val string) {
	newNode := &DNode{Data: val}
	if dl.head == nil {
		dl.head = newNode
		dl.tail = newNode
	} else {
		newNode.Next = dl.head
		dl.head.Prev = newNode
		dl.head = newNode
	}
	dl.size++
}

func (dl *DoublyList) PushBack(val string) {
	newNode := &DNode{Data: val}
	if dl.tail == nil {
		dl.head = newNode
		dl.tail = newNode
	} else {
		dl.tail.Next = newNode
		newNode.Prev = dl.tail
		dl.tail = newNode
	}
	dl.size++
}

func (dl *DoublyList) PopBack() {
	if dl.tail == nil {
		return
	}
	if dl.head == dl.tail {
		dl.head = nil
		dl.tail = nil
	} else {
		dl.tail = dl.tail.Prev
		dl.tail.Next = nil
	}
	dl.size--
}

func (dl *DoublyList) PopFront() {
	if dl.head == nil {
		return
	}
	if dl.head == dl.tail {
		dl.head = nil
		dl.tail = nil
	} else {
		dl.head = dl.head.Next
		dl.head.Prev = nil
	}
	dl.size--
}

func (dl *DoublyList) GetTail() string {
	if dl.tail != nil {
		return dl.tail.Data
	}
	return ""
}

func (dl *DoublyList) GetSize() int {
	return dl.size
}

func (dl *DoublyList) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(dl.size))
	curr := dl.head
	for curr != nil {
		b := []byte(curr.Data)
		binary.Write(file, binary.LittleEndian, int32(len(b)))
		file.Write(b)
		curr = curr.Next
	}
	return nil
}

func (dl *DoublyList) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	dl.Clear()

	var size int32
	binary.Read(file, binary.LittleEndian, &size)
	for i := 0; i < int(size); i++ {
		var l int32
		binary.Read(file, binary.LittleEndian, &l)
		buf := make([]byte, l)
		io.ReadFull(file, buf)
		dl.PushBack(string(buf))
	}
	return nil
}
