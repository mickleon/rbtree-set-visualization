# Red/black tree set visualisation

## What is this?

There is C++ class for red/black tree set, that have method for **console output** for tree **without any specific libraries**. The program correctly outputs all types, that can be output using the `<<` operator.

## What is it for?

This is a training example of a red/black tree. This may be necessary for those who study C++ and dynamic structures such as trees.

## Run

```bash
make
./main
```

or

```bash
g++ main.cpp -o main
./main
```

## Available features

`RBTree<T>` is a template class that supports all types, that are comparable and can be output using the `<<` operator.

The class supports iteration, so the tree elements can be iterated through in a loop:

```cpp
RBTree<int> tr;
for (int el : tr) {
    cout << el << ' ';
}
```

The following methods are available for working with tree:

- `void RBTree<T>::insert(const T value)`: inserts node with value `value` to a tree and calls the fixup function.
- `void RBTree<T>::erase(const T value)`: erases the node with value `value` from tree and calls the fixup function.
- `int RBTree<T>::max() const`: returns maximum value in the tree.
- `int RBTree<T>::min() const`: returns minimum value in the tree.
- `void RBTree::clear()`: erases all the nodes from the tree.
- `ostream &operator<<(ostream &out, const RBTree<T> &tr)`: outputs the tree to the ostream. If `RBTree::show_null_leaves` is `true` (`false` by default) it displays null leaves.
- `Iterator RBTree<T>::begin() const`: returns an iterator to the element with the minimal value.
- `Iterator RBTree<T>::end() const`: returns an iterator to the element which is after the element with the maximum value (`nullptr`).

## Example of output

![Tree output](/img/tree_output.png)

With null leaves:

![Tree output with null leaves](/img/tree_output_null_leaves.png)

With `std::string` nodes:

![Output of a tree with string nodes](/img/tree_output_strings.png)
