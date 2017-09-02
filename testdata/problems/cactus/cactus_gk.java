import java.io.*;
import java.util.*;
import java.math.*;

public class cactus_gk implements Runnable {
    private StreamTokenizer t;

    private int nextInt() throws IOException {
        t.nextToken();
        return (int) t.nval;
    }

    private final static int WHITE = 0;
    private final static int GREY  = 1;
    private final static int BLACK = 2;
    private BigInteger result = BigInteger.ONE;

    private static class Edge {
        private final int v1;
        private final int v2;
        private boolean inCycle;

        public Edge(int v1, int v2) {
            this.v1 = v1;
            this.v2 = v2;
        }

        public int other(int v) {
            return v == v1 ? v2 : v1;
        }
    }

    private Edge[] in;
    private int[] state;
    private List<Edge>[] edges;
    private boolean f;

    void dfs(int i, Edge e) {
        in[i] = e;
        state[i] = GREY;
        for (Edge edge : edges[i]) {
            int j = edge.other(i);

            if (state[j] == WHITE) {
                dfs(j, edge);
            } else if (state[j] == GREY && j != in[i].other(i)) {
                f &= !edge.inCycle;
                edge.inCycle = true;

                int q = 2;
                for (int k = i; k != j; k = in[k].other(k)) {
                    f &= !in[k].inCycle;
                    in[k].inCycle = true;
                    q++;
                }
                result = result.multiply(BigInteger.valueOf(q));
            }
        }
        state[i] = BLACK;
    }

    public void run() {
        try {
            Locale.setDefault(Locale.US);

            t = new StreamTokenizer(new BufferedReader(new FileReader("cactus.in")));
            PrintWriter writer = new PrintWriter(new FileOutputStream("cactus.out"));

            int n = nextInt();
            edges = new List[n];

            for (int i = 0; i < n; i++) {
                edges[i] = new ArrayList<Edge>();
            }

            int m = nextInt();
            for (int i = 0; i < m; i++) {
                int l = nextInt();
                int b = nextInt() - 1;
                for (int j = 1; j < l; j++) {
                    int e = nextInt() - 1;
                    Edge edge = new Edge(b, e);
                    edges[b].add(edge);
                    edges[e].add(edge);
                    b = e;
                }
            }

            in = new Edge[n];
            state = new int[n];

            f = true;
            dfs(0, new Edge(0, 0));

            if (f) {
                for (int i = 0; i < n; i++) {
                    f &= state[i] == BLACK;
                }
            }

            if (f) {
                writer.println(result);
            } else {
                writer.println(0);
            }

            writer.close();
        } catch (IOException e) {
        }
    }

    public static void main(String[] args) {
        new Thread(new cactus_gk()).start();
    }
}
