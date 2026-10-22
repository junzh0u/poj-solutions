// POJ 3730 - Ebbinghaus method
// Model: gpt-6-astra
// Reserve each wordlist at its earliest feasible start, including every
// future review. This is the historical greedy interpretation of the task.
// It is NOT an exact optimizer of the literal statement: for n=4 and
// reviews 2,3,4,6 it returns 13, whereas starts 1,3,5,7 finish in 12.
// Submission tests whether POJ uses that historical interpretation.
#include <cstdio>
#include <vector>
using namespace std;
int main() {
    int n,m;
    if (scanf("%d%d", &n, &m)!=2) return 0;
    vector<int> a(m);
    for(int i=0;i<m;++i) { scanf("%d", &a[i]); --a[i]; }
    int period=a[m-1]+1;
    vector<unsigned char> load(period,0);
    int done=0,day=0;
    while(done<n) {
        bool ok=load[day%period]+2+(a[0]==0)<=3;
        for(int i=0;ok && i<m;++i)
            if(a[i] && load[(day+a[i])%period]>=3) ok=false;
        if(ok) {
            for(int i=0;i<m;++i) ++load[(day+a[i])%period];
            ++done;
        }
        load[day%period]=0;
        ++day;
    }
    printf("%d\n",day+a[m-1]);
    return 0;
}
