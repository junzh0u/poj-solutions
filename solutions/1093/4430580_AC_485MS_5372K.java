import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final int UNINITIALIZED=-1,SINGLE_WORD_LINE_BADNESS=500;
	private int width;
	private List<String> words=new ArrayList<String>();
	private int[] badness,lineEnds;
	public Main(int width){
		this.width=width;
		while(true){
			String line=in.nextLine();
			if(line.length()==0)
				break;
			Scanner lineScanner=new Scanner(line);
			while(lineScanner.hasNext())
				words.add(lineScanner.next());
		}
		badness=new int[words.size()];
		Arrays.fill(badness,UNINITIALIZED);
		lineEnds=new int[words.size()];
		Arrays.fill(lineEnds,UNINITIALIZED);
		Divide(0);
	}
	private int Divide(int begin){
		if(begin==words.size())
			return 0;
		if(badness[begin]!=UNINITIALIZED)
			return badness[begin];
		int lineEnd=begin;
		int badnessMin=Integer.MAX_VALUE;
		int length=-1;
		for(int mark=begin+1;mark<=words.size();mark++){
			length+=words.get(mark-1).length()+1;
			if(length>width)
				break;
			int badness=Badness(begin,mark)+Divide(mark);
			if(badnessMin>=badness){
				badnessMin=badness;
				lineEnd=mark;
			}
		}
		lineEnds[begin]=lineEnd;
		return badness[begin]=badnessMin;
	}
	private int Badness(int begin,int end){
		if(end-begin==1)
			return SINGLE_WORD_LINE_BADNESS;
		int gapWidthTotal=width;
		for(int i=begin;i<end;i++)
			gapWidthTotal-=words.get(i).length();
		int gapNumTotal=end-begin-1;
		int gapWidth1=gapWidthTotal/gapNumTotal-1;
		int gapNum1=gapNumTotal-gapWidthTotal%gapNumTotal;
		int gapWidth2=gapWidthTotal/gapNumTotal;
		int gapNum2=gapWidthTotal%gapNumTotal;
		return gapNum1*gapWidth1*gapWidth1+gapNum2*gapWidth2*gapWidth2;
	}
	public void Output(){
		int begin=0,end=lineEnds[begin];
		while(true){
			System.out.println(Line(begin,end));
			if(end==words.size())
				break;
			begin=end;
			end=lineEnds[begin];
		}
		System.out.println();
	}
	private String Line(int begin,int end){
		if(end-begin==1)
			return words.get(begin);
		int gapWidthTotal=width;
		for(int i=begin;i<end;i++)
			gapWidthTotal-=words.get(i).length();
		int gapNumTotal=end-begin-1;
		int gapWidth1=gapWidthTotal/gapNumTotal;
		int gapNum1=gapNumTotal-gapWidthTotal%gapNumTotal;
		int gapWidth2=gapWidthTotal/gapNumTotal+1;
		String line=words.get(begin);
		for(int i=1;i<=gapNumTotal;i++){
			if(i<=gapNum1)
				for(int j=0;j<gapWidth1;j++)
					line+=" ";
			else
				for(int j=0;j<gapWidth2;j++)
					line+=" ";
			line+=words.get(begin+i);
		}
		return line;
	}
	public static void main(String[] args){
		while(true){
			int width=Integer.parseInt(in.nextLine());
			if(width==0)
				break;
			new Main(width).Output();
		}
	}
}