import java.util.*;
import java.io.*;

public class Main {
	private static final int CONTEST_NUM=2;
	private static final int UNINITIALIZED=-1;
	private SortedMap<Integer,Team> teams=new TreeMap<Integer,Team>();
	private SortedSet<Team> rankedTeams=new TreeSet<Team>();
	private class Team implements Comparable<Team>{
		private int ID;
		private int[] ranks=new int[CONTEST_NUM];
		private int valueI=UNINITIALIZED,valueII=UNINITIALIZED;
		public Team(int ID){
			this.ID=ID;
			Arrays.fill(ranks,UNINITIALIZED);
		}
		public void SetRank(int contest,int rank){
			ranks[contest]=rank;
		}
		public int GetRank(int contest){
			return ranks[contest];
		}
		public boolean ApplyRuleI(){
			if(!ParticipatedAllContest())
				return false;
			valueI=valueII=0;
			for(int contest=0;contest<CONTEST_NUM;contest++)
				valueI+=ranks[contest];
			return true;
		}
		private boolean ParticipatedAllContest(){
			for(int contest=0;contest<CONTEST_NUM;contest++)
				if(ranks[contest]==UNINITIALIZED)
					return false;
			return true;
		}
		public boolean ApplyRuleII(){
			if(ParticipatedAllContest())
				return false;
			if(!ApplyRuleA())
				return ApplyRuleB();
			return true;
		}
		private boolean ApplyRuleA(){
			int contest=ContestParticipated();
			for(Iterator<Team> iter=teams.values().iterator();iter.hasNext();){
				Team team=iter.next();
				if(team.ParticipatedAllContest() && ranks[contest]==team.ranks[contest]){
					if(valueI==UNINITIALIZED)
						valueI=team.valueI;
					else if(valueI!=team.valueI){
						valueI=UNINITIALIZED;
						return true;
					}
				}
			}
			if(valueI==UNINITIALIZED)
				return false;
			valueII=0;
			return true;
		}
		private boolean ApplyRuleB(){
			int contest=ContestParticipated();
			int lower=0,upper=Integer.MAX_VALUE;
			for(Iterator<Team> iter=teams.values().iterator();iter.hasNext();){
				Team team=iter.next();
				if(!team.ParticipatedAllContest())
					continue;
				if(ranks[contest]<team.ranks[contest]){
					if(upper>team.valueI)
						upper=team.valueI;
				}
				else if(ranks[contest]>team.ranks[contest]){
					if(lower<team.valueI)
						lower=team.valueI;
				}
				else
					return false;
			}
			if(lower>=upper)
				return false;
			valueI=lower;
			valueII=ranks[contest];
			return true;
		}
		private int ContestParticipated(){
			for(int contest=0;contest<CONTEST_NUM;contest++)
				if(ranks[contest]!=UNINITIALIZED)
					return contest;
			return CONTEST_NUM;
		}
		public boolean Ranked(){
			return valueI!=UNINITIALIZED && valueII!=UNINITIALIZED;
		}
		public int RankCompare(Team another){
			if(valueI<another.valueI)
				return -1;
			if(valueI==another.valueI){
				if(valueII<another.valueII)
					return -1;
				if(valueII==another.valueII)
					return 0;
				if(valueII>another.valueII)
					return 1;
			}
			return 1;
		}
		public int compareTo(Team another){
			if(RankCompare(another)!=0)
				return RankCompare(another);
			if(ID<another.ID)
				return -1;
			if(ID==another.ID)
				return 0;
			return 1;
		}
		public boolean equals(Team another){
			return compareTo(another)==0;
		}
	}
	public Main(){
		Input();
		Analyze();
	}
	private void Input(){
		for(int contest=0;contest<CONTEST_NUM;contest++){
			int rank=1;
			Scanner test=new Scanner(ReadLine());
			int lineNum=test.nextInt();
			while(lineNum-->0){
				int teamNum=0;
				Scanner scanner=new Scanner(ReadLine());
				while(scanner.hasNextInt()){
					int teamID=scanner.nextInt();
					teamNum++;
					if(!teams.containsKey(teamID))
						teams.put(teamID,new Team(teamID));
					Team team=teams.get(teamID);
					team.SetRank(contest,rank);
				}
				rank+=teamNum;
			}
		}
	}
	private String ReadLine(){
		StringBuffer buffer=new StringBuffer();
		while(true){
			try{
				while(true){
					int c=System.in.read();
					if(c==-1 || c=='\n')
						break;
					buffer.append((char)c);
				}
			}
			catch(IOException ex){
			}
			if((new Scanner(buffer.toString())).hasNextInt())
				break;
		}
		return buffer.toString();
	}
	private void Analyze(){
		for(Iterator<Team> iter=teams.values().iterator();iter.hasNext();)
			iter.next().ApplyRuleI();
		for(Iterator<Team> iter=teams.values().iterator();iter.hasNext();)
			iter.next().ApplyRuleII();
		for(Iterator<Team> iter=teams.values().iterator();iter.hasNext();){
			Team team=iter.next();
			if(team.Ranked())
				rankedTeams.add(team);
		}
	}
	public void Output(){
		Team prev=null;
		for(Iterator<Team> iter=rankedTeams.iterator();iter.hasNext();){
			Team current=iter.next();
			if(prev!=null){
				if(current.RankCompare(prev)==0)
					System.out.print(" ");
				else
					System.out.print("\n");
			}
			System.out.print(current.ID);
			prev=current;
		}
	}
	public static void main(String[] args){
		Main testCase=new Main();
		testCase.Output();
	}
}