import java.util.*;

public class Main {
	public final static char STAR='1';
	public final static char BACKGROUND='0';
	private Scanner in=new Scanner(System.in);
	private int width,height;
	private char[][] map;
	public Main(){
		width=in.nextInt();
		height=in.nextInt();
		map=new char[height][width];
		for(int line=0;line<height;line++)
			map[line]=in.next().toCharArray();
	}
	private class Coordinate{
		public int x,y;
		public Coordinate(int x,int y){
			this.x=x;
			this.y=y;
		}
	}
	private class Cluster{
		public List<Coordinate> coordinates=new ArrayList<Coordinate>();
		public Cluster(int x,int y){
			map[y][x]=BACKGROUND;
			coordinates.add(new Coordinate(x,y));
			Expand();
		}
		private void Expand(){
			for(int i=0;i<coordinates.size();i++){
				int x=coordinates.get(i).x;
				int y=coordinates.get(i).y;
				int[][] off={{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};
				for(int dir=0;dir<off.length;dir++){
					if(	x+off[dir][0]>=0 && x+off[dir][0]<width &&
						y+off[dir][1]>=0 && y+off[dir][1]<height &&
						map[y+off[dir][1]][x+off[dir][0]]==STAR){
						map[y+off[dir][1]][x+off[dir][0]]=BACKGROUND;
						coordinates.add(new Coordinate(x+off[dir][0],y+off[dir][1]));
					}
				}
			}
		}
		public void Paint(char letter){
			for(int i=0;i<coordinates.size();i++)
				map[coordinates.get(i).y][coordinates.get(i).x]=letter;
		}
	}
	private class ClusterStyle{
		int clusterWidth,clusterHeight;
		char[][] clusterMap;
		String feature;
		public ClusterStyle(Cluster cluster){
			int xMin=width,xMax=0,yMin=height,yMax=0;
			for(int i=0;i<cluster.coordinates.size();i++){
				int x=cluster.coordinates.get(i).x;
				int y=cluster.coordinates.get(i).y;
				if(xMin>x)
					xMin=x;
				if(xMax<x)
					xMax=x;
				if(yMin>y)
					yMin=y;
				if(yMax<y)
					yMax=y;
			}
			clusterWidth=xMax-xMin+1;
			clusterHeight=yMax-yMin+1;
			clusterMap=new char[clusterHeight][clusterWidth];
			for(int line=0;line<clusterHeight;line++)
				Arrays.fill(clusterMap[line],BACKGROUND);
			for(int i=0;i<cluster.coordinates.size();i++)
				clusterMap[cluster.coordinates.get(i).y-yMin][cluster.coordinates.get(i).x-xMin]=STAR;
			Adjust();
		}
		private void Adjust(){
			if(clusterWidth<clusterHeight)
				Rotate();
			feature=BestFeature();
			if(clusterWidth==clusterHeight){
				Rotate();
				String newFeature=BestFeature();
				if(feature.compareTo(newFeature)<0)
					feature=newFeature;
			}
		}
		private void Rotate(){
			int temp=clusterWidth;
			clusterWidth=clusterHeight;
			clusterHeight=temp;
			char[][] mapTemp=clusterMap;
			clusterMap=new char[clusterHeight][clusterWidth];
			for(int line=0;line<clusterHeight;line++)
				for(int col=0;col<clusterWidth;col++)
					clusterMap[line][col]=mapTemp[col][line];
		}
		private String BestFeature(){
			String feature=null;
			int[] x={0,clusterWidth-1,clusterWidth-1,0};
			int[] xStep={1,-1,-1,1};
			int[] y={0,0,clusterHeight-1,clusterHeight-1};
			int[] yStep={1,1,-1,-1};
			for(int i=0;i<x.length;i++){
				String newFeature=CreateFeature(x[i],xStep[i],y[i],yStep[i]);
				if(feature==null || feature.compareTo(newFeature)<0)
					feature=newFeature;
			}
			return feature;
		}
		private String CreateFeature(int x,int xStep,int y,int yStep){
			StringBuffer buffer=new StringBuffer();
			for(int line=y;line>=0 && line<clusterHeight;line+=yStep)
				for(int col=x;col>=0 && col<clusterWidth;col+=xStep)
					buffer.append(clusterMap[line][col]);
			return buffer.toString();
		}
		public int hashCode(){
			return clusterWidth;
		}
		public boolean equals(Object o){
			return this.feature.equals(((ClusterStyle)o).feature);
		}
	}
	Map<ClusterStyle,Character> styles=new HashMap<ClusterStyle,Character>();
	public void Analyze(){
		for(int y=0;y<height;y++){
			for(int x=0;x<width;x++){
				if(map[y][x]==STAR){
					Cluster cluster=new Cluster(x,y);
					ClusterStyle style=new ClusterStyle(cluster);
					if(!styles.containsKey(style))
						styles.put(style,(char)('a'+styles.size()));
					cluster.Paint(styles.get(style).charValue());
				}
			}
		}
	}
	public void Output(){
		for(int line=0;line<height;line++)
			System.out.println(map[line]);
	}
	public static void main(String[] args) {
		Main sky=new Main();
		sky.Analyze();
		sky.Output();
	}
}