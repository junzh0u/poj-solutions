#include<cstdio>
#include<deque>
#include<algorithm>
#include<string>
using namespace std;

const int DIGIT_NUM_MAX=10,LETTER_NUM_MAX=40;
class Huffman{
public:
	Huffman();
	bool Read();
	bool Solve();
private:
	bool Initialize();
	int digitNum,letterNum,ID;
	struct Node{
		char value;
		int frequency;
		string code;
		Node *child,*sibling;
	};
	Node* letter[LETTER_NUM_MAX];
	deque<Node*> tree;
	bool Coding(Node*,string);
	static bool DeleteNode(Node*);
	static bool NodeCompare(Node*,Node*);
};

Huffman::Huffman(){
	ID=0;
	Initialize();
}

bool Huffman::Read(){
	ID++;
	Initialize();
	scanf("%d",&digitNum);
	if(digitNum==0)
		return false;
	scanf("%d",&letterNum);
	for(int i=0;i<letterNum;i++){
		letter[i]=new Node;
		letter[i]->value='A'+i;
		scanf("%d",&letter[i]->frequency);
		letter[i]->child=letter[i]->sibling=NULL;
		tree.push_back(letter[i]);
	}
	while((tree.size()-1)%(digitNum-1)!=0){
		Node* fictitious=new Node;
		fictitious->value=CHAR_MAX;
		fictitious->frequency=0;
		fictitious->child=fictitious->sibling=NULL;
		tree.push_back(fictitious);
	}
	return true;
}

bool Huffman::Solve(){
	while(tree.size()>=digitNum){
		sort(tree.begin(),tree.end(),NodeCompare);
		Node* combine=new Node;
		combine->value=CHAR_MAX;
		combine->frequency=0;
		combine->child=tree.front();
		combine->sibling=NULL;
		for(int i=0;i<digitNum;i++){
			if(combine->value>tree[i]->value)
				combine->value=tree[i]->value;
			combine->frequency+=tree[i]->frequency;
			if(i+1<digitNum)
				tree[i]->sibling=tree[i+1];
		}
		tree.erase(tree.begin(),tree.begin()+digitNum);
		tree.push_front(combine);
	}
	Coding(tree.front(),"");
	double average=0.0;
	for(int i=0;i<letterNum;i++)
		average+=letter[i]->frequency*letter[i]->code.length();
	average/=tree.front()->frequency;
	printf("Set %d; average length %.2lf\n",ID,average);
	for(i=0;i<letterNum;i++)
		printf("    %c: %s\n",letter[i]->value,letter[i]->code.c_str());
	printf("\n");
	return true;
}

bool Huffman::Initialize(){
	digitNum=letterNum=0;
	while(!tree.empty()){
		DeleteNode(tree.front());
		tree.pop_front();
	}
	return true;
}

bool Huffman::Coding(Node* root,string newCode){
	root->code=newCode;
	if(root->child!=NULL)
		Coding(root->child,newCode+"0");
	if(root->sibling!=NULL){
		newCode[newCode.length()-1]++;
		Coding(root->sibling,newCode);
	}
	return true;
}

bool Huffman::DeleteNode(Node* root){
	if(root==NULL)
		return false;
	DeleteNode(root->child);
	DeleteNode(root->sibling);
	delete root;
}

bool Huffman::NodeCompare(Node* left,Node* right){
	if(left->frequency!=right->frequency)
		return left->frequency<right->frequency;
	return left->value<right->value;
}

int main(){
	Huffman test;
	while(test.Read())
		test.Solve();
	return 0;
}