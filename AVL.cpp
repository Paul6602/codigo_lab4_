#include <bits/stdc++.h>
using namespace std;

template<typename data_type>
struct AVL {

    struct TreeNode {
        int height;
        data_type data;
        TreeNode* left;
        TreeNode* right;

        TreeNode(data_type data = data_type(), int height = 0,
                 TreeNode* left = nullptr,
                 TreeNode* right = nullptr) :
                 data(data), height(height), left(left), right(right) {}
    };

    TreeNode* root;

    AVL() {
        root = nullptr;
    }

    ~AVL() {
        destroy_tree(root);
    }

    void destroy_tree(TreeNode* u) {
        if (u != nullptr) {
            destroy_tree(u->left);
            destroy_tree(u->right);
            delete u;
        }
    }

    void update(TreeNode* u) {
        u->height = 1 + max(h(u->left), h(u->right));
    }

    int h(TreeNode* u) {
        return u == nullptr ? -1 : u->height;
    }

    int FB(TreeNode* u) {
        if (u == nullptr) return 0;
        return h(u->left) - h(u->right);
    }

    bool search(data_type key) {
        TreeNode* current = root;
        while (current != nullptr) {
            if (current->data == key) return true;
            if (current->data > key) current = current->left;
            else current = current->right;
        }
        return false;
    }

    TreeNode* min_element(TreeNode* u) {
        if (u == nullptr) return nullptr;
        TreeNode* current = u;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    TreeNode* max_element(TreeNode* u) {
        if (u == nullptr) return nullptr;
        TreeNode* current = u;
        while (current->right != nullptr) {
            current = current->right;
        }
        return current;
    }

    TreeNode* min_element() {
        return min_element(root);
    }

    TreeNode* max_element() {
        return max_element(root);
    }

    TreeNode* rotate_left(TreeNode* u) {
        TreeNode* v = u->right;
        u->right = v->left;
        v->left = u;
        update(u);
        update(v);
        return v;
    }

    TreeNode* rotate_right(TreeNode* u) {
        TreeNode* v = u->left;
        u->left = v->right;
        v->right = u;
        update(u);
        update(v);
        return v;
    }

    TreeNode* rebalance(TreeNode* u) {
        update(u);
        if (FB(u) > 1) {
            
            if (FB(u->left) < 0) {
                
                u->left = rotate_left(u->left);
            }
            return rotate_right(u);
        }
        if (FB(u) < -1) {
            
            if (FB(u->right) > 0) {
                
                u->right = rotate_right(u->right);
            }
            return rotate_left(u);
        }
        return u;
    }

    TreeNode* insert(TreeNode* u, data_type value) {
        if (u == nullptr) {
            return new TreeNode(value, 0);
        }
        if (value < u->data) {
            u->left = insert(u->left, value);
        }
        else if (value > u->data) {
            u->right = insert(u->right, value);
        } 
        else {
        
            return u;
        }
        return rebalance(u);
    }

    void insert(data_type value) {
        root = insert(root, value);
    }

   
    TreeNode* successor(data_type key) {
        TreeNode* current = root;
        TreeNode* succ = nullptr;
        while (current != nullptr) {
            if (current->data > key) {
                succ = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return succ;
    }

    TreeNode* predecessor(data_type key) {
        TreeNode* current = root;
        TreeNode* pred = nullptr;
        while (current != nullptr) {
            if (current->data < key) {
                pred = current;
                current = current->right;
            } else {
                current = current->left;
            }
        }
        return pred;
    }

    TreeNode* erase(TreeNode* u, const data_type& value) {
        if (u == nullptr) return nullptr;

        if (value < u->data) {
            u->left = erase(u->left, value);
        }
        else if (u->data < value) {
            u->right = erase(u->right, value);
        }
        else {
            if (u->left == nullptr || u->right == nullptr) {
                TreeNode* child = (u->left != nullptr) ? u->left : u->right;
                delete u;
                return child;
            }
            TreeNode* succ = min_element(u->right);
            u->data = succ->data;
            u->right = erase(u->right, succ->data);
        }
        return rebalance(u);
    }

    void erase(data_type value) {
        root = erase(root, value);
    }

    void print_inorder() {
        print_subtree_inorder(root);
        cout << "\n";
    }

    void print_subtree_inorder(TreeNode* u) {
        if (u == nullptr) return;
        print_subtree_inorder(u->left);
        cout << u->data << " ";
        print_subtree_inorder(u->right);
    }
};

int main() {

    int n;
    if (cin >> n) {
        AVL<int> T;
        for (int i = 0; i < n; ++i) {
            int x;
            cin >> x;
            T.insert(x);
            cout << T.h(T.root) << " " << T.root->data << '\n';
        }
    }
    return 0;
}