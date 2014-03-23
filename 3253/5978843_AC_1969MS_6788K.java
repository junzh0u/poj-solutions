import java.math.BigInteger;
import java.util.PriorityQueue;
import java.util.Scanner;

public class Main{
	private static final Scanner in = new Scanner(System.in);

	public static void main(String[] args){
		int plankNum = in.nextInt();
		PriorityQueue<BigInteger> huffman = new PriorityQueue<BigInteger>();
		while(plankNum-- > 0)
			huffman.add(in.nextBigInteger());
		BigInteger cost = BigInteger.ZERO;
		while(huffman.size() >= 2){
			BigInteger length = huffman.poll().add(huffman.poll());
			cost = cost.add(length);
			huffman.add(length);
		}
		System.out.println(cost);
	}
}