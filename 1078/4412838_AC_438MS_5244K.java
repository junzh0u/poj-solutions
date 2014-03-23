import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private int higher,lower;
	public Main(){
		higher=in.nextInt();
		lower=in.nextInt();
		if(higher<lower){
			int temp=higher;
			higher=lower;
			lower=temp;
		}
	}
	public void Output(){
		Set<Set<Integer>> lowerPossibleTruths=AllPossibleTruth(lower,1,new HashSet<Integer>());
		if(lowerPossibleTruths.isEmpty()){
			System.out.println(higher);
			return;
		}
		for(Set<Integer> banList:lowerPossibleTruths){
			if(!AllPossibleTruth(higher,1,banList).isEmpty()){
				System.out.println(higher);
				return;
			}
		}
		System.out.println(lower);
	}
	private Set<Set<Integer>> AllPossibleTruth(int score,int lowerBound,Set<Integer> banList){
		Set<Set<Integer>> truths=new HashSet<Set<Integer>>();
		if(score==1){
			truths.add(new TreeSet<Integer>());
			return truths;
		}
		for(int factor=lowerBound;factor<=100 && factor<=score;factor++){
			if(score%factor==0 && !banList.contains(factor)){
				Set<Set<Integer>> truthsRemain=AllPossibleTruth(score/factor,factor+1,banList);
				if(truths!=null){
					for(Set<Integer> truth:truthsRemain)
						truth.add(factor);
					truths.addAll(truthsRemain);
				}
			}
		}
		return truths;
	}
	public static void main(String[] args){
		while(in.hasNextInt()){
			Main game=new Main();
			game.Output();
		}
	}
}