import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	@SuppressWarnings("unused")
    private static final char MINED='*',UNMINED='.',TOUCHED='x',UNTOUCHED='.';
	public static void main(String[] args){
		new Main(in.nextInt()).Output();
	}
	private int size;
	private boolean[][] mined,touched;
	public Main(int size){
		this.size=size;
		Input();
	}
	private void Input(){
		mined=new boolean[size][size];
		for(int row=0;row<size;row++){
			String line=in.next();
			for(int col=0;col<size;col++)
				mined[row][col]=line.charAt(col)==MINED;
		}
		touched=new boolean[size][size];
		for(int row=0;row<size;row++){
			String line=in.next();
			for(int col=0;col<size;col++)
				touched[row][col]=line.charAt(col)==TOUCHED;
		}
	}
	public void Output(){
		char[][] board=new char[size][size];
		boolean mineTouched=false;
		for(int row=0;row<size;row++)
			for(int col=0;col<size;col++){
				if(touched[row][col]){
					if(mined[row][col])
						mineTouched=true;
					else
						board[row][col]=(char)(MineAdjacentNum(row,col)+'0');
				}
				else
					board[row][col]=UNTOUCHED;
			}
		if(mineTouched)
			for(int row=0;row<size;row++)
				for(int col=0;col<size;col++)
					if(mined[row][col])
						board[row][col]=MINED;
		for(char[] row:board)
			System.out.println(row);
	}
	private int MineAdjacentNum(int row,int col){
		int count=0;
		if(LegalAndMined(row-1,col-1))
			count++;
		if(LegalAndMined(row-1,col))
			count++;
		if(LegalAndMined(row-1,col+1))
			count++;
		if(LegalAndMined(row,col-1))
			count++;
		if(LegalAndMined(row,col+1))
			count++;
		if(LegalAndMined(row+1,col-1))
			count++;
		if(LegalAndMined(row+1,col))
			count++;
		if(LegalAndMined(row+1,col+1))
			count++;
		return count;
	}
	private boolean LegalAndMined(int row,int col){
		return row>=0 && row<size && col>=0 && col<size && mined[row][col];
	}
}