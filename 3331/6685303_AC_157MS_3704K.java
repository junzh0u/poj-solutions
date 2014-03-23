import java.math.BigInteger;
import java.util.Scanner;

public class Main{
	private static final Scanner in = new Scanner(System.in);

	private static BigInteger factorial(int number){
		BigInteger product = BigInteger.ONE;
		for(int factor = 2;factor <= number;factor++)
			product = product.multiply(BigInteger.valueOf(factor));
		return product;
	}
	public static void main(String[] args){
		int caseNum = in.nextInt();
		while(caseNum-- > 0){
			int day = in.nextInt(),digit = in.nextInt();
			BigInteger factorial = factorial(day);
			String factorialString = factorial.toString();
			int count = 0;
			for(int index = 0;index < factorialString.length();index++)
				if(factorialString.charAt(index) == '0' + digit)
					count++;
			System.out.println(count);
		}
	}
}