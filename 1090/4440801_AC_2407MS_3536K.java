import java.math.*;
import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		int[] original;
		original=new int[in.nextInt()];
		for(int i=0;i<original.length;i++)
			original[i]=in.nextInt();
		int last=0;
		BigInteger moveNum=BigInteger.ZERO;
		for(int i=original.length-1;i>=0;i--){
			if(original[i]!=last){
				moveNum=moveNum.setBit(i);
				last=1;
			}
			else
				last=0;
		}
		System.out.println(moveNum);
	}
}