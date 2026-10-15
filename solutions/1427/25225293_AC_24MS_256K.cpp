// POJ 1427 - Get Them All
// Model: gpt-6.1-sol
// Process arrivals in chronological order, breaking ties by first departure.
// Each junction remembers every departure's destination, including full buses.
// Requests at the same instant, even at different junctions, create one bus.
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
struct Event {
    int time, id, junction;
    Event(int a,int b,int c):time(a),id(b),junction(c){}
    bool operator<(const Event &b) const {
        if(time!=b.time) return time>b.time;
        return id>b.id;
    }
};
int main(){
    string name;
    while(cin>>name && name!="TheEnd"){
        int n,s,t,d[11][11],waiting[11]={0},last[11],limit,total=0;
        cin>>n>>s>>t;
        for(int i=0;i<n;++i){
            last[i]=-1;
            for(int j=0;j<n;++j) if(i!=j) cin>>d[i][j];
        }
        for(int i=1;i<n;++i){cin>>waiting[i];total+=waiting[i];}
        cin>>limit;
        vector<int> capacity(1,max(s,3)),load(1,0);
        priority_queue<Event> q;
        q.push(Event(0,0,0));
        int delivered=0,finish=0,request=-1,nextCapacity=max(3,max(s,3)-t);
        while(!q.empty() && delivered<total){
            Event e=q.top();q.pop();
            int j=e.junction,id=e.id,now=e.time;
            if(j==0){
                delivered+=load[id];load[id]=0;
                if(delivered==total){finish=now;break;}
            }else{
                int take=min(waiting[j],capacity[id]-load[id]);
                waiting[j]-=take;load[id]+=take;
                if(waiting[j] && request!=now){
                    request=now;
                    if(limit-now>=2){
                        int newId=capacity.size();
                        capacity.push_back(nextCapacity);load.push_back(0);
                        nextCapacity=max(3,nextCapacity-t);
                        q.push(Event(now+2,newId,0));
                    }
                }
            }
            int k;
            if(load[id]==capacity[id]) k=0;
            else if(last[j]<0) k=(j+1)%n;
            else {k=(last[j]+1)%n;if(k==j) k=(k+1)%n;}
            last[j]=k;
            if(d[j][k]<=limit-now) q.push(Event(now+d[j][k],id,k));
        }
        cout<<name<<'\n';
        if(delivered==total) cout<<finish<<" seconds needed\n";
        else cout<<delivered<<" contestants reached\n";
    }
    return 0;
}
