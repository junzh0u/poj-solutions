import java.util.*;
import java.math.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class cactus_as implements Runnable {
    int n;
    int m;

    List<Edge>[] edges;
    boolean[] visited;
    boolean[] processed;
    int[] enter;
    int[] retenter;
    int time;

    int colors;
    boolean cactus;

    public void dfs(int i) {
        visited[i] = true;
        enter[i] = time++;
        retenter[i] = enter[i];

        for (Edge e : edges[i]) {
            if (!visited[e.d]) {
                dfs(e.d);
                if (retenter[e.d] < retenter[i]) {
                    retenter[i] = retenter[e.d];
                }
            } else {
                if (enter[e.d] < retenter[i]) {
                    retenter[i] = enter[e.d];
                }
            }
        }
    }

    public void color(int i, int c) {
        visited[i] = true;
        for (Edge e : edges[i]) {
            if (!visited[e.d]) {
                if (retenter[e.d] < enter[i]) {
                    color(e.d, c);
                } else {
                    ++colors;
                    color(e.d, colors);
                }
            } else if (!processed[e.d]) {
                e.c = c;
            }
        }
        processed[i] = true;
    }

    public void run() {
        Scanner in;
        try {
            in = new Scanner(new File("cactus.in"));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return;
        }
        n = in.nextInt();
        m = in.nextInt();

        edges = new List[n];
        for (int i = 0; i < n; i++) {
            edges[i] = new ArrayList<Edge>();
        }

        int ec = 0;
        for (int i = 0; i < m; i++) {
            int k = in.nextInt();
            int a = in.nextInt();
            for (int j = 1; j < k; j++) {
                int b = in.nextInt();
                edges[a - 1].add(new Edge(a - 1, b - 1));
                edges[b - 1].add(new Edge(b - 1, a - 1));
                ec++;
                a = b;
            }
        }
        in.close();

        visited = new boolean[n];
        enter = new int[n];
        retenter = new int[n];
        time = 0;

        dfs(0);

        cactus = true;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                cactus = false;
            }
        }

        BigInteger ans = BigInteger.ZERO;
        if (cactus) {
            Arrays.fill(visited, false);
            processed = new boolean[n];

            colors = 0;
            color(0, 0);

            Map<Pair,Integer> pairs = new HashMap<Pair, Integer>();
            int[] nec = new int[colors];
            Pair pp;
            int c;
            for (int i = 0; i < n; i++) {
                for (Edge e : edges[i]) {
                    if (e.c > 0) {
                    	nec[e.c - 1]++;
                        pp = new Pair(e.s, e.c);
                        if (pairs.containsKey(pp)) {
                            c = pairs.get(pp);
                        } else {
                            c = 0;
                        }
                        if (c >= 2) {
                            cactus = false;
                        }
                        pairs.put(pp, c + 1);

                        pp = new Pair(e.d, e.c);
                        if (pairs.containsKey(pp)) {
                            c = pairs.get(pp);
                        } else {
                            c = 0;
                        }
                        if (c >= 2) {
                            cactus = false;
                        }
                        pairs.put(pp, c + 1);
                    }
                }
            }
            if (cactus) {
                ans = BigInteger.ONE;
                for (int i = 0; i < colors; i++) {
                	if (nec[i] > 1) {
                		ans = ans.multiply(BigInteger.valueOf(nec[i] + 1));
                	}
                }
            }
        }

        PrintWriter out;
        try {
            out = new PrintWriter(new File("cactus.out"));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return;
        }
        out.println(ans);
        out.close();
    }

    public class Edge {
        int s;
        int d;
        int c;

        public Edge(int s, int d) {
            this.s = s;
            this.d = d;
            c = 0;
        }
    }

    public class Pair {
        int v;
        int c;

        public Pair(int v, int c) {
            this.v = v;
            this.c = c;
        }

        public boolean equals(Object o) {
            final Pair pair = (Pair) o;

            if (c != pair.c) return false;
            if (v != pair.v) return false;

            return true;
        }

        public int hashCode() {
            int result;
            result = v;
            result = 29 * result + c;
            return result;
        }
    }

    public static void main(String[] args) throws FileNotFoundException {
        new Thread(new cactus_as()).start();
    }

}
