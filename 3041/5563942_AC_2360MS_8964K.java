import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		System.out.println(new Main(in.nextInt(),in.nextInt()).GetMaxMatchNum());
	}
	
	private class Shot{
		private int value,coverCount;
		private boolean covered,checked;
		private Shot prev;
		public Shot(int value){
			this.value=value;
			coverCount=0;
			covered=checked=false;
			prev=null;
		}
		public int GetValue(){
			return value;
		}
		public boolean IsCovered(){
			return covered;
		}
		public void Cover(){
			coverCount++;
			covered=true;
		}
		public void Uncover(){
			coverCount--;
			if(coverCount==0)
				covered=false;
		}
		public boolean IsChecked(){
			return checked;
		}
		public void SetChecked(boolean value){
			checked=value;
		}
		public Shot GetPrev(){
			return prev;
		}
		public void SetPrev(Shot value){
			prev=value;
		}
	}
	private class Row extends Shot{
		public Row(int value){
			super(value);
		}
	}
	private class Col extends Shot{
		public Col(int value){
			super(value);
		}
	}
	private class Edge{
		private boolean exist,matched;
		public Edge(){
			exist=matched=false;
		}
		public boolean Exist(){
			return exist;
		}
		public void SetExist(boolean value){
			exist=value;
		}
		public boolean IsMatched(){
			return matched;
		}
		public void SetMatched(boolean value){
			matched=value;
		}
	}
	private int size,matchCount;
	private Row rows[];
	private Col cols[];
	private Edge edges[][];
	public Main(int fieldSize,int asteroidNum){
		size=fieldSize;
		rows=new Row[size];
		cols=new Col[size];
		for(int i=0;i<size;i++){
			rows[i]=new Row(i);
			cols[i]=new Col(i);
		}
		edges=new Edge[size][size];
		for(int i=0;i<size;i++)
			for(int j=0;j<size;j++)
				edges[i][j]=new Edge();
		while(asteroidNum-->0)
			edges[in.nextInt()-1][in.nextInt()-1].SetExist(true);
		Hungary();
		matchCount=0;
		for(Row row:rows)
			for(Col col:cols){
				Edge edge=GetEdge(row,col);
				if(edge.Exist() && edge.IsMatched())
					matchCount++;
			}
	}
	private void Hungary(){
		while(true){
			Queue<Shot> augumentPath=FindAugumentPath();
			if(augumentPath==null)
				break;
			FlipAugumentPath(augumentPath);
		}
	}
	private Queue<Shot> FindAugumentPath(){
		for(Row row:rows)
			if(!row.IsCovered()){
				for(Row temp:rows){
					temp.SetChecked(false);
					temp.SetPrev(null);
				}
				for(Col temp:cols){
					temp.SetChecked(false);
					temp.SetPrev(null);
				}
				row.SetChecked(true);
				Queue<Shot> augumentPath=FindAugumentPathFrom(row);
				if(augumentPath!=null)
					return augumentPath;
			}
		return null;
	}
	private Queue<Shot> FindAugumentPathFrom(Row rowBegin){
		for(Col col:cols)
			if(!col.IsChecked()){
				Edge edge=GetEdge(rowBegin,col);
				if(edge.Exist() && !edge.IsMatched()){
					col.SetPrev(rowBegin);
					col.SetChecked(true);
					if(!col.IsCovered()){
						LinkedList<Shot> path=new LinkedList<Shot>();
						for(Shot shot=col;shot!=null;shot=shot.prev)
							path.addLast(shot);
						return path;
					}
					for(Row rowNext:rows)
						if(!rowNext.IsChecked()){
							edge=GetEdge(rowNext,col);
							if(edge.Exist() && edge.IsMatched()){
								rowNext.SetPrev(col);
								rowNext.SetChecked(true);
								Queue<Shot> path=FindAugumentPathFrom(rowNext);
								if(path!=null)
									return path;
							}
						}
				}
			}
		return null;
	}
	private void FlipAugumentPath(Queue<Shot> augumentPath){
		Shot prev=augumentPath.poll();
		while(!augumentPath.isEmpty()){
			Shot current=augumentPath.poll();
			Edge edge=GetEdge(prev,current);
			edge.SetMatched(!edge.IsMatched());
			if(edge.IsMatched()){
				prev.Cover();
				current.Cover();
			}
			else{
				prev.Uncover();
				current.Uncover();
			}
			prev=current;
		}
	}
	private Edge GetEdge(Shot a,Shot b){
		if(a instanceof Row && b instanceof Col)
			return edges[a.GetValue()][b.GetValue()];
		if(b instanceof Row && a instanceof Col)
			return edges[b.GetValue()][a.GetValue()];
		return null;
	}
	public int GetMaxMatchNum(){
		return matchCount;
	}
}