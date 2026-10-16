// POJ 3948 - Common Polynomial
// Model: gpt-6.1-sol
// Parse the expression grammar into coefficient vectors. Primitive pseudo-
// remainders implement Euclid over Q[x], removing integer content after each
// cancellation. Normalize the result to positive leading coefficient.
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
using namespace std;
typedef long long Int;
typedef vector<Int> Poly;
Int mag(Int x) { return x<0?-x:x; }
Int gcdnum(Int a,Int b) { a=mag(a); b=mag(b); while(b) { Int t=a%b; a=b; b=t; } return a; }
void trim(Poly &a) { while(!a.empty() && a.back()==0) a.pop_back(); }
void primitive(Poly &a) { trim(a); Int g=0; for(size_t i=0;i<a.size();++i) g=gcdnum(g,a[i]); if(!g) return; if(a.back()<0) g=-g; for(size_t i=0;i<a.size();++i) a[i]/=g; }
Poly add(Poly a,const Poly &b,int sign) { a.resize(max(a.size(),b.size()),0); for(size_t i=0;i<b.size();++i) a[i]+=sign*b[i]; trim(a); return a; }
Poly mul(const Poly &a,const Poly &b) { if(a.empty()||b.empty()) return Poly(); Poly c(a.size()+b.size()-1,0); for(size_t i=0;i<a.size();++i) for(size_t j=0;j<b.size();++j) c[i+j]+=a[i]*b[j]; trim(c); return c; }
struct Parser {
 string s; size_t p;
 Parser(string t):s(t),p(0) {}
 Poly expr() { int sign=1; if(p<s.size()&&s[p]=='-') { sign=-1; ++p; } Poly a=term(); if(sign<0) for(size_t i=0;i<a.size();++i) a[i]=-a[i]; while(p<s.size()&&(s[p]=='+'||s[p]=='-')) { sign=s[p++]=='+'?1:-1; a=add(a,term(),sign); } return a; }
 Poly term() { Poly a(1,1); while(p<s.size()&&(s[p]=='x'||s[p]=='('||isdigit(s[p]))) a=mul(a,factor()); return a; }
 Poly factor() { Poly a; if(s[p]=='x') { a.resize(2,0); a[1]=1; ++p; } else if(s[p]=='(') { ++p; a=expr(); ++p; } else { Int n=0; while(p<s.size()&&isdigit(s[p])) n=n*10+s[p++]-'0'; a.push_back(n); trim(a); } if(p<s.size()&&s[p]=='^') { ++p; int n=0; while(p<s.size()&&isdigit(s[p])) n=n*10+s[p++]-'0'; Poly b(1,1); while(n) { if(n&1) b=mul(b,a); n/=2; if(n) a=mul(a,a); } a=b; } return a; }
};
Poly common(Poly a,Poly b) { primitive(a); primitive(b); while(!b.empty()) { while(a.size()>=b.size()&&!a.empty()) { size_t shift=a.size()-b.size(); Int x=a.back(),y=b.back(),g=gcdnum(x,y); x/=g; y/=g; for(size_t i=0;i<a.size();++i) a[i]*=y; for(size_t i=0;i<b.size();++i) a[i+shift]-=x*b[i]; primitive(a); } a.swap(b); } primitive(a); return a; }
void print(const Poly &a) { bool first=true; for(int i=(int)a.size()-1;i>=0;--i) if(a[i]) { if(a[i]<0) cout<<'-'; else if(!first) cout<<'+'; Int c=mag(a[i]); if(c!=1||i==0) cout<<c; if(i) { cout<<'x'; if(i>1) cout<<'^'<<i; } first=false; } if(first) cout<<0; cout<<'\n'; }
int main() { string s,t; while(getline(cin,s)) { s.erase(remove_if(s.begin(),s.end(),::isspace),s.end()); if(s==".") break; if(s.empty()) continue; if(!getline(cin,t)) break; t.erase(remove_if(t.begin(),t.end(),::isspace),t.end()); Parser a(s),b(t); print(common(a.expr(),b.expr())); } }
