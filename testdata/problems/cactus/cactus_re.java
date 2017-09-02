/*
    Solution for NEERC'2005 Problem C: Cactus
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;
import java.math.BigInteger;

public class cactus_re {
	public static void main(String[] args) throws IOException {
		new cactus_re().run();
	}

	static class Edge {
		int a;
		int b;

		public Edge(int a, int b) {
			this.a = a;
			this.b = b;
		}

		public boolean equals(Object other) {
			if (!(other instanceof Edge)) return false;
			Edge e = (Edge)other;
			return (a == e.a && b == e.b) || (a == e.b && b == e.a);
		}

		public int hashCode() {
			return (Math.max(a, b) << 16) + Math.min(a, b);
		}
	}

	int n;
	HashMap<Integer, HashSet<Integer>> g = new HashMap<Integer, HashSet<Integer>>();

	HashSet<Edge> loops = new HashSet<Edge>();
	HashSet<Edge> used = new HashSet<Edge>();
	Stack<Edge> stack = new Stack<Edge>();
	boolean[] f; // on path from root
	boolean[] v; // ever visited
	int[] w; // parent in path from root

	void run() throws IOException {
		// read input
		BufferedReader in = new BufferedReader(new FileReader("cactus.in"));
		StringTokenizer st = new StringTokenizer(in.readLine());
        n = Integer.parseInt(st.nextToken());
		if (n < 1 || n > 20000)
			throw new IllegalArgumentException("n is out of range");
		int m = Integer.parseInt(st.nextToken());
		if (st.hasMoreTokens())
			throw new IllegalArgumentException("extra data on 1st line");
		for (int i = 0; i < m; i++) {
			st = new StringTokenizer(in.readLine());
			int k = Integer.parseInt(st.nextToken());
			if (k < 2 || k > 1000)
				throw new IllegalArgumentException("k is out of range");
			int p = Integer.parseInt(st.nextToken());
			for (int j = 1; j < k; j++) {
				int q = Integer.parseInt(st.nextToken());
				if (p == q)
					throw new IllegalArgumentException("illegal loop edge");
				add(p, q);
				add(q, p);
				p = q;
			}
			if (st.hasMoreTokens())
				throw new IllegalArgumentException("extra data on line");
		}
		if (in.readLine() != null)
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// solve
		f = new boolean[n + 1];
		v = new boolean[n + 1];
		w = new int[n + 1];
		stack.add(new Edge(0, 1));
		f[1] = true;
		BigInteger cactusness = BigInteger.ONE;
	loop:
 		while (!stack.isEmpty()) {
			Edge e = stack.pop();
			int i = e.b;
			if (!used.add(e)) {
				f[i] = false;
				continue;
			}
			f[i] = true;
			v[i] = true;
			w[i] = e.a;
			stack.push(e);
			HashSet<Integer> t = g.get(i);
			if (t != null)
				for (Iterator<Integer> it = t.iterator(); it.hasNext();) {
					int j = it.next();
					if (j != w[i] && f[j]) {
						int c = 0;
						int p = j;
						int q = i;
						do {
							if (!loops.add(new Edge(p, q))) {
								cactusness = BigInteger.ZERO;
								break loop;
							}
							p = q;
							q = w[q];
							c++;
						} while (p != j);
						cactusness = cactusness.multiply(BigInteger.valueOf(c + 1));
					}
					if (!f[j]) {
						Edge ee = new Edge(i, j);
						if (!used.contains(ee))
							stack.add(ee);
					}
				}
		}
		for (int i = 1; i <= n; i++)
			if (!v[i])
				cactusness = BigInteger.ZERO;

		// write answer
		Formatter out = new Formatter(new File("cactus.out"));
		out.format("%d", cactusness);
		out.close();
	}

	void add(int a, int b) {
		HashSet<Integer> t = g.get(a);
		if (t == null)
			g.put(a, t = new HashSet<Integer>());
		if (!t.add(b))
			throw new IllegalArgumentException("Edge already exists");
	}
}