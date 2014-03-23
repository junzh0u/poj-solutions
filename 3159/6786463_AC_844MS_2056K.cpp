#include<cstdio>
#include<algorithm>

struct Edge{
	int from,to,weight;
};

bool edgeLess(const Edge &left,const Edge &right){
	if(left.from!=right.from)
		return left.from<right.from;
	return left.to<right.to;
}

int *getIndexInEdges(int vertexNum,int edgeNum,Edge *edges){
	int *indexInEdges=new int[vertexNum];
	std::fill(indexInEdges,indexInEdges+vertexNum,-1);
	int vertexIndex=-1;
	for(int edgeIndex=0;edgeIndex<edgeNum;edgeIndex++)
		if(vertexIndex<edges[edgeIndex].from){
			vertexIndex=edges[edgeIndex].from;
			indexInEdges[vertexIndex]=edgeIndex;
		}
	return indexInEdges;
}

void swap(int *heap,int *indexInHeap,int left,int right){
	std::swap(heap[left],heap[right]);
	indexInHeap[heap[left]]=left;
	indexInHeap[heap[right]]=right;
}

void shiftDown(int *heap,int *indexInHeap,const int index,const int *distance){
	if(index==0)
		return;
	int father=(index-1)/2;
	if(distance[heap[index]]<distance[heap[father]]){
		swap(heap,indexInHeap,index,father);
		shiftDown(heap,indexInHeap,father,distance);
	}
}

void makeheap(int *heap,int *indexInHeap,const int size,const int *distance){
	for(int index=0;index<size;index++)
		shiftDown(heap,indexInHeap,index,distance);
}

void shiftUp(int *heap,int *indexInHeap,const int index,const int size,const int *distance){
	if(index>=size)
		return;
	int left=index*2+1;
	if(left>=size)
		return;
	int next=left;
	int right=index*2+2;
	if(right<size && distance[heap[left]]>distance[heap[right]])
		next=right;
	swap(heap,indexInHeap,index,next);
	shiftUp(heap,indexInHeap,next,size,distance);
}

int pop(int *heap,int *indexInHeap,int &size,const int *distance){
	int value=heap[0];
	swap(heap,indexInHeap,0,size-1);
	size--;
	shiftUp(heap,indexInHeap,0,size,distance);
	return value;
}

void dijkstra(int vertexNum,int *distance,int source,int edgeNum,Edge *edges){
	std::fill(distance,distance+vertexNum,INT_MAX);
	distance[source]=0;
	bool *in=new bool[vertexNum];
	std::fill(in,in+vertexNum,false);
	
	int *heap=new int[vertexNum];
	int *indexInHeap=new int[vertexNum];
	for(int index=0;index<vertexNum;index++){
		heap[index]=index;
		indexInHeap[index]=index;
	}
	int size=vertexNum;
	makeheap(heap,indexInHeap,size,distance);
	
	int *indexInEdges=getIndexInEdges(vertexNum,edgeNum,edges);
	
	while(size){
		int head=pop(heap,indexInHeap,size,distance);
		in[head]=true;
		for(int index=indexInEdges[head];index!=-1 && index<edgeNum && edges[index].from==head;index++){
			int to=edges[index].to;
			if(!in[to] && distance[to]>distance[head]+edges[index].weight){
				distance[to]=distance[head]+edges[index].weight;
				shiftDown(heap,indexInHeap,indexInHeap[to],distance);
			}
		}
	}

	delete[] indexInEdges;
	delete[] indexInHeap;
	delete[] heap;
	delete[] in;
}

int main(){
	int vertexNum,edgeNum;
	scanf("%d%d",&vertexNum,&edgeNum);
	Edge *edges=new Edge[edgeNum];
	for(int index=0;index<edgeNum;index++){
		scanf("%d%d%d",&edges[index].from,&edges[index].to,&edges[index].weight);
		edges[index].from--;
		edges[index].to--;
	}
	std::sort(edges,edges+edgeNum,edgeLess);

	int *distance=new int[vertexNum];
	dijkstra(vertexNum,distance,0,edgeNum,edges);

	printf("%d\n",distance[vertexNum-1]);

	delete[] distance;
	delete[] edges;
	return 0;
}