#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>

struct instruction{
	int type;
	int pos;
	char *s;
	long double p;
}ins[100][100];

long double p[100],a[101][101];
int n,m,deg[100],out_deg[100],flag[100];
char s[120],s1[120],s2[120],name[100][120],g[100][100];

void swap(long double &a,long double &b){
	long double c;
	c=a,a=b,b=c;
}

long double compute(int t)
{
	int i,j,k,n=deg[t];
	long double x[100],r;

	for(i=0;i<n;i++){
		for(j=0;j<=n;j++)
			a[i][j]=0;
	}
	for(i=0;i<n;i++) a[i][i]=1;

	for(i=0;i<n;i++){
		if (ins[t][i].type==4){
			a[i][n]=0;
		}else if (ins[t][i].type==0){
			a[i][i+1]=-1;
			a[i][n]=1;
		}else if (ins[t][i].type==2){
			a[i][i+1]=-1;
			a[i][n]+=1+ins[t][i].p*p[ins[t][i].pos];
		}else{
			a[i][i+1]-=(1-ins[t][i].p);
			a[i][ins[t][i].pos]-=ins[t][i].p;
			a[i][n]=1;
		}
	}

	for(i=0;i<n;i++){
		if (fabs(a[i][i])<1e-8){
			for(j=i+1;j<n;j++) if (fabs(a[i][j])>1e-8) break;
			if (j==n) while(1);
			for(k=0;k<=n;k++) swap(a[i][k],a[j][k]);
		}
		for(j=i+1;j<n;j++) if (fabs(a[j][i])>1e-8){
			r=a[j][i]/a[i][i];
			for(k=i;k<=n;k++) a[j][k]-=r*a[i][k];
		}
	}

	for(i=n-1;i>=0;i--){
		r=0;
		for(j=n-1;j>i;j--) r+=x[j]*a[i][j];
		x[i]=(a[i][n]-r)/a[i][i];
	}

	return x[0];
}

void solve()
{
	int i,j,k;
	memset(flag,0,sizeof(flag));
	for(i=0;i<n;i++){
		for(j=0;j<n;j++) if (!flag[j]&&out_deg[j]==0) break;
		flag[j]=1;
		p[j]=compute(j);
		for(k=0;k<n;k++) if (g[k][j]) out_deg[k]--;
	}
}

void process(char *s)
{
	int i,j,l=strlen(s);
	for(i=0,j=0;i<l;i++) if (s[i]!=' ') s[j++]=s[i];
	s[j]=0;
}

int main()
{
	freopen("random.in","r",stdin);

	int i,j,k,l;
	bool end;
	char *t;
	long double pp;

	while(1){
		gets(s);
		if (strcmp(s,"PROG_START")==0) break;
	}

	end=0,n=0;
	while(1){
		while(1){
			gets(s);
			if (strcmp(s,"PROG_END")==0){
				end=1;
				break;
			}
			sscanf(s,"%s%s",s1,s2);
			if (strcmp(s1,"PROC")==0) break;
		}
		if (end) break;
		strcpy(name[n],s2);
		m=0;

		while(1){
			gets(s);
			process(s);
			if (s[0]==0) continue;
			if (strcmp(s,"END;")==0){
				ins[n][m].type=4;
				m++;
				break;
			}
			if (strcmp(s,"NOP;")==0){
				ins[n][m].type=0;
			}else{
				t=s;
				while(*t!='>'&&*t!='<') t++;
				if (*t=='>'){
					t++;
					sscanf(t,"%Lf",&pp);
					if (pp<0) pp=0;
					if (pp>1) pp=1;
					ins[n][m].p=(1-pp);

				}else{
					t++;
					sscanf(t,"%Lf",&pp);
					if (pp<0) pp=0;
					if (pp>1) pp=1;
					ins[n][m].p=pp;
				}
				while(*t!='P'&&*t!='G') t++;
				if (*t=='P'){
					ins[n][m].type=2;
					while(*t!='C') t++;
					t++;
					sscanf(t,"%s",s1);
					l=strlen(s1);
					s1[l-1]=0;
					ins[n][m].s=new char[l+1];
					strcpy(ins[n][m].s,s1);

				}else{
					ins[n][m].type=1;
					while(*t!='T') t++;
					while(*t!='O') t++;
					t++;
					sscanf(t,"%d",&l);
					ins[n][m].pos=l-1;
				}
			}
			m++;
		}

		deg[n]=m;
		n++;
	}

	memset(g,0,sizeof(g));
	memset(out_deg,0,sizeof(out_deg));

	for(i=0;i<n;i++){
		for(j=0;j<deg[i];j++) if (ins[i][j].type==2){
			for(k=0;k<n;k++) if (strcmp(ins[i][j].s,name[k])==0) break;
			ins[i][j].pos=k;
			delete ins[i][j].s;
			if (g[i][k]) continue;
			g[i][k]=1;
			out_deg[i]++;
		}
	}

	solve();

	while(1){
		gets(s);
		if (s[0]==0) continue;
		if (strcmp(s,"REQUEST_END")==0) break;
		for(i=0;i<n;i++) if (strcmp(s,name[i])==0) break;
		printf("%.3Lf\n",p[i]);
	}

	fclose(stdin);
	return 0;
}

