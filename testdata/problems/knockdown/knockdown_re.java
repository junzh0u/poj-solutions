/*
    Solution for NEERC'2005 Problem K: Knockdown
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class knockdown_re {
	static class Point {
		double x;
		double y;
		double z;

		public Point(double x, double y, double z) {
			this.x = x;
			this.y = y;
			this.z = z;
		}
	}

	Point[] w; // bomb centers

	public static void main(String[] args) throws FileNotFoundException {
		new knockdown_re().run();
	}

	void run() throws FileNotFoundException {
		// read input
		Scanner in = new Scanner(new File("knockdown.in"));
		int n = in.nextInt();
		if (n < 1 || n > 20)
			throw new IllegalArgumentException("n is out of range");
		int[] phi = new int[n];
		int[] lambda = new int[n];
		for (int i = 0; i < n; i++) {
			phi[i] = in.nextInt();
			lambda[i] = in.nextInt();
			if (phi[i] <= -90 || phi[i] >= 90)
				throw new IllegalArgumentException("phi is out of range");
			if (lambda[i] <= -180 || lambda[i] > 180)
				throw new IllegalArgumentException("lambda is out of range");
		}
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				if (phi[i] == phi[j] && lambda[i] == lambda[j])
					throw new IllegalArgumentException("bombs in the same place");
			}
		}
		if (in.hasNext())
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// convert into Cartesian coordinates
		w = new Point[n];
		for (int i = 0; i < n; i++)
			w[i] = rotate(1, 0, 0, phi[i], lambda[i]);

		// binary search for the answer
		double l = 0;
		double h = Math.PI;
		while (Math.abs(h - l) > 1e-8) {
			double mid = (h + l) / 2;
			// convert into Cartesian distance
			double r = Math.sqrt(sqr(Math.sin(mid)) + sqr(1 - Math.cos(mid)));
			boolean f = true; // covered flag
			for (int i = 0; f && i < n; i++) {
				// check that some point on the destruction circle is covered
				if (!covered(rotate(Math.cos(mid), Math.sin(mid), 0, phi[i], lambda[i]), r))
					f = false;
			}
			for (int i = 0; f && i < n; i++) {
				for (int j = i + 1; f && j < n; j++) {
					double dd = dist(w[i], w[j]);
					double s = Math.sqrt(sqr(r) - sqr(dd / 2));
					if (Double.isNaN(s))
						continue; // do not intersect
					Point cv = center(w[i], w[j]);
					double c = len(cv);
					if (c < 1e-10)
						continue; // opposite points
					double p = (1 + sqr(c) - sqr(s)) / (2 * c);
					if (Math.abs(p) >= 1)
						continue; // no intersection on the geosphere
					double q = Math.sqrt(1 - sqr(p));
					Point pv = normVector(cv, p);
					Point qv = normVector(crossProd(cv, sub(w[i], w[j])), q);
					if (!covered(add(pv, qv), r) || !covered(sub(pv, qv), r))
						f = false;
				}
			}
			if (f)
				h = mid;
			else
				l = mid;
		}

		// write answer
		PrintWriter out = new PrintWriter(new File("knockdown.out"));
		out.println((h + l) / 2);
		out.close();
	}


	Point rotate(double x0, double y0, double z0, int phi, int lambda) {
		double p = phi * Math.PI / 180;
		double l = lambda * Math.PI / 180;
		double x1 = Math.cos(p) * x0 - Math.sin(p) * z0;
		double y1 = y0;
		double z1 = Math.sin(p) * x0 + Math.cos(p) * z0;
		return new Point(
			Math.cos(l) * x1 - Math.sin(l) * y1,
			Math.sin(l) * x1 + Math.cos(l) * y1,
			z1);
	}

	Point mul(Point a, double d) {
		return new Point(a.x * d, a.y * d, a.z * d);
	}

	Point sub(Point a, Point b) {
		return new Point(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	Point add(Point a, Point b) {
		return new Point(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	Point center(Point a, Point b) {
		return mul(add(a, b), 0.5);
	}

	double len(Point a) {
		return Math.sqrt(sqr(a.x) + sqr(a.y) + sqr(a.z));
	}

	Point normVector(Point a, double d) {
		return mul(a, d / len(a));
	}

	Point crossProd(Point a, Point b) {
		return new Point(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	private double dist(Point a, Point b) {
		return len(sub(a, b));
	}

	double sqr(double x) {
		return x * x;
	}

	// returns "true" is point is corevered fully by some destruction circle
	boolean covered(Point a, double r) {
		for (int i = 0; i < w.length; i++) {
			if (dist(a, w[i]) < r - 1e-10)
				return true;
		}
		return false;
	}
}
