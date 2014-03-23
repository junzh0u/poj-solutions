import java.util.Calendar;
import java.util.Scanner;

public class Main{
	private static final Scanner	in	= new Scanner(System.in);
	
	public static void main(String[] args){
		Calendar calendar = Calendar.getInstance();
		calendar.set(in.nextInt(), in.nextInt() - 1, in.nextInt());
		String[] NAME = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
		System.out.println(NAME[calendar.get(Calendar.DAY_OF_WEEK) - 1]);
	}
}