#include<cstdio>
#include<algorithm>
using namespace std;

bool TryChannel(int repeater,int* channels,int channelNum,int repeaterNum,bool **adjacent){
    if(repeater>=repeaterNum){
        return true;
    }
    for(int channel=0;channel<channelNum;channel++){
        bool interfere=false;
        for(int prev=0;prev<repeater;prev++){
            if(adjacent[repeater][prev] && channels[prev]==channel){
                interfere=true;
                break;
            }
        }
        if(interfere){
            continue;
        }
        channels[repeater]=channel;
        if(TryChannel(repeater+1,channels,channelNum,repeaterNum,adjacent)){
            return true;
        }
    }
    return false;
}

bool ChannelEnough(int channelNum,int repeaterNum,bool **adjacent){
    int *channels=new int[repeaterNum];
    bool result=TryChannel(0,channels,channelNum,repeaterNum,adjacent);
    delete[] channels;
    return result;
}
int main(){
    while(true){
        int repeaterNum;
        scanf("%d",&repeaterNum);
        if(repeaterNum==0){
            break;
        }
        bool **adjacent=new bool*[repeaterNum];
        for(int i=0;i<repeaterNum;i++){
            adjacent[i]=new bool[repeaterNum];
            fill(adjacent[i],adjacent[i]+repeaterNum,false);
        }
        char *line=new char[repeaterNum+2];
        for(int i=0;i<repeaterNum;i++){
            scanf("%s",line);
            for(int j=2;line[j]!='\0';j++){
                adjacent[line[0]-'A'][line[j]-'A']=true;
            }
        }
        delete[] line;
        for(int channelNum=1;channelNum<=repeaterNum;channelNum++){
            if(ChannelEnough(channelNum,repeaterNum,adjacent)){
                printf("%d %s needed.\n",channelNum,channelNum==1?"channel":"channels");
                break;
            }
        }
        for(int i=0;i<repeaterNum;i++){
            delete[] adjacent[i];
        }
        delete[] adjacent;
    }
    return 0;
}