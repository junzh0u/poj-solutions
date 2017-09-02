import java.util.Scanner;
import java.util.Set;
import java.util.HashSet;
import java.util.Iterator;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class hardwood_as {
    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("hardwood.in"));
        int m = in.nextInt();
        int n = in.nextInt();
        in.nextLine();
        String[] s = new String[m];
        for (int i = 0; i < m; i++) {
            s[i] = in.nextLine();
        }
        in.close();

        Piece p = new Piece(m, n, s);
        Set<Piece> pp = cut(p);
        for (Iterator<Piece> it = pp.iterator(); it.hasNext();) {
            Piece piece = it.next();
            System.out.println(piece.toString() + '\n');
        }

        PrintWriter out = new PrintWriter(new File("hardwood.out"));
        out.println(pp.size());
        out.close();
    }

    public Set<Piece> cut(Piece p) {
        Set<Piece> t = p.cut();
        if (t.size() == 1) {
            return t;
        }
        Set<Piece> r = new HashSet<Piece>();
        for (Iterator<Piece> it = t.iterator(); it.hasNext();) {
            Piece piece = it.next();
            r.addAll(cut(piece));
        }
        return r;
    }

    public class Piece {
        int m;
        int n;
        int[][] a;

        boolean[][] u;
        boolean[][] v;

        public Piece(int m, int n, String[] s) {
            this.m = m;
            this.n = n;
            a = new int[2 * m + 1][2 * n + 1];
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    a[2 * i + 1][2 * j + 1] = s[i].charAt(j);
                }
            }
        }

        public Piece(int m, int n, int[][] a) {
            this.m = m;
            this.n = n;
            this.a = a;
        }

        public Set<Piece> cut() {
            Set<Piece> r = new HashSet<Piece>();

            for (int i = 0; i < 2 * m + 1; i++) {
                a[i][0] = -1;
                a[i][2 * n] = -1;
            }

            for (int i = 0; i < 2 * n + 1; i++) {
                a[0][i] = -1;
                a[2 * m][i] = -1;
            }

            for (int i = 1; i < 2 * m; i++) {
                int j = 1;
                while (j < 2 * n && a[i][j] <= 0 &&
                        (a[i - 1][j] != a[i + 1][j] || a[i - 1][j] <= 0)) {
                    a[i][j] = -1;
                    j++;
                }
                j = 2 * n - 1;
                while (j > 0 && a[i][j] <= 0 &&
                        (a[i - 1][j] != a[i + 1][j] || a[i - 1][j] <= 0)) {
                    a[i][j] = -1;
                    j--;
                }
            }
            for (int i = 1; i < 2 * n; i++) {
                int j = 1;
                while (j < 2 * m && a[j][i] <= 0 &&
                        (a[j][i - 1] != a[j][i + 1] || a[j][i - 1] <= 0)) {
                    a[j][i] = -1;
                    j++;
                }
                j = 2 * m - 1;
                while (j > 0 && a[j][i] <= 0 &&
                        (a[j][i - 1] != a[j][i + 1] || a[j][i - 1] <= 0)) {
                    a[j][i] = -1;
                    j--;
                }
            }

            u = new boolean[2 * m + 1][2 * n + 1];

            for (int i = 0; i < 2 * m + 1; i++) {
                for (int j = 0; j < 2 * n + 1; j++) {
                    if (!u[i][j] && a[i][j] >= 0) {
                        v = new boolean[2 * m + 1][2 * n + 1];
                        fill(i, j);
                        int[][] b = new int[2 * m + 1][2 * n + 1];
                        for (int k = 0; k < 2 * m + 1; k++) {
                            for (int l = 0; l < 2 * n + 1; l++) {
                                if (v[k][l]) {
                                    b[k][l] = a[k][l];
                                }
                            }
                        }
                        Piece p = new Piece(m, n, b);
                        if (!p.isEmpty()) {
                            r.add(p);
                        }
                    }
                }
            }

            return r;
        }

        void fill(int i, int j) {
            if (u[i][j] || a[i][j] == -1) {
                return;
            }
            u[i][j] = true;
            v[i][j] = true;
            fill(i - 1, j);
            fill(i + 1, j);
            fill(i, j - 1);
            fill(i, j + 1);
        }

        public boolean isEmpty() {
            for (int i = 0; i < 2 * m + 1; i++) {
                for (int j = 0; j < 2 * n + 1; j++) {
                    if (a[i][j] > 0) {
                        return false;
                    }
                }
            }
            return true;
        }

        public String toString() {
            String r = "";
            for (int i = 0; i < 2 * m + 1; i++) {
                for (int j = 0; j < 2 * n + 1; j++) {
                    if (i % 2 == 1 && j % 2 == 1) {
                        if (a[i][j] == 0) {
                            r += ".";
                        } else {
                            r += (char) a[i][j];
                        }
                    }
                }
                if (i % 2 == 1) {
                    r += '\n';
                }
            }
            return r;
        }
    }

    public static void main(String[] args) throws FileNotFoundException {
        new hardwood_as().run();
    }
}
