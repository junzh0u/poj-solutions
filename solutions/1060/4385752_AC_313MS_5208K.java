import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private class Polynomial{
		int[] coefficients;
		public Polynomial(){
			coefficients=new int[1];
		}
		public Polynomial(Scanner in){
			int length=in.nextInt();
			coefficients=new int[length];
			for(int i=length-1;i>=0;i--)
				coefficients[i]=in.nextInt();
		}
		public Polynomial(int[] coefficients){
			this.coefficients=coefficients.clone();
		}
		public int Degree(){
			for(int degree=coefficients.length-1;degree>=0;degree--)
				if(coefficients[degree]!=0)
					return degree;
			return 0;
		}
		public String toString(){
			StringBuffer buffer=new StringBuffer();
			buffer.append(Degree()+1);
			for(int i=Degree();i>=0;i--)
				buffer.append(" "+coefficients[i]);
			return buffer.toString();
		}
		public Polynomial Multiply(Polynomial multiplier){
			Polynomial product=new Polynomial();
			for(int i=0;i<=Degree();i++)
				if(coefficients[i]==1)
					product=product.Add(multiplier.MultiplyPowerX(i));
			return product;
		}
		public Polynomial MultiplyPowerX(int exponent){
			int[] newCoefficients=new int[Degree()+1+exponent];
			System.arraycopy(coefficients,0,newCoefficients,exponent,Degree()+1);
			return new Polynomial(newCoefficients);
		}
		public Polynomial Add(Polynomial addend){
			int[] newCoefficients=new int[Math.max(Degree(),addend.Degree())+1];
			System.arraycopy(coefficients,0,newCoefficients,0,Degree()+1);
			for(int i=0;i<=addend.Degree();i++)
				newCoefficients[i]=newCoefficients[i]^addend.coefficients[i];
			return new Polynomial(newCoefficients);
		}
		public Polynomial Modulus(Polynomial dividend){
			Polynomial remainder=new Polynomial(coefficients);
			while(remainder.Degree()>=dividend.Degree())
				remainder=remainder.Minus(dividend.MultiplyPowerX(remainder.Degree()-dividend.Degree()));
			return remainder;
		}
		public Polynomial Minus(Polynomial minuend){
			return Add(minuend);
		}
	}
	Polynomial f,g,h;
	public Main(){
		f=new Polynomial(in);
		g=new Polynomial(in);
		h=new Polynomial(in);
	}
	public void Output(){
		System.out.println(f.Multiply(g).Modulus(h));
	}
	public static void main(String[] args){
		int caseNum=in.nextInt();
		while(caseNum-->0){
			Main testCase=new Main();
			testCase.Output();
		}
	}
}