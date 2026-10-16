#include <iostream>

using namespace std;

enum Color { RED, BLACK };

template <typename data_type>
class RedBlackTree {
private:
    struct Node {
        data_type data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;


        Node() : color(BLACK), left(nullptr), right(nullptr), parent(nullptr) {}


        Node(data_type val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;
    Node* nil;


    void destroy_tree(Node* u) {
        if (u != nil) {
            destroy_tree(u->left);
            destroy_tree(u->right);
            delete u;
        }
    }


    void rotate_left(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rotate_right(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nil) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }


    void insert_fixup(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == RED) {

                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {

                        z = z->parent;
                        rotate_left(z);
                    }

                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotate_right(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y->color == RED) {

                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {

                        z = z->parent;
                        rotate_right(z);
                    }

                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotate_left(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }


    void transplant(Node* u, Node* v) {
        if (u->parent == nil) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node* minimum(Node* u) {
        while (u->left != nil) {
            u = u->left;
        }
        return u;
    }

    void erase_fixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {

                    w->color = BLACK;
                    x->parent->color = RED;
                    rotate_left(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {

                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {

                        w->left->color = BLACK;
                        w->color = RED;
                        rotate_right(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotate_left(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {

                    w->color = BLACK;
                    x->parent->color = RED;
                    rotate_right(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {

                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {

                        w->right->color = BLACK;
                        w->color = RED;
                        rotate_left(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotate_right(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    Node* search_node(data_type key) {
        Node* current = root;
        while (current != nil && key != current->data) {
            if (key < current->data) current = current->left;
            else current = current->right;
        }
        return current;
    }

    void inorder_helper(Node* u) {
        if (u != nil) {
            inorder_helper(u->left);
            cout << u->data << " ";
            inorder_helper(u->right);
        }
    }

public:
    RedBlackTree() {
        nil = new Node();
        root = nil;
    }

    ~RedBlackTree() {
        destroy_tree(root);
        delete nil;
    }

    void insert(data_type key) {
        Node* z = new Node(key);
        Node* y = nil;
        Node* x = root;

        while (x != nil) {
            y = x;
            if (z->data < x->data) {
                x = x->left;
            } else if (z->data > x->data) {
                x = x->right;
            } else {
                delete z;
                return;
            }
        }

        z->parent = y;
        if (y == nil) {
            root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->left = nil;
        z->right = nil;
        z->color = RED;

        insert_fixup(z);
    }

    void erase(data_type key) {
        Node* z = search_node(key);
        if (z == nil) return;

        Node* y = z;
        Color y_original_color = y->color;
        Node* x;

        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;


        if (y_original_color == BLACK) {
            erase_fixup(x);
        }
    }

    bool search(data_type key) {
        return search_node(key) != nil;
    }

    void print_inorder() {
        inorder_helper(root);
        cout << "\n";
    }
};

int main() {

    RedBlackTree<int> rbt;

    rbt.insert(41);
    rbt.insert(38);
    rbt.insert(31);
    rbt.insert(12);
    rbt.insert(19);
    rbt.insert(8);

    cout << "Arbol luego de insertar: ";
    rbt.print_inorder();

    rbt.erase(38);
    cout << "Arbol luego de eliminar el 38: ";
    rbt.print_inorder();

    return 0;
}