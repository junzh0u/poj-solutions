#include <iostream>
#include <string>

using namespace std;
int cmp ( const void *a ,const void *b ) 
{
    return *(int*)a - *(int*)b;
} 

void main()
{  
	string str1,str2;
	cin>>str1>>str2;
	int a[26],b[26],i;
	for(i=0;i<26;i++)
	{
		a[i]=0;
		b[i]=0;
	}	
	for( i=0;i<str1.length();i++)
		a[str1[i]-'A']++;
	for( i=0;i<str1.length();i++)
		b[str2[i]-'A']++;
	qsort(a,26,sizeof(int),cmp);
	qsort(b,26,sizeof(int),cmp);
	int result=0;
	for(i=0;i<26;i++)
	{
		if(a[i]==b[i])
			result++;
	}
	if(result==26)
		cout<<"YES"<<endl;
	else
		cout<<"NO"<<endl;
}
