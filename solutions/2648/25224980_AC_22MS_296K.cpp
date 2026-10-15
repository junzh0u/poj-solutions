// POJ 2648 - CDVII
// Model: gpt-6.1-sol
// Sort each vehicle's records and charge only adjacent enter/exit pairs.
// The entry hour chooses the rate; omit vehicles without a completed trip.
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>
using namespace std;
struct Photo {
    int time, hour;
    long long location;
    bool enter;
    bool operator<(const Photo& b) const { return time < b.time; }
};
int main() {
    long long rate[24];
    for (int i=0;i<24;++i) if (!(cin>>rate[i])) return 0;
    map<string, vector<Photo> > records;
    string plate, stamp, action;
    long long location;
    while (cin>>plate>>stamp>>action>>location) {
        int month, day, hour, minute;
        sscanf(stamp.c_str(), "%d:%d:%d:%d", &month,&day,&hour,&minute);
        Photo p;
        p.time=((month*32+day)*24+hour)*60+minute;
        p.hour=hour; p.location=location; p.enter=(action=="enter");
        records[plate].push_back(p);
    }
    for (map<string, vector<Photo> >::iterator it=records.begin();it!=records.end();++it) {
        vector<Photo>& a=it->second;
        sort(a.begin(),a.end());
        long long bill=0;
        for (size_t i=1;i<a.size();++i) if(a[i-1].enter && !a[i].enter) {
            long long distance=a[i].location-a[i-1].location;
            if(distance<0) distance=-distance;
            bill+=100+distance*rate[a[i-1].hour];
        }
        if (bill) {
            bill+=200;
            cout<<it->first<<" $"<<bill/100<<'.'<<setfill('0')<<setw(2)<<bill%100<<'\n';
        }
    }
    return 0;
}
