// doors.cpp
#include <fstream.h>
#include <iomanip.h>
#include <math.h>
const XDIM = 18+2;              // max number of x coord:  18 walls, 2 ends
const YDIM = 2*2;               // number of y coord for each wall
const BIGDIST = 100.0;          // edge length if edge disallowed
int nw;                         // number of inside walls with doors
float x[XDIM],                   // x coord of walls, x[0]=0;x[nw+1]=10;
      y[XDIM][YDIM];             // all y coord, depending on xcoord
float d[XDIM][YDIM][XDIM][YDIM]; // d[i1][j1][12][j2] is direct dist 
     // between points (x[i1],y[i1][j1]) and (x[i2],y[i2][j2]) if possible

void findLineDist()            // calc straight line dist, where allowed
{
  for(int i1 = 0; i1 <= nw; i1++)              // vary init pt
    for(int j1 = 0; j1 < YDIM; j1++) {
      for(int i3 = i1+1; i3 <= nw+1; i3++)     // vary final pt
        for(int j3 = 0; j3 < YDIM; j3++) {
          float dy = y[i3][j3] - y[i1][j1],
                dx = x[i3] - x[i1], 
                m = dy/dx;
          int segAllowed = 1;
          for(int i2 = i1+1; i2 < i3; i2++) {  // find blocking walls
            float ymid =  y[i1][j1] + m*(x[i2] - x[i1]); // where hit wall
            if ((ymid < y[i2][0]) || ((ymid > y[i2][1]) && (ymid < y[i2][2]))
                                  || (ymid > y[i2][3])) {
              segAllowed = 0;                  // not through open doorway
              break;
            }
          }
          d[i1][j1][i3][j3] = (segAllowed ? sqrt(dx*dx + dy*dy) :  BIGDIST);
          if (i3 == nw+1) break;           // x[nw+1][1..3] not used
        }
      if (i1 == 0) break;                  // x[0][1..3] not used
    }
}

float mindist()
{
  int i2, i3, j2, j3, edges;
  float dmin[XDIM][YDIM]; // shortest path length from starting point

  findLineDist();
  for(i2 = 1; i2 <= nw+1; i2++)
    for(j2 = 0; j2 < YDIM; j2++) {
      dmin[i2][j2] = d[0][0][i2][j2];
      if (i2 == nw+1) break;       // x[nw+1][1..3] not used
    }
  for(edges = 1; edges <= nw; edges++)
    for(i3 = 2; i3 <= nw+1; i3++)
      for(j3 = 0; j3 < YDIM; j3++) {
        for(i2 = 1; i2 < i3; i2++)
          for(j2 = 0; j2 < YDIM; j2++) {
            float temp = dmin[i2][j2]+d[i2][j2][i3][j3];
              if (temp < dmin[i3][j3]) dmin[i3][j3] = temp;
            }
        if (i3 == nw+1) break;         // x[nw+1][1..3] not used
      }
  return dmin[nw+1][0];
}

void main()
{
  ifstream inf("doors.in");
  ofstream outf("doors.out");
  outf << setiosflags(ios::showpoint);  // show trailing 00's
  while (inf >> nw, nw >= 0) {
    for(int i=1; i <= nw; i++)
      inf >> x[i] >> y[i][0] >> y[i][1] >> y[i][2] >> y[i][3];
    x[0] = 0;  x[nw+1] = 10;     // end walls
    y[0][0] = y[nw+1][0] = 5;    // x[0][1..3], x[nw+1][1..3] not used
    outf << setw(5) << setprecision(2) << mindist() << endl;
  }
}
