#include<string.h>
char t[100];
char proc[101][100],gotoproc[101][200][100];
int command[101][200],gotoline[101][200];
int i,j,k;
double rate[101][200];
double t1;
double time[101][200];
 
double dotime(int i,int j,double r)
{double s=0,f=1;
 int a,b;              
 if(r<0.00000000000000000000000000000000000000000000000000001) return(0);
 a=i,b=j;
 if(time[i][j]!=-1) return(time[i][j]);
 for(;;j++)
  {if(command[i][j]==0) break;
   if(command[i][j]==1) s=s+1*f;  
   if(command[i][j]==2) 
    {s=s+1*f+f*rate[i][j]*dotime(i,gotoline[i][j],r*f*rate[i][j]);f=f*(1-rate[i][j]);}
   if(command[i][j]==3)
    {for(k=1;;k++)
      if(strcmp(gotoproc[i][j],proc[k])==0) break;
     s=s+1*f+f*rate[i][j]*dotime(k,1,r*f*rate[i][j]);
    }
  }
 time[a][b]=s;
 return(s);
}

main()
{for(i=1,j=1;;)
 {scanf("%s",t);
  if(strcmp(t,"NOP;")==0)
   {command[i][j]=1;j++;}   

  else if(strcmp(t,"IF")==0)
   {scanf("%c%c%c%lf",&t[0],&t[1],&t[2],&t1);
    if(t[2]=='>') rate[i][j]=1-t1;
    else rate[i][j]=t1;
    scanf("%s",t);
    if(strcmp(t,"GOTO")==0) 
     {command[i][j]=2;
      scanf("%d%c",&gotoline[i][j],&t[0]);
     }
    else if(strcmp(t,"PROC")==0)
     {command[i][j]=3;
      scanf("%c",&t[0]);
      for(k=0;;k++)
       {scanf("%c",&gotoproc[i][j][k]);
        if(gotoproc[i][j][k]==';') break;
       }
      gotoproc[i][j][k]='\0';
     }  
    j++;
   }
  else if(strcmp(t,"END;")==0)
   {command[i][j]=0;i++;j=1;}
  else if(strcmp(t,"PROC")==0)
   scanf("%s",&proc[i][0]);
  else if(strcmp(t,"PROG END")==0) break;
 }

 for(i=1;i<=100;i++)
  for(j=1;j<=200;j++)
   time[i][j]=-1; 
 for(;;)
 {scanf("%s",t);
  if(strcmp(t,"REQUEST END")==0) break;
  for(i=1;;i++)
   if(strcmp(t,proc[i])==0) break;
  printf("%.3lf\n",dotime(i,1,1));
 }
}
