import java.util.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class double_as {

    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("double.in"));

        char[][] card = new char[9][4];
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 4; j++) {
                String s = in.next();
                card[i][j] = s.charAt(0);
            }
        }
        in.close();

        Position start = new Position();
        start.p = 1.0;
        LinkedList<Position> l = new LinkedList<Position>();
        Map<Position, Position> s = new HashMap<Position, Position>();
        l.add(start);
        s.put(start, start);

        double ans = 0;
        while (l.size() > 0) {
            Position h = l.removeFirst();
            if (h.good()) {
                ans = h.p;
                break;
            }

            int q = 0;
            for (int i = 0; i < 9; i++) {
                for (int j = i + 1; j < 9; j++) {
                    if (h.s[i] > 0 && h.s[j] > 0 && card[i][h.s[i] - 1] == card[j][h.s[j] - 1]) {
                        q++;
                    }
                }
            }
            if (q > 0) {
                for (int i = 0; i < 9; i++) {
                    for (int j = i + 1; j < 9; j++) {
                        if (h.s[i] > 0 && h.s[j] > 0 && card[i][h.s[i] - 1] == card[j][h.s[j] - 1]) {
                            Position np = new Position(h.s, i, j);
                            Position nq = s.get(np);
                            if (nq != null) {
                                nq.p += h.p / q;
                            } else {
                                np.p = h.p / q;
                                s.put(np, np);
                                l.add(np);
                            }
                        }
                    }
                }
            }

        }
        
        PrintWriter out = new PrintWriter(new File("double.out"));
        out.printf("%.9f\n", ans);
        out.close();
    }

    class Position {
        int[] s;
        double p;

        public Position() {
            s = new int[9];
            for (int i = 0; i < 9; i++) {
                s[i] = 4;
            }
            p = 0;
        }

        public Position(int[] s, int x, int y) {
            this.s = new int[9];
            System.arraycopy(s, 0, this.s, 0, 9);
            this.s[x]--;
            this.s[y]--;
            p = 0;
        }

        boolean good() {
            for (int i = 0; i < 9; i++) {
                if (s[i] != 0) {
                    return false;
                }
            }
            return true;
        }

        public boolean equals(Object o) {
            final Position position = (Position) o;
            return Arrays.equals(s, position.s);
        }

        public int hashCode() {
            int h = 0;
            for (int i = 0; i < 9; i++) {
                h = (h + s[i]) * 43;
            }
            return h;
        }
    }

    public static void main(String[] args) throws FileNotFoundException {
        Locale.setDefault(Locale.US);
        new double_as().run();
    }
}
