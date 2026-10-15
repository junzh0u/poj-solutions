// POJ 2911 - Simplified lambda-evaluations
// Model: gpt-6.1-sol
// Parse the prefix syntax into an expression tree and evaluate function first.
// Beta substitution skips shadowing binders, deliberately permits capture,
// and copies the unevaluated argument. Lambda bodies are not evaluated.
// Count beta applications only; allow 1000 and reject the 1001st.
#include <iostream>
#include <string>
using namespace std;
struct Node {
    char kind, name;
    Node *a, *b;
    Node(char k, char n=0, Node* x=0, Node* y=0):kind(k),name(n),a(x),b(y){}
    ~Node(){delete a; delete b;}
};
Node* parse(const string& s, int& p) {
    char c=s[p++];
    if(c=='L') {char v=s[p++]; ++p; return new Node('L',v,parse(s,p));}
    if(c=='(') {Node* a=parse(s,p); ++p; Node* b=parse(s,p); return new Node('A',0,a,b);}
    return new Node('V',c);
}
Node* copyTree(Node* t) {
    if(!t) return 0;
    return new Node(t->kind,t->name,copyTree(t->a),copyTree(t->b));
}
void substitute(Node*& t,char v,Node* arg) {
    if(t->kind=='V') {if(t->name==v){delete t;t=copyTree(arg);} return;}
    if(t->kind=='L' && t->name==v) return;
    substitute(t->a,v,arg);
    if(t->b) substitute(t->b,v,arg);
}
int applications;
void evaluate(Node*& t) {
    if(t->kind!='A') return;
    evaluate(t->a);
    if(t->a->kind=='L') {
        if(++applications>1000) throw 1;
        Node* body=t->a->a;
        t->a->a=0;
        substitute(body,t->a->name,t->b);
        delete t;
        t=body;
        evaluate(t);
    } else evaluate(t->b);
}
void print(Node* t) {
    if(t->kind=='V') cout<<t->name;
    else if(t->kind=='L'){cout<<'L'<<t->name<<'.';print(t->a);}
    else {cout<<'(';print(t->a);cout<<')';print(t->b);}
}
int main(){
    string s;
    while(cin>>s){int p=0;Node* t=parse(s,p);applications=0;
        try{evaluate(t);print(t);cout<<'\n';}
        catch(int){cout<<"unterminated\n";}
        delete t;
        if(s=="z")break;
    }
    return 0;
}
