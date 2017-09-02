/*
    Solution for NEERC'2005 Problem E: Exploring Pyramids
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class exploring_re {
	public static void main(String[] args) throws FileNotFoundException {
		// read input
		Scanner in = new Scanner(new File("exploring.in"));
		String str = in.next();
		int n = str.length();
		if (n < 1 || n > 300)
			throw new IllegalArgumentException("invalid tape length");
		char[] c = str.toCharArray();
		for (int i = 0; i < n; i++) {
			 if (c[i] < 'A' || c[i] > 'Z')
				 throw new IllegalArgumentException("invalid tape character");
		}
		if (in.hasNext())
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// solve
		int[][] cc = new int[n][n];
		for (int i = 0; i < n; i++)
			cc[i][i] = 1;
		for (int len = 2; len < n; len++)
			for (int i = 0; i < n - len; i++) {
				int j = i + len;
				if (c[i] == c[j]) {
					for (int k = 2; k <= j; k++) {
						if (c[i] == c[k])
							cc[i][j] = (int)((cc[i][j] + (long)cc[i + 1][k - 1] * cc[k][j]) % 1000000000);
					}
				}
			}

		// write output
		PrintWriter out = new PrintWriter(new File("exploring.out"));
		out.println(cc[0][n - 1]);
		out.close();
	}
}
