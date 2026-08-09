// POJ 1234 - Ball Toss
// Model: gpt-5.6-terra
#include <iostream>
using namespace std;
int main(){int n;while(cin>>n&&n){int hand;cin>>hand;--hand;int d[30];bool seen[30]={0};for(int i=0;i<n;++i){char c;cin>>c;d[i]=c=='L'?-1:1;}int from=0, got=1, toss=1;seen[hand]=true;while(got<n){int next=(from+d[hand]+n)%n;if(next==hand)next=(next+d[hand]+n)%n;d[hand]=-d[hand];from=hand;hand=next;++toss;if(!seen[hand]){seen[hand]=true;++got;}}cout<<"Classmate "<<hand+1<<" got the ball last after "<<toss<<" tosses.\n";}return 0;}
