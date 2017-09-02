import java.io.*;
import java.util.*;

public class exploring_gk {
    private char[] s;
    private boolean[][] u;
    private int[][] v;

    private int f(int i, int j) {
        if (!u[i][j]) {
            u[i][j] = true;
            if (i < j) {
                v[i][j] = 0;
                if (s[i] == s[j] && (j - i) % 2 == 0) {
                    for (int k = i + 2; k <= j; k += 2) {
                        v[i][j] = (int) ((v[i][j] + (long) f(i + 1, k - 1) * f(k, j)) % 1000000000);
                    }
                }
            } else {
                v[i][j] = 1;
            }
        }
        return v[i][j];
    }

    public void run() throws IOException {
        Scanner scanner = new Scanner(new File("exploring.in"));
        PrintWriter writer = new PrintWriter(new FileOutputStream("exploring.out"));

        s = scanner.nextLine().toCharArray();
        System.out.println(s.length);

        u = new boolean[s.length][s.length];
        v = new int[s.length][s.length];
        writer.print(f(0, s.length - 1));

        scanner.close();
        writer.close();
    }

    public static void main(String[] args) throws IOException {
        new exploring_gk().run();
    }
}
