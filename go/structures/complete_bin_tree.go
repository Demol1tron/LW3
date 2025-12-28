package structures

import (
	"container/list"
	"encoding/binary"
	"os"
)

type TreeNode struct {
	Data  int
	Left  *TreeNode
	Right *TreeNode
}

type CompleteBinaryTree struct {
	root *TreeNode
	size int
}

func NewCompleteBinaryTree() *CompleteBinaryTree {
	return &CompleteBinaryTree{}
}

func (t *CompleteBinaryTree) Insert(val int) {
	newNode := &TreeNode{Data: val}
	if t.root == nil {
		t.root = newNode
		t.size++
		return
	}

	queue := list.New()
	queue.PushBack(t.root)

	for queue.Len() > 0 {
		element := queue.Front()
		queue.Remove(element)
		curr := element.Value.(*TreeNode)

		if curr.Left == nil {
			curr.Left = newNode
			t.size++
			return
		} else {
			queue.PushBack(curr.Left)
		}

		if curr.Right == nil {
			curr.Right = newNode
			t.size++
			return
		} else {
			queue.PushBack(curr.Right)
		}
	}
}

func (t *CompleteBinaryTree) Find(val int) bool {
	if t.root == nil {
		return false
	}
	queue := list.New()
	queue.PushBack(t.root)

	for queue.Len() > 0 {
		element := queue.Front()
		queue.Remove(element)
		curr := element.Value.(*TreeNode)

		if curr.Data == val {
			return true
		}
		if curr.Left != nil {
			queue.PushBack(curr.Left)
		}
		if curr.Right != nil {
			queue.PushBack(curr.Right)
		}
	}
	return false
}

func (t *CompleteBinaryTree) IsComplete() bool {
	if t.root == nil {
		return true
	}
	queue := list.New()
	queue.PushBack(t.root)
	flag := false

	for queue.Len() > 0 {
		element := queue.Front()
		queue.Remove(element)
		curr := element.Value.(*TreeNode)

		if curr.Left != nil {
			if flag {
				return false
			}
			queue.PushBack(curr.Left)
		} else {
			flag = true
		}

		if curr.Right != nil {
			if flag {
				return false
			}
			queue.PushBack(curr.Right)
		} else {
			flag = true
		}
	}
	return true
}

func (t *CompleteBinaryTree) SaveToBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(t.size))
	if t.root == nil {
		return nil
	}

	queue := list.New()
	queue.PushBack(t.root)

	for queue.Len() > 0 {
		element := queue.Front()
		queue.Remove(element)
		curr := element.Value.(*TreeNode)

		binary.Write(file, binary.LittleEndian, int32(curr.Data))

		if curr.Left != nil {
			queue.PushBack(curr.Left)
		}
		if curr.Right != nil {
			queue.PushBack(curr.Right)
		}
	}
	return nil
}

func (t *CompleteBinaryTree) LoadFromBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	t.root = nil
	t.size = 0 // Insert сам инкрементирует

	var count int32
	binary.Read(file, binary.LittleEndian, &count)

	for i := 0; i < int(count); i++ {
		var val int32
		binary.Read(file, binary.LittleEndian, &val)
		t.Insert(int(val))
	}
	return nil
}
