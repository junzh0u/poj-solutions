import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final int YEAR_MAX=10000;
	public int[] displays,fixeds,bugs;
	public Main(int computerNum){
		displays=new int[computerNum];
		fixeds=new int[computerNum];
		bugs=new int[computerNum];
		for(int computer=0;computer<computerNum;computer++){
			displays[computer]=in.nextInt();
			fixeds[computer]=in.nextInt();
			bugs[computer]=in.nextInt();
		}
	}
	public void Output(){
		int year=0;
		for(int display:displays)
			if(year<display)
				year=display;
		while(year<YEAR_MAX){
			if(Possible(year)){
				System.out.println("The actual year is "+year+".");
				break;
			}
			year++;
		}
		if(year>=YEAR_MAX)
			System.out.println("Unknown bugs detected.");
	}
	private boolean Possible(int year){
		for(int computer=0;computer<displays.length;computer++)
			if((year-displays[computer])%(bugs[computer]-fixeds[computer])!=0)
				return false;
		return true;
	}
	public static void main(String[] args){
		int caseNum=0;
		while(true){
			int computerNum=in.nextInt();
			if(computerNum==0)
				break;
			Main testCase=new Main(computerNum);
			System.out.println("Case #"+(++caseNum)+":");
			testCase.Output();
			System.out.println();
		}
	}
}