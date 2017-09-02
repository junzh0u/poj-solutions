import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class ip_as {
    public int parseIp(String s) {
        int r = 0;
        s = s + ".";
        int st = 0;
        for (int i = 0; i < 4; i++) {
            int fn = s.indexOf('.', st);
            int q = Integer.parseInt(s.substring(st, fn));
            st = fn + 1;
            r = (r << 8) + q;
        }
        return r;
    }

    public String createIp(int k) {
        String r = "";
        for (int i = 0; i < 4; i++) {
            r = (k & 0xff) + r;
            if (i != 3) {
                r = "." + r;
            }
            k >>= 8;
        }
        return r;
    }

    public void solve() throws FileNotFoundException {
        Scanner in = new Scanner(new File("ip.in"));
        int n = in.nextInt();
        int[] a = new int[n];
        for (int i = 0; i < n; i++) {
            String s = in.next();
            a[i] = parseIp(s);
            System.out.println(a[i]);
        }
        in.close();

        int mask = 0;
        for (int i = 0; i <= 32; i++) {
            boolean ok = true;
            for (int j = 0; j < n; j++) {
                if ((a[j] & ~mask) != (a[0] & ~mask)) {
                    ok = false;
                }
            }
            if (ok) {
                break;
            }
            mask = (mask << 1) | 1;
        }
        String ipNet = createIp(a[0] & ~mask);
        String ipMask = createIp(~mask);

        PrintWriter out = new PrintWriter(new File("ip.out"));
        out.println(ipNet);
        out.println(ipMask);
        out.close();
    }

    public static void main(String[] args) throws FileNotFoundException {
        new ip_as().solve();
    }
}
