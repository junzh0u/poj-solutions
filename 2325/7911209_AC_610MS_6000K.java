import java.math.BigInteger;
import java.util.Scanner;
import java.util.Stack;

public class Main{
	private static final Scanner	in	= new Scanner(System.in);
	
	public static void main(String[] args){
		while(true){
			BigInteger number = in.nextBigInteger();
			if(number.equals(BigInteger.valueOf(-1)))
				break;
			if(number.compareTo(BigInteger.valueOf(10)) < 0){
				System.out.println("1" + number);
				continue;
			}
			int count2 = 0, count3 = 0, count5 = 0, count7 = 0;
			while(number.mod(BigInteger.valueOf(2)).equals(BigInteger.ZERO)){
				count2++;
				number = number.divide(BigInteger.valueOf(2));
			}
			while(number.mod(BigInteger.valueOf(3)).equals(BigInteger.ZERO)){
				count3++;
				number = number.divide(BigInteger.valueOf(3));
			}
			while(number.mod(BigInteger.valueOf(5)).equals(BigInteger.ZERO)){
				count5++;
				number = number.divide(BigInteger.valueOf(5));
			}
			while(number.mod(BigInteger.valueOf(7)).equals(BigInteger.ZERO)){
				count7++;
				number = number.divide(BigInteger.valueOf(7));
			}
			if(!number.equals(BigInteger.ONE)){
				System.out.println("There is no such number.");
				continue;
			}
			Stack<Integer> digits = new Stack<Integer>();
			while(count3 >= 2){
				digits.push(9);
				count3 -= 2;
			}
			while(count2 >= 3){
				digits.push(8);
				count2 -= 3;
			}
			while(count7 >= 1){
				digits.push(7);
				count7--;
			}
			while(count2 >= 1 && count3 >= 1){
				digits.push(6);
				count2--;
				count3--;
			}
			while(count5 >= 1){
				digits.push(5);
				count5--;
			}
			while(count2 >= 2){
				digits.push(4);
				count2 -= 2;
			}
			while(count3 >= 1){
				digits.push(3);
				count3--;
			}
			while(count2 >= 1){
				digits.push(2);
				count2 -= 1;
			}
			// System.out.println(digits);
			while(!digits.empty())
				System.out.print(digits.pop());
			System.out.println();
		}
	}
}
