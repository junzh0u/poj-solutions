#include<iostream>
#include<map>
#include<string>
#include<cctype>
using namespace std;

bool IsConsonant(char letter)
{
	if(!isalpha(letter))
		return false;
	letter=tolower(letter);
	if(letter=='a' || letter=='e' || letter=='i' || letter=='o' || letter=='u')
		return false;
	return true;
}

void main()
{
	typedef map<string,string> DICTIONARY;
	DICTIONARY irregularWord;
	int irregularNum,wordNum;
	cin>>irregularNum>>wordNum;
	string word,plural;
	while(irregularNum--)
	{
		cin>>word>>plural;
		irregularWord[word]=plural;
	}
	while(wordNum--)
	{
		cin>>word;
		if(irregularWord.find(word)!=irregularWord.end())
			cout<<irregularWord[word]<<endl;
		else if(word[word.length()-1]=='y' && IsConsonant(word[word.length()-2]))
		{
			word.erase(&word[word.length()-1],word.end());
			cout<<word<<"ies"<<endl;
		}
		else if(word[word.length()-1]=='o'
			|| word[word.length()-1]=='x'
			|| word[word.length()-1]=='s'
			|| (word[word.length()-1]=='h' && word[word.length()-2]=='c')
			|| (word[word.length()-1]=='h' && word[word.length()-2]=='s'))
			cout<<word<<"es"<<endl;
		else
			cout<<word<<"s"<<endl;
	}
}