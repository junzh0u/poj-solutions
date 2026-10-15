// POJ 1567 - WIMP
// Model: gpt-6.1-sol
// Simulate the back-to-front window stack and the current mouse gesture.
// Coordinates are inclusive; zoom restores the pre-zoom rectangle, including
// its position. Both AT and the final UP report a drag's current rectangle.
#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct Rect { int l,t,r,b; };
struct Window { Rect box, saved; int id; bool zoom; };
int area(const Rect &a,int x,int y) {
    if(x<a.l || x>a.r || y<a.t || y>a.b) return 0;
    if(y>=a.t+25) return 4;
    if(x<a.l+25) return 1;
    if(x>a.r-25) return 2;
    return 3;
}
void print(const string &prefix,const Window &w,const string &join) {
    cout<<prefix<<w.id<<join<<w.box.l<<", "<<w.box.t<<", "<<w.box.r<<", "<<w.box.b<<'\n';
}
int main() {
    vector<Window> ws;
    string op; int next=0, gesture=0, px=0,py=0;
    while(cin>>op && op!="ZZ") {
        if(op=="CR") {
            Window w; cin>>w.box.l>>w.box.t>>w.box.r>>w.box.b;
            w.id=next++; w.zoom=false; w.saved=w.box;
            ws.push_back(w); print("Created window ",w," at ");
        } else if(op=="RE") {
            for(unsigned i=0;i<ws.size();++i) print("Window ",ws[i]," at ");
        } else {
            int x,y; cin>>x>>y;
            if(op=="DN") {
                for(int i=(int)ws.size()-1;i>=0;--i) if(area(ws[i].box,x,y)) {
                    Window w=ws[i]; ws.erase(ws.begin()+i); ws.push_back(w);
                    cout<<"Selected window "<<w.id<<'\n';
                    gesture=area(w.box,x,y);
                    if(gesture==4 || (gesture==3 && w.zoom)) gesture=0;
                    px=x; py=y; break;
                }
            } else if(op=="AT" || op=="UP") {
                if(!ws.empty()) {
                    Window &w=ws.back();
                    if(gesture==3) {
                        w.box.l+=x-px; w.box.r+=x-px;
                        w.box.t+=y-py; w.box.b+=y-py;
                        px=x; py=y; print("Moved window ",w," to ");
                    } else if(op=="UP" && gesture==area(w.box,x,y)) {
                        if(gesture==1) {
                            cout<<"Closed window "<<w.id<<'\n'; ws.pop_back();
                        } else if(gesture==2) {
                            if(w.zoom) w.box=w.saved;
                            else { w.saved=w.box; w.box.l=w.box.t=0; w.box.r=w.box.b=1023; }
                            w.zoom=!w.zoom; print("Resized window ",w," to ");
                        }
                    }
                }
                if(op=="UP") gesture=0;
            }
        }
    }
    return 0;
}
