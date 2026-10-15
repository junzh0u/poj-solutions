// POJ 2716 - Open and Close
// Model: gpt-6.1-sol
// Apply the stated set definitions directly. Erosion tests every offset;
// dilation adds offsets and clips to the image. Empty B makes erosion full
// and dilation empty. All intermediate images stay within the image bounds;
// official contest data settles this implicit restriction on erosion.
#include <cstdio>
#include <cstring>
int m,n,s,k,dr[81],dc[81];
unsigned char a[280][280],e[280][280],d[280][280],o[280][280],c[280][280];
const int P=8;
void erode(unsigned char x[][280],unsigned char y[][280]) {
    memset(y,0,280*280);
    for(int r=0;r<m;++r) for(int t=0;t<n;++t) {
        bool ok=true;
        for(int j=0;j<k;++j) {
            int rr=r+dr[j],cc=t+dc[j];
            if(rr<0||rr>=m||cc<0||cc>=n||!x[rr+P][cc+P]) {ok=false;break;}
        }
        y[r+P][t+P]=ok;
    }
}
void dilate(unsigned char x[][280],unsigned char y[][280]) {
    memset(y,0,280*280);
    for(int r=0;r<m;++r) for(int t=0;t<n;++t) if(x[r+P][t+P])
        for(int j=0;j<k;++j) {
            int rr=r+dr[j],cc=t+dc[j];
            if(rr>=0&&rr<m&&cc>=0&&cc<n) y[rr+P][cc+P]=1;
        }
}
void print(unsigned char x[][280]) {
    char row[280];
    for(int r=0;r<m;++r) {
        for(int t=0;t<n;++t) row[t]=x[r+P][t+P]?'*':'.';
        row[n]=0; puts(row);
    }
}
int main() {
    int cs=0; char row[280];
    while(scanf("%d%d%d",&m,&n,&s)==3&&(m||n||s)) {
        memset(a,0,sizeof(a)); k=0;
        for(int r=0;r<m;++r) {scanf("%s",row);for(int t=0;t<n;++t)a[r+P][t+P]=row[t]=='*';}
        for(int r=-s;r<=s;++r) {scanf("%s",row);for(int t=-s;t<=s;++t)if(row[t+s]=='*'){dr[k]=r;dc[k++]=t;}}
        erode(a,e); dilate(e,o);
        dilate(a,d); erode(d,c);
        if(cs) puts("===========================================================================");
        printf("Case %d\n\n",++cs); print(o); puts(""); print(c);
    }
    return 0;
}
