#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>
#include<set>

struct Rectangle{
	int height,width;
};

struct RectangleLess{
	bool operator()(const Rectangle& left,const Rectangle& right){
		return left.height<right.height;
	}
};

int solutionArea=INT_MAX;

std::set<Rectangle,RectangleLess> solutions;

void updateSolution(Rectangle pack){
	if(pack.height>pack.width)
		std::swap(pack.height,pack.width);
	if(solutionArea>=pack.height*pack.width){
		if(solutionArea>pack.height*pack.width){
			solutions.clear();
			solutionArea=pack.height*pack.width;
		}
		solutions.insert(pack);
	}
}

void layout1(Rectangle rectangles[4]){
	Rectangle pack={0,0};
	for(int i=0;i<4;i++){
		if(pack.height<rectangles[i].height)
			pack.height=rectangles[i].height;
		pack.width+=rectangles[i].width;
	}
	updateSolution(pack);
}

void layout2(Rectangle rectangles[4]){
	for(int bottom=0;bottom<4;bottom++){
		Rectangle pack={0,0};
		for(int i=0;i<4;i++)
			if(i!=bottom){
				if(pack.height<rectangles[i].height)
					pack.height=rectangles[i].height;
				pack.width+=rectangles[i].width;
			}
		pack.height+=rectangles[bottom].height;
		if(pack.width<rectangles[bottom].width)
			pack.width=rectangles[bottom].width;
		updateSolution(pack);
	}
}

void layout3(Rectangle rectangles[4]){
	for(int bottom=0;bottom<4;bottom++)
		for(int right=0;right<4;right++){
			if(bottom==right)
				continue;
			Rectangle pack={0,0};
			for(int i=0;i<4;i++)
				if(i!=bottom && i!=right){
					if(pack.height<rectangles[i].height)
						pack.height=rectangles[i].height;
					pack.width+=rectangles[i].width;
				}
			pack.height+=rectangles[bottom].height;
			if(pack.width<rectangles[bottom].width)
				pack.width=rectangles[bottom].width;
			if(pack.height<rectangles[right].height)
				pack.height=rectangles[right].height;
			pack.width+=rectangles[right].width;
			updateSolution(pack);
		}
}

void layout4(Rectangle rectangles[4]){
	for(int left=0;left<4;left++)
		for(int right=0;right<4;right++){
			if(left==right)
				continue;
			Rectangle pack={0,0};
			for(int i=0;i<4;i++)
				if(i!=left && i!=right){
					pack.height+=rectangles[i].height;
					if(pack.width<rectangles[i].width)
						pack.width=rectangles[i].width;
				}
			if(pack.height<std::max(rectangles[left].height,rectangles[right].height))
				pack.height=std::max(rectangles[left].height,rectangles[right].height);
			pack.width+=rectangles[left].width+rectangles[right].width;
			updateSolution(pack);
		}
}

void layout5(Rectangle rectangles[4]){
	int sequence[4]={0,1,2,3};
	do{
		Rectangle pack={0,0};
		pack.height=std::max(	rectangles[sequence[0]].height+rectangles[sequence[1]].height,
								rectangles[sequence[2]].height+rectangles[sequence[3]].height);
		if(rectangles[sequence[0]].width>=rectangles[sequence[1]].width){
			if(rectangles[sequence[2]].height<rectangles[sequence[0]].height)
				pack.width=rectangles[sequence[0]].width+std::max(	rectangles[sequence[2]].width,
																	rectangles[sequence[3]].width);
			else if(rectangles[sequence[2]].height<rectangles[sequence[0]].height+rectangles[sequence[1]].height)
				pack.width=std::max(rectangles[sequence[0]].width+rectangles[sequence[2]].width,
									rectangles[sequence[1]].width+rectangles[sequence[3]].width);
			else
				pack.width=std::max(rectangles[sequence[0]].width+rectangles[sequence[2]].width,
									rectangles[sequence[3]].width);
		}
		else
			continue;
		updateSolution(pack);
	}while(std::next_permutation(sequence,sequence+4));
}

void tryLayout(Rectangle rectangles[4]){
	layout1(rectangles);
	layout2(rectangles);
	layout3(rectangles);
	layout4(rectangles);
	layout5(rectangles);
}

void rotate(Rectangle rectangles[4],int index){
	if(index==4)
		return tryLayout(rectangles);
	rotate(rectangles,index+1);
	if(rectangles[index].height!=rectangles[index].width){
		std::swap(rectangles[index].height,rectangles[index].width);
		rotate(rectangles,index+1);
	}
}

void tryRotate(Rectangle rectangles[4]){
	rotate(rectangles,0);
}

int main(){
	Rectangle rectangles[4];
	for(int i=0;i<4;i++)
		scanf("%d%d",&rectangles[i].height,&rectangles[i].width);
	tryRotate(rectangles);
	printf("%d\n",solutionArea);
	for(std::set<Rectangle,RectangleLess>::iterator iter=solutions.begin();iter!=solutions.end();iter++)
		printf("%d %d\n",iter->height,iter->width);
	return 0;
}