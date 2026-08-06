#include<iostream>
#include<vector>
using namespace std;

int main(){
	int caseID=1;
	while(true){
		vector<int> heights;
		while(true){
			int height;
			cin>>height;
			if(height==-1)
				break;
			heights.push_back(height);
		}
		if(heights.empty())
			break;
		int size=heights.size();
		int *decrease=new int[size],max=0;
		for(int i=0;i<size;i++){
			decrease[i]=1;
			for(int j=0;j<i;j++)
				if(heights[i]<=heights[j] && decrease[i]<decrease[j]+1)
					decrease[i]=decrease[j]+1;
			if(max<decrease[i])
				max=decrease[i];
		}
		delete[] decrease;
		cout<<"Test #"<<caseID++<<":\n  maximum possible interceptions: "<<max<<endl<<endl;
	}
	return 0;
}