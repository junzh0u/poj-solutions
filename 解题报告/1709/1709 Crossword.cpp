#include<iostream.h>
#include<string.h>
int len,ok,n;
int length[1000],flag[1000];
char word[1000][20] ;
char get1[100],get2[100];
int can[200];
char re[100] = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
void srch(int l1, int l2)
{
	int i,l ;
	char tmp[100];           //保存当前情况下多出来的字符串的临时变量
	if ( l1 > len || l2 > len ) return ;
	if ( l1 == l2  )
	{
		if ( l1 == len )
		{
			ok = 1;
			if ( strcmp(re,get1) > 0 )
				strcpy(re,get1);
		}
		return;
	}
	if ( l1 > l2 )          //当前时刻第一个字符串比较长
	{
		strcpy(tmp,get1+l2) ;
		l = l1-l2 ;
		for( i = can[tmp[0]] ; i < n ; i++ )
		{
 			if ( tmp[0] < word[i][0]) break;
			if ( flag[i] == 0 && (l1 > length[i] && strstr(tmp,word[i]) == tmp 
				|| l <= length[i] && strstr(word[i],tmp) == word[i]))
			{
				strcpy( get2+l2,word[i]) ;
				if ( strcmp(re,get2) < 0 ) break;
				flag[i] = 1;
				srch(l1,l2+length[i]);
				flag[i] = 0;
			}
		}
		
	}
	else                   //当前时刻第二个字符串比较长
	{
		strcpy( tmp,get2+l1 );
		l = l2-l1 ;
		for( i = can[tmp[0]] ; i < n ; i++ )
		{
			if ( tmp[0] < word[i][0])  break;
			if ( flag[i] == 0 && (l > length[i] && strstr(tmp,word[i]) == tmp ||
				l <= length[i] && strstr(word[i],tmp) == word[i]))
			{
				strcpy( get1+l1,word[i]) ;
				if ( strcmp(re,get1) < 0 ) break;
				flag[i] = 1;
				srch(l1+length[i],l2);
				flag[i] = 0;
			}
		}
	}
}


void main()
{
	int i ;
	char last;
	cin >> len >> n ;
	last= ' ';
	for( i = 0 ; i < n ; i++ )
	{
		cin >> word[i] ;
		if ( word[i][0] != last )
		{
			last = word[i][0] ;
			can[last] = i ;
		}
		length[i] = strlen(word[i]);
		flag[i] = 0 ;
	}
	for( i = 0 ; i < n ; i++ )
	{
		flag[i] = 1 ;
		strcpy( get1,word[i] ) ;
		if ( strcmp(re,get1) < 0 ) break;
		srch( length[i],0 );
		flag[i] = 0 ;
	}
	if (ok == 1 )
		cout << re << endl;
	else
		cout << "NO SOLUTION" << endl;

}

