// POJ 3019 - Image Rotation
// Model: gpt-6.1-sol
// Rotate the rectangle of pixel centers and inverse-map its bounding grid.
// Extents are h-1 and w-1; the output grid starts at the exact top-left
// bounding corner, not at a grid recentered after flooring its dimensions.
#include <cstdio>
#include <cmath>
int main() {
    int h,w;
    double a;
    const double eps=1e-9;
    while (std::scanf("%d%d%lf", &h,&w,&a)==3) {
        double s=std::sin(a*std::acos(-1.0)/180.0);
        double c=std::cos(a*std::acos(-1.0)/180.0);
        if (std::fabs(s)<eps) s=0;
        if (std::fabs(c)<eps) c=0;
        double height=(h-1)*std::fabs(c)+(w-1)*std::fabs(s);
        double width=(w-1)*std::fabs(c)+(h-1)*std::fabs(s);
        int H=(int)std::floor(height+eps)+1;
        int W=(int)std::floor(width+eps)+1;
        std::printf("%d %d\n",H,W);
        for(int i=0;i<H;++i) {
            for(int j=0;j<W;++j) {
                double X=j-width/2, Y=height/2-i;
                double x=c*X+s*Y+(w-1)/2.0;
                double y=-s*X+c*Y+(h-1)/2.0;
                if(x < -eps || x > w-1+eps || y < -eps || y > h-1+eps)
                    x=y=-1;
                else {
                    if(std::fabs(x)<eps) x=0;
                    if(std::fabs(y)<eps) y=0;
                }
                if(j) std::printf(" ");
                std::printf("(%.2f,%.2f)",y,x);
            }
            std::printf("\n");
        }
        std::printf("\n");
    }
    return 0;
}
