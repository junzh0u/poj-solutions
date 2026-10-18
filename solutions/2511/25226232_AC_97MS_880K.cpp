// POJ 2511 - The Top Shelf
// Model: gpt-6-astra
// Sort original titles lexicographically; only adjacency comparisons strip
// nonletters and ignore case. The discussion confirms case-sensitive sorting.
// DP by last book and shelf size finds a maximum-weight path of at most ten
// vertices in the sorted compatibility DAG. Predecessors reconstruct the shelf.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Book {
    string title, letters;
    int value;
    bool operator<(const Book& b) const { return title < b.title; }
};
struct State { int value, prev; State(): value(-1000000000), prev(-1) {} };
int main() {
    int n;
    if (!(cin >> n)) return 0;
    vector<Book> books(n);
    for (int i=0; i<n; ++i) {
        cin >> books[i].value;
        cin.get();
        getline(cin, books[i].title);
        string& s=books[i].title;
        if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
        for (size_t p=0; p<s.size(); ++p) {
            char c=s[p];
            if (c>='A' && c<='Z') c=char(c-'A'+'a');
            if (c>='a' && c<='z') books[i].letters+=c;
        }
    }
    sort(books.begin(),books.end());
    vector< vector<State> > dp(n,vector<State>(11));
    int best=0, last=-1, count=0;
    for (int i=0; i<n; ++i) {
        dp[i][1].value=books[i].value;
        for (int j=0; j<i; ++j) {
            const string &a=books[i].letters, &b=books[j].letters;
            size_t p=0, lim=min(a.size(),b.size());
            while (p<lim && a[p]!=b[p]) ++p;
            if (p<lim) continue;
            for (int k=2; k<=10; ++k) {
                if (dp[j][k-1].value==-1000000000) continue;
                int v=dp[j][k-1].value+books[i].value;
                if (v>dp[i][k].value) {
                    dp[i][k].value=v;
                    dp[i][k].prev=j;
                }
            }
        }
        for (int k=1; k<=10; ++k) if (dp[i][k].value>best) {
            best=dp[i][k].value; last=i; count=k;
        }
    }
    vector<int> answer;
    for (int k=count; k>0; --k) {
        answer.push_back(last);
        last=dp[last][k].prev;
    }
    cout << count << '\n' << best << '\n';
    for (int k=count-1; k>=0; --k) cout << books[answer[k]].title << '\n';
    return 0;
}
