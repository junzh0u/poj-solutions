#include<cstdio>
#include<cstring>

const int TOPPING_CONSTRAINT_NUM_MAX=20;
const int TOPPING_TYPE_NUM=16;
const int TOPPING_REQUEST_NUM_MAX=20;
const char TOPPING_CONSTRAINT_END_STRING[]=".";

int main(){
	while(true){
		//char toppingConstraints[TOPPING_CONSTRAINT_NUM_MAX][2*TOPPINT_REQUEST_NUM_MAX+2];
		int toppingConstraintNum=0;
		int includeConstraints[TOPPING_CONSTRAINT_NUM_MAX]={0},
			omitConstraints[TOPPING_CONSTRAINT_NUM_MAX]={0};
		while(true){
			char toppingConstraintString[2*TOPPING_REQUEST_NUM_MAX+2];
			if(scanf("%s",toppingConstraintString)==EOF)
				return 0;
			if(strcmp(toppingConstraintString,TOPPING_CONSTRAINT_END_STRING)==0)
				break;
			includeConstraints[toppingConstraintNum]=omitConstraints[toppingConstraintNum]=0;
			for(int toppingRequestID=0;toppingConstraintString[2*toppingRequestID]!=';';toppingRequestID++){
				int toppingID=toppingConstraintString[2*toppingRequestID+1]-'A';
				char toppingRequestType=toppingConstraintString[2*toppingRequestID];
				if(toppingRequestType=='+')
					includeConstraints[toppingConstraintNum]|=(1<<toppingID);
				else if(toppingRequestType=='-')
					omitConstraints[toppingConstraintNum]|=(1<<toppingID);
			}
			toppingConstraintNum++;
		}
		bool anyoneSatisfied=false;
		for(int topping=0;topping<(1<<TOPPING_TYPE_NUM);topping++){
			bool satisfied=true;
			for(int toppingConstraintID=0;toppingConstraintID<toppingConstraintNum;toppingConstraintID++){
				if(topping & includeConstraints[toppingConstraintID])
					continue;
				if((topping & omitConstraints[toppingConstraintID]) ^ omitConstraints[toppingConstraintID])
					continue;
				satisfied=false;
				break;
			}
			if(satisfied){
				anyoneSatisfied=true;
				printf("Toppings: ");
				for(int toppingID=0;toppingID<TOPPING_TYPE_NUM;toppingID++)
					if(topping&(1<<toppingID))
						printf("%c",'A'+toppingID);
				printf("\n");
				break;
			}
		}
		if(!anyoneSatisfied)
			printf("No pizza can satisfy these requests.\n");
	}
	return 0;
}