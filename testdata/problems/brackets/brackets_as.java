import sun.reflect.generics.tree.Tree;

import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class brackets_as {
    String s;
    int p;

    Token token;
    char var;

    public void run() throws FileNotFoundException {
        Scanner in = new Scanner(new File("brackets.in"));
        s = in.nextLine() + "$";
        in.close();

        assert s.length() <= 1000;

        p = 0;
        nextToken();
        Tree t = expression();

        PrintWriter out = new PrintWriter(new File("brackets.out"));
        t.print(out, false);
        out.close();
    }

    public Tree expression() {
        Tree r = term();
        while (token == Token.PLUS || token == Token.MINUS) {
            Token op = token;
            nextToken();
            r = new Tree(r, term(), op);
        }
        return r;
    }

    public Tree term() {
        Tree r = factor();
        while (token == Token.MUL || token == Token.DIV) {
            Token op = token;
            nextToken();
            r = new Tree(r, factor(), op);
        }
        return r;
    }

    public Tree factor() {
        Tree r = null;
        switch (token) {
            case LB:
                nextToken();
                r = expression();
                assert token == Token.RB;
                nextToken();
                break;
            case VAR:
                r = new Tree(var);
                nextToken();
                break;
        }
        return r;
    }

    public void nextToken() {
        switch (s.charAt(p)) {
            case '(':
                token = Token.LB;
                break;
            case ')':
                token = Token.RB;
                break;
            case '+':
                token = Token.PLUS;
                break;
            case '-':
                token = Token.MINUS;
                break;
            case '*':
                token = Token.MUL;
                break;
            case '/':
                token = Token.DIV;
                break;
            case '$':
                token = Token.END;
                break;
            default:
                token = Token.VAR;
                var = s.charAt(p);
                break;
        }
        System.out.println(token);
        p++;
    }

    class Tree {
        Tree l;
        Tree r;
        Token op;
        char var;

        public Tree(Tree l, Tree r, Token op) {
            this.l = l;
            this.r = r;
            this.op = op;
        }

        public Tree(char var) {
            this.op = Token.VAR;
            this.var = var;
        }

        public void print(PrintWriter out, boolean swap) {
            if (op == Token.VAR) {
                out.print(var);
            } else {
                if (l.op.getPrec() < op.getPrec()) {
                    out.print("(");
                    l.print(out, false);
                    out.print(")");
                } else if (l.op.getPrec() > op.getPrec()) {
                    l.print(out, false);
                } else {
                    l.print(out, swap);
                }

                if (swap) {
                    out.print(op.rev());
                } else {
                    out.print(op);
                }

                if (r.op.getPrec() < op.getPrec()) {
                    out.print("(");
                    r.print(out, false);
                    out.print(")");
                } else if (r.op.getPrec() > op.getPrec()) {
                    r.print(out, false);
                } else {
                    if (op.equals(Token.MINUS) || op.equals(Token.DIV)) {
                        r.print(out, !swap);
                    } else {
                        r.print(out, swap);
                    }
                }
            }
        }
    }

    enum Token {
        LB, RB, PLUS, MINUS, MUL, DIV, VAR, END;

        int getPrec() {
            if (this.equals(PLUS) || this.equals(MINUS)) {
                return 1;
            }
            if (this.equals(MUL) || this.equals(DIV)) {
                return 2;
            }
            return 3;
        }

        Token rev() {
            if (this.equals(PLUS)) {
                return MINUS;
            }
            if (this.equals(MINUS)) {
                return PLUS;
            }
            if (this.equals(MUL)) {
                return DIV;
            }
            if (this.equals(DIV)) {
                return MUL;
            }
            return null;
        }

        public String toString() {
            switch (this) {
                case PLUS: return "+";
                case MINUS: return "-";
                case MUL: return "*";
                case DIV: return "/";
                case LB: return "(";
                case RB: return ")";
                case VAR: return "A";
                case END: return "$";
            }
            return "?";
        }
    }

    public static void main(String[] args) throws FileNotFoundException {
        new brackets_as().run();
    }
}
