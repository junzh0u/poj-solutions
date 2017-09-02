import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class exploring_as {
    public final long MOD = 1000000000;

    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("exploring.in"));
        String s = in.nextLine();
        in.close();

        int n = s.length();
        char[] c = new char[n];
        for (int i = 0; i < n; i++) {
            c[i] = s.charAt(i);
        }
        long[][] a = new long[n][n];

        for (int i = 0; i < n; i += 2) {
            for (int j = 0; j < n - i; j++) {
                if (i == 0) {
                    a[j][j] = 1;
                } else {
                    a[j][j + i] = 0;
                    for (int k = j + 2; k <= j + i; k += 2) {
                        if (c[j] == c[k] && c[k] == c[j + i]) {
                            a[j][j + i] = (a[j][j + i] + a[j + 1][k - 1] * a[k][j + i]) % MOD;
                        }
                    }
                }
            }
        }

        PrintWriter out = new PrintWriter(new File("exploring.out"));
        out.println(a[0][n - 1]);
        out.close();
    }

    public static void main(String[] args) throws FileNotFoundException {
        new exploring_as().run();
    }
}