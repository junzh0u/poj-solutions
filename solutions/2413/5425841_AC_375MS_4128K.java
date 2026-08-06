import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		List<BigInteger> fibonacci=new ArrayList<BigInteger>();
		fibonacci.add(BigInteger.ONE);
		fibonacci.add(BigInteger.valueOf(2));
		while(true){
			fibonacci.add(fibonacci.get(fibonacci.size()-1).add(fibonacci.get(fibonacci.size()-2)));
			if(fibonacci.get(fibonacci.size()-1).compareTo(BigInteger.TEN.pow(100))>0)
				break;
		}
		while(true){
			BigInteger lower=in.nextBigInteger(),upper=in.nextBigInteger();
			if(lower.compareTo(BigInteger.ZERO)==0 && upper.compareTo(BigInteger.ZERO)==0)
				break;
			int begin=0;
			while(begin<fibonacci.size() && fibonacci.get(begin).compareTo(lower)<0)
				begin++;
			int end=begin;
			while(end<fibonacci.size() && fibonacci.get(end).compareTo(upper)<=0)
				end++;
			System.out.println(end-begin);
		}
	}
}