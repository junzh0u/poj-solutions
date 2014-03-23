import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private class Coordinate{
		int x,y,z,grid;
		public Coordinate(int grid){
			this.grid=grid;
			z=grid/(length*width);
			y=(grid%(length*width))/length;
			x=grid%length;
		}
		public Coordinate(int x,int y,int z){
			this.x=x;
			this.y=y;
			this.z=z;
			grid=x+length*y+length*width*z;
		}
		public boolean Legal(){
			return x>=0 && x<length && y>=0 && y<width && z>=0 && z<height;
		}
		public boolean AtEdge(){
			return x==0 || x==length-1 || y==0 || y==width-1 || z==0 || z==height-1;
		}
		public Collection<Coordinate> Adjacences(){
			Collection<Coordinate> adjacences=new ArrayList<Coordinate>();
			adjacences.add(new Coordinate(x-1,y,z));
			adjacences.add(new Coordinate(x+1,y,z));
			adjacences.add(new Coordinate(x,y-1,z));
			adjacences.add(new Coordinate(x,y+1,z));
			adjacences.add(new Coordinate(x,y,z-1));
			adjacences.add(new Coordinate(x,y,z+1));
			return adjacences;
		}
		public int hashCode(){
			return grid;
		}
		public boolean equals(Object another){
			return (another instanceof Coordinate) && hashCode()==another.hashCode();
		}
	}
	private int length,width,height,faceNum=0;
	private Set<Coordinate> acms=new HashSet<Coordinate>();
	private boolean[] outside;
	public Main(int length,int width,int height,int acmNum){
		this.length=length;
		this.width=width;
		this.height=height;
		outside=new boolean[length*width*height];
		while(acmNum-->0)
			acms.add(new Coordinate(in.nextInt()));
		for(int grid=0;grid<length*width*height;grid++){
			if(outside[grid])
				continue;
			Coordinate coordinate=new Coordinate(grid);
			if(coordinate.AtEdge() && !acms.contains(coordinate))
				FillOutside(coordinate);
		}
	}
	private void FillOutside(Coordinate coordinate){
		Queue<Coordinate> BFSQueue=new LinkedList<Coordinate>();
		BFSQueue.add(coordinate);
		while(!BFSQueue.isEmpty()){
			Coordinate current=BFSQueue.poll();
			if(!current.Legal() || outside[current.grid])
				continue;
			if(!acms.contains(current)){
				outside[current.grid]=true;
				BFSQueue.addAll(current.Adjacences());
			}
		}
	}	
	public void Output(){
		for(int grid=0;grid<length*width*height;grid++){
			Coordinate coordinate=new Coordinate(grid);
			if(acms.contains(coordinate))
				for(Coordinate adjacence:coordinate.Adjacences())
					if(!adjacence.Legal() || outside[adjacence.grid])
						faceNum++;
		}
		System.out.println("The number of faces needing shielding is "+faceNum+".");
	}
	public static void main(String[] args){
		while(true){
			int length=in.nextInt(),width=in.nextInt(),height=in.nextInt(),acmNum=in.nextInt();
			if(length==0 && width==0 && height==0 && acmNum==0)
				break;
			new Main(length,width,height,acmNum).Output();
		}
	}
}