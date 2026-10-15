// POJ 2077 - Balanced Budget Initiative
// Model: gpt-6.1-sol
// Match transactions by (type, number), and compare balance changes in cents.
// Register duplicates count even when absent from the statement, as the board
// confirms. Each math check starts from the preceding recorded balance.
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <utility>
using namespace std;
int cents(const string &s) {
    int sign=1, value=0;
    for (size_t i=0;i<s.size();++i) {
        if(s[i]=='-') sign=-1;
        else if(s[i]>='0' && s[i]<='9') value=value*10+s[i]-'0';
    }
    return sign*value;
}
int main() {
    string type, money;
    if(!(cin>>type>>money)) return 0;
    typedef pair<string,int> Key;
    map<Key,int> bank;
    set<Key> seen;
    int number;
    while(cin>>type) {
        if(type=="balance") {cin>>money; break;}
        cin>>number>>money;
        bank[Key(type,number)]=cents(money);
    }
    if(!(cin>>money)) return 0;
    int previous=cents(money);
    while(cin>>type>>number>>money) {
        int amount=cents(money);
        cin>>money;
        int current=cents(money);
        int delta=(current-previous)*(type=="check" ? -1 : 1);
        previous=current;
        Key key(type,number);
        map<Key,int>::iterator it=bank.find(key);
        bool present=it!=bank.end();
        bool repeated=!seen.insert(key).second;
        bool incorrect=present && amount!=it->second;
        bool correctMath=present && delta==it->second;
        bool mistake=delta!=amount && !correctMath;
        cout<<type<<' '<<number;
        if(!present) cout<<" is not in statement";
        if(repeated) cout<<" repeated transaction";
        if(incorrect) cout<<" incorrect amount";
        if(incorrect && correctMath) cout<<" math uses correct value";
        if(mistake) cout<<" math mistake";
        if(present && !repeated && !incorrect && !mistake) cout<<" is correct";
        cout<<'\n';
    }
    for(map<Key,int>::iterator it=bank.begin();it!=bank.end();++it)
        if(!seen.count(it->first)) cout<<"missed "<<it->first.first<<' '<<it->first.second<<'\n';
    return 0;
}
