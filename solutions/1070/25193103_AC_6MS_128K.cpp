// POJ 1070 - Deformed Wheel
//
// The hill is a chain of segments whose slopes decrease down to zero.  The
// first (steepest) segment is the right-most one and its right end-point (the
// hill top) is given; every following segment is attached to the left end of
// the previous one, so the hill descends towards the left and finishes with the
// horizontal segment.  Hence the wheel rolls leftwards, i.e. counter-clockwise.
//
// Quasi-static simulation: the wheel is supported by the contact points between
// its boundary and the hill.  If the centre of gravity lies to the left of the
// left-most contact it tips over it (counter-clockwise); if it lies to the
// right of the right-most contact it tips over that one (clockwise); otherwise
// it rests.  A tip is a rotation about that contact point, stopped either by
// the first new touch between the wheel and the hill or by the centre of
// gravity reaching its lowest position (straight below the pivot).

#include<cstdio>
#include<cmath>
#include<vector>
#include<algorithm>
using namespace std;

const double PI=acos(-1.0);
const double EPS=1e-9;
const double TOUCH_EPS=1e-7;    // slack used when collecting the contact points
const double ANG_EPS=1e-9;      // rotations below this are considered to be none

struct Point{
    double x,y;
    Point(double x_=0,double y_=0):x(x_),y(y_){}
};

Point operator+(const Point &a,const Point &b){return Point(a.x+b.x,a.y+b.y);}
Point operator-(const Point &a,const Point &b){return Point(a.x-b.x,a.y-b.y);}
Point operator*(const Point &a,double t){return Point(a.x*t,a.y*t);}
double Dot(const Point &a,const Point &b){return a.x*b.x+a.y*b.y;}
double Length(const Point &a){return sqrt(Dot(a,a));}
double Angle(const Point &a){return atan2(a.y,a.x);}

Point Rotate(const Point &p,const Point &pivot,double angle){
    Point d=p-pivot;
    double c=cos(angle),s=sin(angle);
    return Point(pivot.x+d.x*c-d.y*s,pivot.y+d.x*s+d.y*c);
}

double DistanceToSegment(const Point &p,const Point &a,const Point &b){
    Point ab=b-a;
    double squaredLength=Dot(ab,ab);
    double t=squaredLength<EPS?0.0:Dot(p-a,ab)/squaredLength;
    if(t<0)t=0;
    else if(t>1)t=1;
    return Length(p-(a+ab*t));
}

// Amount to turn counter-clockwise to get from angle 'from' to angle 'to'.
// Turns of (almost) zero are reported as a full turn so that contacts which are
// already touching, and which the rotation immediately lifts, are ignored.
double CounterClockwiseDelta(double from,double to){
    double delta=fmod(to-from,2*PI);
    while(delta<ANG_EPS)delta+=2*PI;
    while(delta>=2*PI+ANG_EPS)delta-=2*PI;
    return delta;
}

// The points where the circle (centre,radius) crosses the segment [a,b].
void CircleSegmentPoints(const Point &centre,double radius,const Point &a,const Point &b,vector<Point> &result){
    Point ab=b-a,ac=a-centre;
    double qa=Dot(ab,ab);
    if(qa<EPS)return;
    double qb=2*Dot(ac,ab),qc=Dot(ac,ac)-radius*radius;
    double discriminant=qb*qb-4*qa*qc;
    if(discriminant<0)return;
    double root=sqrt(discriminant);
    for(int sign=-1;sign<=1;sign+=2){
        double t=(-qb+sign*root)/(2*qa);
        if(t>-EPS && t<1+EPS)
            result.push_back(a+ab*t);
        if(root<EPS)break;
    }
}

class Wheel{
private:
    vector<Point> vertices;         // the wheel, counter-clockwise or not, it does not matter
    Point gravity;
    vector<Point> hill;             // hill vertices, from the left (horizontal) end to the top

public:
    void Input(){
        int vertexNum;
        scanf("%d",&vertexNum);
        vertices.resize(vertexNum);
        for(int i=0;i<vertexNum;i++)
            scanf("%lf %lf",&vertices[i].x,&vertices[i].y);
        scanf("%lf %lf",&gravity.x,&gravity.y);
        vector<double> lengths,slopes;
        while(true){
            double length,slope;
            if(scanf("%lf %lf",&length,&slope)!=2)break;
            lengths.push_back(length);
            slopes.push_back(slope);
            if(fabs(slope)<EPS)break;
        }
        Point top;
        scanf("%lf %lf",&top.x,&top.y);
        // Walk down the hill: each segment hangs on the left end of the previous one.
        hill.assign(1,top);
        for(unsigned i=0;i<lengths.size();i++){
            double norm=sqrt(1+slopes[i]*slopes[i]);
            Point direction(1/norm,slopes[i]/norm);
            hill.push_back(hill.back()-direction*lengths[i]);
        }
        reverse(hill.begin(),hill.end());
    }

    void Roll(){
        for(int step=0;step<1000;step++){
            Point left,right;
            Contacts(left,right);
            int direction;
            if(gravity.x<left.x-TOUCH_EPS)direction=1;          // tips counter-clockwise over the left contact
            else if(gravity.x>right.x+TOUCH_EPS)direction=-1;   // tips clockwise over the right contact
            else break;                                         // the wheel rests
            Point pivot=direction>0?left:right;
            double angle=min(TouchAngle(pivot,direction),LowestGravityAngle(pivot,direction));
            if(angle<ANG_EPS)break;
            for(unsigned i=0;i<vertices.size();i++)
                vertices[i]=Rotate(vertices[i],pivot,direction*angle);
            gravity=Rotate(gravity,pivot,direction*angle);
        }
    }

    void Output()const{
        // avoid printing "-0.000"
        double x=gravity.x,y=gravity.y;
        if(fabs(x)<5e-4)x=0;
        if(fabs(y)<5e-4)y=0;
        printf("%.3f %.3f\n",x,y);
    }

private:
    // The left-most and the right-most point where the wheel touches the hill.
    // A contact is either a wheel vertex lying on a hill segment or a hill
    // vertex lying on a wheel edge; the closest ones are taken so that inputs
    // which do not touch exactly still behave sensibly.
    void Contacts(Point &left,Point &right)const{
        vector<Point> points;
        vector<double> distances;
        for(unsigned i=0;i<vertices.size();i++)
            for(unsigned j=0;j+1<hill.size();j++){
                points.push_back(vertices[i]);
                distances.push_back(DistanceToSegment(vertices[i],hill[j],hill[j+1]));
            }
        for(unsigned i=0;i<hill.size();i++)
            for(unsigned j=0;j<vertices.size();j++){
                points.push_back(hill[i]);
                distances.push_back(DistanceToSegment(hill[i],vertices[j],vertices[(j+1)%vertices.size()]));
            }
        double best=distances[0];
        for(unsigned i=1;i<distances.size();i++)
            best=min(best,distances[i]);
        left=right=points[0];
        bool first=true;
        for(unsigned i=0;i<points.size();i++){
            if(distances[i]>best+TOUCH_EPS)continue;
            if(first || points[i].x<left.x)left=points[i];
            if(first || points[i].x>right.x)right=points[i];
            first=false;
        }
    }

    // How far the wheel can turn around the pivot before touching the hill again.
    double TouchAngle(const Point &pivot,int direction)const{
        double best=2*PI;
        for(unsigned i=0;i<vertices.size();i++){
            double radius=Length(vertices[i]-pivot);
            if(radius<EPS)continue;
            double from=Angle(vertices[i]-pivot);
            for(unsigned j=0;j+1<hill.size();j++){
                vector<Point> hits;
                CircleSegmentPoints(pivot,radius,hill[j],hill[j+1],hits);
                for(unsigned k=0;k<hits.size();k++){
                    double to=Angle(hits[k]-pivot);
                    best=min(best,direction>0?CounterClockwiseDelta(from,to):CounterClockwiseDelta(to,from));
                }
            }
        }
        for(unsigned i=0;i<hill.size();i++){
            double radius=Length(hill[i]-pivot);
            if(radius<EPS)continue;
            double from=Angle(hill[i]-pivot);
            for(unsigned j=0;j<vertices.size();j++){
                vector<Point> hits;
                CircleSegmentPoints(pivot,radius,vertices[j],vertices[(j+1)%vertices.size()],hits);
                for(unsigned k=0;k<hits.size();k++){
                    double to=Angle(hits[k]-pivot);
                    // in the frame of the wheel the hill vertex turns the other way round
                    best=min(best,direction>0?CounterClockwiseDelta(to,from):CounterClockwiseDelta(from,to));
                }
            }
        }
        return best;
    }

    // How far the wheel can turn around the pivot before the centre of gravity
    // stops descending, that is, before it hangs right below the pivot.
    double LowestGravityAngle(const Point &pivot,int direction)const{
        Point relative=gravity-pivot;
        if(Length(relative)<EPS)return 0;
        double from=Angle(relative),bottom=-PI/2;
        return direction>0?CounterClockwiseDelta(from,bottom):CounterClockwiseDelta(bottom,from);
    }
};

int main(){
    int caseNum;
    scanf("%d",&caseNum);
    Wheel wheel;
    while(caseNum-->0){
        wheel.Input();
        wheel.Roll();
        wheel.Output();
    }
    return 0;
}
