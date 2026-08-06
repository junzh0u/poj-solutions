#include<cstdio>

const int WIDTH_MAX=150,HEIGHT_MAX=10,THREE_POW_TEN=59049;
const int THREE_POW[HEIGHT_MAX+1]={1,3,9,27,81,243,729,2187,6561,19683,59049};
int BIT[THREE_POW_TEN][HEIGHT_MAX]={0};

int main(){
	for(int mask=0;mask<THREE_POW[HEIGHT_MAX];mask++){
		int remain=mask;
		for(int digit=0;digit<HEIGHT_MAX && remain>0;digit++){
			BIT[mask][digit]=remain%3;
			remain/=3;
		}
	}
	int plateNum;
	scanf("%d",&plateNum);
	while(plateNum-->0){
		int width,height,badSquareNum;
		scanf("%d%d%d",&width,&height,&badSquareNum);
		bool bad[WIDTH_MAX][HEIGHT_MAX]={false};
		while(badSquareNum-->0){
			int x,y;
			scanf("%d%d",&x,&y);
			bad[x-1][y-1]=true;
		}
		int blankLeft[HEIGHT_MAX];
		for(int row=0;row<height;row++)
			blankLeft[row]=bad[0][row]?0:1;
		int count[4][THREE_POW_TEN]={0},current=0;
		for(int column=1;column<width;column++){
			for(int row=0;row<height;row++){
				current=(current+1)%4;
				blankLeft[row]=bad[column][row]?0:(blankLeft[row]+1);
				for(int mask=0;mask<THREE_POW[height];mask++){
					if(BIT[mask][row]>0)
						count[current][mask]=count[(current+3)%4][mask-THREE_POW[row]];
					else{
						count[current][mask]=count[(current+3)%4][mask];
						if(row>=1 && BIT[mask][row-1]==0){
							if(blankLeft[row]>=3 &&
								blankLeft[row-1]>=3 &&
								count[current][mask]<count[(current+2)%4][mask+2*THREE_POW[row]+2*THREE_POW[row-1]]+1)
								count[current][mask]=count[(current+2)%4][mask+2*THREE_POW[row]+2*THREE_POW[row-1]]+1;
							if(row>=2 &&
								blankLeft[row]>=2 &&
								blankLeft[row-1]>=2 &&
								blankLeft[row-2]>=2 &&
								BIT[mask][row-2]==0 &&
								count[current][mask]<count[(current+1)%4][mask+THREE_POW[row]+THREE_POW[row-1]+THREE_POW[row-2]]+1)
								count[current][mask]=count[(current+1)%4][mask+THREE_POW[row]+THREE_POW[row-1]+THREE_POW[row-2]]+1;
						}
					}
				}
			}
		}
		printf("%d\n",count[current][0]);
	}
}