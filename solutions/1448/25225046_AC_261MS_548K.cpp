// POJ 1448 - Cube
// Model: gpt-6.1-sol
// Enumerate face assignments and the eight rotations/reflections of each piece.
// Shared edge interiors must be covered once; corners may not overlap.
// The total shell volume then guarantees that all eight corners are covered.
// A global cube symmetry fixes piece zero on face zero in its input orientation.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;
int a[6][8][6][6], cell[6][8][6][216];
bool ok[6][6][8][6][6][8];
int chosenP[6], chosenO[6];
void xyz(int f,int r,int c,int &x,int &y,int &z){
 x=c;y=r;z=0;
 if(f==1){x=c;y=0;z=r;}
 if(f==2){x=0;y=c;z=r;}
 if(f==3){x=c;y=r;z=5;}
 if(f==4){x=c;y=5;z=r;}
 if(f==5){x=5;y=c;z=r;}
}
bool dfs(int f,int used){
 if(f==6)return true;
 for(int p=0;p<6;p++)if(!(used&(1<<p)))for(int o=0;o<8;o++){
  bool good=true;
  for(int j=0;j<f;j++)if(!ok[j][chosenP[j]][chosenO[j]][f][p][o]){good=false;break;}
  if(good)for(int x=0;x<=5;x+=5)for(int y=0;y<=5;y+=5)for(int z=0;z<=5;z+=5){
   int faces[3]={z==0?0:3,y==0?1:4,x==0?2:5};
   int sum=0;bool complete=true;
   for(int k=0;k<3;k++){
    int h=faces[k];if(h>f){complete=false;break;}
    sum+=cell[h==f?p:chosenP[h]][h==f?o:chosenO[h]][h][x*36+y*6+z];
   }
   if(complete&&sum!=1)good=false;
  }
  if(good){chosenP[f]=p;chosenO[f]=o;if(dfs(f+1,used|(1<<p)))return true;}
 }
 return false;
}
int main(){
 int T;cin>>T;
 for(int tc=1;tc<=T;tc++){
  string s;int total=0;
  for(int r=0;r<6;r++){cin>>s;for(int p=0;p<6;p++)for(int c=0;c<6;c++){a[p][0][r][c]=(s[p*7+c]=='X');total+=a[p][0][r][c];}}
  for(int p=0;p<6;p++){
   for(int r=0;r<6;r++)for(int c=0;c<6;c++)a[p][4][r][c]=a[p][0][r][5-c];
   for(int o=1;o<8;o++)if(o!=4)for(int r=0;r<6;r++)for(int c=0;c<6;c++)a[p][o][r][c]=a[p][o-1][5-c][r];
  }
  memset(cell,0,sizeof(cell));
  for(int f=0;f<6;f++)for(int p=0;p<6;p++)for(int o=0;o<8;o++)for(int r=0;r<6;r++)for(int c=0;c<6;c++){
   int x,y,z;xyz(f,r,c,x,y,z);cell[p][o][f][x*36+y*6+z]=a[p][o][r][c];
  }
  if(total==152)for(int f=0;f<6;f++)for(int g=f+1;g<6;g++){
   int ids[6],need[6],n=0;
   for(int r=0;r<6;r++)for(int c=0;c<6;c++){
    int x,y,z;xyz(f,r,c,x,y,z);
    bool ong=(g==0?z==0:g==1?y==0:g==2?x==0:g==3?z==5:g==4?y==5:x==5);
    if(ong){ids[n]=x*36+y*6+z;need[n++]=((x==0||x==5)+(y==0||y==5)+(z==0||z==5)==2);}
   }
   for(int p=0;p<6;p++)for(int q=0;q<6;q++)if(p!=q)for(int o=0;o<8;o++)for(int v=0;v<8;v++){
    bool good=true;
    for(int k=0;k<n;k++){
     int sum=cell[p][o][f][ids[k]]+cell[q][v][g][ids[k]];
     if(sum>1||(need[k]&&sum!=1)){good=false;break;}
    }
    ok[f][p][o][g][q][v]=good;
   }
  }
  chosenP[0]=0;chosenO[0]=0;
  bool ans=total==152&&dfs(1,1);
  printf("Scenario #%d:\n%s\n\n",tc,ans?"Yes":"No");
 }
 return 0;
}
