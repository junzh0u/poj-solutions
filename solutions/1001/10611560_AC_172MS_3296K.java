import java.math.BigDecimal;
import java.util.Scanner;

public class Main{
	private static final Scanner	in	= new Scanner(System.in);

	public static void main(String[] args){
		while(in.hasNext()){
			BigDecimal base = in.nextBigDecimal();
			int exponent = in.nextInt();
			BigDecimal power = base.pow(exponent).stripTrailingZeros();
			String result = power.toPlainString();
			if(result.startsWith("0."))
				result = result.substring(1);
			System.out.println(result);
		}
	}
}
