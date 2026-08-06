import java.util.Scanner;

public class Main{
	private static final Scanner in = new Scanner(System.in);

	public static void main(String[] args){
		while(in.hasNext()){
			String string = in.next();
			if(string.equals("0"))
				break;
			System.out.println(new Main(new StringBuffer(string)).isTautology()?"tautology":"not");
		}
	}

	public Formula formula;

	public Main(StringBuffer string){
		formula = parse(string);
	}
	public Formula parse(StringBuffer string){
		char head = string.charAt(0);
		string.deleteCharAt(0);
		switch(head){
			case 'N':
				return new Not(parse(string));
			case 'K':
				return new And(parse(string),parse(string));
			case 'A':
				return new Or(parse(string),parse(string));
			case 'C':
				return new Imply(parse(string),parse(string));
			case 'E':
				return new Equal(parse(string),parse(string));
			default:
				return new Element(head);
		}
	}
	public boolean isTautology(){
		for(int valueTable = 0;valueTable < 32;valueTable++){
			Element.setValueTable(valueTable);
			if(!formula.value())
				return false;
		}
		return true;
	}

	public static abstract class Formula{
		public abstract boolean value();
	}

	public static class Element extends Formula{
		private static int valueTable;

		public static void setValueTable(int valueTable){
			Element.valueTable = valueTable;
		}

		private char code;

		public Element(char code){
			this.code = code;
		}
		@Override
		public boolean value(){
			return (valueTable >> code - 'p' & 1) != 0;
		}
		@Override
		public String toString(){
			return code + "";
		}
	}

	public class Not extends Formula{
		private Formula formula;

		public Not(Formula formula){
			this.formula = formula;
		}
		@Override
		public boolean value(){
			return !formula.value();
		}
		@Override
		public String toString(){
			return "Not(" + formula + ")";
		}
	}

	public class And extends Formula{
		private Formula left,right;

		public And(Formula left,Formula right){
			this.left = left;
			this.right = right;
		}
		@Override
		public boolean value(){
			return left.value() && right.value();
		}
		@Override
		public String toString(){
			return "And(" + left + "," + right + ")";
		}
	}

	public class Or extends Formula{
		private Formula left,right;

		public Or(Formula left,Formula right){
			this.left = left;
			this.right = right;
		}
		@Override
		public boolean value(){
			return left.value() || right.value();
		}
		@Override
		public String toString(){
			return "Or(" + left + "," + right + ")";
		}
	}

	public class Imply extends Formula{
		private Formula left,right;

		public Imply(Formula left,Formula right){
			this.left = left;
			this.right = right;
		}
		@Override
		public boolean value(){
			return !left.value() || right.value();
		}
		@Override
		public String toString(){
			return "Imply(" + left + "," + right + ")";
		}
	}

	public class Equal extends Formula{
		private Formula left,right;

		public Equal(Formula left,Formula right){
			this.left = left;
			this.right = right;
		}
		@Override
		public boolean value(){
			return left.value() == right.value();
		}
		@Override
		public String toString(){
			return "Euqal(" + left + "," + right + ")";
		}
	}
}
