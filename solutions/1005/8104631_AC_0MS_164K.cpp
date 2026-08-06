#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cmath>

int main(){
	int propertyNum;
	scanf("%d",&propertyNum);
	for(int propertyIndex=1;propertyIndex<=propertyNum;propertyIndex++){
		double x,y;
		scanf("%lf%lf",&x,&y);
		double area=0;
		for(int z=1;;z++){
			area+=50.0;
			if(area>=(x*x+y*y)*acos(-1.0)/2.0){
				printf("Property %d: This property will begin eroding in year %d.\n",propertyIndex,z);
				break;
			}
		}
	}
	printf("END OF OUTPUT.\n");
	return 0;
}