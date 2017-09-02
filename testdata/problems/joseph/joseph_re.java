/*
    Solution for NEERC'2005 Problem J: Joseph's Problem
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class joseph_re {
	public static void main(String[] args) throws FileNotFoundException {
		// read input
		Scanner in = new Scanner(new File("joseph.in"));
        int n = in.nextInt();
		int k = in.nextInt();
		if (n < 1 || n > 1000000000)
			throw new IllegalArgumentException("n is out of range");
		if (k < 1 || k > 1000000000)
			throw new IllegalArgumentException("k is out of range");
		if (in.hasNext())
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// solve
		long sum = 0;
		int sqrt = Math.min((int)Math.sqrt(k), n);
		for (int i = 1; i <= sqrt; i++)
			sum += k % i;

        int i = sqrt + 1;
		while (i <= n) {
			int q = k / i;
			int r = k % i;
			if (q == 0) {
				sum += (long)r * (n - i + 1);
				break;
			}
			int j = Math.min(r / q, n - i);
			sum += (long)(j + 1) * r - (long)j * (j + 1) / 2 * q;
			i += j + 1;
		}

		// write output
		PrintWriter out = new PrintWriter(new File("joseph.out"));
		out.println(sum);
		out.close();
	}
}
