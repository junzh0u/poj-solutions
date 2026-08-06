#include<iostream.h>

void main()
{
	char cCard[81][6],cTemp[6];
	int iCount=0,i,j,k,l,iFound;
	while(1)
	{
		cin.getline(cTemp,6,'\n');
		if(!(cin.eof() || cTemp[0]=='\0'))
		{
			for(i=0;i<6;i++)
				cCard[iCount][i]=cTemp[i];
			iCount++;
		}
		else if(cin.eof() && !iCount)
			break;
		else
		{
			cout<<"CARDS:  ";
			for(i=0;i<iCount;i++)
				cout<<cCard[i]<<" ";
			cout<<"\nSETS:   ";
			iFound=0;
			for(i=0;i<iCount-2;i++)
				for(j=i+1;j<iCount-1;j++)
					for(k=j+1;k<iCount;k++)
					{
						for(l=0;l<4;l++)
							if(!((cCard[i][l]==cCard[j][l] && cCard[i][l]==cCard[k][l]) || (cCard[i][l]!=cCard[j][l] && cCard[i][l]!=cCard[k][l]&& cCard[j][l]!=cCard[k][l])))
								break;
						if(l==4)
						{
							if(iFound)
								cout<<"        ";
							iFound++;
							cout<<iFound<<".  "<<cCard[i]<<" "<<cCard[j]<<" "<<cCard[k]<<endl;
						}
					}
			if(!iFound)
				cout<<"*** None Found ***"<<endl;
			cout<<endl;
			if(cin.eof())
				break;
			iCount=0;
			//continue;
		}
	}
}