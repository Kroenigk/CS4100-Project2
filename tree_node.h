#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class tree {
public:
    tree() {
        name = "";
        weight = 0;
        isachildof = nullptr;
    }

    tree(const string& n, int w) {
        name = n;
        weight = w;
    }

    tree(const string& n, int w, tree* p) {
        name = n;
        weight = w;
        isachildof = p;
    }

    void set_name(const string& n) {
        name = n;
    }

    void set_weight(int w) {
        weight = w;
    }

    void set_parent(tree* p) {
        isachildof = p;
    }

    string get_name() const {
        return name;
    }

    int get_weight() const {
        return weight;
    }

    tree* get_parent() const {
        return isachildof;
    }

    vector<tree*> get_children() const {
        return children;
    }

    void add_child(tree* c)
    {
        if(c != nullptr)
        {
            children.push_back(c);
            c->set_parent(this);
        }
    }

    void print_tree() {
        cout << name;
        if(!children.empty()){
            cout << " [";
            for(int i = 0; i < children.size(); ++i){
                children[i]->print_tree();
                if (i != children.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "]";
        }

    }

private:
    string name;
    int weight;
    tree* isachildof;
    vector<tree*> children;
};


#endif
