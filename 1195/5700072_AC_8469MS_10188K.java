import java.util.Scanner;

public class Main{
	private static Scanner in = new Scanner(System.in);

	public static void main(String[] args){
		Main main = null;
		while(true){
			int instruction = in.nextInt();
			switch(instruction){
				case 0:
					main = new Main(in.nextShort());
					break;
				case 1:
					main.increase(in.nextShort(),in.nextShort(),in.nextShort());
					break;
				case 2:
					System.out.println(main.area(in.nextShort(),in.nextShort(),(short)(in.nextShort() + 1),(short)(in.nextShort() + 1)));
					break;
				case 3:
					return;
			}
		}
	}

	int nodes[][];

	public Main(short size){
		nodes = new int[size][size];
		for(short row = 0;row < size;row++)
			for(short col = 0;col < size;col++)
				nodes[row][col] = 0;
	}
	public void increase(short col,short row,short value){
		for(short i = row;i < nodes.length;i += lowbit((short)(i + 1)))
			for(short j = col;j < nodes[0].length;j += lowbit((short)(j + 1)))
				nodes[i][j] += value;
	}
	private short lowbit(short x){
		return (short)(x & (x ^ x - 1));
	}
	public int sum(short height,short width){
		int sum = 0;
		for(short i = height;i > 0;i -= lowbit(i))
			for(short j = width;j > 0;j -= lowbit(j))
				sum += nodes[i - 1][j - 1];
		return sum;
	}
	public int area(short left,short bottom,short right,short top){
		return sum(top,right) + sum(bottom,left) - sum(bottom,right) - sum(top,left);
	}
}