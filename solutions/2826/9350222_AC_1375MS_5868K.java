import java.util.Scanner;

public class Main{
	private static final boolean	DEBUG	= false;
	private static final Scanner	in		= new Scanner(System.in);

	public static void main(String[] args){
		int caseNum = in.nextInt();
		while(caseNum-- > 0){
			Main testCase = new Main();
			testCase.input();
			System.out.printf("%.2f\n", testCase.rain());
		}
	}

	private class Vector{
		public double	x, y;

		public Vector(double x, double y){
			this.x = x;
			this.y = y;
		}

		public Vector subtract(Vector subtrahend){
			return new Vector(x - subtrahend.x, y - subtrahend.y);
		}

		public double cross(Vector a){
			return x * a.y - y * a.x;
		}
	}

	private class Segment{
		public Vector	lower, higher, increment;

		public Segment(Vector from, Vector to){
			if(from.y <= to.y){
				lower = from;
				higher = to;
				increment = to.subtract(from);
			}
			else{
				lower = to;
				higher = from;
				increment = from.subtract(to);
			}
		}

		public boolean parallel(Segment s){
			return increment.cross(s.increment) == 0.0;
		}

		public Vector intersect(Segment s){
			double x1 = lower.x, y1 = lower.y, u1 = increment.x, v1 = increment.y;
			double x2 = s.lower.x, y2 = s.lower.y, u2 = s.increment.x, v2 = s.increment.y;
			double t1 = (v2 * x1 - v2 * x2 - u2 * y1 + u2 * y2) / (u2 * v1 - u1 * v2);
			double t2 = (v1 * x1 - v1 * x2 - u1 * y1 + u1 * y2) / (u2 * v1 - u1 * v2);
			if(t1 < 0.0 || t1 >= 1.0 || t2 < 0.0 || t2 >= 1.0)
				return null;
			return new Vector(x1 + t1 * u1, y1 + t1 * v1);
		}

		public double getX(double y){
			if(lower.y == y)
				return lower.x;
			if(increment.y == 0.0)
				return 0.0;
			return lower.x + (y - lower.y) * increment.x / increment.y;
		}
	}

	private Segment	left, right;

	public Main(){
	}

	public void input(){
		Segment a = new Segment(new Vector(in.nextDouble(), in.nextDouble()), new Vector(
			in.nextDouble(), in.nextDouble()));
		Segment b = new Segment(new Vector(in.nextDouble(), in.nextDouble()), new Vector(
			in.nextDouble(), in.nextDouble()));
		if(a.increment.cross(b.increment) >= 0.0){
			left = b;
			right = a;
		}
		else{
			left = a;
			right = b;
		}
	}

	public double rain(){
		if(left.parallel(right)){
			if(DEBUG)
				System.err.println("The two segments is parall.");
			return 0.0;
		}
		if(left.increment.y == 0.0 || right.increment.y == 0.0){
			if(DEBUG)
				System.err.println("One segment is horizontal.");
			return 0.0;
		}
		Vector intersection = left.intersect(right);
		if(intersection == null){
			if(DEBUG)
				System.err.println("Don't intersect or intersect at the higher endpoint.");
			return 0.0;
		}
		if(right.higher.x <= left.higher.x){
			if(DEBUG)
				System.err.println("The gap is covered.");
			return 0.0;
		}
		if(DEBUG)
			System.err.println("Intersect at (" + intersection.x + ", " + intersection.y + " )");
		double level = Math.min(left.higher.y, right.higher.y);
		double xLeft = left.getX(level), xRight = right.getX(level);
		if(DEBUG)
			System.err.println("Waterline from (" + xLeft + ", " + level + " ) to (" + xRight
				+ ", " + level + " )");
		double area = (xRight - xLeft) * (level - intersection.y) * 0.5;
		return area;
	}
}
