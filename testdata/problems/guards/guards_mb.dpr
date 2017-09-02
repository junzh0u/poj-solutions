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
	x, z : integer;
	bestcost, cost : integer;
	ymin, tmin : integer;
	y3tmin : integer;
	
	procedure Update(y, t : integer);
	begin
		cost := 3 * x + y + 4 * z + 5 * t;
		if cost > bestcost then
			exit;
		if (y < ymin) or (t < tmin) or (y + 3 * t < y3tmin) then
			exit;
		bestcost := cost;
		xopt := x;
		yopt := y;
		zopt := z;
		topt := t;
	end;

begin
    {
      minimize 3x + y + 4z + 5t
      subject to
      	x >= 0, y >= 0, z >= 0, t >= 0
        x + y + 2z + 3t >= alpha
        x +     2z      >= beta
        x +      z + 2t >= gamma
    }    
	bestcost := INFINITY;
	for x := 0 to MAX_V do begin
		if 3 * x > bestcost then
			break;
		for z := 0 to MAX_V do begin
			if x + 2 * z < beta then
				continue;
			if 3 * x + 4 * z > bestcost then
				break;
            ymin := 0;
			tmin := max(0, (gamma - x - z + 1) div 2);
			y3tmin := alpha - x - 2 * z;
			
			{
				minimize (3x + 4z) + (y + 5t)
				subject to
				  t >= tmin
				  y + 3t >= y3tmin
			}

			Update(ymin, tmin);
			Update(ymin, (y3tmin - ymin + 2) div 3);
			Update(y3tmin - 3 * tmin, tmin);
		end;
	end;
end;

begin
	ReadInput;
	Solve;
	WriteOutput;
end.
