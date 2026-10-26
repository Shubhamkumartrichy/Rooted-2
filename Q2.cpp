#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Node {
    ll key;
    int size;
    bool red;
    Node *left, *right, *parent;

    Node(ll key, bool red) {
        this->key = key;
        this->red = red;
        size = 1;
        left = right = parent = nullptr;
    }
};

Node* NIL;
Node* root;

// Update subtree size
void update(Node* x) {
    if (x != NIL) {
        x->size = 1 + x->left->size + x->right->size;
    }
}

void updateUp(Node* x) {
    while (x != NIL) {
        update(x);
        x = x->parent;
    }
}

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

    update(x);
    update(y);
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

    update(y);
    update(x);
}

Node* search(ll key) {
    Node* cur = root;

    while (cur != NIL) {
        if (cur->key == key) {
            return cur;
        }

        if (key < cur->key) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }

    return NIL;
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

bool insertKey(ll key) {
    if (search(key) != NIL) {
        return false;
    }

    Node* z = new Node(key, true);
    z->left = z->right = NIL;

    Node* y = NIL;
    Node* x = root;

    while (x != NIL) {
        y = x;

        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
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

    updateUp(y);
    insertFix(z);

    return true;
}

// Replace one subtree with another
void transplant(Node* u, Node* v) {
    if (u->parent == NIL) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

Node* minimum(Node* x) {
    while (x->left != NIL) {
        x = x->left;
    }

    return x;
}

// Restore Red-Black properties after deletion
void deleteFix(Node* x) {
    while (x != root && !x->red) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            
            if (w->red) {
                w->red = false;
                x->parent->red = true;
                leftRotate(x->parent);
                w = x->parent->right;
            }

            if (!w->left->red && !w->right->red) {
                w->red = true;
                x = x->parent;
            } else {
                if (!w->right->red) {
                    w->left->red = false;
                    w->red = true;
                    rightRotate(w);
                    w = x->parent->right;
                }

                w->red = x->parent->red;
                x->parent->red = false;
                w->right->red = false;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            Node* w = x->parent->left;

            if (w->red) {
                w->red = false;
                x->parent->red = true;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (!w->right->red && !w->left->red) {
                w->red = true;
                x = x->parent;
            } else {
                if (!w->left->red) {
                    w->right->red = false;
                    w->red = true;
                    leftRotate(w);
                    w = x->parent->left;
                }

                w->red = x->parent->red;
                x->parent->red = false;
                w->left->red = false;
                rightRotate(x->parent);
                x = root;
            }
        }
    }

    x->red = false;
}

bool deleteKey(ll key) {
    Node* z = search(key);

    if (z == NIL) {
        return false;
    }

    Node* y = z;
    Node* x;
    bool originalRed = y->red;

    if (z->left == NIL) {
        x = z->right;
        Node* p = z->parent;

        transplant(z, z->right);
        updateUp(p);
    } else if (z->right == NIL) {
        x = z->left;
        Node* p = z->parent;

        transplant(z, z->left);
        updateUp(p);
    } else {
        y = minimum(z->right);
        originalRed = y->red;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            Node* p = y->parent;

            transplant(y, y->right);
            updateUp(p);

            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);

        y->left = z->left;
        y->left->parent = y;
        y->red = z->red;

        update(y);
        updateUp(y->parent);
    }

    delete z;

    if (!originalRed) {
        deleteFix(x);
    }

    // Rotations preserve subtree totals; refresh affected ancestors
    updateUp(x == NIL ? x->parent : x);

    return true;
}

// Return the k-th smallest key (1-based)
ll kthSmallest(int k) {
    Node* cur = root;

    while (cur != NIL) {
        int leftSize = cur->left->size;

        if (k == leftSize + 1) {
            return cur->key;
        }

        if (k <= leftSize) {
            cur = cur->left;
        } else {
            k -= leftSize + 1;
            cur = cur->right;
        }
    }

    return -1; // Type-3 queries are guaranteed valid
}

// Count keys strictly smaller than key
int rankKey(ll key) {
    int ans = 0;
    Node* cur = root;

    while (cur != NIL) {
        if (key <= cur->key) {
            cur = cur->left;
        } else {
            ans += cur->left->size + 1;
            cur = cur->right;
        }
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    NIL = new Node(0, false);
    NIL->size = 0;
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
        int type;
        ll x;
        cin >> type >> x;

        if (type == 1) {
            if (insertKey(x)) {
                cout << x << " is inserted\n";
            } else {
                cout << x << " is already present. So no need to insert\n";
            }
        } else if (type == 2) {
            if (deleteKey(x)) {
                cout << x << " is deleted\n";
            } else {
                cout << x << " is not present. So it can not be deleted\n";
            }
        } else if (type == 3) {
            cout << kthSmallest((int)x) << '\n';
        } else if (type == 4) {
            cout << rankKey(x) << '\n';
        }
    }

    return 0;
}
