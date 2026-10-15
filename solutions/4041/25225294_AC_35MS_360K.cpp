// POJ 4041 - Peer Review
// Model: gpt-6.1-sol
// Collect reviewers for each paper, then check the required count,
// repeated reviewers, and institution conflicts. Count each bad paper once.
// POJ's empty statement was recovered from the SWERC 2011 problem set.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    int k,n;
    while (cin >> k >> n && (k || n)) {
        vector<string> institution(n);
        vector< vector<int> > reviewers(n);
        for (int i=0;i<n;++i) {
            cin >> institution[i];
            for (int j=0;j<k;++j) {
                int p; cin >> p;
                reviewers[p-1].push_back(i);
            }
        }
        int problems=0;
        for (int p=0;p<n;++p) {
            bool bad=(int)reviewers[p].size()!=k;
            vector<bool> seen(n,false);
            for (int j=0;j<(int)reviewers[p].size();++j) {
                int r=reviewers[p][j];
                if (seen[r] || institution[r]==institution[p]) bad=true;
                seen[r]=true;
            }
            if (bad) ++problems;
        }
        if (!problems) cout << "NO PROBLEMS FOUND\n";
        else cout << problems << (problems==1 ? " PROBLEM FOUND\n" : " PROBLEMS FOUND\n");
    }
    return 0;
}
