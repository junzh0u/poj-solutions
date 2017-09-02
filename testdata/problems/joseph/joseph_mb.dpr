{$apptype CONSOLE}
{$assertions ON}

uses Math;

const
	IN_FILE = 'joseph.in';
	OUT_FILE = 'joseph.out';

var
	n, k : int64;
	answer : int64;
		
procedure ReadInput;
begin
	reset(input, IN_FILE);
	read(n);
	assert((n >= 1) and (n <= 1000000000));
	assert(not eoln);
	read(k);
	assert((k >= 1) and (k <= 1000000000));
	assert(eoln);
	readln;
	assert(eof);
	close(input);	
end;

procedure WriteOutput;
begin
	rewrite(output, OUT_FILE);
	writeln(answer);
	close(output);
end;

function Sum(lo, hi : int64) : int64;
begin
	result := (lo + hi) * (hi - lo + 1) div 2;
end;

procedure Solve;
var
	d, _d, t : int64;

begin
	answer := n * k;

	d := 1;
	t := k;

	repeat
		_d := 1 + k div t;

//		writeln('t: ', t);
//		writeln('d: ', d, '->', _d);

		answer := answer - t * Sum(d, min(n, _d - 1));

		d := _d;
		t := k div d;
	until (t = 0) or (d > n);
end;

begin
	ReadInput;
	Solve;
	WriteOutput;
end.
