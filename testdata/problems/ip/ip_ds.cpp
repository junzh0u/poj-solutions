#include <stdio.h>

union ip { 
  unsigned a32; 
  unsigned char i [4];
};

void fscanip (FILE* f, ip& i) {
  unsigned int a,b,c,d;
  fscanf (f, "%u.%u.%u.%u\n", &a,&b,&c,&d);
  i.i[3] = a; i.i[2] = b; i.i[1] = c; i.i[0] = d;
}

int main () {
  FILE* fi = fopen ("ip.in","rt");
  FILE* fo = fopen ("ip.out","wt");

  int m;
  ip base, mask;
  mask.a32 = 0xFFFFFFFF;

  fscanf (fi, "%d\n", &m);
  fscanip (fi, base);
  for (int i = 1; i < m; i++) {
    ip curr;
    fscanip (fi, curr);
    for (unsigned diff = (curr.a32 ^ base.a32); diff; diff &= diff - 1)
      mask.a32 &= ~(diff | (diff-1));
  }

  base.a32 &= mask.a32;

  fprintf (fo, "%u.%u.%u.%u\n", base.i[3], base.i[2], base.i[1], base.i[0]);
  fprintf (fo, "%u.%u.%u.%u\n", mask.i[3], mask.i[2], mask.i[1], mask.i[0]);

  fclose (fo);
  fclose (fi);
  return 0;
}
