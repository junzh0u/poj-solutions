#include <iostream.h>
#include <string.h>

void main()
{
	char solve[30]={"VWXYZABCDEFGHIJKLMNOPQRSTU"};
	char temp[30];
	char key[100];
	while(1)
	{
		cin.getline(temp,30);
		if(strlen(temp)==10)
			break;
		cin.getline(key,100);
		for(int i=0;key[i]!='\0';i++)
			if(key[i]>='A'&&key[i]<='Z')
			key[i]=solve[key[i]-'A'];
		cout<<key<<endl;
		cin.getline(temp,30);
	}
}