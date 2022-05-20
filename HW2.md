```python
import numpy as np
import graphviz
```

## Binary Tree

```python
def uniqints(rng=range(1, 100), size=16):
  return np.random.choice(rng, size=size, replace=False)
class BinaryTree():
    @staticmethod
    def createTree(collection):
        assert len(collection) > 1

        rootVal = collection[len(collection)//2]
        rootNode = BinaryTree(rootVal)

        for nodeVal in collection:
            if nodeVal == rootVal:
              continue
            rootNode.addNode(nodeVal)

        return rootNode

    @staticmethod
    def createGraph(tree):
      return graphviz.Source(f'digraph G {{{tree.createDot()}}}', format='png');

    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None

    def addNode(self, data):
        if data == self.data:
            return

        if data < self.data:
            if self.left:
                self.left.addNode(data)
            else:
                self.left = BinaryTree(data)
        else:
            if self.right:
                self.right.addNode(data)
            else:
                self.right = BinaryTree(data)

    def minValueNode(self, node):
        current = node

        while(current.left is not None):
            current = current.left

        return current

    def _deleteNode(self, root, data):
        if root is None:
            return root
        if data < root.data:
            root.left = self._deleteNode(root.left, data)
        elif data > root.data:
            root.right = self._deleteNode(root.right, data)
        else:
            # only one child or no child
            if root.left is None:
                temp = root.right
                root = None
                return temp
            elif root.right is None:
                temp = root.left
                root = None
                return temp

            temp = self.minValueNode(root.right)
            root.data = temp.data
            root.right = self._deleteNode(root.right, temp.data)

        return root

    def deleteNode(self, data):
        return self._deleteNode(self, data)

    def inOrderTraverse(self):  # Left - Root - Right
        elements = []

        if self.left:
            elements += self.left.inOrderTraverse()
        elements.append(self.data)

        if self.right:
            elements += self.right.inOrderTraverse()
        return elements

    def preOrderTraverse(self):  # Root - Left - Right
        elements = []

        elements.append(self.data)

        if self.left:
            elements += self.left.preOrderTraverse()

        if self.right:
            elements += self.right.preOrderTraverse()

        return elements

    def postOrderTraverse(self):
        elements = []

        if self.left:
            elements += self.left.postOrderTraverse()

        if self.right:
            elements += self.right.postOrderTraverse()

        elements.append(self.data)

        return elements

    def searchElement(self, elem):
        if self.data == elem:
            return True
        elif elem < self.data:
            if self.left:
                return self.left.seachElement(elem)
            else:
                return False

        else:
            if self.right:
                return self.right.searchElement(elem)
            else:
                return False

    def print(self, depth=0, label=''):
        spacing = depth * "--"

        if label:
            print(f'{spacing}{label}:{self.data}')
        else:
            print(f'{spacing}{self.data}')

        if self.left:
            self.left.print(depth + 1, 'L')

        if self.right:
            self.right.print(depth + 1, 'R')

    def createDot(self):
      template = ''

      if not self.left and not self.right:
        return template

      if self.left:
        template += f'{self.data} -> {self.left.data};\n'
        template += self.left.createDot()
      else:
        template += f'left_{self.data} [shape=point];\n'
        template += f'{self.data} -> left_{self.data};\n'
      
      if self.right:
        template += f'{self.data} -> {self.right.data};\n'
        template += self.right.createDot()
      else:
        template += f'right_{self.data} [shape=point];\n'
        template += f'{self.data} -> right_{self.data};\n'
      
      return template
data = uniqints(range(1,25))

print('Data', data)
tree = BinaryTree.createTree(data)

print("In Order Traversal", tree.inOrderTraverse())
print("Post Order Traversal", tree.postOrderTraverse())
print("Pre Order Traversal", tree.preOrderTraverse())


BinaryTree.createGraph(tree)
```

![image](https://user-images.githubusercontent.com/72040182/169512627-0b3b65fe-74dc-44a1-b82e-db3ef7157975.png)

```python
tree.addNode(30)
print('\nAfter adding an element')
BinaryTree.createGraph(tree)
```

![image](https://user-images.githubusercontent.com/72040182/169512738-9cfa99b9-3b41-4e3a-b42d-f2910dd90d7f.png)

```python
tree.deleteNode(20)
print('\nAfter removing an element')
BinaryTree.createGraph(tree)
```

![image](https://user-images.githubusercontent.com/72040182/169512791-fd9c6f12-6aad-40db-993a-377685b5b191.png)

## R-B tree

```python
class Node():
  def __init__(self, valu, parent=None, left=None, right=None, color=1):
    self.val = valu
    self.parent = parent
    self.left = left
    self.right = right
    self.color = color    # 1- red, 0 - black


class RedBlackTree():
  def __init__(self):
    self.NULL = Node(0, color=0)
    self.root = self.NULL

  def insertNode(self, key):
    node = Node(key, left=self.NULL, right=self.NULL, color=1)

    y = None
    x = self.root

    while x != self.NULL:
      y = x
      if node.val < x.val:
        x = x.left
      else:
        x = x.right

    node.parent = y
    if y is None:
      self.root = node
    elif node.val < y.val:
      y.left = node
    else:
      y.right = node

    if node.parent is None:
      node.color = 0
      return

    if node.parent.parent is None:
      return

    self.fixInsert(node)

  def minimum(self, node):
    while node.left != self.NULL:
      node = node.left
    return node

  def leftRotate(self, x):
    y = x.right
    x.right = y.left
    if y.left != self.NULL:
      y.left.parent = x

    y.parent = x.parent
    if x.parent is None:
      self.root = y
    elif x == x.parent.left:
      x.parent.left = y
    else:
      x.parent.right = y
    y.left = x
    x.parent = y

  def rightRotate(self, x):
    y = x.left
    x.left = y.right
    if y.right != self.NULL:
      y.right.parent = x

    y.parent = x.parent
    if x.parent is None:
      self.root = y
    elif x == x.parent.right:
      x.parent.right = y
    else:
      x.parent.left = y
    y.right = x
    x.parent = y

  def fixInsert(self, k):
    while k.parent.color == 1:
      if k.parent == k.parent.parent.right:
        u = k.parent.parent.left
        if u.color == 1:
          u.color = 0
          k.parent.color = 0
          k.parent.parent.color = 1
          k = k.parent.parent
        else:
          if k == k.parent.left:
            k = k.parent
            self.rightRotate(k)
          k.parent.color = 0
          k.parent.parent.color = 1
          self.leftRotate(k.parent.parent)
      else:
        u = k.parent.parent.right
        if u.color == 1:
          u.color = 0
          k.parent.color = 0
          k.parent.parent.color = 1
          k = k.parent.parent
        else:
          if k == k.parent.right:
            k = k.parent
            self.leftRotate(k)
          k.parent.color = 0
          k.parent.parent.color = 1
          self.rightRotate(k.parent.parent)
      if k == self.root:
        break
    self.root.color = 0

  def fixDelete(self, x):
    while x != self.root and x.color == 0:
      if x == x.parent.left:
        s = x.parent.right
        if s.color == 1:
          s.color = 0
          x.parent.color = 1
          self.leftRotate(x.parent)
          s = x.parent.right

        if s.left.color == 0 and s.right.color == 0:
          s.color = 1
          x = x.parent
        else:
          if s.right.color == 0:
            s.left.color = 0
            s.color = 1
            self.rightRotate(s)
            s = x.parent.right

          s.color = x.parent.color
          x.parent.color = 0
          s.right.color = 0
          self.leftRotate(x.parent)
          x = self.root
      else:
        s = x.parent.left
        if s.color == 1:
          s.color = 0
          x.parent.color = 1
          self.rightRotate(x.parent)
          s = x.parent.left

        if s.right.color == 0 and s.right.color == 0:
          s.color = 1
          x = x.parent
        else:
          if s.left.color == 0:
            s.right.color = 0
            s.color = 1
            self.leftRotate(s)
            s = x.parent.left

          s.color = x.parent.color
          x.parent.color = 0
          s.left.color = 0
          self.rightRotate(x.parent)
          x = self.root
    x.color = 0

  def _transplantNodes(self, u, v):
    if u.parent is None:
      self.root = v
    elif u == u.parent.left:
      u.parent.left = v
    else:
      u.parent.right = v
    v.parent = u.parent

  def removeNode_helper(self, node, key):
    z = self.NULL
    while node != self.NULL:
      if node.val == key:
        z = node

      if node.val <= key:
        node = node.right
      else:
        node = node.left

    if z == self.NULL:
      print("Value not present in Tree !!")
      return

    y = z
    y_original_color = y.color
    if z.left == self.NULL:
      x = z.right
      self._transplantNodes(z, z.right)
    elif (z.right == self.NULL):
      x = z.left
      self._transplantNodes(z, z.left)
    else:
      y = self.minimum(z.right)
      y_original_color = y.color
      x = y.right
      if y.parent == z:
        x.parent = y
      else:
        self._transplantNodes(y, y.right)
        y.right = z.right
        y.right.parent = y

      self._transplantNodes(z, y)
      y.left = z.left
      y.left.parent = y
      y.color = z.color
    if y_original_color == 0:
      self.fixDelete(x)

  def removeNode(self, val, node=None):
    if not node:
      node = self.root
      
    z = self.NULL
    while node != self.NULL:
      if node.val == val:
        z = node

      if node.val <= val:
        node = node.right
      else:
        node = node.left

    if z == self.NULL:
      print(f'Value not found: {val}')
      return

    y = z
    y_original_color = y.color
    if z.left == self.NULL:
      x = z.right
      self._transplantNodes(z, z.right)
    elif (z.right == self.NULL):
      x = z.left
      self._transplantNodes(z, z.left)
    else:
      y = self.minimum(z.right)
      y_original_color = y.color
      x = y.right
      if y.parent == z:
        x.parent = y
      else:
        self._transplantNodes(y, y.right)
        y.right = z.right
        y.right.parent = y

      self._transplantNodes(z, y)
      y.left = z.left
      y.left.parent = y
      y.color = z.color
    if y_original_color == 0:
      self.fixDelete(x)

  def _print(self, node, indent, last):
    if node != self.NULL:
      print(indent, end=' ')

      color = { 0:'black', 1:'red' }.get(node.color)

      if last:
        print(f"right:{color}----", end=' ')
        indent += "    "
      else:
        print(f"left:{color}----", end=' ')
        indent += " |  "

      print(str(node.val))
      self._print(node.left, indent, False)
      self._print(node.right, indent, True)

  def print(self):
    self._print(self.root, "", True)



bst = RedBlackTree()

dataset = uniqints()

for x in dataset:
  bst.insertNode(x)

bst.print()

bst.removeNode(dataset[0])
print("\ndelete", dataset[0])

bst.print()
```

![image](https://user-images.githubusercontent.com/72040182/169512852-62ed20a4-9674-4297-9f94-e968d56b5083.png)

## Binary Heap

```python
class BinaryHeap(object):
    def __init__(self):
        self.heap = [0]
        self.currentSize = 0

    def __repr__(self):
        heap = self.heap[1:]
        return ' '.join(str(i) for i in heap)

    def shiftUp(self, index):
        while (index // 2) > 0:
            if self.heap[index] < self.heap[index // 2]: 
                temp = self.heap[index // 2]
                self.heap[index // 2] = self.heap[index]
                self.heap[index] = temp
            index = index // 2

    def insert(self, key):
        self.heap.append(key)
        self.currentSize += 1
        self.shiftUp(self.currentSize)

    def shiftDown(self, index):
        while(index * 2) <= self.currentSize:
            minimumChild = self.minChild(index)
            if self.heap[index] > self.heap[minimumChild]:
                temp = self.heap[index]
                self.heap[index] = self.heap[minimumChild]
                self.heap[minimumChild] = temp
            index = minimumChild

    def minChild(self,i):
        if i * 2 + 1 > self.currentSize:
            return i * 2
        else:
            if self.heap[i * 2] < self.heap[i * 2 + 1]:
                return i * 2
            else:
                return i * 2 + 1

    def delete(self):
        deletedNode = self.heap[1]
        self.heap[1] = self.heap[self.currentSize]
        self.currentSize -= 1
        self.heap.pop()
        self.shiftDown(1)
        return deletedNode

    def buildHeap(self, alist):
        i = len(alist) // 2
        self.currentSize = len(alist)
        self.heap = [0] + alist[:]
        while (i > 0):
            self.shiftDown(i)
            i = i - 1

bh = BinaryHeap()

bh.buildHeap([9,5,6,2,3, 4, 10, 7, 11, 15, 18, 14, 1])

print(bh)

print('delete', bh.delete())
print('delete', bh.delete())
print('delete', bh.delete())

bh.insert(8)
print('insert', 8)
print(bh)
print('delete', bh.delete())
print(bh)
```

## Binomial Heap

```python
from copy import copy
from random import randint


class Node:
    def __init__(self, value=0.0):
        self.parent = None
        self.value = value
        self.childs = []
        self.index = 0

    def __repr__(self):
        return "'%r' -> %r" % (self.value, self.childs)

    def create_dot(self):
      template = ''

      for a in self.childs:
        template += f'{self.value} -> {a.value};\n'
        template += a.create_dot()
      
      return template


class Binomial:
    def __init__(self):
        self.head = {}
        self.minimum = None

    def insert(self, x, ind=0):
        index = ind
        new_node = x
        while index in self.head:
            if new_node.value < self.head[index].value:
                self.head[index].parent = new_node
                new_node.childs.append(self.head[index])
            else:
                new_node.parent = self.head[index]
                self.head[index].childs.append(new_node)
                new_node = self.head[index]
            new_node.index += 1
            del self.head[index]
            index += 1
        self.head[index] = new_node
        self.update_min()

    def update_min(self):
        minimum = float("inf")
        minimum_node = None
        for i in self.head.values():
            if min(minimum, i.value) == i.value:
                minimum_node = i
                minimum = i.value
        self.minimum = minimum_node

    def min(self):
        return self.minimum

    def merge(self, tree):
        tree2_keys = tree.head.keys()
        for i in tree2_keys:
            self.insert(tree.head[i], ind=i)
        self.update_min()

    def __repr__(self):
        return repr(self.head)

    def delete_min(self):
        if self.head:
            minimum = self.min()
            new_tree = copy(minimum.childs)
            for node in new_tree:
                node.parent = None
            del self.head[minimum.index]
            del minimum
            for node in new_tree:
                self.insert(node, node.index)
        self.update_min()

    def decrease_key(self, node, new_value):
        node.value = new_value
        while node.parent:
            if node.parent.value > node.value:
                temp = node.parent.value
                node.parent.value = node.value
                node.value = temp
                node = node.parent
            else:
                break
        self.update_min()

    def delete(self, node):
        self.decrease_key(node, -1.0*float('inf'))
        self.delete_min()
        self.update_min()

    def create_dot(self):
      template = 'digraph G {\n'
        
      for a in self.head.values():
        template += a.create_dot() + '\n'

      template += '}'
      return template

binomial_tree = Binomial()

for valu in uniqints():
    binomial_tree.insert(Node(valu))

template = binomial_tree.create_dot()

graphviz.Source(template, format="png")
```

![image](https://user-images.githubusercontent.com/72040182/169512972-d8b411ca-532f-435e-bbcd-98c2b05707ab.png)

## K-smallest

Використовувати чергу купи; він може дати вам K найбільших чи найменших чисел зі списку розміру N за O(NlogK) часу.

Внутрішньо це створює купу розміром K з першими K елементами вхідного списку, потім перебирає решту N-K елементів, переміщуючи кожен до купи, а потім вириваючи найбільший. Таке push and pop займає log K часу, що робить загальну операцію O(NlogK).

Можлива оптимізація для граничних випадків:

     Якщо K дорівнює 1, лінійний пошук мінімального, що дає результат O(N).
     Якщо K >= N, використати сортування, оскільки в цьому випадку O(NlogN) перевершить O(NlogK).

```python
import heapq


dataset = uniqints()
k_smallest = heapq.nsmallest(5, dataset)

print('data', dataset)
print('k-smallest', k_smallest)
```

![image](https://user-images.githubusercontent.com/72040182/169513023-95952bf6-11ab-4cb4-ab89-146f0982f2cb.png)



