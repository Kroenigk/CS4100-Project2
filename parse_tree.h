#ifndef PARSE_TREE_H
#define PARSE_TREE_H
#include <map>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>
#include "tree_node.h"

using namespace std;

static std::string clean_quotes(const char* raw) {
    std::string s(raw);
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

class integer_expression {
public:
    virtual int evaluate_expression(map<string, int> &sym_tab) = 0;
};

class boolean_expression {
public:
    virtual bool evaluate_expression(map<string, int> &sym_tab) = 0;
};

class string_expression {
public:
    virtual string evaluate_expression(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) = 0;
};

class int_constant : public integer_expression {
public:
    int_constant(int val) { saved_val = val; }
    virtual int evaluate_expression(map<string, int> &sym_tab) { return saved_val; }

private:
    int saved_val;
};

class variable: public integer_expression {
public:
    variable(char *in_val) { saved_val = in_val; }
    virtual int evaluate_expression(map<string, int> &sym_tab) {
        map<string, int>::iterator p;
        p = sym_tab.find(saved_val);
        if (p != sym_tab.end()) {
            return p->second;
        } else {
            return -1;
        }
    }

private:
    string saved_val;
};

class neg_constant: public integer_expression {
public:
    neg_constant(integer_expression *ptr) { eval_ptr = ptr; }
    virtual int evaluate_expression(map<string, int> &sym_tab) { return -eval_ptr->evaluate_expression(sym_tab); }

    integer_expression *eval_ptr;
};

class plus_expr: public integer_expression {
public:
    plus_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual int evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class minus_expr: public integer_expression {
public:
    minus_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual int evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) - r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class mult_expr: public integer_expression {
public:
    mult_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual int evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) * r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class div_expr: public integer_expression {
public:
    div_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual int evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) / r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class mod_expr: public integer_expression {
public:
    mod_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual int evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) % r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class less_expr: public boolean_expression {
public:
    less_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual bool evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) < r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class greater_expr: public boolean_expression {
public:
    greater_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual bool evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) > r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class ge_expr: public boolean_expression {
public:
    ge_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual bool evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) >= r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class le_expr: public boolean_expression {
public:
    le_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual bool evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) <= r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class ee_expr: public boolean_expression {
public:
    ee_expr(integer_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }
    virtual bool evaluate_expression(map<string, int> &sym_tab) {
        return l->evaluate_expression(sym_tab) == r->evaluate_expression(sym_tab);
    }

private:
    integer_expression *l;
    integer_expression *r;
};

class statement {
public:
    virtual void evaluate_statement(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab ) = 0;
};

class built_tree_store {
public:
    static map<string, tree*> &nodes() {
        static map<string, tree*> all_nodes;
        return all_nodes;
    }

    static tree *get_or_create(const string &name) {
        map<string, tree*> &all_nodes = nodes();
        map<string, tree*>::iterator p = all_nodes.find(name);
        if (p != all_nodes.end()) {
            return p->second;
        }
        tree *new_node = new tree();
        new_node->set_name(name);
        all_nodes[name] = new_node;
        return new_node;
    }
};

class compound_statement: public statement {
public:
    compound_statement(statement *first, compound_statement *rest) {
        f = first;
        r = rest;
    }
    virtual void evaluate_statement(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) {
        if (f != NULL) {
            f->evaluate_statement(int_sym_tab, str_sym_tab);
        }
        if (r != NULL) {
            r->evaluate_statement(int_sym_tab, str_sym_tab);
        }
    }

private:
    compound_statement *r;
    statement *f;
};

class string_list_node {
public:
    string_list_node(const char* val, string_list_node* next = nullptr) {
        value = clean_quotes(val);
        rest = next;
    }

    // Getter for value
    const string& get_value() const {
        return value;
    }

    /// Getter for the next node
    string_list_node* get_next() const {
        return rest;
    }
    
private:
    string value;
    string_list_node* rest;
};

class for_statement: public statement {
public:
    // constructor for integer iteration
    for_statement(char* var, int start, int end, compound_statement* b) 
        : var_name(var), start_val(start), end_val(end), body(b), str_vals(nullptr) {}
    // constructor for string loop
    for_statement(char* var, string_list_node* values, compound_statement* b) 
        : var_name(var), start_val(0), end_val(0), body(b), str_vals(values) {}

    virtual void evaluate_statement(map<string,int>& int_sym_tab, map<string,string>& str_sym_tab) override {
        if (str_vals != nullptr) {
            for (string_list_node *i = str_vals; i != nullptr; i = i->get_next()) {
                str_sym_tab[var_name] = i->get_value();

                if (body != nullptr) {
                    body->evaluate_statement(int_sym_tab, str_sym_tab);
                }
            }   
        } else {
            for (int i = start_val; i <= end_val; i++) {
                int_sym_tab[var_name] = i;

                if (body != nullptr) {
                    body->evaluate_statement(int_sym_tab, str_sym_tab);
                }
            }
        }
    }

private:
    string var_name;
    int start_val;
    int end_val;
    compound_statement *body;
    string_list_node *str_vals;
};

class assignment_statement: public statement {
public:
    assignment_statement(char *id, integer_expression *rhs) {
        ident = id;
        int_r_side = rhs;
        str_r_side = nullptr;
    }
    assignment_statement(char *id, string_expression *rhs) {
        ident = id;
        int_r_side = nullptr;
        str_r_side = rhs;
    }
    virtual void evaluate_statement(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) {
        if (int_r_side != nullptr) {
            int_sym_tab[ident] = int_r_side->evaluate_expression(int_sym_tab);
        } else if (str_r_side != nullptr) {
            str_sym_tab[ident] = str_r_side->evaluate_expression(int_sym_tab, str_sym_tab);
        }
    }

private:
    string ident;
    integer_expression *int_r_side;
    string_expression *str_r_side;
};

class print_statement: public statement {
public:
    print_statement(string_expression *expr) {
        e = expr;
    }
    virtual void evaluate_statement(map<string, int> &int_sym_tab, map<string,string>& str_sym_tab) {
        string requested_name = e->evaluate_expression(int_sym_tab, str_sym_tab);
        map<string, tree*> &all_nodes = built_tree_store::nodes();
        map<string, tree*>::iterator p = all_nodes.find(requested_name);
        if (p != all_nodes.end() && p->second != nullptr) {
            p->second->print_tree();
            cout << endl;
            return;
        }
        cout << requested_name << endl;
    }

private:
    string_expression *e;
};

class build_node_statement: public statement {
public:
    build_node_statement(compound_statement *body_in) {
        body = body_in;
    }

    virtual void evaluate_statement(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) {
        str_sym_tab["name"] = "";
        str_sym_tab["isachildof"] = "";
        int_sym_tab["weight"] = 0;

        if (body != nullptr) {
            body->evaluate_statement(int_sym_tab, str_sym_tab);
        }

        string node_name = str_sym_tab["name"];
        if (node_name.empty()) {
            return;
        }

        int node_weight = int_sym_tab["weight"];
        string parent_name = str_sym_tab["isachildof"];

        tree *node = built_tree_store::get_or_create(node_name);
        node->set_name(node_name);
        node->set_weight(node_weight);

        if (!parent_name.empty()) {
            tree *parent = built_tree_store::get_or_create(parent_name);
            vector<tree*> children = parent->get_children();
            bool already_linked = false;
            for (size_t i = 0; i < children.size(); i++) {
                if (children[i] == node) {
                    already_linked = true;
                    break;
                }
            }
            if (!already_linked) {
                parent->add_child(node);
            }
        }
    }

private:
    compound_statement *body;
};

class str_lit_expr: public string_expression {
public:
    str_lit_expr(char *in_val) {
        saved_val = clean_quotes(in_val);
        if (saved_val.size() >= 2 && saved_val.front() == '"' && saved_val.back() == '"') {
            saved_val = saved_val.substr(1, saved_val.size() - 2);
        }
    }

    virtual string evaluate_expression(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) {
        (void)int_sym_tab;
        (void)str_sym_tab;
        return saved_val;
    }

private:
    string saved_val;
};

class str_var_expr: public string_expression {
public:
    str_var_expr(char *in_val) {
        ident = in_val;
    }

    virtual string evaluate_expression(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) {
        (void)int_sym_tab;
        map<string, string>::iterator p = str_sym_tab.find(ident);
        if (p != str_sym_tab.end()) {
            return p->second;
        }
        return "";
    }

private:
    string ident;
};

class str_str_expr: public string_expression {
public:
    str_str_expr(string_expression *left, string_expression *right) {
        l = left;
        r = right;
    }

    virtual string evaluate_expression(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) {
        return l->evaluate_expression(int_sym_tab, str_sym_tab) + r->evaluate_expression(int_sym_tab, str_sym_tab);
    }

private:
    string_expression *l;
    string_expression *r;
};
class str_int_expr: public string_expression {
public:
    str_int_expr(string_expression *left, integer_expression *right) {
        l = left;
        r = right;
    }

    virtual string evaluate_expression(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) {
        return l->evaluate_expression(int_sym_tab, str_sym_tab) + to_string(r->evaluate_expression(int_sym_tab));
    }

private:
    string_expression *l;
    integer_expression *r;
};
class int_str_expr: public string_expression {
public:
    int_str_expr(integer_expression *left, string_expression *right) {
        l = left;
        r = right;
    }

    virtual string evaluate_expression(map<string, int> &int_sym_tab, map<string, string> &str_sym_tab) {
        return to_string(l->evaluate_expression(int_sym_tab)) + r->evaluate_expression(int_sym_tab, str_sym_tab);
    }

private:
    integer_expression *l;
    string_expression *r;
};

#endif
