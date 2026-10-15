// POJ 3341 - Calculating Taxi Fare
// Model: gpt-6.1-sol
// Simulate each kilometer, applying its distance tier and night surcharge.
// Night requires positive overlap: ending at midnight or starting at 06:00
// alone is insufficient. Apply the whole-trip traffic surcharge afterward.
#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct Street { string name; int length; long long minutes; };
int main() {
    string name;
    while (cin >> name && name != "--") {
        vector<Street> roads;
        while (name != "$") {
            Street s; s.name = name; cin >> s.length >> s.minutes;
            roads.push_back(s); cin >> name;
        }
        string source, dest, clock, end;
        cin >> source >> dest >> clock >> end;
        int first = 0, last = 0;
        for (int i = 0; i < (int)roads.size(); ++i) {
            if (roads[i].name == source) first = i;
            if (roads[i].name == dest) last = i;
        }
        int time = (clock[0]-'0')*600 + (clock[1]-'0')*60
                 + (clock[3]-'0')*10 + clock[4]-'0';
        long long duration = 0, fare = 0;
        int distance = 0;
        for (int i = first; i <= last; ++i) {
            long long m = roads[i].minutes;
            for (int k = 0; k < roads[i].length; ++k) {
                ++distance;
                int price = distance <= 10 ? 1000 : distance <= 30 ? 250 : 100;
                if (time < 360 || m > 1440-time) price = price*12/10;
                fare += price;
                duration += m;
                time = (time + m%1440)%1440;
            }
        }
        if (duration > 2LL*distance) fare = fare*11/10;
        cout << fare << '\n';
    }
    return 0;
}
