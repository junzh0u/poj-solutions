import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.math.*;

public class exploring_as_long {
    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("exploring.in"));
        String s = in.nextLine();
        in.close();

        int n = s.length();
        char[] c = new char[n];
        for (int i = 0; i < n; i++) {
            c[i] = s.charAt(i);
        }
        BigInteger[][] a = new BigInteger[n][n];

        for (int i = 0; i < n; i += 2) {
            for (int j = 0; j < n - i; j++) {
                if (i == 0) {
                    a[j][j] = BigInteger.ONE;
                } else {
                    a[j][j + i] = BigInteger.ZERO;
                    for (int k = j + 2; k <= j + i; k += 2) {
                        if (c[j] == c[k] && c[k] == c[j + i]) {
                            a[j][j + i] = a[j][j + i].add(a[j + 1][k - 1].multiply(a[k][j + i]));
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
        new exploring_as_long().run();
    }
}
