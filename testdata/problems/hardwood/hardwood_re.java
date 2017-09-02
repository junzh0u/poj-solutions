/*
    Solution for NEERC'2005 Problem H: Hardwood Cutting
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class hardwood_re {
	public static void main(String[] args) throws FileNotFoundException {
		new hardwood_re().run();
	}

	int m;
	int n;
	char[][] b;
	char[][] a;
	int[][] p;
	boolean[][] v;
	boolean[][][] c;

	void run() throws FileNotFoundException {
		// read input
		Scanner in = new Scanner(new File("hardwood.in"));
		m = in.nextInt();
		n = in.nextInt();
		if (m < 1 || m > 20)
			throw new IllegalArgumentException("m is out of range");
		if (n < 1 || n > 20)
			throw new IllegalArgumentException("n is out of range");
		b = new char[m][];
		for (int i = 0; i < m; i++) {
			String s = in.next();
			if (s.length() != n)
				throw new IllegalArgumentException("invalid line length");
			b[i] = s.toCharArray();
			for (int j = 0; j < n; j++) {
				char c = b[i][j];
				if ((c < '0' || c > '9') && (c < 'a' || c > 'z') && (c < 'A' && c > 'Z'))
					throw new IllegalArgumentException("Invalid character");
			}
		}
		if (in.hasNext())
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// allocate arrays
    	a = new char[m + 2][n + 2];
		p = new int[m + 2][n + 2];
		c = new boolean[m + 2][n + 2][4];
		v = new boolean[m][n];

		// check that board is connected
		boolean[] seen = new boolean[256];
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (!v[i][j]) {
					char c = b[i][j];
					if (seen[c])
						throw new IllegalArgumentException("piece is not connected");
					seen[c] = true;
					visitPiece(i, j, c);
				}
			}
		}

		// solve
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				a[i + 1][j + 1] = b[i][j];
				p[i + 1][j + 1] = 1;
			}
		}

		boolean changes = true;
		int num = 1;
		while (changes) {
			// make all possible cuts from edges
			for (int i = 0; i <= m; i++) {
				for (int j = 0; j <= n; j++) {
					if (isEdgePoint(i, j)) {
						cutRight(i, j);
						cutLeft(i, j);
						cutDown(i, j);
						cutUp(i, j);
					}
				}
			}

			// find pieces
			int old_num = num;
			num = 0;
			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					v[i][j] = false;
				}
			}
			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					if (!v[i][j]) {
						num++;
						visitCut(i, j, num);
					}
				}
			}
			changes = num != old_num;
		}

		// write answer
		PrintWriter out = new PrintWriter(new File("hardwood.out"));
		out.println(num);
		out.close();
	}

	void visitPiece(int i, int j, char c) {
		if (i < 0 || i >= m || j < 0 || j >= n || v[i][j] || b[i][j] != c)
			return;
		v[i][j] = true;
		visitPiece(i - 1, j, c);
		visitPiece(i, j - 1, c);
		visitPiece(i + 1, j, c);
		visitPiece(i, j + 1, c);
	}

	void visitCut(int i, int j, int num) {
		if (i < 0 || i >= m || j < 0 || j >= n || v[i][j])
			return;
		v[i][j] = true;
		p[i + 1][j + 1] = num;
		boolean[] cc = c[i + 1][j + 1];
		if (!cc[0])
			visitCut(i - 1, j, num);
		if (!cc[1])
			visitCut(i, j - 1, num);
		if (!cc[2])
			visitCut(i + 1, j, num);
		if (!cc[3])
			visitCut(i, j + 1, num);
	}

	boolean isEdgePoint(int i, int j) {
        return (p[i][j] != p[i + 1][j]) || (p[i + 1][j] != p[i + 1][j + 1]) ||
			(p[i + 1][j + 1] != p[i][j + 1]) || (p[i][j + 1] != p[i][j]);
	}

	void cutRight(int i, int j) {
		while (j <= m && a[i][j + 1] != a[i + 1][j + 1]) {
			c[i][j + 1][2] = true;
			c[i + 1][j + 1][0] = true;
			j++;
		}
	}

	void cutLeft(int i, int j) {
		while (j >= 0 && a[i][j] != a[i + 1][j]) {
			c[i][j][2] = true;
			c[i + 1][j][0] = true;
			j--;
		}
	}

	void cutDown(int i, int j) {
		while (i <= m && a[i + 1][j] != a[i + 1][j + 1]) {
			c[i + 1][j][3] = true;
			c[i + 1][j + 1][1] = true;
			i++;
		}
	}

	void cutUp(int i, int j) {
		while (i >= 0 && a[i][j] != a[i][j + 1]) {
			c[i][j][3] = true;
			c[i][j + 1][1] = true;
			i--;
		}
	}

}
