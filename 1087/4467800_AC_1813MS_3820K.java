import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		new Main().Output();
	}
	private static class Node{
		public static final Node SOURCE=new Node(),DESTINATION=new Node();
		protected Map<Node,Integer> nexts=new HashMap<Node,Integer>();
		public void AddNext(Node next){
			if(nexts.containsKey(next))
				nexts.put(next,nexts.get(next)+1);
			else
				nexts.put(next,1);
		}
		public void RemoveNext(Node next){
			nexts.put(next,nexts.get(next)-1);
		}
	}
	private static class Receptacle extends Node{
		private static Map<String,Receptacle> receptacleList=new HashMap<String,Receptacle>();
		public static Receptacle GetByName(String name){
			if(!receptacleList.containsKey(name))
				receptacleList.put(name,new Receptacle());
			return receptacleList.get(name);
		}
		public void AdaptTo(Receptacle another){
			nexts.put(another,Integer.MAX_VALUE);
		}
	}
	private static class Device extends Node{
		private static Map<String,Device> deviceList=new HashMap<String,Device>();
		public static Device GetByName(String name){
			if(!deviceList.containsKey(name))
				deviceList.put(name,new Device());
			return deviceList.get(name);
		}
		public Device(){
			SOURCE.AddNext(this);
		}
		public boolean PluggedIn(){
			for(Node next:nexts.keySet())
				if(nexts.get(next)==0)
					return true;
			return false;
		}
	}
	public Main(){
		Input();
		Analyze();
	}
	private void Input(){
		int receptacleNum=in.nextInt();
		while(receptacleNum-->0)
			Receptacle.GetByName(in.next()).AddNext(Node.DESTINATION);
		int deviceNum=in.nextInt();
		while(deviceNum-->0)
			Device.GetByName(in.next()).AddNext(Receptacle.GetByName(in.next()));
		int adapterTypeNum=in.nextInt();
		while(adapterTypeNum-->0)
			Receptacle.GetByName(in.next()).AdaptTo(Receptacle.GetByName(in.next()));
	}
	private void Analyze(){
		while(true){
			Stack<Node> augmentingPath=FindAugmentingPath();
			if(augmentingPath==null)
				break;
			AddPath(augmentingPath);
		}
	}
	private Stack<Node> FindAugmentingPath(){
		Queue<Node> BFS=new LinkedList<Node>();
		Map<Node,Node> father=new HashMap<Node,Node>();
		BFS.add(Node.SOURCE);
		father.put(Node.SOURCE,null);
		while(!BFS.isEmpty()){
			Node current=BFS.poll();
			if(current==Node.DESTINATION){
				Stack<Node> augmentingPath=new Stack<Node>();
				while(current!=null){
					augmentingPath.push(current);
					current=father.get(current);
				}
				return augmentingPath;
			}
			for(Node next:current.nexts.keySet()){
				if(current.nexts.get(next)!=0 && !father.containsKey(next)){
					BFS.add(next);
					father.put(next,current);
				}
			}
		}
		return null;
	}
	private void AddPath(Stack<Node> path){
		Node current=path.pop();
		while(!path.empty()){
			Node next=path.pop();
			current.RemoveNext(next);
			next.AddNext(current);
			current=next;
		}
	}
	public void Output(){
		int count=0;
		for(Device device:Device.deviceList.values())
			if(!device.PluggedIn())
				count++;
		System.out.println(count);
	}
}
