import java.util.Arrays;
import java.util.Scanner;

public class Main{
	private static final Scanner in = new Scanner(System.in);

	public static void main(String[] args){
		while(true){
			int frontSprocketNum = in.nextInt();
			if(frontSprocketNum == 0)
				break;
			int rearSprocketNum = in.nextInt();
			new Main(frontSprocketNum,rearSprocketNum);
		}
	}
	public Main(int frontSprocketNum,int rearSprocketNum){
		int frontTeethNum[] = new int[frontSprocketNum],rearTeethNum[] = new int[rearSprocketNum];
		for(int i = 0;i < frontTeethNum.length;i++)
			frontTeethNum[i] = in.nextInt();
		for(int i = 0;i < rearTeethNum.length;i++)
			rearTeethNum[i] = in.nextInt();
		double driveRatios[] = new double[frontTeethNum.length * rearTeethNum.length];
		for(int front = 0;front < frontTeethNum.length;front++)
			for(int rear = 0;rear < rearTeethNum.length;rear++)
				driveRatios[front * rearTeethNum.length + rear] = (double)rearTeethNum[rear] / frontTeethNum[front];
		Arrays.sort(driveRatios);
		double spreadMax = 0.0;
		for(int i = 0;i + 1 < driveRatios.length;i++){
			double spread = driveRatios[i + 1] / driveRatios[i];
			if(spreadMax < spread)
				spreadMax = spread;
		}
		System.out.printf("%.2f\n",spreadMax);
	}
}
