#include<cstdio>

//#define DEBUG

const int HEIGHT_MAX=16,WIDTH_MAX=300;

struct Line{
	int bit[WIDTH_MAX/30];
};

Line matrix[HEIGHT_MAX];

int main(){
	int height,width;
	while(scanf("%d%d",&height,&width)!=EOF){
		for(int row=0;row<height;row++){
			for(int i=0;i*30<width;i++)
				matrix[row].bit[i]=0;
			for(int col=0;col<width;col++){
				int element;
				scanf("%d",&element);
				if(element==1)
					matrix[row].bit[col/30]|=1<<(col%30);
			}
		}
		Line full;
		for(int i=0;i*30<width;i++)
			full.bit[i]=0;
		for(int col=0;col<width;col++)
			full.bit[col/30]|=1<<(col%30);
		bool found=false;
		for(int choice=1;choice<(1<<height) && !found;choice++){
			bool duplicate=false;
			Line xors,adds;
			for(int i=0;i*30<width;i++)
				xors.bit[i]=adds.bit[i]=0;
			for(int row=0;row<16 && !duplicate;row++){
				if((1<<row)&choice){
					for(int i=0;i*30<width;i++){
						xors.bit[i]^=matrix[row].bit[i];
						adds.bit[i]+=matrix[row].bit[i];
						if(xors.bit[i]!=adds.bit[i]){
							duplicate=true;
							break;
						}
					}
				}
			}
			found=true;
			for(int i=0;i*30<width;i++)
				if(xors.bit[i]!=full.bit[i]){
					found=false;
					break;
				}
		}
		printf(found?"Yes, I found it\n":"It is impossible\n");
	}
	return 0;
}