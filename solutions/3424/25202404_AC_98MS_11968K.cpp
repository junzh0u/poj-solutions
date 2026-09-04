// POJ 3424 - Better security
// Model: claude-sonnet-5
//
// The 3x3 keypad occupies a 300x300 pixel block; buttons are 100x100 with
// no gaps. Column c covers relative x in [100c, 100c+99], row r covers
// relative y in [100r, 100r+99], with row 0 = top (7 8 9) since the board
// clarifies x increases left-to-right and y increases top-to-bottom
// (confirmed by sample 1). The window's top-left corner (ox, oy) is an
// unknown integer offset (it may place the window partially off-screen).
//
// For a fixed ox, every click's column is determined; ox is only
// constrained per axis independently (x-offset doesn't affect rows and
// vice versa), so the set of achievable (row-tuple, column-tuple) pairs is
// exactly the cross product of the achievable column-tuples (ranging ox
// over all integers making every click's relative x land in [0,299]) and
// achievable row-tuples (ranging oy similarly). Enumerate both small
// ranges (width <= 300, else empty), dedupe tuples, cross them to build
// candidate passwords, dedupe again, sort, print (or NONE).
#include <cstdio>
#include <string>
#include <set>
#include <vector>
using namespace std;

int main(){
    int n;
    if(scanf("%d",&n)!=1) return 0;
    vector<int> x(n), y(n);
    for(int i=0;i<n;i++) scanf("%d %d",&x[i],&y[i]);

    int minX=x[0],maxX=x[0],minY=y[0],maxY=y[0];
    for(int i=1;i<n;i++){
        if(x[i]<minX) minX=x[i];
        if(x[i]>maxX) maxX=x[i];
        if(y[i]<minY) minY=y[i];
        if(y[i]>maxY) maxY=y[i];
    }

    set<string> colTuples, rowTuples;
    if(maxX-minX<=299){
        int loOx=maxX-299, hiOx=minX;
        for(int ox=loOx; ox<=hiOx; ox++){
            string t(n,'0');
            for(int i=0;i<n;i++){
                int c=(x[i]-ox)/100;
                t[i]=char('0'+c);
            }
            colTuples.insert(t);
        }
    }
    if(maxY-minY<=299){
        int loOy=maxY-299, hiOy=minY;
        for(int oy=loOy; oy<=hiOy; oy++){
            string t(n,'0');
            for(int i=0;i<n;i++){
                int r=(y[i]-oy)/100;
                t[i]=char('0'+r);
            }
            rowTuples.insert(t);
        }
    }

    static const int grid[3][3] = {{7,8,9},{4,5,6},{1,2,3}};
    set<string> results;
    for(set<string>::iterator itc=colTuples.begin(); itc!=colTuples.end(); ++itc){
        const string& c=*itc;
        for(set<string>::iterator itr=rowTuples.begin(); itr!=rowTuples.end(); ++itr){
            const string& r=*itr;
            string pass(n,'0');
            for(int i=0;i<n;i++){
                int col = c[i]-'0';
                int row = r[i]-'0';
                pass[i] = char('0'+grid[row][col]);
            }
            results.insert(pass);
        }
    }

    if(results.empty()){
        printf("NONE\n");
    } else {
        for(set<string>::iterator it=results.begin(); it!=results.end(); ++it){
            printf("%s\n", it->c_str());
        }
    }
    return 0;
}
