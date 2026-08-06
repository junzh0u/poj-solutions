import java.util.Scanner;
import java.util.SortedMap;
import java.util.TreeMap;

public class Main{
	private static final Scanner in = new Scanner(System.in);
	private static final char LETTER_TO_DIGIT[] = "22233344455566670778889990".toCharArray();

	public static void main(final String[] args){
		int numberCount = in.nextInt();
		final SortedMap<String, Integer> phonebook = new TreeMap<String, Integer>();
		while(numberCount-- > 0){
			final String raw = in.next();
			final StringBuffer numberBuffer = new StringBuffer();
			for(final char c:raw.toCharArray()){
				if(Character.isDigit(c))
					numberBuffer.append(c);
				else if(Character.isUpperCase(c))
					numberBuffer.append(LETTER_TO_DIGIT[c - 'A']);
				if(numberBuffer.length() == 3)
					numberBuffer.append('-');
			}
			final String number = numberBuffer.toString();
			if(phonebook.containsKey(numberBuffer.toString()))
				phonebook.put(number, phonebook.get(number) + 1);
			else
				phonebook.put(number, 1);
		}
		boolean duplicate = false;
		for(final String number:phonebook.keySet()){
			final int count = phonebook.get(number);
			if(count > 1){
				duplicate = true;
				System.out.println(number + ' ' + count);
			}
		}
		if(!duplicate)
			System.out.println("No duplicates.");
	}
}