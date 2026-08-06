import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String args[]){
		while(true){
			int objectNum=in.nextInt(),relationNum=in.nextInt();
			if(objectNum==0 && relationNum==0)
				break;
			new Main(objectNum,relationNum);
		}
	}
	private class Object{
		private Set<Object> greater=new HashSet<Object>(),less=new HashSet<Object>();
		public void LessThan(Object another){
			greater.add(another);
			greater.addAll(another.greater);
			for(Object object:less)
				object.greater.addAll(greater);
		}
		public void GreaterThan(Object another){
			less.add(another);
			less.addAll(another.less);
			for(Object object:greater)
				object.less.addAll(less);
		}
	}
	private Object[] objects;
	public Main(int objectNum,int relationNum){
		objects=new Object[objectNum];
		for(int i=0;i<objects.length;i++)
			objects[i]=new Object();
		boolean solved=false;
		for(int step=1;step<=relationNum;step++){
			String relation=in.next();
			if(solved)
				continue;
			Object less=objects[relation.charAt(0)-'A'],greater=objects[relation.charAt(2)-'A'];
			if(less.less.contains(greater) || greater.greater.contains(less) || Overlap(less.less,greater.greater)){
				System.out.println("Inconsistency found after "+step+" relations.");
				solved=true;
			}
			less.LessThan(greater);
			greater.GreaterThan(less);
			if(Determined()){
				System.out.println("Sorted sequence determined after "+step+" relations: "+Sequence()+".");
				solved=true;
			}
		}
		if(!solved)
			System.out.println("Sorted sequence cannot be determined.");
	}
	private boolean Overlap(Set<Object> left,Set<Object> right){
		for(Object object:left)
			if(right.contains(object))
				return true;
		return false;
	}
	private boolean Determined(){
		for(Object object:objects)
			if(object.less.size()+object.greater.size()+1!=objects.length)
				return false;
		return true;
	}
	private String Sequence(){
		char[] sequence=new char[objects.length];
		for(int i=0;i<objects.length;i++)
			sequence[objects[i].less.size()]=(char)('A'+i);
		return new String(sequence);
	}
}