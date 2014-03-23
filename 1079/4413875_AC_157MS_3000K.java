import java.util.Scanner;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		while(in.hasNextInt()){
			double gain=in.nextInt(),lose=in.nextInt(),quotient=gain/lose,error=Double.MAX_VALUE;
			for(double denominator=1;denominator<=lose;denominator++){
				double numerator=(int)(quotient*denominator);
				if(Math.abs(numerator/denominator-quotient)>=Math.abs((numerator+1)/denominator-quotient))
					numerator++;
				if(Math.abs(numerator/denominator-quotient)<error){
					System.out.println((int)numerator+"/"+(int)denominator);
					error=Math.abs(numerator/denominator-quotient);
				}
			}
			System.out.println();
		}
	}
}