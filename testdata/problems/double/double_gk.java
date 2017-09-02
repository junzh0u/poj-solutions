import java.io.*;

public class double_gk {
    private int[][] v = new int[9][5];
    private double[] c = new double[1953126];
    private boolean[] u = new boolean[1953126];

    private int encode(int[] p) {
        int result = 0;
        for (int i = 0, q = 1; i < 9; i++, q *= 5) {
            result += p[i] * q;
        }
        return result;
    }

    int t = 100;
    private double pr(int[] p) {
        int code = encode(p);
        if (!u[code]) {
            u[code] = true;
            c[code] = 0;
            int q = 0;

            for (int i = 0; i < 9; i++) {
                for (int j = i + 1; j < 9; j++) {
                    if (v[i][p[i]] == v[j][p[j]]) {
                        p[i]--;
                        p[j]--;
                        c[code] += pr(p);
                        p[i]++;
                        p[j]++;

                        q++;
                    }
                }
            }
            if (q != 0) {
                c[code] /= q;
            }
        }
        return c[code];
    }


    public void run() throws IOException {
        Reader reader = new FileReader("double.in");
        PrintWriter writer = new PrintWriter(new FileOutputStream("double.out"));

        for (int i = 0; i < 9; i++) {
            for (int j = 1; j < 5; j++) {
                v[i][j] = reader.read();
                reader.read();
                reader.read();
            }
            reader.read();
        }

        for (int i = 0; i < 9; i++) {
            v[i][0] = i;
        }

        int[] p = new int[9];
        for (int i = 0; i < 9; i++) {
            p[i] = 4;
        }

        u[0] = true;
        c[0] = 1;
        writer.print(pr(p));

        reader.close();
        writer.close();
    }

    public static void main(String[] args) throws IOException {
        new double_gk().run();
    }
}
