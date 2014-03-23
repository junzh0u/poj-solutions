import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private static final int FINAL=100;
	private static LinkedList<Integer> DIE_THROWS=new LinkedList<Integer>();
	private Queue<Integer> dieThrows;
	private Bonus[] bonuses=new Bonus[FINAL];
	private int playerNum;
	private class Player{
		private int position=0;
		private boolean loseTurn=false,winner=false;;
		public void Throw(){
			if(loseTurn){
				loseTurn=false;
				return;
			}
			int dieThrow=dieThrows.poll();
			if(position+dieThrow<=FINAL)
				position+=dieThrow;
			if(position==FINAL)
				winner=true;
			bonuses[position-1].Act(this);
		}
	}
	private abstract class Bonus{
		public abstract void Act(Player player);
	}
	private class NonBonus extends Bonus{
		public void Act(Player player){
		}
	}
	private final Bonus NON_BONUS=new NonBonus();
	private class ExtraTurn extends Bonus{
		public void Act(Player player){
			player.Throw();
		}
	}
	private class LoseTurn extends Bonus{
		public void Act(Player player){
			player.loseTurn=true;
		}
	}
	private class Jump extends Bonus{
		private int to;
		public Jump(int to){
			this.to=to;
		}
		public void Act(Player player){
			player.position=to;
		}
	}
	private Main(int playerNum){
		dieThrows=new LinkedList<Integer>(DIE_THROWS);
		this.playerNum=playerNum;
		for(int i=0;i<bonuses.length;i++)
			bonuses[i]=NON_BONUS;
		while(true){
			int from=in.nextInt();
			int to=in.nextInt();
			if(from==0 && to==0)
				break;
			bonuses[from-1]=new Jump(to);
		}
		while(true){
			int bonus=in.nextInt();
			if(bonus==0)
				break;
			if(bonus<0)
				bonuses[-bonus-1]=new LoseTurn();
			else
				bonuses[bonus-1]=new ExtraTurn();
		}
	}
	private void Run(){
		Player[] players=new Player[playerNum];
		for(int ID=0;ID<playerNum;ID++)
			players[ID]=new Player();
		int ID=0;
		while(true){
			players[ID].Throw();
			if(players[ID].winner){
				System.out.println((ID+1));
				break;
			}
			ID=(ID+1)%playerNum;
		}
	}
	public static void main(String[] args){
		while(true){
			int dieThrow=in.nextInt();
			if(dieThrow==0)
				break;
			DIE_THROWS.add(dieThrow);
		}
		while(true){
			int playerNum=in.nextInt();
			if(playerNum==0)
				break;
			Main game=new Main(playerNum);
			game.Run();
		}
	}
}