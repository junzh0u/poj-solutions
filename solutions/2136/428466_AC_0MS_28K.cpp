//2136:Vertical Histogram

#include<iostream.h>

int main()
{
	int i,j,k,spaceCount=0,maxNum=0;
	static char line1[73],line2[73],line3[73],line4[73];
	int letterNum[26];
	for(i=0;i<26;i++)
		letterNum[i]=0;
	cin.getline(line1,72);
	cin.getline(line2,72);
	cin.getline(line3,72);
	cin.getline(line4,72);
	for(i=0;i<72;i++)
	{
		if(line1[i]>='A'&&line1[i]<='Z')
			letterNum[line1[i]-'A']++;
		if(line1[i]>='a'&&line1[i]<='z')
			letterNum[line1[i]-'a']++;
		if(line2[i]>='A'&&line2[i]<='Z')
			letterNum[line2[i]-'A']++;
		if(line2[i]>='a'&&line2[i]<='z')
			letterNum[line2[i]-'a']++;
		if(line3[i]>='A'&&line3[i]<='Z')
			letterNum[line3[i]-'A']++;
		if(line3[i]>='a'&&line3[i]<='z')
			letterNum[line3[i]-'a']++;
		if(line4[i]>='A'&&line4[i]<='Z')
			letterNum[line4[i]-'A']++;
		if(line4[i]>='a'&&line4[i]<='z')
			letterNum[line4[i]-'a']++;
	}
	for(i=0;i<26;i++)
	{
		if(letterNum[i]>maxNum)
			maxNum=letterNum[i];
	}
	for(i=0;i<maxNum;i++)
	{
		for(j=0;j<26;j++)
		{
			if(letterNum[j]==maxNum)
			{
				for(k=0;k<2*spaceCount;k++)
					cout<<" ";
				if(j!=0)
					cout<<" ";
				spaceCount=0;
				cout<<"*";
			}
			else
			{
				if(j==0)
					cout<<" ";
				else
					spaceCount++;
				letterNum[j]++;
			}
		}
		cout<<endl;
		spaceCount=0;
	}
	cout<<"A B C D E F G H I J K L M N O P Q R S T U V W X Y Z "<<endl;
	return 0;
}