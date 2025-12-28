package structures

import (
	"encoding/binary"
	"io"
	"os"
)

type Queue struct {
	data     []string
	front    int
	rear     int
	size     int
	capacity int
}

func NewQueue(cap int) *Queue {
	if cap <= 0 {
		cap = 10
	}
	return &Queue{
		data:     make([]string, cap),
		front:    0,
		rear:     -1,
		size:     0,
		capacity: cap,
	}
}

func (q *Queue) resize() {
	newCap := q.capacity * 2
	newData := make([]string, newCap)
	for i := 0; i < q.size; i++ {
		newData[i] = q.data[(q.front+i)%q.capacity]
	}
	q.data = newData
	q.capacity = newCap
	q.front = 0
	q.rear = q.size - 1
}

func (q *Queue) Push(val string) {
	if q.size == q.capacity {
		q.resize()
	}
	q.rear = (q.rear + 1) % q.capacity
	q.data[q.rear] = val
	q.size++
}

func (q *Queue) Pop() string {
	if q.size == 0 {
		return ""
	}
	val := q.data[q.front]
	q.front = (q.front + 1) % q.capacity
	q.size--
	return val
}

func (q *Queue) Peek() string {
	if q.size == 0 {
		return ""
	}
	return q.data[q.front]
}

func (q *Queue) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(q.size))
	for i := 0; i < q.size; i++ {
		val := q.data[(q.front+i)%q.capacity]
		b := []byte(val)
		binary.Write(file, binary.LittleEndian, int32(len(b)))
		file.Write(b)
	}
	return nil
}

func (q *Queue) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	var size int32
	binary.Read(file, binary.LittleEndian, &size)

	// Сброс
	q.capacity = int(size) * 2
	if q.capacity == 0 {
		q.capacity = 10
	}
	q.data = make([]string, q.capacity)
	q.front = 0
	q.rear = -1
	q.size = 0

	for i := 0; i < int(size); i++ {
		var l int32
		binary.Read(file, binary.LittleEndian, &l)
		buf := make([]byte, l)
		io.ReadFull(file, buf)
		q.Push(string(buf))
	}
	return nil
}
