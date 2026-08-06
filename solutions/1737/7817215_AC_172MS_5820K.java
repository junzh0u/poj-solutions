import java.math.BigInteger;
import java.util.Scanner;

public class Main{
	private static final Scanner	in				= new Scanner(System.in);
	private static final int		N_MAX			= 60;
	public static BigInteger		answers[]		= new BigInteger[N_MAX + 1];
	public static BigInteger		combination[][]	= new BigInteger[N_MAX + 1][N_MAX + 1];
	
	public static void main(String[] args){
		generateCombinationTable();
		answers[1] = BigInteger.ONE;
		for(int n = 2; n <= N_MAX; n++ ){
			answers[n] = BigInteger.valueOf(2).pow(combination[n][2].intValue());
			for(int i = 1; i <= n - 1; i++ ){
				BigInteger minuend = combination[n - 1][i - 1];
				minuend = minuend.multiply(answers[i]);
				minuend = minuend.multiply(BigInteger.valueOf(2).pow(combination[n - i][2].intValue()));
				answers[n] = answers[n].subtract(minuend);
			}
		}
		while(true){
			int n = in.nextInt();
			if(n == 0)
				return;
			System.out.println(answers[n]);
		}
	}
	
	private static void generateCombinationTable(){
		for(int n = 0; n <= N_MAX; n++ )
			combination[n][0] = BigInteger.ONE;
		for(int n = 0; n <= N_MAX; n++ )
			for(int m = 1; m < N_MAX; m++ )
				if(n < m)
					combination[n][m] = BigInteger.ZERO;
				else
					combination[n][m] = combination[n - 1][m].add(combination[n - 1][m - 1]);
	}
}
