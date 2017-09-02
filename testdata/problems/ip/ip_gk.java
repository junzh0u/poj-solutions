import java.io.*;
import java.util.*;

public class ip_gk {
    public static String ip(int n) {
        return ((n >>> 24) & 0xff) + "." + ((n >>> 16) & 0xff) + "." + ((n >>> 8) & 0xff) + "." + (n & 0xff);
    }
    public static void main(String[] args) throws IOException {
        Scanner scanner = new Scanner(new File("ip.in"));
        PrintWriter writer = new PrintWriter(new FileOutputStream("ip.out"));

        scanner.useDelimiter("(\\s|\\.)+");

        int n = scanner.nextInt();
        int[] ip = new int[n];
        for (int i = 0; i < n; i++) {
            ip[i] = (scanner.nextInt() << 24) + (scanner.nextInt() << 16) + (scanner.nextInt() << 8) + scanner.nextInt();
        }

        for (int i = 0; i <= 32; i++) {
            boolean f = true;
            int mask = ~(i == 0 ? 0 : (1 << (i - 1) << 1) - 1);
            for (int j = 1; j < n; j++) {
                f &= (ip[0] & mask) == (ip[j] & mask);
            }
            if (f) {
                writer.println(ip(ip[0] & mask));
                writer.println(ip(mask));
                break;
            }
        }

        scanner.close();
        writer.close();
    }
}