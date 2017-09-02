{$R-,Q-,O+}
{$apptype CONSOLE}
{$assertions on}

uses Math;

const
	IN_FILE = 'guards.in';
	OUT_FILE = 'guards.out';

	MAX_V = 1000;
	INFINITY = maxlongint;
	
var 
	alpha, beta, gamma : integer;
	xopt, yopt, zopt, topt : integer;
			
procedure ReadInput;
begin
	reset(input, IN_FILE);
	read(alpha);
	assert((alpha >= 0) and (alpha <= MAX_V));
	assert(not eoln);
	read(beta);
	assert((beta >= 0) and (beta <= MAX_V));
	read(gamma);
	assert((gamma >= 0) and (gamma <= MAX_V));
	assert(eoln);
	readln;
	assert(eof);
	close(input);
end;

procedure WriteOutput;
begin
	rewrite(output, OUT_FILE);
	writeln(3 * xopt, ' ', yopt, ' ', 4 * zopt, ' ', 5 * topt);
	close(output);
end;

procedure Solve;
var
	x, y, z, t : integer;
	bestcost, cost : integer;
	tmin : integer;

begin
	bestcost := INFINITY;
	for x := 0 to MAX_V do begin
		if 3 * x > bestcost then
			break;
		for y := 0 to MAX_V do begin
			if 3 * x + y > bestcost then
				break;
			for z := 0 to MAX_V do begin
				if 3 * x + y + 4 * z > bestcost then
					break;
				if x + y + 2 * z < beta then
					continue;
					
				tmin := max(0, gamma - x - z);
				tmin := max(tmin, (alpha - x - y - 2 * z) div 3);
				
				for t := tmin to MAX_V do begin
					cost := 3 * x + y + 4 * z + 5 * t;
					if cost > bestcost then
						break;
					if x + y + 2 * z + 3 * t < alpha then
						continue;
					if x + z + t < gamma then
						continue;
						
					bestcost := cost;
					xopt := x;
					yopt := y;
					zopt := z;
					topt := t;
				end;
			end;
		end;
	end;
end;

begin
	ReadInput;
	Solve;
	WriteOutput;
end.
