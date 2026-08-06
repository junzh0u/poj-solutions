import java.math.BigInteger;
import java.util.Scanner;

public class Main{
	private static final Scanner	in	= new Scanner(System.in);

	public static void main(String[] args){
		final BigInteger TWO = BigInteger.valueOf(2);
		while(true){
			int n = in.nextInt();
			if(n < 0)
				break;
			System.out.println("N=" + n + ":");
			System.out.println(factorial(n).multiply(TWO));
		}
	}

	private static BigInteger factorial(int n){
		BigInteger result = BigInteger.ONE;
		while(n-- > 1)
			result = result.multiply(BigInteger.valueOf(n));
		return result;
	}
}
