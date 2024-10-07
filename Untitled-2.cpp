#include <iostream>
#include <vector>
#include <string>
#include <stack>

class Node {
public:
    int *keys;
    Node **child;
    int n;
    bool leaf;

    Node(int t, bool leaf) {
        keys = new int[2 * t - 1];
        child = new Node *[2 * t];
        n = 0;
        this->leaf = leaf;
    }
};

class BTree {
private:
    Node *root;
    int t;

    void insertNonFull(Node *node, int key) {
        int i = node->n - 1;

        if (node->leaf) {
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
            node->n++;
        } else {
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;

            if (node->child[i]->n == 2 * t - 1) {
                splitChild(node, i, node->child[i]);

                if (key > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->child[i], key);
        }
    }

    void splitChild(Node *x, int i, Node *y) {
        Node *z = new Node(t, y->leaf);
        x->child[i + 1] = z;
        x->n++;

        for (int j = x->n - 1; j > i; j--) {
            x->keys[j] = x->keys[j - 1];
        }
        for (int j = x->n; j > i + 1; j--) {
            x->child[j] = x->child[j - 1];
        }

        if (y->leaf) {
            x->keys[i] = y->keys[t - 1];
            z->n = t - 1;
            for (int j = 0; j < t - 1; j++) {
                z->keys[j] = y->keys[j + t];
            }
        } else {
            x->keys[i] = y->keys[t - 1];
            z->n = t;
            for (int j = 0; j < t; j++) {
                z->keys[j] = y->keys[j + t];
            }
            for (int j = 0; j <= t; j++) {
                z->child[j] = y->child[j + t];
            }
        }
        y->n = t - 1;
    }

public:
    BTree(int t) {
        root = nullptr;
        this->t = t;
    }

    void insert(int key) {
        if (root == nullptr) {
            root = new Node(t, true);
            root->keys[0] = key;
            root->n = 1;
        } else {
            if (root->n == 2 * t - 1) {
                Node *newRoot = new Node(t, false);
                newRoot->child[0] = root;
                splitChild(newRoot, 0, root);
                int i = 0;
                if (newRoot->keys[0] < key) {
                    i++;
                }
                insertNonFull(newRoot->child[i], key);
                root = newRoot;
            } else {
                insertNonFull(root, key);
            }
        }
    }

    void print(Node *node, int indent = 0) {
        for (int i = 0; i < indent; i++) {
            std::cout << "  ";
        }
        for (int i = 0; i < node->n; i++) {
            std::cout << node->keys[i] << " ";
        }
        std::cout << std::endl;
        if (!node->leaf) {
            for (int i = 0; i <= node->n; i++) {
                print(node->child[i], indent + 1);
            }
        }
    }

    void print() {
        print(root);
    }
};

int main() {
    BTree bTree(3);
    bTree.insert(10);
    bTree.insert(20);
    bTree.insert(5);
    bTree.insert(6);
    bTree.insert(12);
    bTree.insert(30);
    bTree.insert(7);
    bTree.insert(17);

    std::cout << "B-Tree:" << std::endl;
    bTree.print();

    return 0;
}