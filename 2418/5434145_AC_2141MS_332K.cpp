#include<cstdio>
#include<map>
#include<string>
using namespace std;

const int NAME_LENGTH_MAX=30;

int main(){
	char treeCstr[NAME_LENGTH_MAX+1];
	map<string,int> species;
	int count=0;
	while(gets(treeCstr)){
		//puts(tree);
		string treeString(treeCstr);
		if(species.find(treeString)!=species.end())
			species[treeString]++;
		else
			species[treeString]=1;
		count++;
		//printf("%d\n",species[tree]);
	}
	for(map<string,int>::iterator iter=species.begin();iter!=species.end();iter++)
		printf("%s %.4f\n",iter->first.c_str(),iter->second*100.0/count);
	return 0;
}