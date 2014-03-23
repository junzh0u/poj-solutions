import java.math.BigInteger;
import java.util.Scanner;

public class Main{
	private static final Scanner	in		= new Scanner(System.in);
	private static final int		N_MAX	= 200;
	
	public static void main(String[] args){
		generateCombineTable();
		while(in.hasNext())
			new Main(in.nextInt(), in.nextInt()).output();
	}
	
	private static BigInteger	combineTable[][];
	
	private static void generateCombineTable(){
		combineTable = new BigInteger[N_MAX + 1][N_MAX + 1];
		for(int n = 0; n <= N_MAX; n++ )
			for(int m = 0; m <= N_MAX; m++ )
				combineTable[n][m] = Combine(n, m);
	}
	
	private static BigInteger Combine(int n, int m){
		if(n - m < m)
			return Combine(n, n - m);
		BigInteger result = BigInteger.ONE;
		for(int i = 0; i < m; i++ )
			result = result.multiply(BigInteger.valueOf(n - i));
		for(int i = 1; i <= m; i++ )
			result = result.divide(BigInteger.valueOf(i));
		return result;
	}
	
	private BigInteger	a[];
	private BigInteger	b[];
	
	public Main(int n, int t){
		generatePowTable(BigInteger.valueOf(t), n);
		b = new BigInteger[n + 1];
		for(int k = 0; k <= n; k++ )
			b[k] = BigInteger.ZERO;
		a = new BigInteger[n + 1];
		for(int k = 0; k <= n; k++ ){
			a[k] = in.nextBigInteger();
			for(int i = 0; i <= k; i++ )
				b[i] = b[i].add(a[k].multiply(combineTable[k][i]).multiply(powTable[k - i]));
		}
	}
	
	private BigInteger	powTable[];
	
	private void generatePowTable(BigInteger base, int size){
		powTable = new BigInteger[size + 1];
		powTable[0] = BigInteger.ONE;
		for(int i = 1; i <= size; i++ )
			powTable[i] = powTable[i - 1].multiply(base);
	}
	
	public void output(){
		for(BigInteger v: b)
			System.out.print(v + " ");
		System.out.println();
	}
}
