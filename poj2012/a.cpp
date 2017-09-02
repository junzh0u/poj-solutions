#include<iostream>
using namespace std;
 
int main()
{
    char cards[13][5];
    int point =0;
    for(int i=0;i<13;i++){
        cin>>cards[i];
        switch(cards[i][1]){
            case 'A':
                point+=4;
                break;
            case 'K':
                point+=3;
                break;
            case 'Q':
                point+=2;
                break;
            case 'J':
                point+=1;
                break;
        }
    }
    cout<<point<<endl;
    return 0;
}
