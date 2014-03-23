import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final String END_OF_INPUT="#";
	private int wordCount=0;
	Map<Integer,Set<Word>> dictionary=new HashMap<Integer,Set<Word>>();
	Map<String,Word> allocate=new HashMap<String,Word>();
	public Word GetWord(String word){
		if(!allocate.containsKey(word))
			allocate.put(word,new Word(word));
		return allocate.get(word);
	}
	private class Word implements Comparable<Word>{
		int ID;
		String word;
		Set<String> shrinks=null;
		private Word(String word){
			ID=wordCount++;
			this.word=word;
		}
		Set<String> GetShrinks(){
			if(shrinks==null){
				shrinks=new HashSet<String>();
				for(int cut=0;cut<word.length();cut++)
					shrinks.add(word.substring(0,cut)+word.substring(cut+1));
			}
			return shrinks;
		}
		public int compareTo(Word another){
			return Integer.valueOf(ID).compareTo(Integer.valueOf(another.ID));
		}
		public boolean equals(Word another){
			return compareTo(another)==0;
		}
	}
	public Main(){
		while(true){
			String word=in.next();
			if(word.equals(END_OF_INPUT))
				break;
			if(!dictionary.containsKey(word.length()))
				dictionary.put(word.length(),new HashSet<Word>());
			dictionary.get(word.length()).add(GetWord(word));
		}
	}
	public void Check(String word){
		Word toCheck=GetWord(word);
		if(dictionary.containsKey(word.length()) && dictionary.get(word.length()).contains(toCheck)){
			System.out.println(word+" is correct");
			return;
		}
		SortedSet<Word> replacements=new TreeSet<Word>();
		if(dictionary.containsKey(word.length()-1))
			for(Word remove:dictionary.get(word.length()-1))
				if(toCheck.GetShrinks().contains(remove.word))
					replacements.add(remove);
		if(dictionary.containsKey(word.length())){
			for(Word replace:dictionary.get(word.length())){
				int difference=0;
				for(int i=0;i<word.length();i++)
					if(replace.word.charAt(i)!=word.charAt(i))
						difference++;
				if(difference==1)
					replacements.add(replace);
			}
		}
		if(dictionary.containsKey(word.length()+1))
			for(Word add:dictionary.get(word.length()+1))
				if(add.GetShrinks().contains(toCheck.word))
					replacements.add(add);
		System.out.print(word+":");
		for(Word replacement:replacements){
			System.out.print(" "+replacement.word);
		}
		System.out.println();
	}
	public static void main(String[] args){
		Main checker=new Main();
		while(true){
			String word=in.next();
			if(word.equals(END_OF_INPUT))
				break;
			checker.Check(word);
		}
	}
}