import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	int cableNum,requestedPieceNum;
	ArrayList<Integer> cableLengths;
	public Main(){
		cableNum=in.nextInt();
		cableLengths=new ArrayList<Integer>(cableNum);
		requestedPieceNum=in.nextInt();
		for(int i=0;i<cableNum;i++)
			cableLengths.add((int)(in.nextDouble()*100));
	}
	public void Output(){
		int lengthMax=Collections.max(cableLengths);
		System.out.printf("%.2f\n",BinarySearch(0,lengthMax+1)/100.0);
	}
	private int BinarySearch(int min,int max){
		if(min+1>=max)
			return min;
		int middle=(min+max)/2,pieceNum=0;
		for(int cableLength:cableLengths)
			pieceNum+=cableLength/middle;
		if(pieceNum<requestedPieceNum)
			return BinarySearch(min,middle);
		return BinarySearch(middle,max);
	}
	public static void main(String[] args){
		Main network=new Main();
		network.Output();
	}
}