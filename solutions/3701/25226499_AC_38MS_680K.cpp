// POJ 3701 - Rubik's Cube: 0-1 Version
// Model: gpt-6-astra
// Construct moves using separate corner and edge pattern searches.
// Face 3 is the top; face 1 is the front, as in the figures.
// Read the goal literally: every 1 must end on face 1.
// Middle slices position the center. Outer turns solve marked corners.
// Four 18-turn macros fix all corners and span marked-edge patterns.
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
const int perm[18][54] = {
{11,14,17,3,4,5,6,7,8,9,10,53,12,13,52,15,16,51,20,23,26,19,22,25,18,21,24,2,28,29,1,31,32,0,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,27,30,33},
{33,30,27,3,4,5,6,7,8,9,10,0,12,13,1,15,16,2,24,21,18,25,22,19,26,23,20,51,28,29,52,31,32,53,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,17,14,11},
{0,1,2,10,13,16,6,7,8,9,50,11,12,49,14,15,48,17,18,19,20,21,22,23,24,25,26,27,5,29,30,4,32,33,3,35,36,37,38,39,40,41,42,43,44,45,46,47,28,31,34,51,52,53},
{0,1,2,34,31,28,6,7,8,9,3,11,12,4,14,15,5,17,18,19,20,21,22,23,24,25,26,27,48,29,30,49,32,33,50,35,36,37,38,39,40,41,42,43,44,45,46,47,16,13,10,51,52,53},
{0,1,2,3,4,5,9,12,15,47,10,11,46,13,14,45,16,17,18,19,20,21,22,23,24,25,26,27,28,8,30,31,7,33,34,6,42,39,36,43,40,37,44,41,38,29,32,35,48,49,50,51,52,53},
{0,1,2,3,4,5,35,32,29,6,10,11,7,13,14,8,16,17,18,19,20,21,22,23,24,25,26,27,28,45,30,31,46,33,34,47,38,41,44,37,40,43,36,39,42,15,12,9,48,49,50,51,52,53},
{18,1,2,21,4,5,24,7,8,15,12,9,16,13,10,17,14,11,45,19,20,48,22,23,51,25,26,27,28,29,30,31,32,33,34,35,36,37,6,39,40,3,42,43,0,44,46,47,41,49,50,38,52,53},
{44,1,2,41,4,5,38,7,8,11,14,17,10,13,16,9,12,15,0,19,20,3,22,23,6,25,26,27,28,29,30,31,32,33,34,35,36,37,51,39,40,48,42,43,45,18,46,47,21,49,50,24,52,53},
{0,19,2,3,22,5,6,25,8,9,10,11,12,13,14,15,16,17,18,46,20,21,49,23,24,52,26,27,28,29,30,31,32,33,34,35,36,7,38,39,4,41,42,1,44,45,43,47,48,40,50,51,37,53},
{0,43,2,3,40,5,6,37,8,9,10,11,12,13,14,15,16,17,18,1,20,21,4,23,24,7,26,27,28,29,30,31,32,33,34,35,36,52,38,39,49,41,42,46,44,45,19,47,48,22,50,51,25,53},
{0,1,20,3,4,23,6,7,26,9,10,11,12,13,14,15,16,17,18,19,47,21,22,50,24,25,53,29,32,35,28,31,34,27,30,33,8,37,38,5,40,41,2,43,44,45,46,42,48,49,39,51,52,36},
{0,1,42,3,4,39,6,7,36,9,10,11,12,13,14,15,16,17,18,19,2,21,22,5,24,25,8,33,30,27,34,31,28,35,32,29,53,37,38,50,40,41,47,43,44,45,46,20,48,49,23,51,52,26},
{0,1,2,3,4,5,6,7,8,36,37,38,12,13,14,15,16,17,9,10,11,21,22,23,24,25,26,18,19,20,30,31,32,33,34,35,27,28,29,39,40,41,42,43,44,47,50,53,46,49,52,45,48,51},
{0,1,2,3,4,5,6,7,8,18,19,20,12,13,14,15,16,17,27,28,29,21,22,23,24,25,26,36,37,38,30,31,32,33,34,35,9,10,11,39,40,41,42,43,44,51,48,45,52,49,46,53,50,47},
{0,1,2,3,4,5,6,7,8,9,10,11,39,40,41,15,16,17,18,19,20,12,13,14,24,25,26,27,28,29,21,22,23,33,34,35,36,37,38,30,31,32,42,43,44,45,46,47,48,49,50,51,52,53},
{0,1,2,3,4,5,6,7,8,9,10,11,21,22,23,15,16,17,18,19,20,30,31,32,24,25,26,27,28,29,39,40,41,33,34,35,36,37,38,12,13,14,42,43,44,45,46,47,48,49,50,51,52,53},
{6,3,0,7,4,1,8,5,2,9,10,11,12,13,14,42,43,44,18,19,20,21,22,23,15,16,17,27,28,29,30,31,32,24,25,26,36,37,38,39,40,41,33,34,35,45,46,47,48,49,50,51,52,53},
{2,5,8,1,4,7,0,3,6,9,10,11,12,13,14,24,25,26,18,19,20,21,22,23,33,34,35,27,28,29,30,31,32,42,43,44,36,37,38,39,40,41,15,16,17,45,46,47,48,49,50,51,52,53}};
const int corners[24]={0,2,6,8,9,11,15,17,18,20,24,26,27,29,33,35,36,38,42,44,45,47,51,53};
const int edges[24]={1,3,5,7,10,12,14,16,19,21,23,25,28,30,32,34,37,39,41,43,46,48,50,52};
int a[54];
vector<int> answer;
void turn(int m) {
    int b[54];
    for(int i=0;i<54;++i)b[perm[m][i]]=a[i];
    for(int i=0;i<54;++i)a[i]=b[i];
    answer.push_back(m+1);
}
struct Node { unsigned mask; int parent,move; Node(unsigned x,int p,int m):mask(x),parent(p),move(m){} };
bool solve(const int *pos, const vector<vector<int> >& moves) {
    vector<vector<int> > trans(moves.size(),vector<int>(24));
    for(unsigned m=0;m<moves.size();++m)for(int i=0;i<24;++i) {
        int p=pos[i];
        for(unsigned k=0;k<moves[m].size();++k)p=perm[moves[m][k]][p];
        trans[m][i]=int(find(pos,pos+24,p)-pos);
    }
    unsigned start=0,target=0;
    for(int i=0;i<24;++i) {
        if(a[pos[i]])start|=1u<<i;
        if(pos[i]<9)target|=1u<<i;
    }
    vector<Node> q; map<unsigned,int> seen;
    q.push_back(Node(start,-1,-1));seen[start]=0;
    for(unsigned head=0;head<q.size();++head) {
        unsigned state=q[head].mask;
        if((state & ~target)==0) {
            vector<int> path;
            for(int x=int(head);q[x].parent!=-1;x=q[x].parent)path.push_back(q[x].move);
            reverse(path.begin(),path.end());
            for(unsigned k=0;k<path.size();++k)
                for(unsigned j=0;j<moves[path[k]].size();++j)turn(moves[path[k]][j]);
            return true;
        }
        for(unsigned m=0;m<moves.size();++m) {
            unsigned next=0;
            for(int i=0;i<24;++i)if(state&(1u<<i))next|=1u<<trans[m][i];
            if(seen.find(next)==seen.end()) {
                seen[next]=int(q.size());q.push_back(Node(next,int(head),int(m)));
            }
        }
    }
    return false;
}
int main() {
    vector<int> input;int ch;
    while((ch=getchar())!=EOF)if(ch=='0'||ch=='1')input.push_back(ch-'0');
    if(input.size()!=54)return 0;
    int t=0;
    for(int i=0;i<9;++i)a[45+i]=input[t++];
    for(int r=0;r<3;++r)for(int f=1;f<=4;++f)for(int c=0;c<3;++c)a[9*f+3*r+c]=input[t++];
    for(int i=0;i<9;++i)a[i]=input[t++];
    int center=-1,ncenter=0,ncorner=0,nedge=0;
    for(int f=0;f<6;++f)if(a[9*f+4]){center=9*f+4;++ncenter;}
    for(int i=0;i<24;++i){ncorner+=a[corners[i]];nedge+=a[edges[i]];}
    if(ncenter>1||ncorner>4||nedge>4){puts("no");return 0;}
    if(a[0]+a[17]+a[24]>1){puts("no");return 0;}
    if(a[2]+a[26]+a[33]>1){puts("no");return 0;}
    if(a[6]+a[15]+a[44]>1){puts("no");return 0;}
    if(a[8]+a[35]+a[42]>1){puts("no");return 0;}
    if(a[9]+a[38]+a[45]>1){puts("no");return 0;}
    if(a[11]+a[18]+a[51]>1){puts("no");return 0;}
    if(a[20]+a[27]+a[53]>1){puts("no");return 0;}
    if(a[29]+a[36]+a[47]>1){puts("no");return 0;}
    if(a[1]+a[25]>1){puts("no");return 0;}
    if(a[3]+a[16]>1){puts("no");return 0;}
    if(a[5]+a[34]>1){puts("no");return 0;}
    if(a[7]+a[43]>1){puts("no");return 0;}
    if(a[10]+a[48]>1){puts("no");return 0;}
    if(a[12]+a[41]>1){puts("no");return 0;}
    if(a[14]+a[21]>1){puts("no");return 0;}
    if(a[19]+a[52]>1){puts("no");return 0;}
    if(a[23]+a[30]>1){puts("no");return 0;}
    if(a[28]+a[50]>1){puts("no");return 0;}
    if(a[32]+a[39]>1){puts("no");return 0;}
    if(a[37]+a[46]>1){puts("no");return 0;}
    if(center!=-1 && center!=4) {
        int pre[54],pm[54],q[54],h=0,n=1;q[0]=center;
        fill(pre,pre+54,-2);pre[center]=-1;
        const int mid[6]={2,3,8,9,14,15};
        while(h<n && pre[4]==-2) {
            int x=q[h++];
            for(int k=0;k<6;++k){int y=perm[mid[k]][x];if(pre[y]==-2){pre[y]=x;pm[y]=mid[k];q[n++]=y;}}
        }
        vector<int> path;
        for(int x=4;pre[x]!=-1;x=pre[x])path.push_back(pm[x]);
        reverse(path.begin(),path.end());for(unsigned k=0;k<path.size();++k)turn(path[k]);
    }
    vector<vector<int> > cm,em;
    const int outer[12]={0,1,4,5,6,7,10,11,12,13,16,17};
    for(int k=0;k<12;++k)cm.push_back(vector<int>(1,outer[k]));
    const int second[4]={6,11,13,16};
    for(int k=0;k<4;++k) {
        vector<int> v;for(int j=0;j<9;++j){v.push_back(0);v.push_back(second[k]);}
        em.push_back(v);
    }
    if(!solve(corners,cm)||!solve(edges,em)){puts("no");return 0;}
    for(int i=9;i<54;++i)if(a[i]){puts("no");return 0;}
    printf("yes\n%d\n",int(answer.size()));
    for(unsigned k=0;k<answer.size();++k)printf("%d\n",answer[k]);
    return 0;
}
