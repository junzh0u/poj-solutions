import java.util.*;

public class Main{
	private static final boolean DEBUG=false;
	
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		int caseNum=in.nextInt();
		while(caseNum-->0)
			new Main().Output();
	}
	//private Set<Integer> allSquare=new HashSet<Integer>();
	//private List<Set<Integer>> squareContaining=new ArrayList<Set<Integer>>();
	private List<BitSet> squareContaining=new ArrayList<BitSet>();
	private int squareNum=0;
	public Main(){
		Input();
	}
	public void Input(){
		int squareSize=in.nextInt();
		boolean[] missing=new boolean[2*squareSize*(squareSize+1)];
		int missingNum=in.nextInt();
		while(missingNum-->0)
			missing[in.nextInt()-1]=true;
		Set<Integer> allSquare=new HashSet<Integer>();
		Map<Integer,Set<Integer>> stickInSquare=new HashMap<Integer,Set<Integer>>();
		for(int rowBegin=0;rowBegin<squareSize;rowBegin++)
			for(int colBegin=0;colBegin<squareSize;colBegin++)
				for(int size=1;size<=squareSize-rowBegin && size<=squareSize-colBegin;size++){
					Set<Integer> sticks=Sticks(rowBegin,colBegin,size,squareSize);
					if(NoneMissing(sticks,missing)){
						for(int stick:sticks){
							if(!stickInSquare.containsKey(stick))
								stickInSquare.put(stick,new HashSet<Integer>());
							stickInSquare.get(stick).add(squareNum);
						}
						allSquare.add(squareNum);
						squareNum++;
					}
				}
		
		if(DEBUG)
			System.out.println(stickInSquare.values());
		
		for(Set<Integer> squares:stickInSquare.values()){
			BitSet bits=new BitSet(squareNum);
			for(int square:squares)
				bits.set(square);
			squareContaining.add(bits);
		}
		while(true){
			boolean anyChange=false;
			BitSet toRemove=null;
			for(BitSet squares:squareContaining)
				for(BitSet another:squareContaining)
					if(another!= squares && Contain(another,squares)){
						toRemove=squares;
						anyChange=true;
						break;
					}
			if(!anyChange)
				break;
			else
				squareContaining.remove(toRemove);
		}
		
		if(DEBUG){
			System.out.println(allSquare);
			System.out.println(squareContaining);
		}
	}
	private Set<Integer> Sticks(int rowBegin,int colBegin,int squareSize,int totalSize){
		Set<Integer> sticks=new HashSet<Integer>();
		for(int step=0;step<squareSize;step++){
			sticks.add(colBegin+rowBegin*(2*totalSize+1)+step);
			sticks.add(colBegin+(rowBegin+squareSize)*(2*totalSize+1)+step);
			sticks.add(colBegin+rowBegin*(2*totalSize+1)+totalSize+step*(2*totalSize+1));
			sticks.add(colBegin+rowBegin*(2*totalSize+1)+totalSize+squareSize+step*(2*totalSize+1));
		}
		return sticks;
	}
	private boolean NoneMissing(Collection<Integer> sticks,boolean[] missing){
		for(int stick:sticks)
			if(missing[stick])
				return false;
		return true;
	}
	private boolean Contain(BitSet left,BitSet right){
		BitSet leftFlip=(BitSet)left.clone();
		leftFlip.flip(0,leftFlip.size());
		
		//if(DEBUG)
		//	System.out.println(left+"\t"+right+"\t"+!leftFlip.intersects(right)+"\n"+leftFlip);
		
		return !leftFlip.intersects(right);
	}
	public void Output(){
		Initialize();
		Integer minDominatingSetSize=Integer.MAX_VALUE;
		System.out.println(MinDominatingSetSize(new BitSet(squareNum),0,0,minDominatingSetSize));
	}
	List<BitSet> allBehind=new ArrayList<BitSet>();
	private void Initialize(){
		for(int index=0;index<squareContaining.size();index++)
			allBehind.add((BitSet)squareContaining.get(index).clone());
		for(int index=allBehind.size()-2;index>=0;index--)
			allBehind.get(index).or(allBehind.get(index+1));
		
		if(DEBUG)
			System.out.println(allBehind);
	}
	private int MinDominatingSetSize(BitSet current,int point,int size,Integer best){
		if(DEBUG)
			System.out.println("!!!> "+current+"\t"+point+"\t"+size+"\t"+best);
		
		if(size>=best)
			return Integer.MAX_VALUE;
		if(AllContain(current))
			return size;
		if(point>=squareContaining.size())
			return Integer.MAX_VALUE;
		BitSet temp=(BitSet)current.clone();
		temp.or(allBehind.get(point));
		if(!AllContain(temp))
			return Integer.MAX_VALUE;
		int answer;
		if(DEBUG)
			System.out.println("1:"+current);
		if(!Contain(current,squareContaining.get(point))){
			BitSet next=(BitSet)current.clone();
			next.or(squareContaining.get(point));
			answer=MinDominatingSetSize(next,point+1,size+1,best);
			if(answer<best)
				best=answer;
		}
		if(DEBUG)
			System.out.println("2:"+current);
		answer=MinDominatingSetSize(current,point+1,size,best);
		if(answer<best)
			best=answer;
		return best;
	}
	private boolean AllContain(BitSet squares){
		BitSet all=new BitSet(squareNum);
		all.flip(0,squareNum);
		
		if(DEBUG)
			System.out.println(squares+"\t"+all+"\t"+squares.equals(all));
		return squares.equals(all);
	}
}