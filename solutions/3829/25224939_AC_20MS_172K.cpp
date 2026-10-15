// POJ 3829 - Seat taking up is tough
// Model: gpt-6.1-sol
// Process students by arrival time, preserving their original output order.
// Scan every free horizontal block and maximize its western seat's index.
// Only when the full request is impossible, repeat the search for one seat.
#include <cstdio>
#include <algorithm>
using namespace std;
struct Student { int time, count, id; };
bool earlier(const Student &a, const Student &b) { return a.time < b.time; }
int feeling[30][30], answer[50][2];
bool used[30][30];
int main() {
    int n,m,k;
    while (scanf("%d%d%d", &n,&m,&k)==3 && (n || m || k)) {
        for (int r=0;r<n;++r) for (int c=0;c<m;++c) {
            scanf("%d", &feeling[r][c]); used[r][c]=false;
        }
        Student s[50];
        for (int i=0;i<k;++i) {
            int h,mi; scanf("%d:%d%d", &h,&mi,&s[i].count);
            s[i].time=60*h+mi; s[i].id=i;
        }
        sort(s,s+k,earlier);
        for (int i=0;i<k;++i) {
            int br=-1,bc=-1, take=s[i].count;
            for (int pass=0;pass<2;++pass) {
                for (int r=0;r<n;++r) {
                    int run=0;
                    for (int c=m-1;c>=0;--c) {
                        run=used[r][c]?0:run+1;
                        if (run>=take && (br<0 || feeling[r][c]>feeling[br][bc])) {
                            br=r; bc=c;
                        }
                    }
                }
                if (br>=0 || take==1) break;
                take=1;
            }
            answer[s[i].id][0]=br; answer[s[i].id][1]=bc;
            if (br>=0) for (int c=bc;c<bc+take;++c) used[br][c]=true;
        }
        for (int i=0;i<k;++i) {
            if (answer[i][0]<0) puts("-1");
            else printf("%d %d\n",answer[i][0]+1,answer[i][1]+1);
        }
    }
    return 0;
}
