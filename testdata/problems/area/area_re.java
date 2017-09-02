/*
    Solution for NEERC'2005 Problem A: Area 51
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class area_re {
	static final double EPS = 1e-8;

	static class Line implements Comparable<Line> {
		double x;
		double b;

		public Line(double x, double b) {
			this.x = x;
			this.b = b;
		}

		public int compareTo(Line o) {
			if (x < o.x - EPS)
				return -1;
			if (x > o.x + EPS)
				return 1;
			if (b < o.b - EPS)
				return -1;
			if (b > o.b + EPS)
				return 1;
			return 0;
		}

	}

	public static void main(String[] args) throws FileNotFoundException {
        // read input & verify its correctness
		Scanner in = new Scanner(new File("area.in"));
		int m = in.nextInt();
		if (m < 1 || m > 100)
			throw new IllegalArgumentException("m is out of range");
		String sight = in.next();
		if (sight.length() != m)
			throw new IllegalArgumentException("wrong number of chimneys");
		char[] s = sight.toCharArray();
		int[] cnt_verify = new int[26];
		for (int i = 0; i < m; i++) {
			char c = s[i];
			if (c < 'A' || c > 'Z')
				throw new IllegalArgumentException("invalid chimney");
			cnt_verify[c - 'A']++;
		}
		char[] cc = new char[m];
		int[] cx = new int[m];
		int[] cy = new int[m];
		for (int i = 0; i < m; i++) {
			String cstr = in.next();
			if (cstr.length() != 1)
				throw new IllegalArgumentException("illegal chimney token");
			cc[i] = cstr.charAt(0);
			if (cc[i] < 'A' || cc[i] > 'Z')
				throw new IllegalArgumentException("invalid chimney");
			cnt_verify[cc[i] - 'A']--;
			cx[i] = in.nextInt();
			cy[i] = in.nextInt();
			if (cx[i] < -100 || cx[i] > 100)
				throw new IllegalArgumentException("x is out of range");
			if (cy[i] <= 0 || cy[i] > 100)
				throw new IllegalArgumentException("y is out of range");
			for (int j = 0; j < i; j++) {
				if (cx[i] == cx[j] && cy[i] == cy[j])
					throw new IllegalArgumentException("two chimneys in the same place");
			}
		}
		for (int i = 0; i < 26; i++) {
			if (cnt_verify[i] != 0)
				throw new IllegalArgumentException("different number of chimney shapes");
		}
		if (in.hasNext())
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// solve
		// find all lines
		TreeMap<Line, HashSet<Integer>> ls = new TreeMap<Line, HashSet<Integer>>();
		for (int i = 0; i < m; i++) {
			for (int j = i + 1; j < m; j++) {
				double a = cy[i] - cy[j];
				double b = cx[j] - cx[i];
				double len = Math.sqrt(a * a + b * b);
				a /= len;
				b /= len;
				double c = a * cx[i] + b * cy[i];
				if (Math.abs(a) < EPS)
					continue; // horizonal line
				if (a > 0) {
					a = -a;
					b = -b;
					c = -c;
				}
				Line line = new Line(c / a, b);
				HashSet<Integer> cs = ls.get(line);
				if (cs == null)
					ls.put(line, cs = new HashSet<Integer>());
				cs.add(i);
				cs.add(j);
			}
		}

		// find initial view from minus infinity
		int[] w = new int[m]; // position -> chimney
		for (int i = 0; i < m; i++)
			w[i] = i;

		for (int i = 0; i < m; i++) {
			for (int j = i + 1; j < m; j++) {
                if (cy[w[i]] < cy[w[j]] || cy[w[i]] == cy[w[j]] && cx[w[i]] > cx[w[j]]) {
					int t = w[i];
					w[i] = w[j];
					w[j] = t;
				}
			}
		}

		// intialize dfff counter
		int diff = 0;
		for (int i = 0; i < m; i++) {
			if (cc[w[i]] != s[i])
				diff++;
		}

		// intialize range list
		ArrayList<Double> ranges = new ArrayList<Double>();
		if (diff == 0)
			ranges.add(Double.NEGATIVE_INFINITY);

		// transpaose the view
		int[] v = new int[m]; // chimney -> position
		for (int i = 0; i < m; i++)
			v[w[i]] = i;

		for (Map.Entry<Line, HashSet<Integer>> entry: ls.entrySet()) {
			Line line = entry.getKey();
			HashSet<Integer> cs = entry.getValue();
            int l = Integer.MAX_VALUE;
			int r = Integer.MIN_VALUE;
			for (int i: cs) {
				l = Math.min(v[i], l);
				r = Math.max(v[i], r);
			}
			if (r - l + 1 != cs.size())
				throw new IllegalStateException("something wrong with our order -- should be a range");
			if (diff == 0)
				ranges.add(line.x);
            for (int i = l, j = r; i < j; i++, j--) {
				if (cc[w[i]] != s[i])
					diff--;
				if (cc[w[j]] != s[j])
					diff--;
				int t = w[i];
				w[i] = w[j];
				w[j] = t;
				if (cc[w[i]] != s[i])
					diff++;
				if (cc[w[j]] != s[j])
					diff++;
				v[w[i]] = i;
				v[w[j]] = j;
			}
			if (diff == 0)
				ranges.add(line.x);
		}
		if (diff == 0)
			ranges.add(Double.POSITIVE_INFINITY);

		// Now count non-empty ranges and write them
		if (ranges.size() % 2 != 0)
			throw new IllegalStateException("something wrong with our ranges");
		int n = 0;
		for (int i = 0; i < ranges.size(); i += 2) {
			double a = ranges.get(i);
			double b = ranges.get(i + 1);
			if (b - a > EPS)
				n++;
		}
		PrintWriter out = new PrintWriter(new File("area.out"));
		out.println(n);
		n = 0;
		for (int i = 0; i < ranges.size(); i += 2) {
			double a = ranges.get(i);
			double b = ranges.get(i + 1);
			if (b - a > EPS) {
				if (n > 0)
					out.print(" ");
				n++;
				if (Double.isInfinite(a))
					out.print("*");
				else
					out.print(a);
				out.print(" ");
				if (Double.isInfinite(b))
					out.print("*");
				else
					out.print(b);
			}
		}
		out.println();
		out.close();
	}
}
