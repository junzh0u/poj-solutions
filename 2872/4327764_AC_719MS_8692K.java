import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private Set<String> dictionary=new HashSet<String>();
	public Main(){
		int wordNum=in.nextInt();
		while(wordNum-->0)
			dictionary.add(in.next());
		int mailNum=in.nextInt();
		for(int mailID=1;mailID<=mailNum;mailID++){
			List<String> unknown=new ArrayList<String>();
			while(true){
				String word=in.next();
				if(word.equals("-1"))
					break;
				if(!dictionary.contains(word))
					unknown.add(word);
			}
			if(unknown.isEmpty())
				System.out.println("Email "+mailID+" is spelled correctly.");
			else{
				System.out.println("Email "+mailID+" is not spelled correctly.");
				for(String word:unknown)
					System.out.println(word);
			}
		}
		System.out.println("End of Output");
	}
	public static void main(String[] args){
		new Main();
	}
}