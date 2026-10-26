#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int MAX_KEYS, MIN_KEYS;

struct BTreeNode {
    vector<ll> keys;
    vector<BTreeNode*> children;

    BTreeNode() {
        children.assign(MAX_KEYS + 1, nullptr);
    }

    bool isLeaf() {
        return children[0] == nullptr;
    }
};

BTreeNode* root = nullptr;

// Search for a key in the B-tree
bool search(BTreeNode* node, ll key) {
    if (node == nullptr) {
        return false;
    }

    int i = lower_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();

    if (i < (int)node->keys.size() && node->keys[i] == key) {
        return true;
    }

    if (node->isLeaf()) {
        return false;
    }

    return search(node->children[i], key);
}

// Split a full child and promote its middle key
void splitChild(BTreeNode* parent, int idx) {
    BTreeNode* y = parent->children[idx];
    BTreeNode* z = new BTreeNode();

    ll middle = y->keys[MIN_KEYS];

    z->keys.assign(y->keys.begin() + MIN_KEYS + 1, y->keys.end());

    y->keys.resize(MIN_KEYS);

    if (!y->isLeaf()) {
        for (int j = 0; j <= MIN_KEYS; j++) {
            z->children[j] = y->children[MIN_KEYS + 1 + j];
            y->children[MIN_KEYS + 1 + j] = nullptr;
        }
    }

    int oldSize = parent->keys.size();

    for (int j = oldSize; j >= idx + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[idx + 1] = z;

    parent->keys.insert(parent->keys.begin() + idx, middle);
}

// Insert into a node that is not full
void insertNonFull(BTreeNode* node, ll key) {
    int i = (int)node->keys.size() - 1;

    if (node->isLeaf()) {
        node->keys.insert(upper_bound(node->keys.begin(), node->keys.end(), key), key);
        return;
    }

    while (i >= 0 && key < node->keys[i]) {
        i--;
    }

    i++;

    if ((int)node->children[i]->keys.size() == MAX_KEYS) {
        splitChild(node, i);

        if (key > node->keys[i]) {
            i++;
        }
    }

    insertNonFull(node->children[i], key);
}

// Top-down insertion
void insertKey(ll key) {
    if (search(root, key)) {
        return;
    }

    if (root == nullptr) {
        root = new BTreeNode();
        root->keys.push_back(key);
        return;
    }

    if ((int)root->keys.size() == MAX_KEYS) {
        BTreeNode* newRoot = new BTreeNode();
        newRoot->children[0] = root;

        splitChild(newRoot, 0);
        root = newRoot;
    }

    insertNonFull(root, key);
}

// Get the predecessor of a key in an internal node
ll getPredecessor(BTreeNode* node, int idx) {
    BTreeNode* cur = node->children[idx];

    while (!cur->isLeaf()) {
        cur = cur->children[cur->keys.size()];
    }

    return cur->keys.back();
}

// Get the successor of a key in an internal node
ll getSuccessor(BTreeNode* node, int idx) {
    BTreeNode* cur = node->children[idx + 1];

    while (!cur->isLeaf()) {
        cur = cur->children[0];
    }

    return cur->keys[0];
}

// Borrow a key from the left sibling
void borrowFromPrev(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx - 1];

    child->keys.insert(child->keys.begin(), node->keys[idx - 1]);

    node->keys[idx - 1] = sibling->keys.back();
    sibling->keys.pop_back();

    if (!sibling->isLeaf()) {
        int oldSize = child->keys.size() - 1;

        for (int j = oldSize; j >= 0; j--) {
            child->children[j + 1] = child->children[j];
        }

        child->children[0] = sibling->children[sibling->keys.size() + 1];

        sibling->children[sibling->keys.size() + 1] = nullptr;
    }
}

// Borrow a key from the right sibling
void borrowFromNext(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];

    int oldSize = sibling->keys.size();

    child->keys.push_back(node->keys[idx]);
    node->keys[idx] = sibling->keys[0];

    sibling->keys.erase(sibling->keys.begin());

    if (!sibling->isLeaf()) {
        child->children[child->keys.size()] = sibling->children[0];

        for (int j = 0; j < oldSize; j++) {
            sibling->children[j] = sibling->children[j + 1];
        }

        sibling->children[oldSize] = nullptr;
    }
}

// Merge child idx with its right sibling
void mergeChildren(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];

    int oldSize = child->keys.size();

    child->keys.push_back(node->keys[idx]);

    for (ll key : sibling->keys) {
        child->keys.push_back(key);
    }

    if (!sibling->isLeaf()) {
        for (int j = 0; j <= (int)sibling->keys.size(); j++) {
            child->children[oldSize + 1 + j] = sibling->children[j];
        }
    }

    node->keys.erase(node->keys.begin() + idx);

    int parentSize = node->keys.size();

    for (int j = idx + 1; j <= parentSize; j++) {
        node->children[j] = node->children[j + 1];
    }

    node->children[parentSize + 1] = nullptr;

    delete sibling;
}

// Ensure child idx has more than MIN_KEYS before descending
void fillChild(BTreeNode* node, int idx) {
    if (idx > 0 && (int)node->children[idx - 1]->keys.size() > MIN_KEYS) {
        borrowFromPrev(node, idx);
    } else if (idx < (int)node->keys.size() && (int)node->children[idx + 1]->keys.size() > MIN_KEYS) {
        borrowFromNext(node, idx);
    } else if (idx < (int)node->keys.size()) {
        mergeChildren(node, idx);
    } else {
        mergeChildren(node, idx - 1);
    }
}

// Delete a key from a subtree where top-down rules are followed
void removeKey(BTreeNode* node, ll key) {
    int idx = lower_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();

    if (idx < (int)node->keys.size() && node->keys[idx] == key) {

        if (node->isLeaf()) {
            node->keys.erase(node->keys.begin() + idx);
            return;
        }

        BTreeNode* left = node->children[idx];
        BTreeNode* right = node->children[idx + 1];

        if ((int)left->keys.size() > MIN_KEYS) {
            ll pred = getPredecessor(node, idx);
            node->keys[idx] = pred;
            removeKey(left, pred);
        } else if ((int)right->keys.size() > MIN_KEYS) {
            ll succ = getSuccessor(node, idx);
            node->keys[idx] = succ;
            removeKey(right, succ);
        } else {
            mergeChildren(node, idx);
            removeKey(node->children[idx], key);
        }

        return;
    }

    if (node->isLeaf()) {
        return;
    }

    // Prepare the child before descending into it
    bool lastChild = (idx == (int)node->keys.size());

    if ((int)node->children[idx]->keys.size() == MIN_KEYS) {
        fillChild(node, idx);

        if (lastChild && idx > (int)node->keys.size()) {
            idx--;
        }
    }

    removeKey(node->children[idx], key);
}

// Top-down deletion
bool deleteKey(ll key) {
    if (!search(root, key)) {
        return false;
    }

    removeKey(root, key);

    // Shrink an empty root
    if (root->keys.empty()) {
        BTreeNode* oldRoot = root;

        if (root->isLeaf()) {
            root = nullptr;
        } else {
            root = root->children[0];
        }

        delete oldRoot;
    }

    return true;
}

// Level-order display, one level per line
void display() {
    if (root == nullptr) {
        cout << "EMPTY\nEND\n";
        return;
    }

    queue<BTreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();

        for (int i = 0; i < levelSize; i++) {
            BTreeNode* node = q.front();
            q.pop();

            if (i > 0) {
                cout << ' ';
            }

            for (int j = 0; j < (int)node->keys.size(); j++) {
                if (j > 0) {
                    cout << ',';
                }
                cout << node->keys[j];
            }

            if (!node->isLeaf()) {
                for (int j = 0; j <= (int)node->keys.size(); j++) {
                    q.push(node->children[j]);
                }
            }
        }

        cout << '\n';
    }

    cout << "END\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> MAX_KEYS >> MIN_KEYS;

    for (int i = 0; i < n; i++) {
        ll key;
        cin >> key;
        insertKey(key);
    }

    cin >> q;

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            ll x;
            cin >> x;

            if (search(root, x)) {
                cout << x << " is present\n";
            } else {
                cout << x << " is not present\n";
            }

        } else if (type == 2) {
            ll x;
            cin >> x;

            if (search(root, x)) {
                cout << x << " is already present\n";
            } else {
                insertKey(x);
                cout << x << " is inserted\n";
            }

        } else if (type == 3) {
            ll x;
            cin >> x;

            if (deleteKey(x)) {
                cout << x << " is deleted\n";
            } else {
                cout << x << " is not present. So it can not be deleted\n";
            }

        } else if (type == 4) {
            display();
        }
    }

    return 0;
}
