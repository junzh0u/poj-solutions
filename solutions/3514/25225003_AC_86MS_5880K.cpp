// POJ 3514 - The Writers' Club
// Model: gpt-6.1-sol
// Propagate admirer bitsets through writer vertices (Warshall closure).
// Remove original admirers and the writer himself before sorted output.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Person { char name[17]; int next; };
static Person people[100005];
static int head[262147], countPeople;
static unsigned int fans[100][3126], original[100][3126];
static int writer[100];
int identify(const char *s) {
    unsigned int h=0;
    for(int i=0;s[i];++i) h=h*131+(unsigned char)s[i];
    h%=262147;
    for(int i=head[h];i!=-1;i=people[i].next)
        if(strcmp(people[i].name,s)==0) return i;
    int i=countPeople++;
    strcpy(people[i].name,s); people[i].next=head[h]; head[h]=i;
    return i;
}
bool byName(int a,int b) {return strcmp(people[a].name,people[b].name)<0;}
int main() {
    int total,n,cas=0;
    while(scanf("%d%d",&total,&n)==2 && (total||n)) {
        memset(head,-1,sizeof(head)); countPeople=0;
        memset(fans,0,sizeof(fans)); memset(original,0,sizeof(original));
        int c; while((c=getchar())!='\n' && c!=EOF) {}
        for(int i=0;i<n;++i) {
            char name[17]; int len=0; bool first=true;
            while(true) {
                c=getchar();
                if(c>='a' && c<='z') name[len++]=(char)c;
                else {
                    if(len) {
                        name[len]=0; int id=identify(name); len=0;
                        if(first) {writer[i]=id; first=false;}
                        else fans[i][id>>5]|=1u<<(id&31);
                    }
                    if(c=='\n'||c==EOF) break;
                }
            }
        }
        int words=(countPeople+31)/32;
        memcpy(original,fans,sizeof(fans));
        for(int k=0;k<n;++k)
            for(int i=0;i<n;++i)
                if(fans[i][writer[k]>>5]&(1u<<(writer[k]&31)))
                    for(int b=0;b<words;++b) fans[i][b]|=fans[k][b];
        vector<int> order,rows;
        for(int i=0;i<countPeople;++i) order.push_back(i);
        sort(order.begin(),order.end(),byName);
        for(int i=0;i<n;++i) rows.push_back(writer[i]);
        sort(rows.begin(),rows.end(),byName);
        printf("--- CASE %d\n",++cas);
        for(int r=0;r<n;++r) {
            int w=rows[r],i=0; while(writer[i]!=w) ++i;
            bool printed=false;
            for(int p=0;p<countPeople;++p) {
                int id=order[p]; unsigned int mask=1u<<(id&31);
                if(id!=w && (fans[i][id>>5]&mask) && !(original[i][id>>5]&mask)) {
                    if(!printed) {printf("%s",people[w].name); printed=true;}
                    printf(" %s",people[id].name);
                }
            }
            if(printed) putchar('\n');
        }
    }
    return 0;
}
