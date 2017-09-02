#include<iostream.h>
#include<fstream.h>
#include<string.h>
#include<math.h>
#define EPS 1e-7
class Procedure
{
   public:
      char name[102];
      int com[102]; // 0:NOP  -1:END  1:>GOTO  2:<GOTO  3:>PROC  4:<PROC
      double value[102];
      int id[102];  //GOTO LINE NO.||PROC ID
//      int num_com;
      double time;
      Procedure(){time=-1;}
};
Procedure proc[102];
int num_proc;
char name[102];
double array[102][102],b[102];
int getProcID(char * n)
{
   int i;
   for(i=0;i<num_proc;i++)
      if(strcmp(proc[i].name,n)==0)
         return i;
   return -1;
}
int parseProc()
{
   char c;
   int num_com=0;
   cin>>name;
   strcpy(proc[num_proc].name,name);
   cin>>name;
   while(strcmp(name,"END;")!=0)
      {
         switch(name[0])
            {
               case 'N':
                  proc[num_proc].com[num_com]=0;
                  num_com++;
                  break;
               case 'I':
                  cin>>c>>c;
                  cin>>proc[num_proc].value[num_com];
                  cin>>name;
                  if(name[0]=='G')
                     {
                        if(c=='>')
                           proc[num_proc].com[num_com]=1;
                        else
                           proc[num_proc].com[num_com]=2;
                        cin>>proc[num_proc].id[num_com];
                        proc[num_proc].id[num_com]--;
                        cin>>c;
                     }
                  else
                     {
                        if(c=='>')
                           proc[num_proc].com[num_com]=3;
                        else
                           proc[num_proc].com[num_com]=4;
                        cin>>name;
                        name[strlen(name)-1]='\0';
                        proc[num_proc].id[num_com]=getProcID(name);
                     }
                  num_com++;
            }
         cin>>name;
      }
   proc[num_proc].com[num_com]=-1;
   return 0;
}
double getTime(int id);
int getfczresult(int n,double array[102][102],double b[102])
{
   int i,j,k,l;
   double coeff,temp;
   for(i=0;i<n;i++)
      {
         if(fabs(array[i][i])<EPS)
            for(j=i+1;j<n;j++)
                  if(fabs(array[j][i])>=EPS)
                     {
                        for(k=0;k<n;k++)
                           {
                              temp=array[i][k];
                              array[i][k]=array[j][k];
                              array[j][k]=temp;
                           }
                        temp=b[i];
                        b[i]=b[j];
                        b[j]=temp;
                        break;
                     }
         for(j=0;j<n;j++)
            {
               if(i==j)
                  continue;
               coeff=(-1)*array[j][i]/array[i][i];
               for(k=0;k<n;k++)
                  array[j][k]+=array[i][k]*coeff;
               b[j]+=b[i]*coeff;
            }
      }
   for(i=0;i<n;i++)
      {
         if(fabs(array[i][i])<EPS)
            return 1;
         b[i]/=array[i][i];
         if(fabs(b[i])<EPS)
            b[i]=0;
      }
   return 0;
}
double compute(int id)
{
   int line=0;
   memset(array,0,sizeof(array));
   memset(b,0,sizeof(b));
   do
      {
         array[line][line]=1;
         switch(proc[id].com[line])
            {
               case -1:
                  b[line]=0;
                  break;
               case 0:
                  array[line][line+1]=-1;
                  b[line]=1;
                  break;
               case 1:
                  b[line]=1;
                  array[line][line+1]=(-1)*proc[id].value[line];
                  
array[line][proc[id].id[line]]+=(-1)*(1-proc[id].value[line]);
                  break;
               case 2:
                  b[line]=1;
                  array[line][line+1]=(-1)*(1-proc[id].value[line]);
                  array[line][proc[id].id[line]]+=(-1)*proc[id].value[line];
                  break;
               case 3:
                  
b[line]=1+(1-proc[id].value[line])*getTime(proc[id].id[line]);
                  array[line][line+1]=-1;
                  break;
               case 4:
                  b[line]=1+proc[id].value[line]*getTime(proc[id].id[line]);
                  array[line][line+1]=-1;
                  break;
            }
         line++;
      }
   while(proc[id].com[line-1]!=-1);
   getfczresult(line,array,b);
   proc[id].time=b[0];
   return b[0];
}

double getTime(int id)
{
   if(proc[id].time>-EPS)
      return proc[id].time;
   return compute(id);
}
int main()
{
   cin>>name;
   cin>>name;
   num_proc=0;
   while(strcmp(name,"PROG_END")!=0)
      {
         parseProc();
         num_proc++;
         cin>>name;
      }
   cout.setf(ios::fixed,ios::floatfield);
   cout.precision(3);
   cin>>name;
   while(strcmp(name,"REQUEST_END")!=0)
      {
         cout<<getTime(getProcID(name))<<endl;
         cin>>name;
      }
   return 0;
}

