/*
    Solution for NEERC'2005 Problem D: Double
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class double_re {
	public static void main(String[] args) throws FileNotFoundException {
		new double_re().run();
	}

	int[][] d;
	int[] pw;
	double[] pr;

	void run() throws FileNotFoundException {
		// read input
		Scanner in = new Scanner(new File("double.in"));
		d = new int[9][4];
		boolean[] f = new boolean[36];
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 4; j++) {
				String str = in.next();
				if (str.length() != 2)
					throw new IllegalArgumentException("invalid card length");
				int v;
				switch (str.charAt(0)) {
				case '6': v = 6; break;
				case '7': v = 7; break;
				case '8': v = 8; break;
				case '9': v = 9; break;
				case 'T': v = 10; break;
				case 'J': v = 11; break;
				case 'Q': v = 12; break;
				case 'K': v = 13; break;
				case 'A': v = 14; break;
				default:
					throw new IllegalArgumentException("invalid card value");
				}
				int s;
				switch (str.charAt(1)) {
				case 'S': s = 0; break;
				case 'C': s = 1; break;
				case 'D': s = 2; break;
				case 'H': s = 3; break;
				default:
					throw new IllegalArgumentException("invalid card suit");
				}
				int c = (v - 6) * 4 + s;
				d[i][j] = c;
				if (f[c])
					throw new IllegalArgumentException("duplicate card");
			}
		}
		if (in.hasNext())
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// solve
		pw = new int[10];
		pw[0] = 1;
		for (int i = 1; i < 10; i++) {
			pw[i] = pw[i - 1] * 5;
		}

		pr = new double[pw[9]];
		Arrays.fill(pr, -1);
        pr[pw[9] - 1] = 1; // all cards were taken -- wins
        double result = calc(0);

		// write answer
		PrintWriter out = new PrintWriter(new File("double.out"));
		out.println(result);
		out.close();
	}

	int get(int pos, int i) {
		return (pos / pw[i]) % 5;
	}

	int set(int pos, int i, int p) {
		return (pos / pw[i + 1]) * pw[i + 1] + p * pw[i] + pos % pw[i];
	}

	double calc(int pos) {
		if (pr[pos] == -1) {
			double sum = 0;
			int num = 0;
			for (int i = 0; i < 9; i++) {
				int pi = get(pos, i);
				if (pi < 4) {
					int ci = d[i][3 - pi];
					for (int j = i + 1; j < 9; j++) {
						int pj = get(pos, j);
						if (pj < 4) {
							int cj = d[j][3 - pj];
							if (ci / 4 == cj / 4) {
                        		sum += calc(set(set(pos, i, pi + 1), j, pj + 1));
								num++;
							}
						}
					}
				}
			}
			if (num > 0)
				sum /= num;
			pr[pos] = sum;
		}
		return pr[pos];
	}
}
