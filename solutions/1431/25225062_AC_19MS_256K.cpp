// POJ 1431 - Calendar of Maya
// Model: gpt-6.1-sol
// Find a matching offset in one 18980-day Calendar Round, then repeat it.
// Haab days are numbered from 1, including the five days of Wayeb.
// The interval includes 8.0.0.0.0 and excludes 10.0.0.0.0.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    const string days[] = {"Imix","Ik","Akbal","Kan","Chikchan","Kimi","Manik","Lamat","Muluk","Ok","Chuen","Eb","Ben","Ix","Men","Kib","Kaban","Etznab","Kawak","Ajaw"};
    const string months[] = {"Pohp","Wo","Sip","Zotz","Sek","Xul","Yaxkin","Mol","Chen","Yax","Sak","Keh","Mak","Kankin","Muan","Pax","Kayab","Kumku","Wayeb"};
    int cases; cin >> cases;
    while(cases--) {
        int a,c; string b,d; cin >> a >> b >> c >> d;
        int day=-1, month=-1;
        for(int j=0;j<20;++j) if(days[j]==b) day=j;
        for(int j=0;j<19;++j) if(months[j]==d) month=j;
        vector<int> answer;
        if(a>=1 && a<=13 && day>=0 && month>=0 && c>=1 && c<=(month==18?5:20)) {
            for(int x=0;x<18980;++x) {
                if((8+x)%13+1==a && (19+x)%20==day && (42+x)%365==month*20+c-1) {
                    for(int y=x;y<288000;y+=18980) answer.push_back(1152000+y);
                    break;
                }
            }
        }
        cout << answer.size() << '\n';
        for(unsigned int j=0;j<answer.size();++j) {
            int v=answer[j];
            cout << v/144000 << '.'; v%=144000;
            cout << v/7200 << '.'; v%=7200;
            cout << v/360 << '.'; v%=360;
            cout << v/20 << '.' << v%20 << '\n';
        }
    }
    return 0;
}
