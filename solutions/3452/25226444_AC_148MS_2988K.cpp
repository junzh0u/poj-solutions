// POJ 3452 - Railway Transportation
// Model: gpt-6-astra
// Partition the input into nondecreasing FIFO tracks. Track tails are
// decreasing; replace the first tail <= the arriving value, or add a track.
// This patience-sorting partition uses the minimum number of tracks.
// Sort by (value, original index) to emit a valid stable merge of the tracks.
// Equal carriage numbers may share a track and preserve their input order.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
int main() {
    int n,m;
    while(scanf("%d%d",&n,&m)==2 && (n || m)) {
        vector<int> track(n);
        vector<pair<unsigned int,int> > order(n);
        for(int i=0;i<n;++i) {
            unsigned int x; scanf("%u",&x);
            order[i]=make_pair(x,i);
        }
        vector<unsigned int> ends;
        for(int i=0;i<n;++i) {
            unsigned int x=order[i].first;
            int lo=0,hi=(int)ends.size();
            while(lo<hi) {
                int mid=lo+(hi-lo)/2;
                if(ends[mid]>x) lo=mid+1; else hi=mid;
            }
            if(lo==(int)ends.size()) ends.push_back(x);
            else ends[lo]=x;
            track[i]=lo+1;
        }
        if((int)ends.size()>m) { puts("Transportation failed"); continue; }
        for(int i=0;i<n;++i) printf("%d%c",track[i],i+1==n?'\n':' ');
        sort(order.begin(),order.end());
        for(int i=0;i<n;++i) printf("%d%c",track[order[i].second],i+1==n?'\n':' ');
    }
    return 0;
}
