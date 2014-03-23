#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<algorithm>
using namespace std;

template<class T>
class MatrixTool{
public:
	static T** newMatrix(const int size){
		T** matrix=new T*[size];
		for(int row=0;row<size;row++)
			matrix[row]=new T[size];
		return matrix;
	}
	static void deleteMatrix(T** matrix,const int size){
		for(int row=0;row<size;row++)
			delete[] matrix[row];
		delete[] matrix;
	}
	static void fill(T** matrix,const int size,const T value){
		for(int row=0;row<size;row++)
			for(int col=0;col<size;col++)
				matrix[row][col]=value;
	}
};

const int INF=INT_MAX;

class Graph{
private:
	int vertexNum;
	int **edges;
public:
	Graph(const int vertexNum){
		this->vertexNum=vertexNum;
		edges=MatrixTool<int>::newMatrix(vertexNum);
		MatrixTool<int>::fill(edges,vertexNum,INF);
	}
	~Graph(){
		MatrixTool<int>::deleteMatrix(edges,vertexNum);
	}
	int getVertexNum() const{
		return vertexNum;
	}
	int getEdge(const int from,const int to) const{
		return edges[from][to];
	}
	void setEdge(const int from,const int to,const int len){
		edges[from][to]=edges[to][from]=len;
	}
};

class Prim{
private:
	const Graph* graph;
	int vertexNum;
	int* distances;
	int treeSize;
	bool* inTree;
	int treeEdgeSum;
private:
	int getNearestUnconnectIndex(){
		int index=INF;
		for(int i=0;i<vertexNum;i++){
			if(inTree[i])
				continue;
			if(index==INF)
				index=i;
			else if(distances[index]>distances[i])
				index=i;
		}
		return index;
	}
	void connect(int index){
		treeEdgeSum+=distances[index];
		distances[index]=0;
		treeSize++;
		inTree[index]=true;
		for(int to=0;to<vertexNum;to++){
			int edge=graph->getEdge(index,to);
			if(distances[to]==INF || distances[to]>edge)
				distances[to]=edge;
		}
	}
	void buildSpanningTree(){
		distances[0]=0;
		connect(0);
		while(treeSize<vertexNum){
			int next=getNearestUnconnectIndex();
			connect(next);
		}
	}
public:
	Prim(Graph* graph){
		this->graph=graph;
		vertexNum=graph->getVertexNum();
		distances=new int[vertexNum];
		fill(distances,distances+vertexNum,INF);
		treeSize=0;
		inTree=new bool[vertexNum];
		fill(inTree,inTree+vertexNum,false);
		treeEdgeSum=0;
		buildSpanningTree();
	}
	~Prim(){
		delete[] distances;
		delete[] inTree;
	}
	int getTreeEdgeSum(){
		return treeEdgeSum;
	}
};

int vertexNum;
Graph* graph;

bool input(){
	cin>>vertexNum;
	if(!vertexNum)
		return false;
	graph=new Graph(vertexNum);
	int edgeNum;
	cin>>edgeNum;
	while(edgeNum--){
		int from,to,len;
		cin>>from>>to>>len;
		from--;to--;
		if(graph->getEdge(from,to)==INF || graph->getEdge(from,to)>len)
			graph->setEdge(from,to,len);
	}
	return true;
}

int main(){
	while(input()){
		Prim prim(graph);
		cout<<prim.getTreeEdgeSum()<<endl;
	}
	return 0;
}