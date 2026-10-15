// POJ 1615 - Merging Sequences Problem
// Model: gpt-6.1-sol
// Split every sequence at a minimum prefix. Schedule its nonpositive
// blocks greedily by required headroom; handle the positive tails in
// reverse with negated values. No tie-breaking assumption is needed.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Block { int height, sum; Block(int h,int s):height(h),sum(s){} };
int requirement(const vector< vector<int> >& seq) {
    vector< vector<Block> > blocks(seq.size());
    for (unsigned i=0;i<seq.size();++i) {
        int sum=0, height=0;
        for (unsigned j=0;j<seq[i].size();++j) {
            sum+=seq[i][j]; height=max(height,sum);
            if (sum<=0) { blocks[i].push_back(Block(height,sum)); sum=height=0; }
        }
    }
    vector<unsigned> pos(seq.size(),0);
    int current=0, answer=0;
    for (;;) {
        int best=-1;
        for (unsigned i=0;i<seq.size();++i)
            if (pos[i]<blocks[i].size() &&
                (best<0 || blocks[i][pos[i]].height<blocks[best][pos[best]].height)) best=i;
        if (best<0) break;
        Block b=blocks[best][pos[best]++];
        answer=max(answer,current+b.height); current+=b.sum;
    }
    return answer;
}
int main() {
    int cases; if(scanf("%d",&cases)!=1) return 0;
    while(cases--) {
        vector< vector<int> > seq(1); int x,total=0;
        while(scanf("%d",&x)==1 && x!=-9999) {
            if(x==9999) seq.push_back(vector<int>());
            else { seq.back().push_back(x); total+=x; }
        }
        vector< vector<int> > left(seq.size()),right(seq.size());
        for(unsigned i=0;i<seq.size();++i) {
            int sum=0,minimum=0,cut=0;
            for(unsigned j=0;j<seq[i].size();++j) {
                sum+=seq[i][j];
                if(sum<=minimum) {minimum=sum;cut=j+1;}
            }
            for(int j=0;j<cut;++j) left[i].push_back(seq[i][j]);
            for(int j=(int)seq[i].size()-1;j>=cut;--j) right[i].push_back(-seq[i][j]);
        }
        printf("%d\n",max(requirement(left),total+requirement(right)));
    }
    return 0;
}
