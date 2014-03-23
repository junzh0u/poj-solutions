import java.util.Scanner;

public class Main{
	private static final Scanner	in	= new Scanner(System.in);

	public static void main(String[] args){
		System.out.println("INTERSECTING LINES OUTPUT");
		int caseNum = in.nextInt();
		while(caseNum-- > 0){
			Line left =
				new Line(new Vector(in.nextDouble(), in.nextDouble()), new Vector(in.nextDouble(),
					in.nextDouble()));
			Line right =
				new Line(new Vector(in.nextDouble(), in.nextDouble()), new Vector(in.nextDouble(),
					in.nextDouble()));
			if(left.overlap(right))
				System.out.println("LINE");
			else if(left.parallel(right))
				System.out.println("NONE");
			else{
				Vector intersection = left.intersect(right);
				System.out.printf("POINT %.2f %.2f\n", intersection.x, intersection.y);
			}
		}
		System.out.println("END OF OUTPUT");
	}

	private static class Vector{
		public double	x, y;

		public Vector(double x, double y){
			this.x = x;
			this.y = y;
		}

		public Vector subtract(Vector subtrahend){
			return new Vector(x - subtrahend.x, y - subtrahend.y);
		}

		public double cross(Vector v){
			return x * v.y - y * v.x;
		}

		public boolean equals(Vector v){
			return x == v.x && y == v.y;
		}
	}

	private static class Line{
		public Vector	from, increment;

		public Line(Vector from, Vector to){
			this.from = from;
			increment = to.subtract(from);
		}

		public boolean overlap(Line l){
			if(!parallel(l))
				return false;
			if(from.equals(l.from))
				return true;
			return parallel(new Line(from, l.from));
		}

		public boolean parallel(Line l){
			return increment.cross(l.increment) == 0.0;
		}

		public Vector intersect(Line l){
			if(parallel(l))
				return null;
			double shift =
				(l.increment.y * from.x - l.increment.y * l.from.x - l.increment.x * from.y + l.increment.x
					* l.from.y)
					/ (l.increment.x * increment.y - increment.x * l.increment.y);
			return new Vector(from.x + shift * increment.x, from.y + shift * increment.y);
		}
	}
}
