#ifndef RBTREE_H
#define RBTREE_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using std::vector, std::cout, std::setw, std::ostream;

template<typename T>
class RBTree {
public:
    bool show_null_leaves;
    RBTree();

    ~RBTree();

    struct Node {
        enum Color { BLACK, RED };

        T inf;
        Node *left, *right;
        Node *parent;
        Color color;

        Node();
        Node(const T value);
    };

    struct Iterator {
        Node *node;

        T &operator*();
        bool operator!=(const Iterator &other) const;
        Iterator &operator++();
    };

    Iterator begin() const;
    Iterator end() const;

    void insert(const T value);
    void erase(const T value);
    T max() const;
    T min() const;
    void clear();

    template<typename U>
    friend ostream &operator<<(ostream &out, const RBTree<U> &tr);

private:
    Node *root;
    static const Node *NIL;

    void right_rotate(Node *p);
    void left_rotate(Node *p);
    void insert_fixup(Node *node);
    void erase_node(Node *node);
    void erase_fixup(Node *node);
    static Node *find(Node *node, const T value);
    static Node *max(Node *node);
    static Node *min(Node *node);
    static void clear(Node *node);
    static int height(const Node *node);
    void make_array(
        vector<vector<const Node *>> &array, const Node *node, const int depth = 0,
        const int count = 1
    ) const;
    int max_char_count(const Node *node) const;

    template<typename U>
    friend ostream &operator<<(ostream &out, const typename RBTree<U>::Node *node);
};

template<typename T>
using Node = typename RBTree<T>::Node;
template<typename T>
using Color = typename Node<T>::Color;
template<typename T>
using Iterator = typename RBTree<T>::Iterator;

template<typename T>
RBTree<T>::Node::Node() {
    inf = T{};
    left = right = parent = nullptr;
    color = BLACK;
}

template<typename T>
RBTree<T>::Node::Node(const T value) {
    inf = value;
    left = right = parent = nullptr;
    color = RED;
}

template<typename T>
T &RBTree<T>::Iterator::operator*() {
    return this->node->inf;
}

template<typename T>
typename RBTree<T>::Iterator &RBTree<T>::Iterator::operator++() {
    if (this->node->right != nullptr) {
        this->node = RBTree<T>::min(node->right);
        return *this;
    }
    Node *temp = this->node->parent;
    while (temp != nullptr && this->node == temp->right) {
        this->node = temp;
        temp = temp->parent;
    }
    this->node = temp;
    return *this;
}

template<typename T>
bool RBTree<T>::Iterator::operator!=(const Iterator &other) const {
    return this->node != other.node;
}

template<typename T>
typename RBTree<T>::Iterator RBTree<T>::begin() const {
    return {RBTree<T>::min(this->root)};
}

template<typename T>
typename RBTree<T>::Iterator RBTree<T>::end() const {
    return {nullptr};
}

template<typename T>
RBTree<T>::RBTree() {
    root = nullptr;
    show_null_leaves = false;
}

// "nullptr" node that used for output when `show_null_leaves == true`
template<typename T>
const typename RBTree<T>::Node *RBTree<T>::NIL = new typename RBTree<T>::Node();

template<typename T>
RBTree<T>::~RBTree() {
    clear(this->root);
}

// Rotates the tree to the left around the node `p`
template<typename T>
void RBTree<T>::left_rotate(Node *p) {
    if (p == nullptr || p->right == nullptr) {
        return;
    }

    Node *c = p->right;
    p->right = c->left;
    if (c->left != nullptr) {
        c->left->parent = p;
    }

    c->parent = p->parent;
    if (c->parent == nullptr) {
        this->root = c;
    } else if (p == p->parent->left) {
        p->parent->left = c;
    } else {
        p->parent->right = c;
    }

    c->left = p;
    p->parent = c;
}

// Rotates the tree to the right around the node `p`
template<typename T>
void RBTree<T>::right_rotate(Node *p) {
    if (p == nullptr || p->left == nullptr) {
        return;
    }

    Node *c = p->left;
    p->left = c->right;
    if (c->right != nullptr) {
        c->right->parent = p;
    }

    c->parent = p->parent;
    if (c->parent == nullptr) {
        this->root = c;
    } else if (p == p->parent->right) {
        p->parent->right = c;
    } else {
        p->parent->left = c;
    }

    c->right = p;
    p->parent = c;
}

// Inserts node with value `value` to a tree and calls the fixup function
template<typename T>
void RBTree<T>::insert(const T value) {
    Node *temp = this->root;
    while (temp != nullptr) {
        if (value < temp->inf && temp->left != nullptr) {
            temp = temp->left;
        } else if (value > temp->inf && temp->right != nullptr) {
            temp = temp->right;
        } else if (value != temp->inf) {
            break;
        } else {
            return;
        }
    }

    Node *node = new Node(value);
    if (temp == nullptr) {
        this->root = node;
    } else {
        node->parent = temp;
        if (value < temp->inf) {
            temp->left = node;
        } else {
            temp->right = node;
        }
    }
    this->insert_fixup(node);
}

// Auxullary function for `insert`, it does a fixup of a tree
template<typename T>
void RBTree<T>::insert_fixup(Node *node) {
    while (node->parent != nullptr && node->parent->color == Node::RED) {
        Node *g = node->parent->parent;
        if (node->parent == g->left) {
            Node *u = g->right;
            if (u != nullptr && u->color == Node::RED) { // Uncle is red
                node->parent->color = Node::BLACK;
                u->color = Node::BLACK;
                g->color = Node::RED;
                node = g;
            } else {                               // Uncle is black
                if (node == node->parent->right) { // Node is left child
                    node = node->parent;
                    this->left_rotate(node);
                    g = node->parent->parent;
                }
                // Node is right child
                node->parent->color = Node::BLACK;
                g->color = Node::RED;
                this->right_rotate(g);
                break;
            }
        } else { // Symmetricall case
            Node *u = g->left;
            if (u != nullptr && u->color == Node::RED) { // Uncle is red
                node->parent->color = Node::BLACK;
                u->color = Node::BLACK;
                g->color = Node::RED;
                node = g;
            } else {                              // Uncle is black
                if (node == node->parent->left) { // Node is left child
                    node = node->parent;
                    this->right_rotate(node);
                    g = node->parent->parent;
                }
                // Node is right child
                node->parent->color = Node::BLACK;
                g->color = Node::RED;
                this->left_rotate(g);
                break;
            }
        }
    }
    this->root->color = Node::BLACK;
}

// Erases the node with value `value` from tree and calls the fixup function
template<typename T>
void RBTree<T>::erase(const T value) {
    Node *node = this->find(this->root, value);
    if (node != nullptr) {
        this->erase_node(node);
    }
}

// Erases the node `node` from tree and calls the fixup function
template<typename T>
void RBTree<T>::erase_node(Node *node) {
    Node *del, *del_child;
    // Choosing node to delete
    if (node->left == nullptr || node->right == nullptr) {
        del = node;
    } else {
        del = min(node->right);
        node->inf = del->inf;
    }

    if (del->left != nullptr) {
        del_child = del->left;
    } else {
        del_child = del->right;
    }

    if (del->color == Node::BLACK) {
        erase_fixup(del);
    }

    if (del_child != nullptr) {
        del_child->parent = del->parent;
    }

    // Replacing `del` with `del_child`
    if (del->parent == nullptr) {
        this->root = del_child;
        if (this->root != nullptr) {
            this->root->color = Node::BLACK;
        }
    } else if (del == del->parent->left) {
        del->parent->left = del_child;
    } else {
        del->parent->right = del_child;
    }
    delete del;
}

// Auxullary function for `erase`, it does a fixup of a tree
template<typename T>
void RBTree<T>::erase_fixup(Node *node) {
    while (node != this->root && node->color == Node::BLACK) {
        if (node == node->parent->left) {
            Node *s = node->parent->right;
            if (s != nullptr && s->color == Node::RED) { // Sibling is red
                s->color = Node::BLACK;
                node->parent->color = Node::RED;
                this->left_rotate(node->parent);
                s = node->parent->right;
            }
            if ((s->left == nullptr || s->left->color == Node::BLACK) &&
                (s->right == nullptr ||
                 s->right->color == Node::BLACK)) { // Sibling and his children is black
                s->color = Node::RED;
                node = node->parent;
            } else { // Sibling is black, his left child is red, right is black
                if ((s->right == nullptr || s->right->color == Node::BLACK)) {
                    if (s->left != nullptr) {
                        s->left->color = Node::BLACK;
                    }
                    s->color = Node::RED;
                    this->right_rotate(s);
                    s = node->parent->right;
                }
                // Sibling is black, his right child is red
                s->color = node->parent->color;
                node->parent->color = Node::BLACK;
                if (s->right != nullptr) {
                    s->right->color = Node::BLACK;
                }
                this->left_rotate(node->parent);
                node = this->root;
            }
        } else { // Symmetrical case
            Node *s = node->parent->left;
            if (s != nullptr && s->color == Node::RED) { // Sibling is red
                s->color = Node::BLACK;
                node->parent->color = Node::RED;
                this->right_rotate(node->parent);
                s = node->parent->left;
            }
            if ((s->left == nullptr || s->left->color == Node::BLACK) &&
                (s->right == nullptr ||
                 s->right->color == Node::BLACK)) { // Sibling and his children is black
                s->color = Node::RED;
                node = node->parent;
            } else { // Sibling is black, his right child is red, left is black
                if ((s->left == nullptr || s->left->color == Node::BLACK)) {
                    if (s->right != nullptr) {
                        s->right->color = Node::BLACK;
                    }
                    s->color = Node::RED;
                    this->left_rotate(s);
                    s = node->parent->left;
                }
                // Sibling is black, his left child is red
                s->color = node->parent->color;
                node->parent->color = Node::BLACK;
                if (s->left != nullptr) {
                    s->left->color = Node::BLACK;
                }
                this->right_rotate(node->parent);
                node = this->root;
            }
        }
    }
    node->color = Node::BLACK;
}

// Returns a pointer to a node in the subtree `node` with the value `value`
template<typename T>
typename RBTree<T>::Node *RBTree<T>::find(Node *node, const T value) {
    if (node == nullptr || node->inf == value) {
        return node;
    }

    if (value < node->inf) {
        return RBTree::find(node->left, value);
    }
    return RBTree::find(node->right, value);
}

// Returns a pointer to a node in the subtree `node` with the maximal value
template<typename T>
typename RBTree<T>::Node *RBTree<T>::max(Node *node) {
    if (node == nullptr) {
        return nullptr;
    }

    Node *y = node;
    while (y->right != nullptr) {
        y = y->right;
    }
    return y;
}

// Overload of previous function, where `node` is a tree root `this->root`
template<typename T>
T RBTree<T>::max() const {
    return this->max(this->root)->inf;
}

// Returns a pointer to a node in the subtree `node` with the minimal value
template<typename T>
typename RBTree<T>::Node *RBTree<T>::min(Node *node) {
    if (node == nullptr) {
        return nullptr;
    }
    Node *y = node;
    while (y->left != nullptr) {
        y = y->left;
    }
    return y;
}

// Overload of previous function, where `node` is a tree root `this->root`
template<typename T>
T RBTree<T>::min() const {
    return this->min(this->root)->inf;
}

template<typename T>
int RBTree<T>::height(const Node *node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + std::max(height(node->left), height(node->right));
}

// Traversal with depth calculation and node offset from the left edge of the level
template<typename T>
void RBTree<T>::make_array(
    vector<vector<const Node *>> &array, const Node *node, const int depth, const int count
) const {
    array[depth][count - (1 << depth)] = node;

    if (node->left != nullptr) {
        this->make_array(array, node->left, depth + 1, count * 2);
    } else if (this->show_null_leaves) {
        array[depth + 1][count * 2 - (1 << (depth + 1))] = RBTree::NIL;
    }

    if (node->right != nullptr) {
        this->make_array(array, node->right, depth + 1, count * 2 + 1);
    } else if (this->show_null_leaves) {
        array[depth + 1][count * 2 + 1 - (1 << (depth + 1))] = RBTree::NIL;
    }
}

// Returns length of longest `inf` in subtree `node`
template<typename T>
int RBTree<T>::max_char_count(const Node *node) const {
    if (node == nullptr) {
        return this->show_null_leaves ? 1 : 0;
    }

    std::stringstream out;
    out << node->inf;
    return std::max(
        static_cast<int>(out.str().size()),
        std::max(this->max_char_count(node->left), this->max_char_count(node->right))
    );
}

template<typename T>
ostream &operator<<(ostream &out, const typename RBTree<T>::Node *node) {
    const std::streamsize width = out.width();
    out.width(0);
    if (node == nullptr) {
        if (width > 0) {
            out << setw(width) << ' ';
        }
    } else if (node == RBTree<T>::NIL) {
        out << setw(width) << 'n';
    } else if (node->color == Node<T>::RED) {
        out << "\033[31m" << setw(width) << node->inf << "\033[0m";
    } else {
        out << setw(width) << node->inf;
    }
    return out;
}

template<typename T>
ostream &operator<<(ostream &out, const RBTree<T> &tr) {
    if (tr.root == nullptr) {
        out << "NULL TREE\n";
        return out;
    }

    int width, offset = 1;
    vector<vector<const Node<T> *>> array;
    array.assign(
        tr.show_null_leaves ? (RBTree<T>::height(tr.root) + 1) : RBTree<T>::height(tr.root), {}
    );
    for (vector<const Node<T> *> &level : array) {
        level.assign(offset, nullptr);
        offset <<= 1;
    }
    tr.make_array(array, tr.root);

    // Maximum number of digits in node
    const int d = tr.max_char_count(tr.root);
    width = (d + 1) * (offset >> 1);
    offset = 1;
    for (vector<const Node<T> *> &level : array) {
        out.width(width >> 1);
        ::operator<< <T>(out, level[0]);
        for (int i = 1; i < offset; ++i) {
            out.width(width);
            ::operator<< <T>(out, level[i]);
        }
        out << '\n';
        offset <<= 1;
        width >>= 1;
    }
    return out;
}

// Erases all the nodes from the subtree `node`
template<typename T>
void RBTree<T>::clear(Node *node) {
    if (node != nullptr) {
        RBTree::clear(node->left);
        RBTree::clear(node->right);
        delete node;
    }
}

// Overload of previous function, where `node` is a tree root `this->root`
template<typename T>
void RBTree<T>::clear() {
    return this->clear(this->root);
}

#endif
