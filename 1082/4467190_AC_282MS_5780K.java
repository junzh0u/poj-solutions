import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static class Date implements Comparable<Date>{
		public static Date BEGIN=new Date(1900,1,1),END=new Date(2001,11,4);
		private static Map<Date,Boolean> hasWinningStrategy=new TreeMap<Date,Boolean>();
		private int year,month,day;
		public Date(int year,int month,int day){
			this.year=year;
			this.month=month;
			this.day=day;
		}
		public boolean HasWinningStrategy(){
			if(compareTo(END)==0)
				return false;
			if(hasWinningStrategy.containsKey(this))
				return hasWinningStrategy.get(this);
			Date sameDayOfNextMonth=SameDayOfNextMonth();
			if(sameDayOfNextMonth.IsLegal() && !sameDayOfNextMonth.HasWinningStrategy())
				hasWinningStrategy.put(this,true);
			else{
				Date nextDay=NextDay();
				if(!nextDay.HasWinningStrategy())
					hasWinningStrategy.put(this,true);
				else
					hasWinningStrategy.put(this,false);
			}
			return hasWinningStrategy.get(this);
		}
		public boolean IsLegal(){
			if(month<1 || month>12)
				return false;
			if(day<1 || day>DayNumOfMonth())
				return false;
			if(compareTo(BEGIN)<0 || compareTo(END)>0)
				return false;
			return true;
		}
		private int DayNumOfMonth(){
			switch(month){
				case 1:case 3:case 5:case 7:case 8:case 10:case 12:
					return 31;
				case 4:case 6:case 9:case 11:
					return 30;
				case 2:
					return IsLeapYear(year)?29:28;
				default:
					return 0;
			}
		}
		private boolean IsLeapYear(int year){
			return year%100==0?year%400==0:year%4==0;
		}
		public Date NextDay(){
			Date nextDay=new Date(year,month,day+1);
			if(nextDay.day>nextDay.DayNumOfMonth()){
				nextDay.month++;
				nextDay.day=1;
			}
			if(nextDay.month>12){
				nextDay.year++;
				nextDay.month=1;
			}
			return nextDay;
		}
		public Date SameDayOfNextMonth(){
			Date sameDayOfNextMonth=new Date(year,month+1,day);
			if(sameDayOfNextMonth.month>12){
				sameDayOfNextMonth.year++;
				sameDayOfNextMonth.month=1;
			}
			return sameDayOfNextMonth;
		}
		public int compareTo(Date another){
			if(year!=another.year)
				return year-another.year;
			if(month!=another.month)
				return month-another.month;
			return day-another.day;
		}
	}
	public static void main(String[] args){
		int caseNum=in.nextInt();
		while(caseNum-->0)
			System.out.println(new Date(in.nextInt(),in.nextInt(),in.nextInt()).HasWinningStrategy()?"YES":"NO");
	}
}