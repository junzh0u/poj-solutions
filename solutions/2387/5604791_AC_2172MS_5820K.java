import java.util.HashMap;
import java.util.Scanner;
import java.util.Set;

public class Main{
	private static boolean DEBUG = false;
	private static Scanner in = new Scanner(System.in);

	public static void main(String[] args){
		new Main(in.nextInt(),in.nextInt()).output();
	}

	private class Landmark implements Comparable<Landmark>{
		private int distance;
		private HashMap<Landmark,Integer> nexts;
		private boolean linked;

		public Landmark(){
			distance = Integer.MAX_VALUE;
			nexts = new HashMap<Landmark,Integer>();
			linked = false;
		}
		public void addNext(Landmark next,int trailLength){
			if(!nexts.containsKey(next) || nexts.get(next) > trailLength)
				nexts.put(next,trailLength);
		}
		public int getTrailLength(Landmark next){
			return nexts.get(next);
		}
		public Set<Landmark> getNexts(){
			return nexts.keySet();
		}
		public int getDistance(){
			return distance;
		}
		public void setDistance(int distance){
			this.distance = distance;
		}
		public boolean isLinked(){
			return linked;
		}
		public void setLinked(boolean linked){
			this.linked = linked;
		}
		@Override
		public int compareTo(Landmark another){
			return distance - another.distance;
		}
		@Override
		public String toString(){
			if(distance == Integer.MAX_VALUE)
				return "MAX";
			return Integer.toString(distance);
		}
	}
	public class MinHeap<T extends Comparable<T>>{
		private T array[];
		private int size;

		public MinHeap(T array[]){
			this.array = array;
			size = this.array.length;
			makeHeap(0);
		}
		private void makeHeap(int root){
			for(int i = 0;i < size;i++)
				siftUp(i);
		}
		private void siftDown(int index){
			int left = 2 * index + 1,right = 2 * index + 2;
			if(left < size && compare(index,left) > 0){
				swap(index,left);
				siftDown(left);
			}
			if(right < size && compare(index,right) > 0){
				swap(index,right);
				siftDown(right);
			}
		}
		private void swap(int left,int right){
			T transit = array[left];
			array[left] = array[right];
			array[right] = transit;
		}
		public T poll(){
			if(size == 0)
				return null;
			size--;
			swap(0,size);
			siftDown(0);
			return array[size];
		}
		public void siftUp(int index){
			if(index == 0 || index >= size)
				return;
			int father = (index - 1) / 2;
			if(compare(father,index) > 0){
				swap(father,index);
				siftUp(father);
			}
		}
		private int compare(int left,int right){
			return array[left].compareTo(array[right]);
		}
		public int indexOf(T key){
			if(isEmpty())
				return -1;
			return find(key,0);
		}
		private int find(T key,int root){
			if(key == array[root])
				return root;
			if(key.compareTo(array[root]) < 0)
				return -1;
			int left = 2 * root + 1,right = 2 * root + 2;
			if(left < size){
				int index = find(key,left);
				if(index != -1)
					return index;
			}
			if(right < size){
				int index = find(key,right);
				if(index != -1)
					return index;
			}
			return -1;
		}
		public boolean isEmpty(){
			return size == 0;
		}
	}

	Landmark landmarks[];
	Landmark source,destination;

	public Main(int trailNum,int landmarkNum){
		landmarks = new Landmark[landmarkNum];
		for(int i = 0;i < landmarkNum;i++)
			landmarks[i] = new Landmark();
		while(trailNum-- > 0){
			int from = in.nextInt(),to = in.nextInt(),length = in.nextInt();
			landmarks[from - 1].addNext(landmarks[to - 1],length);
			landmarks[to - 1].addNext(landmarks[from - 1],length);
		}
		source = landmarks[landmarkNum - 1];
		destination = landmarks[0];
		dijkstra();
	}
	private void dijkstra(){
		source.setDistance(0);
		MinHeap<Landmark> unlinked = new MinHeap<Landmark>(landmarks);
		while(!destination.isLinked()){
			if(DEBUG)
				System.out.print(unlinked + "\t");
			Landmark tolink = unlinked.poll();
			if(DEBUG)
				System.out.println(tolink);
			tolink.setLinked(true);
			for(Landmark next:tolink.getNexts()){
				int newDistance = tolink.getDistance() + tolink.getTrailLength(next);
				if(!next.isLinked() && next.getDistance() > newDistance){
					int index = unlinked.indexOf(next);
					next.setDistance(newDistance);
					unlinked.siftUp(index);
				}
			}
		}
	}
	public void output(){
		System.out.println(destination.getDistance());
	}
}