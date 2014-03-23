import java.math.BigInteger;
import java.util.Scanner;

public class Main{
	private static final Scanner	in	= new Scanner(System.in);
	
	public static void main(String[] args){
		BigInteger a[] = new BigInteger[100];
		while(in.hasNext()){
			for(int i = 0; i < 3; i++ )
				a[i] = in.nextBigInteger();
			for(int i = 3; i < 100; i++ )
				a[i] = a[i - 1].add(a[i - 2].add(a[i - 3]));
			System.out.println(a[99]);
		}
	}
}
