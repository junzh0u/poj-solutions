import java.util.*;

public class Main{
	private static final boolean DEBUG=true;
	private static final Scanner in=new Scanner(System.in);
	private LinkedHashSet<String> invalid=new LinkedHashSet<String>();
	private SortedMap<String,Integer> fiveDigits=new TreeMap<String,Integer>();
	private SortedMap<String,List<String>> threeDigits=new TreeMap<String,List<String>>();
	private SortedMap<String,Integer> firstClass=new TreeMap<String,Integer>();
	private int mailTotal=0,bundleTotal=0;
	public Main(){
		while(in.hasNext()){
			String code=in.next();
			if(DEBUG)
				if(code.equals("......."))
					break;
			if(!Valid(code)){
				invalid.add(code);
				continue;
			}
			if(fiveDigits.containsKey(code))
				fiveDigits.put(code,fiveDigits.get(code)+1);
			else
				fiveDigits.put(code,1);
		}
	}
	private boolean Valid(String code){
		if(code.length()!=5)
			return false;
		if(code.equals("00000"))
			return false;
		for(int i=0;i<code.length();i++)
			if(!Character.isDigit(code.charAt(i)))
				return false;
		return true;
	}
	public void Output(){
		System.out.println("ZIP         LETTERS     BUNDLES\n");
		BundleFiveDigitsPackets();
		System.out.println();
		BundleThreeDigitsPackets();
		System.out.println();
		FirstClass();
		System.out.printf("\nTOTALS%11d%12d\n\nINVALID ZIP CODES\n\n",mailTotal,bundleTotal);
		for(String code:invalid)
			System.out.println(code);
	}
	private void BundleFiveDigitsPackets(){
		for(String code:fiveDigits.keySet()){
			int count=fiveDigits.get(code);
			if(count>=10){
				if(count>=16 && count<=19){
					PrintLine(code,15,1);
					fiveDigits.put(code,count-15);
				}
				else{
					PrintLine(code,count,(count-1)/15+1);
					fiveDigits.put(code,0);
				}
			}
		}
	}
	private void BundleThreeDigitsPackets(){
		for(String code:fiveDigits.keySet())
			if(fiveDigits.get(code)>0){
				String prefix=code.substring(0,3);
				if(!threeDigits.containsKey(prefix))
					threeDigits.put(prefix,new ArrayList<String>());
				for(int i=0;i<fiveDigits.get(code);i++)
					threeDigits.get(prefix).add(code);
			}
		for(String prefix:threeDigits.keySet()){
			int count=threeDigits.get(prefix).size();
			if(count>=10){
				if(count>=16 && count<=19){
					PrintLine(prefix+"xx",15,1);
					threeDigits.get(prefix).removeAll(threeDigits.get(prefix).subList(0,15));
				}
				else{
					PrintLine(prefix+"xx",count,(count-1)/15+1);
					threeDigits.get(prefix).clear();
				}
			}
		}
	}
	private void FirstClass(){
		for(String prefix:threeDigits.keySet()){
			for(String code:threeDigits.get(prefix)){
				if(!firstClass.containsKey(code))
					firstClass.put(code,0);
				firstClass.put(code,firstClass.get(code)+1);
			}
		}
		for(String code:firstClass.keySet())
			PrintLine(code,firstClass.get(code),0);
	}
	private void PrintLine(String code,int mail,int bundle){
		System.out.printf("%s%12d%12d\n",code,mail,bundle);
		mailTotal+=mail;
		bundleTotal+=bundle;
	}
	public static void main(String[] args){
		Main post=new Main();
		post.Output();
	}
}