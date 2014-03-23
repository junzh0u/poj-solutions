#include<iostream>
#include<string>
#include<algorithm>
#include<map>
using namespace std;

const int SIZE=20,INF=-1;
string IDs[SIZE],names[SIZE];
map<string,string> IDOfName;
int IDNum=0,nameNum=0;

class BoolArray{
public:
	BoolArray(bool default=true){
		fill(array,array+SIZE,default);
	}
	bool& operator[](int sub){
		return array[sub];
	}
	int Count(int size){
		int count=0;
		for(int i=0;i<size;i++)
			if(array[i])
				count++;
		return count;
	}
	void Union(BoolArray right){
		for(int i=0;i<SIZE;i++)
			if(right[i])
				array[i]=true;
	}
private:
	bool array[SIZE];
};

BoolArray inHideout(false),matrix[SIZE];

int Index(string value,string array[],int size){
	for(int i=0;i<size;i++)
		if(array[i]==value)
			return i;
	return INF;
}

int Remain(int begin,int end,int breakPoint){
	if(begin<=breakPoint && breakPoint<end)
		return end-begin-1;
	return end-begin;
}

bool Check(int begin,int end,int breakPoint,BoolArray current,int nameCount,int testID){
	int IDCount=current.Count(IDNum);
	if(IDCount<nameCount)
		return false;
	if(begin==end)
		return true;
	if(begin==breakPoint || matrix[begin][testID])
		return Check(begin+1,end,breakPoint,current,nameCount,testID);
	if(IDCount>=nameCount+Remain(begin,end,breakPoint))
		return true;
	if(!Check(begin+1,end,breakPoint,current,nameCount,testID))
		return false;
	current.Union(matrix[begin]);
	return Check(begin+1,end,breakPoint,current,nameCount+1,testID);
}

bool Only(int name,int ID){
	BoolArray current=matrix[name];
	current[ID]=false;
	return !Check(0,nameNum,name,current,1,ID);
}

int main(){
	cin>>IDNum;
	for(int i=0;i<IDNum;i++)
		cin>>IDs[i];
	char event;
	while(cin>>event){
		if(event=='Q')
			break;
		if(event=='E'){
			string name;
			cin>>name;
			int index=Index(name,names,nameNum);
			if(index==INF)
				names[index=nameNum++]=name;
			inHideout[index]=true;
		}
		if(event=='L'){
			string name;
			cin>>name;
			inHideout[Index(name,names,nameNum)]=false;
		}
		if(event=='M'){
			string ID;
			cin>>ID;
			int index=Index(ID,IDs,IDNum);
			for(int user=0;user<IDNum;user++)
				if(!inHideout[user])
					matrix[user][index]=false;
		}
	}
	for(int name=0;name<nameNum;name++){
		IDOfName[names[name]]="???";
		for(int ID=0;ID<IDNum;ID++)
			if(matrix[name][ID] && Only(name,ID)){
				IDOfName[names[name]]=IDs[ID];
				for(int i=0;i<nameNum;i++)
					if(i!=name)
						matrix[i][ID]=false;
				for(int i=0;i<IDNum;i++)
					if(i!=ID)
						matrix[name][i]=false;
				break;
			}
	}
	for(map<string,string>::iterator iter=IDOfName.begin();iter!=IDOfName.end();iter++)
		cout<<iter->first<<":"<<iter->second<<endl;
	return 0;
}