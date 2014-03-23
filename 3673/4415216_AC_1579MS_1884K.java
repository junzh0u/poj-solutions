import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static int Sum(){
		int sum=0;
		for(char digit:in.next().toCharArray())
			sum+=digit-'0';
		return sum;
	}
	public static void main(String[] args){
		System.out.println(Sum()*Sum());
	}
}