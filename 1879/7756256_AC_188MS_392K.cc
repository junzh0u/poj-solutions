#include<cstdio>
#include<algorithm>
using namespace std;

const int BALL_NUM_MIN=27,BALL_NUM_MAX=127;

int ballNum;
int shiftCode[BALL_NUM_MAX],status[BALL_NUM_MAX],temp[BALL_NUM_MAX];

const int QUEUE_SIZE=BALL_NUM_MAX+1;
int bottom[QUEUE_SIZE],bottomBegin,bottomEnd;
int minute[5],minuteEnd;
int fiveMinute[12],fiveMinuteEnd;
int hour[12],hourEnd;

void generateShiftCode() {
    bottomBegin=0;
    bottomEnd=ballNum;
    for(int i=bottomBegin; i<bottomEnd; i++)
        bottom[i]=i;
    minuteEnd=fiveMinuteEnd=hourEnd=0;

    for(int i=0; i<24*60; i++) {
        int ball=bottom[bottomBegin];
        bottomBegin=(bottomBegin+1)%QUEUE_SIZE;
        if(minuteEnd<4)
            minute[minuteEnd++]=ball;
        else {
            while(minuteEnd>0) {
                bottom[bottomEnd]=minute[minuteEnd-1];
                minuteEnd--;
                bottomEnd=(bottomEnd+1)%QUEUE_SIZE;
            }
            if(fiveMinuteEnd<11)
                fiveMinute[fiveMinuteEnd++]=ball;
            else {
                while(fiveMinuteEnd>0) {
                    bottom[bottomEnd]=fiveMinute[fiveMinuteEnd-1];
                    fiveMinuteEnd--;
                    bottomEnd=(bottomEnd+1)%QUEUE_SIZE;
                }
                if(hourEnd<11)
                    hour[hourEnd++]=ball;
                else {
                    while(hourEnd>0) {
                        bottom[bottomEnd]=hour[hourEnd-1];
                        hourEnd--;
                        bottomEnd=(bottomEnd+1)%QUEUE_SIZE;
                    }
                    bottom[bottomEnd]=ball;
                    bottomEnd=(bottomEnd+1)%QUEUE_SIZE;
                }
            }
        }
    }
    for(int i=0;i<ballNum;i++)
        shiftCode[i]=bottom[(bottomBegin+i)%QUEUE_SIZE];
}

bool returned(){
    for(int i=0;i<ballNum;i++)
        if(status[i]!=i)
            return false;
    return true;
}

int answerTable[BALL_NUM_MAX];

int main() {
    for(ballNum=BALL_NUM_MIN;ballNum<=BALL_NUM_MAX;ballNum++){
        generateShiftCode();
        copy(shiftCode,shiftCode+ballNum,status);
        for(int period=1;;period++){
            if(returned()){
                //printf("%d balls cycle after %d days.\n",ballNum,period);
                answerTable[ballNum]=period;
                break;
            }
            for(int i=0;i<ballNum;i++)
                temp[i]=status[shiftCode[i]];
            copy(temp,temp+ballNum,status);
        }
    }
    //printf("Done!\n");
    while(true) {
        scanf("%d",&ballNum);
        if(ballNum==0)
            return 0;
        printf("%d balls cycle after %d days.\n",ballNum,answerTable[ballNum]);
    }
}
