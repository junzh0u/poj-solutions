// POJ 2071 - Auctions R Us
// Model: gpt-6.1-sol
// Process auctions by closing time, checking every bid against the bidder's
// current balance. Money is stored as integer cents. Equal bids retain the
// earlier bid; bids at the exact closing second remain eligible.
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
typedef long long Money;
Money money(const string &s) {
    Money n=0; size_t p=s.find('.');
    for(size_t i=0;i<(p==string::npos?s.size():p);++i) n=n*10+s[i]-'0';
    n*=100;
    if(p!=string::npos) {
        if(p+1<s.size()) n+=(s[p+1]-'0')*10;
        if(p+2<s.size()) n+=s[p+2]-'0';
    }
    return n;
}
int seconds(const string &s) {
    return ((s[0]-'0')*10+s[1]-'0')*3600+((s[3]-'0')*10+s[4]-'0')*60+(s[6]-'0')*10+s[7]-'0';
}
struct Bid {int bidder,time; Money price;};
struct Item {int id,time; Money reserve; vector<Bid> bids;};
bool itemLess(const Item &a,const Item &b) {return a.time<b.time;}
bool bidLess(const Bid &a,const Bid &b) {
    return a.price!=b.price?a.price>b.price:a.time<b.time;
}
int main() {
    int n; if(!(cin>>n)) return 0;
    vector<Item> items(n); map<int,int> index;
    string p,t;
    for(int i=0;i<n;++i) {
        cin>>items[i].id>>p>>t;
        items[i].reserve=money(p); items[i].time=seconds(t); index[items[i].id]=i;
    }
    map<int,Money> balance; int m,id;
    cin>>m;
    for(int i=0;i<m;++i) {cin>>id>>p; balance[id]=money(p);}
    cin>>m;
    for(int i=0;i<m;++i) {
        Bid b; cin>>id>>b.bidder>>p>>t; b.price=money(p); b.time=seconds(t);
        map<int,int>::iterator it=index.find(id);
        if(it!=index.end()) items[it->second].bids.push_back(b);
    }
    sort(items.begin(),items.end(),itemLess);
    for(int i=0;i<n;++i) {
        Item &a=items[i]; sort(a.bids.begin(),a.bids.end(),bidLess);
        int winner=-1;
        for(size_t j=0;j<a.bids.size();++j) {
            Bid &b=a.bids[j];
            if(b.time<=a.time && b.price>=a.reserve && balance.find(b.bidder)!=balance.end() && balance[b.bidder]>=b.price) {
                winner=int(j); break;
            }
        }
        cout<<"Item "<<a.id;
        if(winner<0) cout<<" Reserve not met.\n";
        else {
            Bid &b=a.bids[winner]; balance[b.bidder]-=b.price;
            cout<<" Bidder "<<b.bidder<<" Price "<<b.price/100<<'.'<<setw(2)<<setfill('0')<<b.price%100<<setfill(' ')<<'\n';
        }
    }
    return 0;
}
