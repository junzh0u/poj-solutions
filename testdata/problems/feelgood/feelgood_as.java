import java.util.Scanner;
import java.util.Arrays;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class feelgood_as {


    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("feelgood.in"));
        int n = in.nextInt();
        Pair p[] = new Pair[n];

        long best = -1;
        int l = -1, r = -1;
        for (int i = 0; i < n; i++) {
            p[i] = new Pair(in.nextInt(), i);
            if (p[i].v * p[i].v > best) {
                best = p[i].v * p[i].v;
                l = i;
                r = i;
            }
        }
        in.close();
        DSU d = new DSU(n, p);

        Arrays.sort(p);

        for (int i = n - 1; i >= 0; i--) {
            if (p[i].i > 0 && d.getMin(p[i].i) <= d.getMin(p[i].i - 1)) {
                d.union(p[i].i,  p[i].i - 1);
            }
            if (p[i].i < n - 1 && d.getMin(p[i].i) <= d.getMin(p[i].i + 1)) {
                d.union(p[i].i,  p[i].i + 1);
            }
            long value = d.getValue(p[i].i);
            if (value > best) {
                best = value;
                l = d.getLeft(p[i].i);
                r = d.getRight(p[i].i);
            }
        }

        PrintWriter out = new PrintWriter(new File("feelgood.out"));
        out.println(best);
        out.println((l + 1) + " " + (r + 1));
        out.close();
    }

    public static void main(String[] args) throws FileNotFoundException {
        new feelgood_as().run();
    }

    class Pair implements Comparable<Pair> {
        long v;
        int i;

        public Pair(long v, int i) {
            this.v = v;
            this.i = i;
        }

        public int compareTo(Pair o) {
            return (int)(v - o.v);
        }

        public String toString() {
            return "[" + i + "] = " + v;
        }
    }

    class DSU {
        int n;
        int[] p;
        int[] r;

        int[] lft;
        int[] rgt;
        long[] sum;
        long[] min;

        public DSU(int n, Pair[] q) {
            this.n = n;
            p = new int[n];
            r = new int[n];
            lft = new int[n];
            rgt = new int[n];
            sum = new long[n];
            min = new long[n];
            for (int i = 0; i < n; i++) {
                p[i] = i;
                min[i] = q[i].v;
                sum[i] = q[i].v;
                lft[i] = q[i].i;
                rgt[i] = q[i].i;
            }
        }

        public int get(int i) {
            if (p[i] != i) {
                p[i] = get(p[i]);
            }
            return p[i];
        }

        public long getValue(int i) {
            i = get(i);
            return min[i] * sum[i];
        }

        public long getMin(int i) {
            return min[get(i)];
        }

        public int getLeft(int i) {
            return lft[get(i)];
        }

        public int getRight(int i) {
            return rgt[get(i)];
        }

        public void union(int i, int j) {
            i = get(i);
            j = get(j);
            if (i == j) {
            	return;
            }
            if (r[i] == r[j]) {
                r[i]++;
            }
            if (r[i] > r[j]) {
                p[j] = i;
                min[i] = Math.min(min[i], min[j]);
                sum[i] = sum[i] + sum[j];
                lft[i] = Math.min(lft[i], lft[j]);
                rgt[i] = Math.max(rgt[i], rgt[j]);
            } else {
                p[i] = j;
                min[j] = Math.min(min[i], min[j]);
                sum[j] = sum[i] + sum[j];
                lft[j] = Math.min(lft[i], lft[j]);
                rgt[j] = Math.max(rgt[i], rgt[j]);
            }
        }
    }
}
