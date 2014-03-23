import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final char SHADOW='X',LIGHT='-';
	private Side[] sides;
	private int size;
	class Side{
		char[][] shadows;
		public Side(){
			shadows=new char[size][size];
			for(char[] line:shadows)
				System.arraycopy(in.next().toCharArray(),0,line,0,size);
		}
		public Side(char[][] shadows){
			this.shadows=shadows;
		}
		public Side Get(int rotate){
			if(rotate==0)
				return this;
			if(rotate==1)
				return Rotate();
			if(rotate==2)
				return Flip();
			return null;
		}
		public Side Rotate(){
			char[][] newShadows=new char[size][size];
			for(int row=0;row<size;row++)
				for(int col=0;col<size;col++)
					newShadows[col][size-1-row]=shadows[row][col];
			return new Side(newShadows);
		}
		public Side Flip(){
			char[][] newShadows=new char[size][size];
			for(int row=0;row<size;row++)
				for(int col=0;col<size;col++)
					newShadows[row][size-1-col]=shadows[row][col];
			return new Side(newShadows);
			
		}
		public int Area(){
			int area=0;
			for(char[] line:shadows)
				for(char grid:line)
					if(grid==SHADOW)
						area++;
			return area;
		}
	}
	class Cube{
		Side sideX,sideY,sideZ;
		boolean[][][] blocks=new boolean[size][size][size];
		public Cube(Side sideX,Side sideY,Side sideZ){
			this.sideX=sideX;
			this.sideY=sideY;
			this.sideZ=sideZ;
			for(boolean[][] face:blocks)
				for(boolean[] row:face)
					Arrays.fill(row,true);
			for(int y=0;y<size;y++)
				for(int z=0;z<size;z++)
					if(sideX.shadows[y][z]==LIGHT)
						for(int x=0;x<size;x++)
							blocks[x][y][z]=false;
			for(int x=0;x<size;x++)
				for(int z=0;z<size;z++)
					if(sideY.shadows[x][z]==LIGHT)
						for(int y=0;y<size;y++)
							blocks[x][y][z]=false;
			for(int x=0;x<size;x++)
				for(int y=0;y<size;y++)
					if(sideZ.shadows[x][y]==LIGHT)
						for(int z=0;z<size;z++)
							blocks[x][y][z]=false;
		}
		private boolean Valid(){
			while(true){
				boolean moreLinkedBlocks=false;
				for(int x=0;x<size;x++)
					for(int y=0;y<size;y++)
						for(int z=0;z<size;z++)
							if(blocks[x][y][z]){
								moreLinkedBlocks=true;
								boolean[][][] linkedBlocks=new boolean[size][size][size];
								SeprateBlocks(x,y,z,linkedBlocks);
								Block block=new Block(linkedBlocks);
								if(block.Suit(sideX,sideY,sideZ))
									return true;
							}
				if(!moreLinkedBlocks)
					break;
			}
			return false;
		}
		private void SeprateBlocks(int x,int y,int z,boolean[][][] linkedBlocks){
			if(x<0 || x>=size || y<0 || y>=size || z<0 || z>=size || !blocks[x][y][z])
				return;
			linkedBlocks[x][y][z]=true;
			blocks[x][y][z]=false;
			SeprateBlocks(x-1,y,z,linkedBlocks);
			SeprateBlocks(x+1,y,z,linkedBlocks);
			SeprateBlocks(x,y-1,z,linkedBlocks);
			SeprateBlocks(x,y+1,z,linkedBlocks);
			SeprateBlocks(x,y,z-1,linkedBlocks);
			SeprateBlocks(x,y,z+1,linkedBlocks);
		}
		private class Block{
			private boolean[][][] linkedBlocks;
			public Block(boolean[][][] linkedBlocks){
				this.linkedBlocks=linkedBlocks;
			}
			public boolean Suit(Side sideX,Side sideY,Side sideZ){
				int count=0;
				for(int y=0;y<size;y++)
					for(int z=0;z<size;z++)
						for(int x=0;x<size;x++)
							if(linkedBlocks[x][y][z]){
								count++;
								break;
							}
				if(count!=sideX.Area())
					return false;
				count=0;
				for(int x=0;x<size;x++)
					for(int z=0;z<size;z++)
						for(int y=0;y<size;y++)
							if(linkedBlocks[x][y][z]){
								count++;
								break;
							}
				if(count!=sideY.Area())
					return false;
				count=0;
				for(int x=0;x<size;x++)
					for(int y=0;y<size;y++)
						for(int z=0;z<size;z++)
							if(linkedBlocks[x][y][z]){
								count++;
								break;
							}
				if(count!=sideZ.Area())
					return false;
				return true;
			}
		}
	}
	public Main(int size){
		this.size=size;
		sides=new Side[3];
		for(int side=0;side<sides.length;side++)
			sides[side]=new Side();
	}
	public boolean Valid(){
		for(int rotateX=0;rotateX<3;rotateX++)
			for(int rotateY=0;rotateY<3;rotateY++)
				for(int rotateZ=0;rotateZ<3;rotateZ++)
					if(new Cube(sides[0].Get(rotateX),sides[1].Get(rotateY),sides[2].Get(rotateZ)).Valid())
						return true;
		return false;
	}
	public static void main(String[] args){
		int setCount=0;
		while(true){
			int size=in.nextInt();
			if(size==0)
				break;
			Main plato=new Main(size);
			System.out.print("Data set "+(++setCount)+": ");
			if(plato.Valid())
				System.out.println("Valid set of patterns");
			else
				System.out.println("Impossible combination");
		}
	}
}