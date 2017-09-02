#include <cstdio>
#include <vector>
#include <iostream>
#define N 1000000
#define M 1000

using namespace std;



//vector <int> ans[5000];

/*
void output3(int x) {
    int p[10] = {0}, k = 0;
    while(x) {
        p[k ++] = x % 3;
        if(x % 3 == 2) break;
        x /= 3;
    }
    for(int i = k - 1; i >= 0; --i) {
        cout << p[i];
    };
    cout << endl;
}
*/
    bool exist[M];
    int sg[N];
    
int main() {
    for(int i=0;i<M;i++)
        exist[i]=false;
    sg[0] = sg[1] = 0;
    for(int i = 0; i < N; ++i) {
        bool has[N] = {0};
        for(int j = 1; j <= i / 2; ++j) {
            has[sg[i - j] ^sg[i - j * 2]] = 1;
        }
        sg[i] = 0;
        while(sg[i] < N && has[sg[i]]) {
            ++sg[i];
        }
        if(!exist[sg[i]]){
            exist[sg[i]]=true;
            printf("%d ",sg[i]);
        }
        
        //cout << i << " " << sg[i] << " ";
        //output3(i);
        //ans[sg[i]].push_back(i);
    }
    /*
    int hook=2;
    for(int i=0;i<9;i++ && hook<N){
        printf("%3d ",sg[hook]);
        hook*=3;
    }*/
    /*
    for(int i=0; i< N; i++){
        printf("%3d",sg[i]);
        if(i%3==2){
            printf("\t");
        }
    }
    printf("\n######\n");
    for(int i=0; i< N; i+=3){
        printf("%3d",sg[i]);
        if((i/3)%3==2){
            printf("\t");
        }
    }
    printf("\n######\n");
    for(int i=0; i< N; i+=9){
        printf("%3d",sg[i]);
        if((i/9)%3==2){
            printf("\t");
        }
    }
    printf("\n######\n");
    for(int i=0; i< N; i+=27){
        printf("%3d",sg[i]);
        if((i/27)%3==2){
            printf("\t");
        }
    }
    printf("\n######\n");
    for(int i=0; i< N; i+=81){
        printf("%3d",sg[i]);
        if((i/81)%3==2){
            printf("\t");
        }
    }
    printf("\n######\n");
    for(int i=0; i< N; i+=243){
        printf("%3d",sg[i]);
        if((i/243)%3==2){
            printf("\t");
        }
    }
    printf("\n######\n");
    */
    /*
    cout << endl;
    
    for(int i = 0; i < 100; ++i) {
        if(ans[i].size() == 0) continue;
        cout << "i=" << i << endl;
        for(int j = 0; j < ans[i].size(); ++j) {
            //cout << ans[i][j] << " ";
        }
        cout << endl;
    }
    */
    system("pause");
}
