#include "../ref/template.hpp"

struct node {
    vector<struct node*> children;
    int cnt;
    node(int alphabets) {
        children.resize(alphabets, nullptr);
        cnt = 0;
    }
};
class Trie {
    node* root;
    int total_nodes;
    int alphabets;
    char base_char;
 
   public:
    node* create_node() {
        node* new_node = new node(this->alphabets);
        return new_node;
    }
    Trie(int alphabets = 26, char base_char = 'a') {
        this->total_nodes = 0;
        this->alphabets = alphabets;
        this->root = create_node();
        this->base_char = base_char;
    }
    void insert(string& str) {
        node* ptr = this->root;
        ptr->cnt++;
        for (auto it : str) {
            int p = it - this->base_char;
            if (ptr->children[p] == nullptr) {
                ptr->children[p] = create_node();
                this->total_nodes++;
            }
            ptr = ptr->children[p];
            ptr->cnt++;
        }
    }
    bool isleaf(node* ptr) {
        int c = 0;
        for (int i = 0; i < this->alphabets; i++) {
            if (ptr->children[i] != nullptr)
                return false;
            c += ptr->children[i]->cnt;
        }
        if (c < ptr->cnt)
            return true;
        else {
            return false;
        }
    }
    bool search(string& str) {
        node* ptr = this->root;
        for (auto it : str) {
            int p = it - this->base_char;
            if (ptr->children[p] == nullptr)
                return false;
            ptr = ptr->children[p];
        }
        if (this->isleaf(ptr))
            return true;
        return false;
    }
    void insert(int x, int bits, int reverse = true) {
        string str;
        if (reverse) {
            for (int i = bits - 1; i >= 0; i--) {
                if ((x & (1ll << i)) > 0) {
                    str.pb('1');
                } else
                    str.pb('0');
            }
        } else {
            for (int i = 0; i < bits; i++) {
                if ((x & (1ll << i) > 0)) {
                    str.pb('1');
                } else
                    str.pb('0');
            }
        }
        this->insert(str);
    }
    int minimum(int x, int bits) {
        node* ptr = this->root;
        int ans = 0;
        for (int i = bits - 1; i >= 0; i--) {
            if ((x & (1ll << i)) > 0) {
                if (ptr->children[1] != nullptr) {
                    ptr = ptr->children[1];
                } else {
                    ans += (1ll << i);
                    ptr = ptr->children[0];
                }
 
            } else {
                if (ptr->children[0] != nullptr) {
                    ptr = ptr->children[0];
                } else {
                    ans += (1ll << i);
                    ptr = ptr->children[1];
                }
            }
        }
        return ans;
    }
    int maximum(int x, int bits) {
        node* ptr = this->root;
        int ans = 0;
        for (int i = bits - 1; i >= 0; i--) {
            if ((x & (1ll << i)) > 0) {
                if (ptr->children[0] != nullptr) {
                    ans += (1ll << i);
                    ptr = ptr->children[0];
                } else {
                    ptr = ptr->children[1];
                }
 
            } else {
                if (ptr->children[1] != nullptr) {
                    ans += (1ll << i);
                    ptr = ptr->children[1];
                } else {
                    ptr = ptr->children[0];
                }
            }
        }
        return ans;
    }
};