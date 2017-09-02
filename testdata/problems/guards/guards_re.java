/*
    Solution for NEERC'2005 Problem G: Guards
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class guards_re {
	public static void main(String[] args) throws IOException {
		Scanner in = new Scanner(new File("guards.in"));
		int n1 = in.nextInt();
		int n2 = in.nextInt();
		int n3 = in.nextInt();
		if (in.hasNext())
			throw new IllegalArgumentException("extra data in file");
		if (n1 < 1 || n1 > 1000)
			throw new IllegalArgumentException("n1");
		if (n2 < 1 || n2 > 1000)
			throw new IllegalArgumentException("n2");
		if (n3 < 1 || n3 > 1000)
			throw new IllegalArgumentException("n3");
		in.close();
		
		// min(3m1 + m2 + 4m3 + 5m4) where:
                //    m1 +  m2 + 2m3 + 3m4 >= n1
                //    m1 +       2m3       >= n2
                //    m1 +        m3 + 2m4 >= n3

                int m1max = Math.max(n1, Math.max(n2, n3));

                int best = 3 * m1max;
                int bm1 = m1max;
                int bm2 = 0;
                int bm3 = 0;
                int bm4 = 0;

                for (int m1 = 0; m1 <= Math.min(best / 3, m1max); m1++) {
               		int m3min = Math.max(0, (n2 - m1 + 1) / 2);
               		int m3max = Math.max((n1 - m1 + 1) / 2, Math.max(m3min, n3 - m1));
               		for (int m3 = m3min; m3 <= Math.min((best - 3*m1) / 4, m3max); m3++) {
               			int m4min = Math.max(0, (n3 - m1 - m3 + 1) / 2);
               			int m4max = Math.max((n1 - m1 - 2*m3 + 2) / 3, m4min);
               			for (int m4 = m4min; m4 <= m4max; m4++) {
               				int m2 = Math.max(0, n1 - m1 - 2 * m3 - 3 * m4);
					int sum = 3*m1 + m2 + 4*m3 + 5*m4;
					if (sum < best) {
						best = sum;
						bm1 = m1;
						bm2 = m2;
						bm3 = m3;
						bm4 = m4;
					}
               			}
               		}
                }

                Formatter out = new Formatter(new File("guards.out"));
                out.format("%d %d %d %d", 3*bm1, bm2, 4*bm3, 5*bm4);
                out.close();
	}
}