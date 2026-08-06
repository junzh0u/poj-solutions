#include<iostream.h>
//#include<math.h>

void main()
{
	//const double MULA=pow(1.0/3.0,1.0/3.0),MULB=pow(1.0/2.0,1.0/3.0);
	const double MULA=0.7,MULB=0.8;
	int iN,iMax,iA,iB,iC;
	//double dTemp;
	cin>>iMax;
	for(iN=6;iN<=iMax;iN++)
	{
		for(iA=2;iA<=iN*MULA;iA++)
			for(iB=iA;iB<=iN*MULB;iB++)
				for(iC=iB;iC<=iN;iC++)
				{
					if(iA*iA*iA+iB*iB*iB+iC*iC*iC==iN*iN*iN)
						cout<<"Cube = "<<iN<<", Triple = ("<<iA<<","<<iB<<","<<iC<<")"<<endl;
					//else
					//	cout<<"\tWA\tCube = "<<iN<<", Triple = ("<<iA<<","<<iB<<","<<iC<<")"<<endl;
				}
	}
}