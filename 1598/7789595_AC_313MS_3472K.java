import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

public class Main{
	private static final Scanner	in		= new Scanner(System.in);
	private static final boolean	DEBUG	= false;
	
	public static void main(String[] args){
		int caseID = 0;
		while(in.hasNext()){
			caseID++ ;
			System.out.println("Excuse Set #" + caseID);
			List<String> worstExcuses = new Main(in.nextInt(), in.nextInt()).worstExcuses();
			for(String excuse: worstExcuses)
				System.out.println(excuse);
			System.out.println();
		}
	}
	
	private Set<String>		keywords;
	private List<String>	excuses;
	
	public Main(int keywordNum, int excuseNum){
		keywords = new HashSet<String>(keywordNum);
		while(keywordNum-- > 0)
			keywords.add(in.next().toLowerCase());
		excuses = new ArrayList<String>(excuseNum);
		in.nextLine();
		while(excuseNum-- > 0)
			excuses.add(in.nextLine());
	}
	
	private int countKeywords(String excuse){
		int num = 0;
		String[] words = excuse.split("[^a-zA-Z]+");
		if(DEBUG){
			System.out.println("# Words in \"" + excuse + "\"");
			for(String word: words)
				System.out.println("# " + word);
			System.out.println();
		}
		for(String word: words)
			if(keywords.contains(word.toLowerCase()))
				num++ ;
		return num;
	}
	
	public List<String> worstExcuses(){
		List<String> worst = new ArrayList<String>();
		int keywordNumMax = - 1;
		for(String excuse: excuses){
			int keywordNum = countKeywords(excuse);
			if(keywordNumMax == keywordNum)
				worst.add(excuse);
			else if(keywordNumMax < keywordNum){
				worst.clear();
				worst.add(excuse);
				keywordNumMax = keywordNum;
			}
		}
		return worst;
	}
}
