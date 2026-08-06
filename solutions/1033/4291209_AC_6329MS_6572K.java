import java.util.*;

public class Main{
	private static final boolean DEBUG=false;
	private static final Scanner in=new Scanner(System.in);
	private static final int EMPTY=0,TOMOVE=1,MOVED=3;
	private static final int NONE=0,SELF=1,CHAIN=2,RING=3;
	private Disk disk;
	private StringBuffer log=new StringBuffer();
	class Cluster{
		private int ID;
		private Cluster to=null,from=null;
		private int status=EMPTY;
		public Cluster(int ID){
			this.ID=ID;
		}
		private int Type(){
			if(status!=TOMOVE)
				return NONE;
			if(to==this)
				return SELF;
			Cluster next=this;
			while(true){
				next=next.to;
				if(next==null)
					return CHAIN;
				if(next==this)
					return RING;
			}
		}
	}
	class Disk{
		private Cluster[] clusters;
		private Set<Cluster> initializedClusters=new HashSet<Cluster>();
		public Disk(int clusterNum){
			clusters=new Cluster[clusterNum];
			Arrays.fill(clusters,null);
		}
		public Cluster GetCluster(int ID){
			if(clusters[ID-1]==null)
				InitializeCluster(ID);
			return clusters[ID-1];
		}
		public Cluster GetEmptyCluster(){
			for(Cluster cluster:initializedClusters)
				if(cluster.status==EMPTY)
					return cluster;
			return null;
		}
		public void CreateEmptyCluster(){
			for(int ID=clusters.length;ID>0;ID--)
				if(clusters[ID-1]==null){
					InitializeCluster(ID);
					break;
				}
		}
		private Cluster InitializeCluster(int ID){
			clusters[ID-1]=new Cluster(ID);
			initializedClusters.add(clusters[ID-1]);
			return clusters[ID-1];
		}
	}
	public Main(){
		Input();
		if(DEBUG){
			for(Cluster cluster:disk.initializedClusters)
				if(cluster.to!=null)
					System.out.println(cluster.ID+" --> "+cluster.to.ID);
		}
		Analyze();
	}
	private void Input(){
		disk=new Disk(in.nextInt());
		int fileNum=in.nextInt();
		Cluster destination=disk.GetCluster(1);
		while(fileNum-->0){
			int clusterInFile=in.nextInt();
			while(clusterInFile-->0){
				Cluster cluster=disk.GetCluster(in.nextInt());
				cluster.to=destination;
				destination.from=cluster;
				if(cluster==destination)
					cluster.status=MOVED;
				else
					cluster.status=TOMOVE;
				destination=disk.GetCluster(destination.ID+1);
			}
		}
		if(disk.GetEmptyCluster()==null)
			disk.CreateEmptyCluster();
	}
	private void Analyze(){
		for(Cluster cluster:disk.initializedClusters){
			switch(cluster.Type()){
			case NONE:break;
			case SELF:break;
			case CHAIN:
				MoveChain(cluster);
				break;
			case RING:
				MoveRing(cluster);
				break;
			}
		}
	}
	private void MoveChain(Cluster cluster){
		if(cluster.status==EMPTY)
			cluster=cluster.from;
		while(cluster.to.status!=EMPTY)
			cluster=cluster.to;
		while(cluster!=null){
			Move(cluster,cluster.to);
			cluster=cluster.from;
		}
	}
	private void MoveRing(Cluster cluster){
		Cluster temp=disk.GetEmptyCluster();
		temp.to=cluster.to;
		temp.to.from=temp;
		Move(cluster,temp);
		MoveChain(temp);
	}
	private void Move(Cluster from,Cluster to){
		if(from==null || to==null)
			return;
		log.append((from.ID)+" "+(to.ID)+"\n");
		to.status=MOVED;
		to.from=null;
		from.status=EMPTY;
		from.to=null;
	}
	public void Output(){
		if(log.length()==0)
			System.out.println("No optimization needed");
		else
			System.out.print(log);
	}
	public static void main(String[] args){
		Main disk=new Main();
		disk.Output();
	}
}