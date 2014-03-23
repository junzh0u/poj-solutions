#include<iostream>
#include<cmath>

using namespace std;

void main()
{
	int iData[6];
	int iSum,iCode=0,i,j,k,l,m,n,iLevel,jLevel,kLevel,lLevel,mLevel,nLevel;
	bool bBreakValue;
	for(i=0;i<6;i++)
		cin>>iData[i];
	while(iData[0]+iData[1]+iData[2]+iData[3]+iData[4]+iData[5]!=0)
	{
		iCode++;
		if(iData[5]>8)
			iData[5]=(iData[5]%2)?7:6;
		if(iData[4]>12)
			iData[4]=(iData[4]%2)?11:12;
		if(iData[3]>8)
			iData[3]=(iData[3]%2)?7:8;
		if(iData[2]>8)
			iData[2]=(iData[2]%2)?7:8;
		if(iData[1]>4)
			iData[1]=(iData[1]%2)?3:4;
		iSum=0;
		for(i=0;i<6;i++)
			iSum+=iData[i]*(i+1);

		//for(i=0;i<6;i++)
		//	cout<<iData[i]<<"\t";
		//cout<<"\nSum = "<<iSum<<endl;

		if(iSum%2)
			cout<<"Collection #"<<iCode<<":\nCan't be divided.\n"<<endl;
		else if(iData[0]>5)
			cout<<"Collection #"<<iCode<<":\nCan be divided.\n"<<endl;
		else
		{
			bBreakValue=false;
			for(i=0;i<=iData[0] && !bBreakValue;i++)
			{
				iLevel=i*1;
				if(iLevel==iSum/2)
				{
					
					//cout<<"While i="<<i<<"\nLevel = "<<iLevel<<endl;

					cout<<"Collection #"<<iCode<<":\nCan be divided.\n"<<endl;
					bBreakValue=true;
				}
				for(j=0;j<=iData[1] && !bBreakValue;j++)
				{
					jLevel=iLevel+j*2;
					if(jLevel==iSum/2)
					{
						
						//cout<<"While i="<<i<<" j= "<<"\nLevel = "<<jLevel<<endl;

						cout<<"Collection #"<<iCode<<":\nCan be divided.\n"<<endl;
						bBreakValue=true;
					}
					for(k=0;k<=iData[2] && !bBreakValue;k++)
					{
						kLevel=jLevel+k*3;
						if(kLevel==iSum/2)
						{
							
							//cout<<"While i="<<i<<" j= "<<j<<" k= "<<k<<"\nLevel = "<<kLevel<<endl;

							cout<<"Collection #"<<iCode<<":\nCan be divided.\n"<<endl;
							bBreakValue=true;
						}
						for(l=0;l<=iData[3] && !bBreakValue;l++)
						{
							lLevel=kLevel+l*4;
							if(lLevel==iSum/2)
							{

								//cout<<"While i="<<i<<" j= "<<j<<" k= "<<k<<" l= "<<l<<"\nLevel = "<<lLevel<<endl;

								cout<<"Collection #"<<iCode<<":\nCan be divided.\n"<<endl;
								bBreakValue=true;
							}
							for(m=0;m<=iData[4] && !bBreakValue;m++)
							{
								mLevel=lLevel+m*5;
								if(mLevel==iSum/2)
								{

									//cout<<"While i="<<i<<" j= "<<j<<" k= "<<k<<" l= "<<l<<" m= "<<m<<"\nLevel = "<<mLevel<<endl;

									cout<<"Collection #"<<iCode<<":\nCan be divided.\n"<<endl;
									bBreakValue=true;
								}
								for(n=0;n<=iData[5] && !bBreakValue;n++)
								{	
									nLevel=mLevel+n*6;
									if(nLevel==iSum/2)
									{

										//cout<<"While i="<<i<<" j= "<<j<<" k= "<<k<<" l= "<<l<<" m= "<<m<<" n= "<<n<<"\nLevel = "<<nLevel<<endl;

										cout<<"Collection #"<<iCode<<":\nCan be divided.\n"<<endl;
										bBreakValue=true;
									}
								}
							}
						}
					}
				}
			}
			if(!bBreakValue)
				cout<<"Collection #"<<iCode<<":\nCan't be divided.\n"<<endl;
		}
		for(i=0;i<6;i++)
			cin>>iData[i];
	}
}