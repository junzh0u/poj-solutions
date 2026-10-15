// POJ 1608 - Banal Tickets
// Model: gpt-6.1-sol
// Count zero products separately. For nonzero products, choose positions of
// 5 and 7, then use a DP for the exponents of 2 and 3 in the other digits.
// Leading zeroes are allowed: ticket numbers are fixed-length digit strings.
// Half counts fit in 64 bits; the final counts use base-10000 arithmetic.
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
using namespace std;
typedef unsigned long long U;
struct Big {
    int d[12];
    Big(U x=0) { memset(d,0,sizeof(d)); for(int i=0;x;i++,x/=10000)d[i]=int(x%10000); }
    Big operator+(const Big& b) const { Big c; int carry=0; for(int i=0;i<12;i++){int v=d[i]+b.d[i]+carry;c.d[i]=v%10000;carry=v/10000;}return c; }
    Big operator-(const Big& b) const { Big c; int borrow=0;for(int i=0;i<12;i++){int v=d[i]-b.d[i]-borrow;borrow=v<0;if(borrow)v+=10000;c.d[i]=v;}return c; }
    Big operator*(const Big& b) const {Big c;for(int i=0;i<12;i++){int carry=0;for(int j=0;i+j<12;j++){int v=c.d[i+j]+d[i]*b.d[j]+carry;c.d[i+j]=v%10000;carry=v/10000;}}return c;}
    void print() const {int i=11;while(i && !d[i])--i;cout<<d[i];while(i--)cout<<setw(4)<<setfill('0')<<d[i];cout<<'\n';}
};
U dp[19][55][37], choose[19][19], p9[19], p10[19];
int e2[10]={0,0,1,0,2,0,1,0,3,0};
int e3[10]={0,0,0,1,0,0,1,0,0,2};
int main(){
    int n; string s;if(!(cin>>n>>s))return 0;
    int q[2]={0,0},e[2][4]={{0,0,0,0},{0,0,0,0}};bool zero[2]={false,false};
    for(int i=0;i<2*n;i++){int h=i/n;if(s[i]=='?'){q[h]++;continue;}int d=s[i]-'0';if(d==0)zero[h]=true;e[h][0]+=e2[d];e[h][1]+=e3[d];e[h][2]+=(d==5);e[h][3]+=(d==7);}
    p9[0]=p10[0]=1;for(int i=1;i<=18;i++){p9[i]=p9[i-1]*9;p10[i]=p10[i-1]*10;}
    U z[2];for(int h=0;h<2;h++)z[h]=zero[h]?p10[q[h]]:p10[q[h]]-p9[q[h]];
    Big answer=Big(z[0])*Big(z[1]);
    if(!zero[0] && !zero[1]){
        for(int i=0;i<=18;i++){choose[i][0]=choose[i][i]=1;for(int j=1;j<i;j++)choose[i][j]=choose[i-1][j-1]+choose[i-1][j];}
        dp[0][0][0]=1;int digits[7]={1,2,3,4,6,8,9};
        for(int m=0;m<18;m++)for(int a=0;a<=3*m;a++)for(int b=0;b<=2*m;b++)if(dp[m][a][b])for(int j=0;j<7;j++){int d=digits[j];dp[m+1][a+e2[d]][b+e3[d]]+=dp[m][a][b];}
        for(int f=0;f<=q[0];f++)for(int g=0;f+g<=q[0];g++){
            int fr=f+e[0][2]-e[1][2],gr=g+e[0][3]-e[1][3];
            if(fr<0||gr<0||fr+gr>q[1])continue;
            int m=q[0]-f-g,mr=q[1]-fr-gr;
            U cl=choose[q[0]][f]*choose[q[0]-f][g],cr=choose[q[1]][fr]*choose[q[1]-fr][gr];
            for(int a=0;a<=3*m;a++)for(int b=0;b<=2*m;b++){
                int ar=a+e[0][0]-e[1][0],br=b+e[0][1]-e[1][1];
                if(ar<0||ar>3*mr||br<0||br>2*mr)continue;
                U l=dp[m][a][b]*cl,r=dp[mr][ar][br]*cr;
                if(l&&r)answer=answer+Big(l)*Big(r);
            }
        }
    }
    answer.print();(Big(p10[q[0]])*Big(p10[q[1]])-answer).print();
}
