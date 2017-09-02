import java.util.Scanner;
import java.util.Random;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class knockdown_petr {
    static final double EPS = 1e-10;

    static class Point {
        static final Point ORIGIN = new Point(0, 0, 0);

        public final double x;
        public final double y;
        public final double z;

        public Point(double x, double y, double z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public Point Inverse() {
            return new Point(-x, -y, -z);
        }

        public static Point Product(Point p1, Point p2) {
            return new Point(
                p1.y * p2.z - p1.z * p2.y,
                p1.z * p2.x - p1.x * p2.z,
                p1.x * p2.y - p1.y * p2.x
            );
        }

        public static Point Subtract(Point p1, Point p2) {
            return new Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
        }

        public Point Normalize() {
            double length = Geometry.LineDistance(this, ORIGIN);
            if (length == 0)
                return new Point(1, 0, 0);
            else
                return new Point(x / length, y / length, z / length);
        }
    }

    static class Geometry {
        static Random random = new Random();

        static double LineDistance(Point p1, Point p2) {
            double dx = p1.x - p2.x;
            double dy = p1.y - p2.y;
            double dz = p1.z - p2.z;
            return Math.sqrt(dx * dx + dy * dy + dz * dz);
        }

        static double SphericalDistance(Point p1, Point p2) {
            double d = LineDistance(p1, p2);

            return 2 * Math.asin(d / 2);
        }

        static Point[] SphericalMiddle(Point p1, Point p2) {
            Point middle = new Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
            double length = LineDistance(middle, Point.ORIGIN);
            if (Math.abs(length) < EPS) {
                Point rnd = new Point(random.nextDouble(), random.nextDouble(), random.nextDouble());
                middle = Point.Product(p1, rnd);
            }
            middle = middle.Normalize();
            return new Point[]{middle, middle.Inverse()};
        }

        static Point[] SphericalMiddle(Point p1, Point p2, Point p3) {
            Point v1 = Point.Subtract(p2, p1);
            Point v2 = Point.Subtract(p3, p1);
            Point v = Point.Product(v1, v2);
            Point middle = v.Normalize();
            return new Point[]{middle, middle.Inverse()};
        }

        static double SphericalDistance(Point p1, Point[] p2) {
            double res = Double.MAX_VALUE;

            for (Point p : p2) {
                double cur = SphericalDistance(p1, p);
                if (cur < res)
                    res = cur;
            }

            return res;
        }
    }

    Point[] readInput() throws FileNotFoundException {
        Scanner scanner = new Scanner(new File("knockdown.in"));

        int numBombs = scanner.nextInt();

        Point[] bombs = new Point[numBombs];

        for (int i = 0; i < numBombs; ++i) {
            double latitude = scanner.nextDouble() * Math.PI / 180;
            double longitude = scanner.nextDouble() * Math.PI / 180;
            double x = Math.cos(latitude) * Math.cos(longitude);
            double y = Math.cos(latitude) * Math.sin(longitude);
            double z = Math.sin(latitude);
            bombs[i] = new Point(x, y, z);
        }

        return bombs;
    }

    double solve(Point[] bombs) {
        double result = 0;

        for (int i = 0; i < bombs.length; ++i) {
            for (int j = i; j < bombs.length; ++j) {
                Point[] middle = Geometry.SphericalMiddle(bombs[i], bombs[j]);
                for (Point point : middle) {
                    double cur = Geometry.SphericalDistance(point, bombs);
                    if (cur > result)
                        result = cur;
                }
                if (j > i)
                    for (int k = j + 1; k < bombs.length; ++k) {
                        Point[] middle3 = Geometry.SphericalMiddle(bombs[i], bombs[j], bombs[k]);
                        for (Point point : middle3) {
                            double cur = Geometry.SphericalDistance(point, bombs);
                            if (cur > result)
                                result = cur;
                        }
                    }
            }
        }

        return result;
    }

    void writeOutput(double result) throws FileNotFoundException {
        PrintWriter printWriter = new PrintWriter("knockdown.out");

        printWriter.println(result);

        printWriter.close();
    }

    void run() throws FileNotFoundException {
        Point[] bombs = readInput();
        double result = solve(bombs);
        writeOutput(result);
    }

    public static void main(String[] args) throws FileNotFoundException {
        new knockdown_petr().run();
    }
}
