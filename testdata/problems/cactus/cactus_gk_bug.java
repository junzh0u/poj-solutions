import java.io.*;
import java.util.*;

public class cactus_gk_bug implements Runnable {
    private final static int WHITE = 0;
    private final static int GREY  = 1;
    private final static int BLACK = 2;

    private int[] parent;
    private int[] state;
    private boolean[] cycle;
    private List<Integer>[] edges;
    private boolean f;

    void dfs(int i, int p) {
        parent[i] = p;
        state[i] = GREY;
        for (int j : edges[i]) {
            if (state[j] == WHITE) {
                dfs(j, i);
            } else if (state[j] == GREY && j != parent[i]) {
                int k = i;
                while (true) {
                    f &= !cycle[k];
                    cycle[k] = true;
                    if (k == j) break;
                    k = parent[k];
                }
            }
        }
        state[i] = BLACK;
    }

    public void run() {
        try {
            Scanner scanner = new Scanner(new File("cactus.in"));
            PrintWriter writer = new PrintWriter(new FileOutputStream("cactus.out"));

            int n = scanner.nextInt();
            edges = new List[n];

            for (int i = 0; i < n; i++) {
                edges[i] = new ArrayList<Integer>();
            }

            int m = scanner.nextInt();
            for (int i = 0; i < m; i++) {
                int l = scanner.nextInt();
                int b = scanner.nextInt() - 1;
                for (int j = 1; j < l; j++) {
                    int e = scanner.nextInt() - 1;
                    edges[b].add(e);
                    edges[e].add(b);
                    b = e;
                }
            }

            parent = new int[n];
            state = new int[n];
            cycle = new boolean[n];

            f = true;
            dfs(0, 0);

            if (f) {
                for (int i = 0; i < n; i++) {
                    f &= state[i] == BLACK;
                }
            }

            writer.println(f ? "YES" : "NO");

            scanner.close();
            writer.close();
        } catch (IOException e) {
        }
    }

    public static void main(String[] args) {
        new Thread(new cactus_gk_bug()).start();
    }
}
