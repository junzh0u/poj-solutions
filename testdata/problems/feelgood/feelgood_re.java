/*
    Solution for NEERC'2005 Problem F: Feelgood
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class feelgood_re {
	static class DataPoint implements Comparable<DataPoint> {
		int a;
		int i;

		public DataPoint(int a, int i) {
			this.a = a;
			this.i = i;
		}

		public int compareTo(DataPoint o) {
			return a - o.a;
		}
	}

	public static void main(String[] args) throws IOException {
		new feelgood_re().run();
	}

	int[] l;
	int[] r;
	long[] s;
	int[] h;

	void run() throws IOException {
		// Read input
		BufferedReader in = new BufferedReader(new FileReader("feelgood.in"));
		StringTokenizer st = new StringTokenizer(in.readLine());
		int n = Integer.parseInt(st.nextToken());
		if (n < 1 || n > 100000)
			throw new IllegalArgumentException("n is out of range");
		if (st.hasMoreTokens())
			throw new IllegalArgumentException("extra data on 1st line");
		int[] a = new int[n];
		for (int i = 0; i < n; i++) {
			if (!st.hasMoreTokens())
				st = new StringTokenizer(in.readLine());
			a[i] = Integer.parseInt(st.nextToken());
			if (a[i] < 0 || a[i] > 1000000)
				throw new IllegalArgumentException("a is out of range");
		}
		if (st.hasMoreTokens() || in.readLine() != null)
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// Solve
		ArrayList<DataPoint> da = new ArrayList<DataPoint>(n);
		for (int i = 0; i < n; i++)
			da.add(new DataPoint(a[i], i));
		Collections.sort(da, Collections.reverseOrder());

		l = new int[n];
		r = new int[n];
		s = new long[n];
		h = new int[n];
		Arrays.fill(h, -1);

		long best = -1;
		int best_l = -1;
		int best_r = -1;

		for (DataPoint d: da) {
            int i = d.i;
			l[i] = i;
			r[i] = i;
			s[i] = d.a;
			h[i] = i;
			int head = i;
        	if (i > 0 && h[i - 1] >= 0)
				head = merge(i - 1, i);
			if (i < n - 1 && h[i + 1] >= 0)
				head = merge(head, i + 1);
			long cur = s[head] * d.a;
			if (cur > best) {
				best = cur;
				best_l = l[head];
				best_r = r[head];
			}
		}

		// write answer
		PrintWriter out = new PrintWriter(new File("feelgood.out"));
		out.println(best);
		out.println((best_l + 1) + " " + (best_r + 1));
		out.close();
	}

	int merge(int i, int j) {
        int h1 = h[i];
		int h2 = h[j];
		s[h1] += s[h2];
		int rn = r[h2];
		r[h1] = rn;
		h[rn] = h1;
		return h1;
	}
}
