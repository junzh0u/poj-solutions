/*
ID:   pinepar1
PROG: lamps
LANG: C++
*/
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>
#include<string>
#include<set>

//#define FILEIO

#ifdef FILEIO
	FILE *in=fopen("lamps.in","r"),*out=fopen("lamps.out","w");
	#define scanf(...) fscanf(in,__VA_ARGS__)
	#define printf(...) fprintf(out,__VA_ARGS__)
#endif

const int BUTTON_EFFECT[4]={63,21,42,9};

int getLampStateMask(){
	int mask=0;
	while(true){
		int lampID;
		scanf("%d",&lampID);
		if(lampID==-1)
			break;
		mask|=(1<<(lampID%6-1));
	}
	return mask;
}

int bitCount(int number){
	int count=0;
	while(number){
		if(number&1)
			count++;
		number>>=1;
	}
	return count;
}

void printState(int lampState,int lampNum){
}

std::string stateToString(int lampState,int lampNum){
	std::string line;
	for(int lampID=0;lampID<lampNum;lampID++){
		int mask=1<<(lampID%6);
		if(lampState&mask)
			line+="1";
		else
			line+="0";
	}
	return line;
}

int main(){
	int lampNum,buttonPressedNum;
	scanf("%d%d",&lampNum,&buttonPressedNum);
	int lampsShouldBeOn=getLampStateMask();
	int lampsShouldBeOff=getLampStateMask();

	std::set<std::string> result;
	const int LAMP_STATE=(1<<6)-1;
	int buttonPressedStateMax=1<<4;
	for(int buttonPressedState=0;buttonPressedState<buttonPressedStateMax;buttonPressedState++){
		int buttonPressedCount=bitCount(buttonPressedState);
		if(buttonPressedCount<=buttonPressedNum && (buttonPressedNum-buttonPressedCount)%2==0){
			int pressedButtonEffect=0;
			for(int buttonID=0;buttonID<4;buttonID++){
				if(buttonPressedState&(1<<buttonID)){
					pressedButtonEffect^=BUTTON_EFFECT[buttonID];
				}
			}
			int finalLampState=LAMP_STATE^pressedButtonEffect;
			if(((finalLampState&lampsShouldBeOn)==lampsShouldBeOn) && ((finalLampState&lampsShouldBeOff)==0))
				result.insert(stateToString(finalLampState,lampNum));
		}
	}
	for(std::set<std::string>::iterator iter=result.begin();iter!=result.end();iter++)
		printf("%s\n",iter->c_str());
#ifdef FILEIO
	fclose(in);
	fclose(out);
#endif
	return 0;
}