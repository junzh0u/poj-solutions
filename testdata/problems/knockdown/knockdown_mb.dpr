{$R+,Q+,O-}
{$apptype CONSOLE}
{$assertions ON}

uses Math, SysUtils;

const
	IN_FILE = 'knockdown.in';
	OUT_FILE = 'knockdown.out';
	MAX_N = 20;
	INF = 1e20;
    EPS = 1e-15;

type
	real = extended;

var
	n : integer;
	lat, long : array[1..MAX_N] of real;
	x, y, z : array[1..MAX_N] of real;
	answer : real;
	pi : real;

procedure ReadInput;
var
	i : integer;
	ilat, ilong : integer;
	
begin
	reset(input, IN_FILE);
	read(n);
	assert(eoln);
	readln;
	assert((n >= 1) and (n <= MAX_N));
	for i := 1 to n do begin
		read(ilat);
		assert(abs(ilat) < 90);
		assert(not eoln);
		read(ilong);
		assert((ilong > -180) and (ilong <= 180));
		assert(eoln);
		readln;
		lat[i] := ilat / 180 * pi;
		long[i] := ilong / 180 * pi;
	end;
	assert(eof);
	close(input);	
end;

procedure WriteOutput;
begin
	rewrite(output, OUT_FILE);
	writeln(answer:0:6);
	close(output);
end;

function EuclidianDist(x1, y1, z1, x2, y2, z2 : real) : real;
begin
	result := sqrt(sqr(x1 - x2) + sqr(y1 - y2) + sqr(z1 - z2));
end;

function SphDist(x1, y1, z1, x2, y2, z2 : real) : real;
var
	x : real;

begin
	x := EuclidianDist(x1, y1, z1, x2, y2, z2) / 2;
    if x > 1 then
    	x := 1;
	result := 2 * arcsin(x);
end;

procedure GetCrossProduct(x1, y1, z1, x2, y2, z2 : real; var mx, my, mz : real);
begin
	mx := y1 * z2 - z1 * y2;
	my := z1 * x2 - x1 * z2;
	mz := x1 * y2 - y1 * x2;
end;

procedure Normalize(var x, y, z : real);
var
	d : real;

begin
	d := sqrt(sqr(x) + sqr(y) + sqr(z));
	x := x / d;
	y := y / d;
	z := z / d;
end;

procedure GetAnyOrth(x, y, z : real; var ox, oy, oz : real);
begin
	if abs(x) < 1 - EPS then begin
		ox := 0;
		oy := -z;
		oz := y;
		Normalize(ox, oy, oz);
		exit;
	end;	
	if abs(y) < 1 - EPS then begin
		ox := -z;
		oy := 0;
		oz := x;
		Normalize(ox, oy, oz);
		exit;
	end;	
	if abs(z) < 1 - EPS then begin
		ox := -y;
		oy := x;
		oz := 0;
		Normalize(ox, oy, oz);
		exit;
	end;
	assert(false);	
end;

procedure GetMiddle(x1, y1, z1, x2, y2, z2 : real; var mx, my, mz : real);
begin
	mx := x1 + x2;
	my := y1 + y2;
	mz := z1 + z2;

	if (abs(mx) > EPS) or (abs(my) > EPS) or (abs(mz) > EPS) then
		Normalize(mx, my, mz)
	else
		GetAnyOrth(x1, y1, z1, mx, my, mz);
end;

procedure GetCenter(x1, y1, z1, x2, y2, z2, x3, y3, z3 : real; var mx, my, mz : real);
begin
	GetCrossProduct(x2 - x1, y2 - y1, z2 - z1, x3 - x1, y3 - y1, z3 - z1, mx, my, mz);
	Normalize(mx, my, mz);
end;

procedure Solve;
var
	i, j, k : integer;
	mx, my, mz : real;

	procedure Update(xx, yy, zz : real);
	var
		cur, min : real;
		i : integer;
		
	begin
		min := INF;
		for i := 1 to n do begin
			cur := SphDist(x[i], y[i], z[i], xx, yy, zz);
			min := math.min(min, cur);
		end;
		answer := max(answer, min);
	end;

begin
	answer := 0;

	for i := 1 to n do begin
		x[i] := cos(long[i]) * cos(lat[i]);
		y[i] := sin(long[i]) * cos(lat[i]);
		z[i] := sin(lat[i]);
	end;

	for i := 1 to n do
		for j := i to n do begin
			GetMiddle(x[i], y[i], z[i], x[j], y[j], z[j], mx, my, mz);
			Update(mx, my, mz);
			Update(-mx, -my, -mz);
		end;

	for i := 1 to n do
		for j := i + 1 to n do
			for k := j + 1 to n do begin
				GetCenter(x[i], y[i], z[i], x[j], y[j], z[j], x[k], y[k], z[k], mx, my, mz);
				Update(mx, my, mz);
				Update(-mx, -my, -mz);
			end;
end;

begin
	pi := 2 * arcsin(1);
	ReadInput;
	Solve;
	WriteOutput;
end.
                      	