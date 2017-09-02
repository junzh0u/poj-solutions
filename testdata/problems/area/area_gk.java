import java.util.*;
import java.io.*;

public class area_gk {
    private final static double EPS = 1e-8;
    private final static double inf = 1.0/0;
    private final List<Double> l = new ArrayList<Double>();
    private final List<Double> r = new ArrayList<Double>();

    private String s;
    private int[] x;
    private int[] y;
    private char[] ch;
    private int n;

    private static class Pair implements Comparable<Pair> {
        private final double value;
        private final int index;

        public Pair(double value, int index) {
            this.value = value;
            this.index = index;
        }

        public int compareTo(Pair that) {
            if (this.value < that.value) {
                return -1;
            } else {
                return 1;
            }
        }
    }

    public void check(double lx, double rx, double cx) {
        Pair[] p = new Pair[n];
        for (int i = 0; i < n; i++) {
            p[i] = new Pair((x[i] - cx) / y[i], i);
        }

        Arrays.sort(p);

        boolean f = true;
        for (int i = 0; i < n; i++) {
            f &= ch[p[i].index] == s.charAt(i);
        }

        if (f) {
            l.add(lx);
            r.add(rx);
        }
    }

    public void run() {
        try {
           Scanner scanner = new Scanner(new File("area.in"));
           PrintWriter writer = new PrintWriter(new FileOutputStream("area.out"));

           n = scanner.nextInt();
           s = scanner.next();

           x = new int[n];
           y = new int[n];
           ch = new char[n];
           for (int i = 0; i < n; i++) {
               ch[i] = scanner.next().charAt(0);
               x[i] = scanner.nextInt();
               y[i] = scanner.nextInt();
           }
           
           List<Double> a = new ArrayList<Double>();

           for (int i = 0; i < n; i++) {
               for (int j = i + 1; j < n; j++) {
                   if (y[i] != y[j]) {
                       a.add((x[j] * y[i] - x[i] * y[j]) / (double) (y[i] - y[j]));
                   }
               }
           }

           Collections.sort(a);

           if (!a.isEmpty()) {
               double p = a.get(0);
               check(-inf, p, p - 1);
               for (int i = 1; i < a.size(); i++) {
                    double c = a.get(i);
                    if (c - p >= EPS) {
                        check(p, c, (p + c) / 2);
                    }
                    p = c;
               }
               check(p, +inf, p + 1);
           } else {
               check(-inf, +inf, 0);
           }

           writer.println(l.size());
           for (int i = 0; i < l.size(); i++) {
               if (l.get(i) == -inf) writer.print('*'); else writer.print(l.get(i));
               writer.print(" ");
               if (r.get(i) == +inf) writer.print('*'); else writer.print(r.get(i));
               writer.print(" ");
           }

           scanner.close();
           writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        new area_gk().run();
    }
}