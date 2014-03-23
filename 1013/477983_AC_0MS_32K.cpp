//Counterfeit Dollar

#include<iostream.h>
#include<string.h>

int main()
{
	char cLef[3][13],cRig[3][13],cBla[3][5];
	char cCom[2][5]={"up","down"},cOut[2][6]={"heavy","light"};
	int iCase,p,i,j,k,l,iTru,iExL,iExR;
	cin>>iCase;
	for(p=0;p<iCase;p++)
	{
		for(i=0;i<3;i++)
			cin>>cLef[i]>>cRig[i]>>cBla[i];
		for(i=0;i<12;i++)
		{
			for(j=0;j<2;j++)//0:Heavy 1:Light
			{
				//cout<<"\n\n"<<i<<" "<<j;//
				iTru=1;
				for(k=0;k<3;k++)
				{
					iExL=0;
					iExR=0;
					for(l=0;l<(int)strlen(cLef[k]);l++)
					{
						if(cLef[k][l]==i+'A')
						{
							iExL=1;
							break;
						}
					}
					//cout<<"\niExL= "<<iExL;//
					if(iExL)
					{
						if(strcmp(cBla[k],cCom[j]))
						{
							iTru=0;
							break;
						}
					}
					else
					{
						for(l=0;l<(int)strlen(cRig[k]);l++)
						{
							if(cRig[k][l]==i+'A')
							{
								iExR=1;
								break;
							}
						}
						//cout<<"\niExR= "<<iExR;//
						if(iExR)
						{
							if(strcmp(cBla[k],cCom[1-j]))
							{
								iTru=0;
								break;
							}
						}
						else
						{
							if(strcmp(cBla[k],"even"))
							{
								iTru=0;
								break;
							}
						}
					}
				}
				//cout<<"\niTru= "<<iTru;//
				if(iTru)
				{
					cout<<char(i+'A')<<" is the counterfeit coin and it is "<<cOut[j]<<"."<<endl;
					break;
				}
			}
			if(iTru)
				break;
		}
	}
	return 0;
}