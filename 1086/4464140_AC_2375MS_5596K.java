import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		int caseNum=in.nextInt();
		for(int caseID=1;caseID<=caseNum;caseID++){
			System.out.println("Case "+caseID+"\n");
			new Main().Output();
			System.out.println();
		}
	}
	private int size;
	private Map<Integer,Integer> testTree=new HashMap<Integer,Integer>(),secretTree=new HashMap<Integer,Integer>();
	private int[][] testImage,secretImage,decodedImage;
	public Main(){
		size=in.nextInt();
		int testTreeLeafNum=in.nextInt();
		while(testTreeLeafNum-->0)
			testTree.put(in.nextInt(),in.nextInt());
		testImage=new int[size][size];
		Fill(0,0,0,size,testTree,testImage);
		int secretTreeLeafNum=in.nextInt();
		while(secretTreeLeafNum-->0)
			secretTree.put(in.nextInt(),in.nextInt());
		secretImage=new int[size][size];
		Fill(0,0,0,size,secretTree,secretImage);
		decodedImage=new int[size][size];
		for(int row=0;row<size;row++)
			for(int col=0;col<size;col++)
				decodedImage[testImage[row][col]/size][testImage[row][col]%size]=secretImage[row][col];
	}
	private void Fill(int node,int rowMin,int colMin,int size,Map<Integer,Integer> tree,int[][] image){
		if(tree.containsKey(node)){
			for(int row=rowMin;row<rowMin+size;row++)
				for(int col=colMin;col<colMin+size;col++)
					image[row][col]=tree.get(node);
			return;
		}
		Fill(4*node+1,rowMin,colMin,size/2,tree,image);
		Fill(4*node+2,rowMin,colMin+size/2,size/2,tree,image);
		Fill(4*node+3,rowMin+size/2,colMin,size/2,tree,image);
		Fill(4*node+4,rowMin+size/2,colMin+size/2,size/2,tree,image);
	}
	public void Output(){
		for(int[] row:decodedImage){
			for(int pixal:row)
				System.out.printf("%4d",pixal);
			System.out.println();
		}
	}
}