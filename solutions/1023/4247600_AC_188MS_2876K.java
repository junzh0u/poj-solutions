import java.util.*;
import java.math.*;

public class Main {
	private static final Scanner in=new Scanner(System.in);
	int bitNum;
	BigInteger data;
	String description;
	public Main(){
		bitNum=in.nextInt();
		description=in.next();
		data=in.nextBigInteger();
		Analyze();
	}
	boolean possible=true;
	int[] digits;
	private void Analyze(){
		BigInteger[] max=new BigInteger[bitNum+1];
		BigInteger[] min=new BigInteger[bitNum+1];
		max[0]=min[0]=BigInteger.valueOf(0);
		for(int i=1;i<=bitNum;i++){
			max[i]=max[i-1];
			min[i]=min[i-1];
			if(description.charAt(bitNum-i)=='p')
				max[i]=max[i].add(BigInteger.valueOf(2).pow(i-1));
			else
				min[i]=min[i].subtract(BigInteger.valueOf(2).pow(i-1));
		}
		BigInteger remain=data;
		digits=new int[bitNum+1];
		Arrays.fill(digits,0);
		for(int i=bitNum;i>=1;i--){
			if(remain.compareTo(max[i])>0 || remain.compareTo(min[i])<0){
				possible=false;
				break;
			}
			if(remain.compareTo(max[i-1])>0 || remain.compareTo(min[i-1])<0){
				digits[i]=1;
				if(description.charAt(bitNum-i)=='p')
					remain=remain.subtract(BigInteger.valueOf(2).pow(i-1));
				else
					remain=remain.add(BigInteger.valueOf(2).pow(i-1));
			}
		}
		if(!remain.equals(BigInteger.valueOf(0)))
			possible=false;
	}
	public void Output(){
		if(!possible){
			System.out.println("Impossible");
			return;
		}
		int i=bitNum;
		for(;i>1 && digits[i]=='0';i--);
		for(;i>=1;i--)
			System.out.print(digits[i]);
		System.out.println();
	}
	public static void main(String[] args) {
		int caseNum=in.nextInt();
		while(caseNum-->0){
			Main system=new Main();
			system.Output();
		}
	}
}