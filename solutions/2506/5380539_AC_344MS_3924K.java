import java.math.BigInteger;
import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static BigInteger POSSIBLE_TILING_NUM[]=new BigInteger[251];
	public static void main(String[] args){
		POSSIBLE_TILING_NUM[0]=POSSIBLE_TILING_NUM[1]=BigInteger.ONE;
		for(int i=2;i<=250;i++)
			POSSIBLE_TILING_NUM[i]=POSSIBLE_TILING_NUM[i-1].add(POSSIBLE_TILING_NUM[i-2].multiply(BigInteger.valueOf(2)));
		while(in.hasNext())
			System.out.println(POSSIBLE_TILING_NUM[in.nextInt()]);
	}
}