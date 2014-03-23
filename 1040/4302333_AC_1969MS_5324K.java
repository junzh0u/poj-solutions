import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private int capacity,sectionNum;
	private Order[] orders;
	private int[] maxPossibleEarning;
	private int maxEarning=0;
	class Order implements Comparable<Order>{
		private int from,to;
		private int[] load;
		private int earning;
		public Order(){
			from=in.nextInt();
			to=in.nextInt();
			int passengerNum=in.nextInt();
			load=new int[sectionNum];
			Arrays.fill(load,0);
			for(int section=from;section<to;section++)
				load[section]=passengerNum;
			earning=(to-from)*passengerNum;
		}
		public int compareTo(Order another){
			if(from<another.from)
				return -1;
			if(from>another.from)
				return 1;
			if(to<another.to)
				return -1;
			if(to>another.to)
				return 1;
			return 0;
		}
	}
	public Main(int capacity,int sectionNum,int orderNum){
		this.capacity=capacity;
		this.sectionNum=sectionNum;
		orders=new Order[orderNum];
		maxPossibleEarning=new int[orderNum];
		Arrays.fill(maxPossibleEarning,0);
		for(int i=0;i<orderNum;i++)
			orders[i]=new Order();
		Arrays.sort(orders);
		for(int i=orderNum-1;i>=0;i--)
			for(int j=0;j<=i;j++)
				maxPossibleEarning[j]+=orders[i].earning;
		int[] load=new int[sectionNum];
		Arrays.fill(load,0);
		CalculateMaxEarning(0,load,0);
	}
	private void CalculateMaxEarning(int orderIndex,int[] load,int currentEarning){
		if(orderIndex>=orders.length){
			if(maxEarning<currentEarning)
				maxEarning=currentEarning;
			return;
		}
		if(currentEarning+maxPossibleEarning[orderIndex]<=maxEarning)
			return;
		CalculateMaxEarning(orderIndex+1,load.clone(),currentEarning);
		for(int section=0;section<load.length;section++){
			load[section]+=orders[orderIndex].load[section];
			if(load[section]>capacity)
				return;
		}
		CalculateMaxEarning(orderIndex+1,load.clone(),currentEarning+orders[orderIndex].earning);
	}
	public void Output(){
		System.out.println(maxEarning);
	}
	public static void main(String[] args){
		while(true){
			int capacity=in.nextInt();
			int sectionNum=in.nextInt();
			int orderNum=in.nextInt();
			if(capacity==0 && sectionNum==0 && orderNum==0)
				break;
			Main railway=new Main(capacity,sectionNum,orderNum);
			railway.Output();
		}
	}
}