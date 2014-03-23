import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private Map<String,String> dictionary=new HashMap<String,String>();
	public Main(){
		while(true){
			String line=in.nextLine();
			if(line.length()==0)
				break;
			Scanner lineReader=new Scanner(line);
			String englishWord=lineReader.next(),foreignWord=lineReader.next();
			dictionary.put(foreignWord,englishWord);
		}
	}
	public void Inquire(){
		while(in.hasNext()){
			String foreignWord=in.next();
			if(dictionary.containsKey(foreignWord))
				System.out.println(dictionary.get(foreignWord));
			else
				System.out.println("eh");
		}
	}
	public static void main(String[] args){
		Main dictionary=new Main();
		dictionary.Inquire();
	}
}