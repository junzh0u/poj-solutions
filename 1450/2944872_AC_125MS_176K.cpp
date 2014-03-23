#include<iostream>
using namespace std;

int main(){
	int caseNum;
	cin>>caseNum;
	for(int ID=1;ID<=caseNum;ID++){
		int row,col;
		cin>>row>>col;
		cout<<"Scenario #"<<ID<<":\n"<<row*col<<((row*col%2==0)?".00\n":".41\n")<<endl;
	}
	return 0;
}