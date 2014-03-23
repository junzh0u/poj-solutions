import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final int EVENING_NUM=5,ARRANGED_EVENING_NUM=3,TABLE_NUM=4,TABLE_SIZE=4;
	private static final int MEMBER_NUM=TABLE_NUM*TABLE_SIZE;
	private static final char MEMBER_BEGIN='A';
	private static final boolean[] NONE_ARRANGED=new boolean[MEMBER_NUM];
	private char[][][] arrangement=new char[EVENING_NUM][TABLE_NUM][TABLE_SIZE];
	private boolean[][] dinedWith=new boolean[MEMBER_NUM][MEMBER_NUM];
	public Main(){
		for(int evening=0;evening<ARRANGED_EVENING_NUM;evening++)
			for(int table=0;table<4;table++){
				arrangement[evening][table]=in.next().toCharArray();
				for(int first=0;first<TABLE_SIZE-1;first++)
					for(int second=first+1;second<TABLE_SIZE;second++){
						int memberFirst=arrangement[evening][table][first]-MEMBER_BEGIN;
						int memberSecond=arrangement[evening][table][second]-MEMBER_BEGIN;
						dinedWith[memberFirst][memberSecond]=dinedWith[memberSecond][memberFirst]=true;
					}
			}
	}
	public void Complete(){
		if(Search(3,0,0,NONE_ARRANGED.clone())){
			for(int evening=0;evening<EVENING_NUM;evening++){
				for(int table=0;table<TABLE_NUM;table++){
					System.out.print(arrangement[evening][table]);
					System.out.print(" ");
				}
				System.out.println();
			}
		}
		else
			System.out.println("It is not possible to complete this schedule.");
	}
	private boolean Search(int evening,int table,int position,boolean[] arranged){
		if(evening>=EVENING_NUM)
			return true;
		if(table>=TABLE_NUM)
			return Search(evening+1,0,0,NONE_ARRANGED.clone());
		if(position>=TABLE_SIZE)
			return Search(evening,table+1,0,arranged);
		if(position==0){
			for(int member=0;member<MEMBER_NUM;member++)
				if(!arranged[member]){
					arrangement[evening][table][0]=(char)(member+MEMBER_BEGIN);
					arranged[member]=true;
					return Search(evening,table,1,arranged);
				}
		}
		for(int member=arrangement[evening][table][position-1]-MEMBER_BEGIN;member<MEMBER_NUM;member++){
			if(!arranged[member] && CanSit(arrangement[evening][table],position,member)){
				arrangement[evening][table][position]=(char)(member+MEMBER_BEGIN);
				arranged[member]=true;
				for(int positionBefore=0;positionBefore<position;positionBefore++){
					int memberBefore=arrangement[evening][table][positionBefore]-MEMBER_BEGIN;
					dinedWith[memberBefore][member]=dinedWith[member][memberBefore]=true;
				}
				if(Search(evening,table,position+1,arranged.clone()))
					return true;
				arranged[member]=false;
				for(int positionBefore=0;positionBefore<position;positionBefore++){
					int memberBefore=arrangement[evening][table][positionBefore]-MEMBER_BEGIN;
					dinedWith[memberBefore][member]=dinedWith[member][memberBefore]=false;
				}
			}
		}
		return false;
	}
	private boolean CanSit(char[] tableArrangment,int position,int member){
		for(int positionBefore=0;positionBefore<position;positionBefore++)
			if(dinedWith[tableArrangment[positionBefore]-MEMBER_BEGIN][member])
				return false;
		return true;
	}
	public static void main(String[] args){
		while(in.hasNext()){
			Main gourmet=new Main();
			gourmet.Complete();
		}
	}
}