#include <iostream.h>
int lim[13]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096};
int *count[13],a,b,n;
void init(){
	cin>>a>>b>>n;
	int i,j;
	for(i=a;i<=b;i++)
		count[i]=new int[lim[i]];
	for(i=a;i<=b;i++)
		for(j=0;j<lim[i];j++)
			count[i][j]=0;
}
void cal(){
	int max=0,i,j,k,m,t;
	for(i=0;i<n;i++){
		max=0;
		for(j=a;j<=b;j++)
			for(k=0;k<lim[j];k++)
				if(max<count[j][k])max=count[j][k];
		if(max>0){
			cout<<max;
			for(j=b;j>=a;j--)
				for(k=lim[j]-1;k>=0;k--)
					if(count[j][k]==max){
						count[j][k]=0;
						cout<<' ';
						m=k;
						for(t=j-1;t>=0;t--){
							cout<<m/lim[t];
							m=m%lim[t];
						}
					}
			cout<<endl;
		}
		else break;
	}
}
void main(){
	init();
	unsigned int i,s=0,ct=1;
	char chr;
	cin>>chr;
	while(chr!='2'){
		s=s<<1;
		s+=chr-'0';
		for(i=a;i<=b;i++)
			if(ct>=i)
				count[i][s%lim[i]]++;
		cin>>chr;
		ct++;
		if(ct>20)break;
	}
	while(chr!='2'){
		s=s<<1;
		s+=chr-'0';
		for(i=a;i<=b;i++)
			count[i][(s<<32-i)>>(32-i)]++;
		cin>>chr;
	}
	cal();
}