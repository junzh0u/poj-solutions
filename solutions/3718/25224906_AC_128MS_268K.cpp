// POJ 3718 - Facer's Chocolate Dream
// Model: gpt-6.1-sol
// Count distinct subsets of triples by their XOR, using symmetry by weight.
// Appending a triple either creates an m-subset (counted m times) or
// cancels a repeated triple, leaving an (m-2)-subset with T-m+2 choices.
// Dishes are distinct, as confirmed by the problem's discussion board.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
const int P=10007;
int c[1001][4],inv[1001],f[3][1001];
int main(){
    c[0][0]=1;
    for(int i=1;i<=1000;++i){c[i][0]=1;for(int j=1;j<=3;++j)c[i][j]=(c[i-1][j]+c[i-1][j-1])%P;}
    inv[1]=1;
    for(int i=2;i<=1000;++i)inv[i]=P-(P/i)*inv[P%i]%P;
    int n,m;
    string a,b;
    while(cin>>n>>m && (n||m)){
        cin>>a>>b;
        int k=0;for(int i=0;i<n;++i)k+=(a[i]!=b[i]);
        if(m>n*(n-1)*(n-2)/6){cout<<0<<'\n';continue;}
        memset(f,0,sizeof(f));f[0][0]=1;
        for(int t=1;t<=m;++t){
            int cur=t%3,prev=(t+2)%3,old=(t+1)%3;
            for(int w=0;w<=n;++w){
                int value=0;
                for(int j=0;j<=3;++j){
                    int q=w+3-2*j;
                    if(j<=w && 3-j<=n-w && q>=0 && q<=n){
                        int ways=c[w][j]*c[n-w][3-j]%P;
                        value=(value+ways*f[prev][q])%P;
                    }
                }
                if(t>=2)value=(value-f[old][w]*((c[n][3]-t+2+P)%P)%P+P)%P;
                f[cur][w]=value*inv[t]%P;
            }
        }
        cout<<f[m%3][k]<<'\n';
    }
}
