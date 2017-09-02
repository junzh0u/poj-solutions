import java.util.Scanner;
import java.util.Arrays;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class guards_as_wrong {
    int wdd;
    int wed;
    int ntd;

    static final int[] cost = new int[]{3, 4, 5, 1};
    static final int[] pwdd = new int[]{1, 2, 3, 1};
    static final int[] pwed = new int[]{1, 2, 0, 0};
    static final int[] pntd = new int[]{1, 1, 1, 0};

    int[] opt;
    int best;

    int[] cur;
    int[] def;
    public void bt(int i, int c, int cwdd, int cwed, int cntd) {
        if (i == 2 && sprod(cur, pwed) < wed) {
            return;
        }
        if (i == 3 && sprod(cur, pntd) < ntd) {
            return;
        }

        if (i == 4) {
            if (c < best && cwdd >= wdd && cwed >= wed && cntd >= ntd) {
                best = c;
                System.arraycopy(cur, 0, opt, 0, 4);
                System.out.println(best);
            }
            return;
        }

//        System.out.println(cur[0] + " " + cur[1] + " " + cur[2] + " " + cur[3]);

        for (int j = 0; j < 1000; j++) {
            if (c + j * cost[i] >= best) {
                break;
            }
            cur[i] = j;
            bt(i + 1, c + j * cost[i], cwdd, cwed, cntd);
            if (cwdd >= wdd && cwed >= wed && cntd >= ntd) {
                break;
            }
            cwdd += pwdd[i];
            cwed += pwed[i];
            cntd += pntd[i];
        }
    }

    int sprod(int[] a, int[] b) {
        int r = 0;
        for (int i = 0; i < 4; i++) {
            r += a[i] * b[i];
        }
        return r;
    }

    public int safe3(int k) {
        return (k + 2) / 3 * 3;
    }

    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("guards.in"));
        wdd = in.nextInt();
        wed = in.nextInt();
        ntd = in.nextInt();
        in.close();

        cur = new int[4];
        opt = new int[4];
        opt[0] = Math.max(wdd, Math.max(wed, ntd));
        best = opt[0] * 3;
        def = new int[4];
        bt(0, 0, 0, 0, 0);

        PrintWriter out = new PrintWriter(new File("guards.out"));
        out.println(opt[0] * cost[0] + " " + opt[3] * cost[3] + " " + opt[1] * cost[1] + " " + opt[2] * cost[2]);
        out.close();
    }

    public static void main(String[] args) throws FileNotFoundException {
        new guards_as_wrong().run();
    }
}
