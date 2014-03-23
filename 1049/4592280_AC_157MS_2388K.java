import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		while(in.hasNext()){
			Main state=new Main(in.next());
			if(state.EndOfInput())
				break;
			state.Output();
		}
	}
	private char[] memory;
	private int position=0;
	private int A=0,B=0;
	public Main(String memory){
		this.memory=memory.toCharArray();
	}
	private void Execute(){
		while(true){
			switch(NextWord()){
				case 0:
					Load();
					break;
				case 1:
					Store();
					break;
				case 2:
					Swap();
					break;
				case 3:
					Add();
					break;
				case 4:
					Increment();
					break;
				case 5:
					Decrement();
					break;
				case 6:
					BreakZero();
					break;
				case 7:
					Break();
					break;
				case 8:
					return;
			}
		}
	}
	private void Load(){
		A=Character.digit(memory[NextAddress()],16);
	}
	private void Store(){
		memory[NextAddress()]=Character.toUpperCase(Character.forDigit(A,16));
	}
	private void Swap(){
		int temp=A;
		A=B;
		B=temp;
	}
	private void Add(){
		A+=B;
		B=A/16;
		A%=16;
	}
	private void Increment(){
		A=(A+1)%16;
	}
	private void Decrement(){
		A=(A+15)%16;
	}
	private void BreakZero(){
		int next=NextAddress();
		if(A==0)
			position=next;
	}
	private void Break(){
		position=NextAddress();
	}
	private int NextWord(){
		return Character.digit(memory[position++],16);
	}
	private int NextAddress(){
		return NextWord()*16+NextWord();
	}
	public boolean EndOfInput(){
		return memory[0]=='8';
	}
	public void Output(){
		Execute();
		System.out.println(memory);
	}
}