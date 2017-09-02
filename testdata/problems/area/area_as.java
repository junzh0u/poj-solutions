import java.util.Scanner;
import java.util.Arrays;
import java.util.ArrayList;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class area_as {
    private final static double EPS = 1e-8;

    int n;
    String see;
    int[] x;
    int[] y;
    char[] c;

    double least;
    double greatest;

    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("area.in"));
        n = in.nextInt();
        in.nextLine();
        see = in.nextLine();
        c = new char[n];
        x = new int[n];
        y = new int[n];
        for (int i = 0; i < n; i++) {
            c[i] = in.next(".").charAt(0);
            x[i] = in.nextInt();
            y[i] = in.nextInt();
        }

        int m = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (y[j] > y[i]) {
                    m++;
                }
            }
        }
        double[] r = new double[m + 2];

        r[0] = -Double.MAX_VALUE;
        r[1] = Double.MAX_VALUE;
        m = 2;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (y[j] > y[i]) {
                    r[m++] = x[i] - (x[i] - x[j]) * y[i] * 1.0 / (y[i] - y[j]);
                }
            }
        }

        Arrays.sort(r);
        if (m == 2) {
            r[0] = 0;
            r[1] = 1;
        } else {
            r[0] = r[1] - 1;
            r[m - 1] = r[m - 2] + 1;
        }
        least = r[0];
        greatest = r[m - 1];

        Tower[] a = new Tower[n];
        for (int i = 0; i < n; i++) {
            a[i] = new Tower();
        }

        ArrayList<Segment> answer = new ArrayList<Segment>();
        for (int i = 0; i < m - 1; i++) {
            if (Math.abs(r[i] - r[i + 1]) > EPS) {
                double xx = (r[i] + r[i + 1]) / 2;
                if (i == 0) {
                    xx = r[i + 1] - 1;
                }
                if (i == m - 2) {
                    xx = r[i] + 1;
                }

                for (int j = 0; j < n; j++) {
                    a[j].name = c[j];
                    a[j].angle = -Math.atan2(y[j], x[j] - xx);
                }
                Arrays.sort(a);
                boolean ok = true;
                for (int j = 0; j < n; j++) {
                    if (see.charAt(j) != a[j].name) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    answer.add(new Segment(r[i], r[i + 1]));
                }
            }
        }

        PrintWriter out = new PrintWriter(new File("area.out"));
        out.println(answer.size());
        for (Segment s : answer) {
            out.println(s);
        }
        out.close();
    }

    class Segment {
        double l, r;

        public Segment(double l, double r) {
            this.l = l;
            this.r = r;
        }

        public String toString() {
            String ls = (l == least) ? "*" : Double.toString(l);
            String rs = (r == greatest) ? "*" : Double.toString(r);
            return ls + " " + rs;
        }
    }

    class Tower implements Comparable<Tower> {
        char name;
        double angle;

        public int compareTo(Tower o) {
            if (angle < o.angle) {
                return -1;
            } else if (angle > o.angle) {
                return 1;
            }
            assert false;
            return 0;
        }

        public String toString() {
            return "" + name;
        }
    }

    public static void main(String[] args) throws FileNotFoundException {
        new area_as().run();
    }
}
