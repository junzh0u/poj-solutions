import java.util.*;
import java.io.*;

public class joseph_gk {
    private static long sum(long a, long d, long n) {
        return (2 * a - d * n) * (n + 1) / 2;
    }

    public static void main(String[] args) throws IOException {
        Scanner scanner = new Scanner(new File("joseph.in"));
        PrintWriter writer = new PrintWriter(new FileOutputStream("joseph.out"));

        int n = scanner.nextInt();
        int k = scanner.nextInt();

        int i = 1;
        long r = 0;

        while (i <= n) {
            int q = k % i;
            int p = k / i;
            int a = (p > 0) ? q / p : Integer.MAX_VALUE;
            r += sum(q, p, Math.min(a, n - i));
            i += Math.min(a, n - i) + 1;
        }

        writer.print(r);

        scanner.close();
        writer.close();
    }
}
