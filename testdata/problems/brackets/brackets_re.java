/*
    Solution for NEERC'2005 Problem B: Brackets removal
    (C) Roman Elizarov
    Note: this solution attempts to check correctness of the input
*/

import java.util.*;
import java.io.*;

public class brackets_re {
	public static void main(String[] args) throws FileNotFoundException {
		new brackets_re().run();
	}

	abstract static class Expr {
		abstract void write(StringBuilder sb, boolean interm, boolean inv);
	}

	static class Op extends Expr {
		char op;
		Expr l;
		Expr r;

		public Op(char op, Expr l, Expr r) {
			this.op = op;
			this.l = l;
			this.r = r;
		}

		void write(StringBuilder sb, boolean infactor, boolean inv) {
			boolean factorop = op == '*' || op == '/';
			if (infactor && !factorop) {
				sb.append('(');
				write(sb, false, false);
				sb.append(')');
				return;
			}
			if (infactor != factorop)
				inv = false;
			l.write(sb, factorop, inv);
			if (inv)
				sb.append(factorop ? (op == '*' ? '/' : '*') : (op == '+' ? '-' : '+'));
			else
				sb.append(op);
			boolean invop = op == '-' || op == '/';
			r.write(sb, factorop, inv ^ invop);
		}
	}

	static class Var extends Expr {
		char v;

		public Var(char v) {
			this.v = v;
		}

		void write(StringBuilder sb, boolean interm, boolean inv) {
			sb.append(v);
		}
	}

	String ins;
	int i;

	void run() throws FileNotFoundException {
        // read input & verify its correctness
		Scanner in = new Scanner(new File("brackets.in"));
        ins = in.next();
		if (ins.length() < 1 || ins.length() > 1000)
			throw new IllegalArgumentException("invalid expression length");
		if (in.hasNext())
			throw new IllegalArgumentException("extra data in file");
		in.close();

		// solve
		Expr e = parseExpression();
		if (i != ins.length())
			throw new IllegalArgumentException("invalid expression");

		// write answer
		StringBuilder sb = new StringBuilder();
		e.write(sb, false, false);
		PrintWriter out = new PrintWriter(new File("brackets.out"));
		out.println(sb);
		out.close();
	}

	Expr parseExpression() {
		Expr e = parseTerm();
		while (i < ins.length() && (ins.charAt(i) == '+' || ins.charAt(i) == '-')) {
			char op = ins.charAt(i);
			i++;
			e = new Op(op, e, parseTerm());
		}
		return e;
	}

	Expr parseTerm() {
		Expr e = parseFactor();
		while (i < ins.length() && (ins.charAt(i) == '*' || ins.charAt(i) == '/')) {
			char op = ins.charAt(i);
			i++;
			e = new Op(op, e, parseFactor());
		}
		return e;
	}

	Expr parseFactor() {
		if (i >= ins.length())
			throw new IllegalArgumentException("invalid factor");
		if (ins.charAt(i) == '(') {
			i++;
			Expr e = parseExpression();
			if (i >= ins.length() || ins.charAt(i) != ')')
				throw new IllegalArgumentException("missing closing bracket at " + i);
			i++;
			return e;
		} else {
			char v = ins.charAt(i);
			if (v < 'a' || v > 'z')
				throw new IllegalArgumentException("invalid variable");
			i++;
			return new Var(v);
		}
	}
}
