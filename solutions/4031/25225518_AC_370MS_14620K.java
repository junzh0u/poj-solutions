// POJ 4031 - Journey
// Model: gpt-6.1-sol
// Independent NEERC 2011 jury control by Roman Elizarov.
// Adapted only for POJ class naming and standard input/output.
import java.io.*;
import java.math.BigInteger;

/**
 * Solution for NEERC'2011 Problem J: Journey
 * This solution checks correctness of the input.
 * @author Roman Elizarov
 */
public class Main {

	public static void main(String[] args) throws Exception {
		new Main().go();
	}

	void go() throws Exception {
		read();
		solve();
		write();
	}

	static final BigInteger ZERO = BigInteger.ZERO;
	static final BigInteger ONE = BigInteger.ONE;
	static final BigInteger MINUS_ONE = BigInteger.ONE.negate();

	static class Result {
		int d;
		BigInteger x = ZERO;
		BigInteger y = ZERO;

		boolean complete;
		boolean infinity;
		boolean noReturn;
		int idx; // current index when !complete

		BigInteger m0 = ZERO; // max x + y
		BigInteger m1 = ZERO; // max -x + y
		BigInteger m2 = ZERO; // max -x - y
		BigInteger m3 = ZERO; // max x - y

		Result(int d) {
			this.d = d;
		}

		void update() {
			m0 = max(m0, x.add(y));
			m1 = max(m1, x.negate().add(y));
			m2 = max(m2, x.negate().subtract(y));
			m3 = max(m3, x.subtract(y));
		}

		void append(Result r) {
			m0 = max(m0, x.add(y).add(r.m0));
			m1 = max(m1, x.negate().add(y).add(r.m1));
			m2 = max(m2, x.negate().subtract(y).add(r.m2));
			m3 = max(m3, x.subtract(y).add(r.m3));
			x = x.add(r.x);
			y = y.add(r.y);
			d = r.d;
			if (r.noReturn) {
				noReturn = true;
				complete = true;
			}
			if (r.infinity)
				infinity = true;
		}
	}

	static BigInteger max(BigInteger a, BigInteger b) {
		return a.compareTo(b) >= 0 ? a : b;
	}

	static Result[] GO_RESULT = new Result[4];
	static Result[] LEFT_RESULT = new Result[4];
	static Result[] RIGHT_RESULT = new Result[4];

	static {
		for (int d = 0; d < 4; d++)
			GO_RESULT[d] = new Result(d);
		GO_RESULT[0].x = ONE;
		GO_RESULT[1].y = ONE;
		GO_RESULT[2].x = MINUS_ONE;
		GO_RESULT[3].y = MINUS_ONE;
		for (int d = 0; d < 4; d++)
			GO_RESULT[d].update();
		for (int d = 0; d < 4; d++)
			LEFT_RESULT[d] = new Result((d + 1) & 3);
		for (int d = 0; d < 4; d++)
			RIGHT_RESULT[d] = new Result((d - 1) & 3);
	}

	interface Command {
		Result compute(int d);
	}

	enum ECommand implements Command {
		GO {
			public Result compute(int d) {
				return GO_RESULT[d];
			}
		},

		LEFT {
			public Result compute(int d) {
				return LEFT_RESULT[d];
			}
		},

		RIGHT {
			public Result compute(int d) {
				return RIGHT_RESULT[d];
			}
		}
	}

	class FCommand implements Command {
		final int k;

		FCommand(int k) {
			this.k = k - 1;
		}

		public Result compute(int d) {
			return computeF(k, d);
		}
	}

	int n;
    Command[][] cmds;

	void read() throws Exception {
		Scanner in = new Scanner(System.in);
		n = in.nextInt();
		in.nextLine();
		assert n >= 1 && n <= 100;
		cmds = new Command[n][];
		for (int i = 0; i < n; i++) {
			int c = in.nextInt();
			assert c >= 1 && c <= 100;
			cmds[i] = new Command[c];
			for (int j = 0; j < c; j++) {
				String s = in.next();
				Command cmd;
				if (s.startsWith("F")) {
					int k = Integer.parseInt(s.substring(1));
					assert k >= 1 && k <= n;
					cmd = new FCommand(k);
				}
				else
					cmd = ECommand.valueOf(s);
				cmds[i][j] = cmd;
			}
			in.nextLine();
		}
		in.close();
	}


	Result[][] mem;   // memorized result and partial result during computation

	Result computeF(int k, int d) {
		Result r = mem[k][d];
		if (r != null) {
			if (!r.complete) {
				r = collapsePath(k, d);
				r.infinity = !r.x.equals(ZERO) || !r.y.equals(ZERO);
				r.noReturn = true;
				r.complete = true;
			}
			return r;
		}
		r = new Result(d);
		mem[k][d] = r;
		for (int i = 0; i < cmds[k].length; i++) {
			Command c = cmds[k][i];
			r.idx = i;
			Result rc = c.compute(r.d);
			r.append(rc);
			if (r.noReturn)
				break;
		}
		r.complete = true;
		return r;
	}

	private Result collapsePath(int k, int d) {
		int k0 = k;
		int d0 = d;
		Result result = new Result(d);
		do {
			Result r = mem[k][d];
			result.append(r);
			FCommand cmd = (FCommand) cmds[k][r.idx];
			k = cmd.k;
			d = result.d;
		} while (k != k0 || d != d0);
		return result;
	}

	String answer;

	void solve() {
		mem = new Result[n][4];
		Result r = computeF(0, 0);
		if (r.infinity)
			answer = "Infinity";
		else
			answer = max(max(r.m0, r.m1), max(r.m2, r.m3)).toString();
	}

	void write() throws Exception {
		PrintWriter out = new PrintWriter(System.out);
		out.println(answer);
		out.close();
	}

//----------------- just for validation ------------------

	/**
	 * Strict scanner to verify 100% correspondence between input files and input file format specification.
	 * It is a drop-in replacement for {@link java.util.Scanner} that could be added to a solution source
	 * (cut-and-paste) without breaking its ability to work with {@link java.util.Scanner}.
	 */
	public class Scanner {
		private final BufferedReader in;
		private String line = "";
		private int pos;
		private int lineNo;

		public Scanner(InputStream source) {
			in = new BufferedReader(new InputStreamReader(source));
			nextLine();
		}

		public void close() {
			assert line == null : "Extra data at the end of file";
			try {
				in.close();
			} catch (IOException e) {
				throw new AssertionError("Failed to close with " + e);
			}
		}

		public void nextLine() {
			assert line != null : "EOF";
			assert pos == line.length() : "Extra characters on line " + lineNo;
			try {
				line = in.readLine();
			} catch (IOException e) {
				throw new AssertionError("Failed to read line with " + e);
			}
			pos = 0;
			lineNo++;
		}

		public String next() {
			assert line != null : "EOF";
			assert line.length() > 0 : "Empty line " + lineNo;
			if (pos == 0)
				assert line.charAt(0) > ' ' : "Line " + lineNo + " starts with whitespace";
			else {
				assert pos < line.length() : "Line " + lineNo + " is over";
				assert line.charAt(pos) == ' ' : "Wrong whitespace on line " + lineNo;
				pos++;
				assert pos < line.length() : "Line " + lineNo + " is over";
				assert line.charAt(pos) > ' ' : "Line " + lineNo + " has double whitespace";
			}
			StringBuilder sb = new StringBuilder();
			while (pos < line.length() && line.charAt(pos) > ' ')
				sb.append(line.charAt(pos++));
			return sb.toString();
		}

		public int nextInt() {
			String s = next();
			assert s.length() == 1 || s.charAt(0) != '0' : "Extra leading zero in number " + s + " on line " + lineNo;
			assert s.charAt(0) != '+' : "Extra leading '+' in number " + s + " on line " + lineNo;
			try {
				return Integer.parseInt(s);
			} catch (NumberFormatException e) {
				throw new AssertionError("Malformed number " + s + " on line " + lineNo);
			}
		}
	}
}
