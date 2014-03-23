import java.math.BigInteger;
import java.util.Scanner;

public class Main{
	private static final Scanner in = new Scanner(System.in);

	public static void main(String[] args){
		while(true){
			int nephewNum = in.nextInt();
			int cdNum = in.nextInt();
			if(nephewNum == 0 && cdNum == 0)
				break;
			System.out.println(BigInteger.valueOf(nephewNum).pow(cdNum));
		}
	}
}