#include<cstdio>

const int CUBE_NUM_MAX=30000;
const char MOVE='M',COUNT='C';

struct Cube{
	Cube *father;
	int branch,size;
};

Cube* GetRoot(Cube* cube){
	if(cube->father==cube)
		return cube;
	Cube *root=GetRoot(cube->father);
	cube->branch+=cube->father->branch;
	cube->father=root;
	return root;
}

void Move(Cube* from,Cube* to){
	from=GetRoot(from);
	to=GetRoot(to);
	from->branch=to->size;
	to->size+=from->size;
	from->father=to;
}

int main(){
	Cube cubes[CUBE_NUM_MAX];
	for(int i=0;i<CUBE_NUM_MAX;i++){
		cubes[i].father=cubes+i;
		cubes[i].branch=0;
		cubes[i].size=1;
	}

	int operationNum;
	scanf("%d",&operationNum);
	while(operationNum--){
		char operation;
		scanf(" %c",&operation);
		if(operation==MOVE){
			int from,to;
			scanf("%d%d",&from,&to);
			Move(cubes+from-1,cubes+to-1);
		}
		else if(operation==COUNT){
			int id;
			scanf("%d",&id);
			GetRoot(cubes+id-1);
			printf("%d\n",cubes[id-1].branch);
		}
		else{
			//Unexpected
		}
	}
	return 0;
}