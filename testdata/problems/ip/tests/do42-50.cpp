#include <stdio.h>
#include <stdlib.h>

union ip { 
  unsigned a; 
  unsigned char i [4];
};

int rr() {
	return (rand() << 16) | rand();
}

int main () {
	srand(23497);

	int t = 41;

	for (int tt = 42; tt <= 50; tt++) {
		t++;
		char fname[3];
		sprintf(fname, "%.2d", t);
		FILE* fo = fopen(fname, "w");

		int i = rand() % 33;

		int n = 1000;
		ip mask;
		if (i != 32) {
			mask.a = (-1) << i;
		} else {
			mask.a = 0;
		}	
		ip addr;
		addr.a = rr() & mask.a;

		ip a[1000];
		a[0].a = addr.a | (~mask.a & ((1 << (i - 1)) | rr()));
		a[1].a = addr.a | (~mask.a & ~(1 << (i - 1)) & rr());
		int j;
		for (j = 2; j < n; j++) {
			a[j].a = addr.a | (~mask.a & rr());
		}

		int p[1000];
		for (j = 0; j < n; j++) {
			int k = rr() % (j + 1);
			p[j] = p[k]; 
			p[k] = j; 
		}

		fprintf(fo, "%d\n", n);
		for (j = 0; j < n; j++) {
			fprintf(fo, "%u.%u.%u.%u\n", a[p[j]].i[3], a[p[j]].i[2], a[p[j]].i[1], a[p[j]].i[0]);
		}
    	fclose (fo);
    }

    return 0;
}
