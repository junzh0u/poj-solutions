import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final int UNALLOTED=-1;
	private class Memory{
		private class Piece{
			private int begin,end;
			public Piece(int begin,int end){
				this.begin=begin;
				this.end=end;
			}
		}
		private List<Piece> alloted=new ArrayList<Piece>();
		public Memory(int capacity){
			alloted.add(new Piece(0,0));
			alloted.add(new Piece(capacity,capacity));
		}
		public boolean CanAllotPiece(int length){
			for(int i=0;i<alloted.size()-1;i++)
				if(alloted.get(i+1).begin-alloted.get(i).end>=length)
					return true;
			return false;
		}
		public int Allot(int length){
			for(int i=0;i<alloted.size()-1;i++)
				if(alloted.get(i+1).begin-alloted.get(i).end>=length){
					alloted.add(i+1,new Piece(alloted.get(i).end,alloted.get(i).end+length));
					return alloted.get(i).end;
				}
			return UNALLOTED;
		}
		public void Release(int begin){
			for(int i=1;i<alloted.size()-1;i++)
				if(alloted.get(i).begin==begin)
					alloted.remove(i);
		}
	}
	private class Process implements Comparable<Process>{
		private int memoryNeed,timeProceed,timeAct,memoryPosition=UNALLOTED;
		public Process(int timeApply,int memoryNeed,int timeProceed){
			timeAct=timeApply;
			this.memoryNeed=memoryNeed;
			this.timeProceed=timeProceed;
		}
		public boolean CanBegin(){
			return memory.CanAllotPiece(memoryNeed);
		}
		public void Begin(int time){
			memoryPosition=memory.Allot(memoryNeed);
			timeAct=time+timeProceed;
			end.add(this);
			timePoints.add(timeAct);
		}
		public void End(){
			memory.Release(memoryPosition);
		}
		public void Wait(){
			wait.add(this);
			waitCount++;
		}
		public int compareTo(Process another){
			return Integer.signum(timeAct-another.timeAct);
		}
	}
	private Memory memory;
	private int timeCurrent=0,waitCount=0;
	private Queue<Process> begin=new LinkedList<Process>();
	private PriorityQueue<Process> end=new PriorityQueue<Process>();
	private Queue<Process> wait=new LinkedList<Process>();
	private SortedSet<Integer> timePoints=new TreeSet<Integer>();
	public Main(){
		memory=new Memory(in.nextInt());
		while(true){
			int timeApply=in.nextInt(),memoryNeed=in.nextInt(),timeProceed=in.nextInt();
			if(timeApply==0 && memoryNeed==0 && timeProceed==0)
				break;
			Process process=new Process(timeApply,memoryNeed,timeProceed);
			begin.add(process);
			timePoints.add(timeApply);
		}
		while(!timePoints.isEmpty()){
			int time=timePoints.first();
			timeCurrent=time;
			timePoints.remove(time);
			while(!end.isEmpty() && end.peek().timeAct==time)
				end.poll().End();
			while(!wait.isEmpty() && wait.peek().CanBegin())
				wait.poll().Begin(time);
			while(!begin.isEmpty() && begin.peek().timeAct==time){
				Process process=begin.poll();
				if(process.CanBegin())
					process.Begin(time);
				else
					process.Wait();
			}
		}
	}
	public void Output(){
		System.out.println(timeCurrent+"\n"+waitCount);
	}
	public static void main(String[] args){
		new Main().Output();
	}
}