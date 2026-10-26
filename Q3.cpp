#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Node {
    ll key;
    bool red;
    Node *left, *right, *parent;

    Node(ll key, bool red) {
        this->key = key;
        this->red = red;
        left = right = parent = nullptr;
    }
};

Node* NIL;
Node* root;

// Left rotation
void leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NIL) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

// Right rotation
void rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;

    if (x->right != NIL) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == NIL) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

// Restore Red-Black properties after insertion
void insertFix(Node* z) {
    while (z->parent->red) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;

            if (y->red) {
                z->parent->red = false;
                y->red = false;
                z->parent->parent->red = true;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }

                z->parent->red = false;
                z->parent->parent->red = true;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;

            if (y->red) {
                z->parent->red = false;
                y->red = false;
                z->parent->parent->red = true;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }

                z->parent->red = false;
                z->parent->parent->red = true;
                leftRotate(z->parent->parent);
            }
        }
    }

    root->red = false;
}

// Insert a distinct key
void insertKey(ll key) {
    Node* z = new Node(key, true);
    z->left = z->right = NIL;

    Node* y = NIL;
    Node* x = root;

    while (x != NIL) {
        y = x;

        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            delete z;
            return;
        }
    }

    z->parent = y;

    if (y == NIL) {
        root = z;
    } else if (key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    insertFix(z);
}

// Print keys in [a, b] in increasing order
void enumerate(Node* node, ll a, ll b, bool& first) {
    if (node == NIL) {
        return;
    }

    // Left subtree can contain keys >= a only if node->key > a
    if (node->key > a) {
        enumerate(node->left, a, b, first);
    }

    if (a <= node->key && node->key <= b) {
        if (!first) {
            cout << ' ';
        }
        cout << node->key;
        first = false;
    }

    // Right subtree can contain keys <= b only if node->key < b
    if (node->key < b) {
        enumerate(node->right, a, b, first);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    NIL = new Node(0, false);
    NIL->left = NIL->right = NIL->parent = NIL;
    root = NIL;

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        ll x;
        cin >> x;
        insertKey(x);
    }

    int q;
    cin >> q;

    while (q--) {
        ll a, b;
        cin >> a >> b;

        bool first = true;
        enumerate(root, a, b, first);

        if (first) {
            cout << "EMPTY";
        }

        cout << '\n';
    }

    return 0;
}
