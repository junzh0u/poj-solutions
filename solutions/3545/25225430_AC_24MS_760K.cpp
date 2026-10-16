// POJ 3545 - K'ak'-u-pakal and Mayan Script
// Model: gpt-6.1-sol
// Parse the composition tree. Each dimension has a minimum and a divisibility
// requirement; combine these with LCM, then draw at the minimum root size.
// Dimensions measure border-to-border distances, so adjacent blocks share
// one border. Square brackets double the distance in the group's direction.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;
struct Node {
    string glyph;
    char dir;
    vector<int> child, weight;
    int step[2], need[2];
    Node():dir(0) {step[0]=step[1]=1;need[0]=need[1]=0;}
};
vector<Node> a;
string s;
int pos=0;
int gcd(int x,int y) {return y?gcd(y,x%y):x;}
int lcm(int x,int y) {return x/gcd(x,y)*y;}
int roundUp(int n,int d) {return (n+d-1)/d*d;}
int parse() {
    int u=(int)a.size(); a.push_back(Node());
    if(isdigit(s[pos])) {
        while(pos<(int)s.size() && isdigit(s[pos])) a[u].glyph+=s[pos++];
        a[u].need[0]=(int)a[u].glyph.size()+3;
        a[u].need[1]=2;
        return u;
    }
    ++pos;
    while(true) {
        int w=1;
        if(s[pos]=='[') {w=2;++pos;}
        int v=parse();
        if(w==2) ++pos;
        a[u].child.push_back(v);a[u].weight.push_back(w);
        if(s[pos]==')') {++pos;break;}
        a[u].dir=s[pos++];
    }
    int axis=a[u].dir=='.'?0:1, total=0;
    for(int d=0;d<2;++d) {
        int step=1, low=0;
        for(int i=0;i<(int)a[u].child.size();++i) {
            int v=a[u].child[i],w=d==axis?a[u].weight[i]:1;
            step=lcm(step,a[v].step[d]/gcd(a[v].step[d],w));
            low=max(low,(a[v].need[d]+w-1)/w);
        }
        if(d==axis) {
            total=0;
            for(int i=0;i<(int)a[u].weight.size();++i) total+=a[u].weight[i];
        }
        int mul=d==axis?total:1;
        a[u].step[d]=step*mul;
        a[u].need[d]=roundUp(low,step)*mul;
    }
    return u;
}
vector<string> canvas;
void draw(int u,int x,int y,int w,int h) {
    if(a[u].dir) {
        int sum=0;
        for(int i=0;i<(int)a[u].weight.size();++i) sum+=a[u].weight[i];
        for(int i=0;i<(int)a[u].child.size();++i) {
            int cw=w,ch=h;
            if(a[u].dir=='.') cw=w/sum*a[u].weight[i];
            else ch=h/sum*a[u].weight[i];
            draw(a[u].child[i],x,y,cw,ch);
            if(a[u].dir=='.') x+=cw;else y+=ch;
        }
        return;
    }
    for(int i=x+1;i<x+w;++i) {
        if(canvas[y][i]!='+') canvas[y][i]='-';
        if(canvas[y+h][i]!='+') canvas[y+h][i]='-';
    }
    for(int j=y+1;j<y+h;++j) {
        if(canvas[j][x]!='+') canvas[j][x]='|';
        if(canvas[j][x+w]!='+') canvas[j][x+w]='|';
    }
    canvas[y][x]=canvas[y+h][x]=canvas[y][x+w]=canvas[y+h][x+w]='+';
    for(int i=0;i<(int)a[u].glyph.size();++i) canvas[y+1][x+2+i]=a[u].glyph[i];
}
int main() {
    if(!(cin>>s)) return 0;
    int root=parse(),w=a[root].need[0],h=a[root].need[1];
    canvas.assign(h+1,string(w+1,' '));
    draw(root,0,0,w,h);
    for(int i=0;i<=h;++i) cout<<canvas[i]<<'\n';
    return 0;
}
