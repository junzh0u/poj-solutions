import java.util.Scanner;

public class Main{
	private static Scanner in = new Scanner(System.in);

	public static void main(String[] args){
		new Main(in.nextInt());
	}

	private class Freckle implements Comparable<Freckle>{
		private double x,y,distance;
		private boolean linked;

		public Freckle(double x,double y){
			this.x = x;
			this.y = y;
			distance = Double.MAX_VALUE;
			linked = false;
		}
		public double distanceFrom(Freckle another){
			double dx = x - another.x,dy = y - another.y;
			return Math.sqrt(dx * dx + dy * dy);
		}
		@Override
		public int compareTo(Freckle another){
			return Double.compare(distance,another.distance);
		}
	}

	private Freckle freckles[];

	public Main(int freckleNum){
		freckles = new Freckle[freckleNum];
		for(int i = 0;i < freckleNum;i++)
			freckles[i] = new Freckle(in.nextDouble(),in.nextDouble());
		freckles[0].distance = 0;
		MinHeap<Freckle> heap = new MinHeap<Freckle>(freckles);
		double lengthTotal = 0.0;
		while(!heap.isEmpty()){
			Freckle from = heap.poll();
			lengthTotal += from.distance;
			from.linked = true;
			for(Freckle to:freckles)
				if(!to.linked && to.distance > to.distanceFrom(from)){
					int index = heap.indexOf(to);
					to.distance = to.distanceFrom(from);
					heap.siftUp(index);
				}
		}
		System.out.printf("%.2f\n",lengthTotal);
	}

	public class MinHeap<T extends Comparable<T>>{
		private T array[];
		private int size;

		public MinHeap(T array[]){
			this.array = array.clone();
			size = this.array.length;
			makeHeap(0);
		}
		private void makeHeap(int root){
			for(int i = 0;i < size;i++)
				siftUp(i);
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
}