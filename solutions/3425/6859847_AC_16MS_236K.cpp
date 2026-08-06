#include<iostream>
#include<map>
using namespace std;

int main(){
	int answerNum;
	cin>>answerNum;
	map<int,int> correctAnswerNum;
	int payment=0;
	while(answerNum--){
		int questionID;
		bool correct,withExplanaion;
		cin>>questionID>>correct>>withExplanaion;
		if(correct){
			if(withExplanaion)
				payment+=40;
			else
				payment+=20;
			if(correctAnswerNum.find(questionID)==correctAnswerNum.end())
				correctAnswerNum[questionID]=1;
			else{
				payment+=10*correctAnswerNum[questionID];
				correctAnswerNum[questionID]++;
			}
		}
		else
			payment+=10;
	}
	cout<<payment<<endl;
	return 0;
}