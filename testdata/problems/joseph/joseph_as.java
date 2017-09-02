import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class joseph_as {

    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("joseph.in"));
        long n = in.nextInt();
        long k = in.nextInt();
        in.close();

        long i = 1;
        long a = 0;
        while (i <= n) {
            long j = k / i;
            long r = k % i;
            if (j == 0) {
                a += r * (n - i + 1);
                i = n + 1;
            } else {
                long s = r / j;
                if (s + 1 > n - i + 1) {
                    s = n - i;
                }
                a += (r + r - s * j) * (s + 1) / 2;
                i += s + 1;
            }
        }

        PrintWriter out = new PrintWriter(new File("joseph.out"));
        out.println(a);
        out.close();
    }

    public static void main(String[] args) throws FileNotFoundException {
        new joseph_as().run();
    }
}
