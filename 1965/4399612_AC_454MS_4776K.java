import java.math.*;
import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static BigInteger CubeRoot(BigInteger cube){
		return BinarySearch(BigInteger.ZERO,cube,cube);
	}
	private static BigInteger BinarySearch(BigInteger min,BigInteger max,BigInteger cube){
		if(max.subtract(min).equals(BigInteger.ONE))
			return min;
		BigInteger middle=min.add(max).divide(BigInteger.valueOf(2));
		if(middle.pow(3).compareTo(cube)>0)
			return BinarySearch(min,middle,cube);
		return BinarySearch(middle,max,cube);
	}
	public static void main(String[] args){
		while(in.hasNextBigInteger()){
			BigInteger cube=in.nextBigInteger();
			String root=CubeRoot(cube.multiply(BigInteger.TEN.pow(30))).toString();
			int checkSum=0;
			for(char digit:root.toCharArray())
				checkSum=(checkSum+digit-'0')%10;
			System.out.println(checkSum+" "+root.substring(0,root.length()-10)+"."+root.substring(root.length()-10));
		}
	}
}