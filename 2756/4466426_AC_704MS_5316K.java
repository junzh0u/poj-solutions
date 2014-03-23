import java.math.*;
import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		int caseNum=in.nextInt();
		while(caseNum-->0)
			System.out.println(NextBigInteger().add(NextBigInteger()));
	}
	private static BigInteger NextBigInteger(){
		String next=in.next();
		if(next.startsWith("+"))
			next=next.substring(1);
		return new BigInteger(next);
	}
}