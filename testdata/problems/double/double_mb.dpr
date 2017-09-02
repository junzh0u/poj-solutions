{$apptype CONSOLE}

uses SysUtils;

const
	IN_FILE = 'double.in';
	OUT_FILE = 'double.out';

	MAX_P = 1953125;
	EPSILON = 1e-100;
	
	pow : array[1..9] of integer = (390625, 78125, 15625, 3125, 625, 125, 25, 5, 1);
	
type
	TPos = array[1..9] of integer;
	real = extended;
	
var
	answer : real;
	desk : array[1..9,1..4] of char;
	prob : array[0..MAX_P] of real;
	
function Decode(num : integer) : TPos;
var
	i : integer;
	
begin
	for i := 9 downto 1 do begin
		result[i] := num mod 5;
		num := num div 5;
	end;
end;

function strtok(var s : string) : string;
var
	i : integer;
	
begin
	s := trim(s);
	i := pos(' ', s);
	if i = 0 then begin
		result := s;
		s := '';
	end else begin
		result := copy(s, 1, i - 1);
		delete(s, 1, i);
	end;		
end;
	
procedure ReadInput;
var	
	i, j : integer;
	s : string;
		
begin
	reset(input, IN_FILE);
	for i := 1 to 9 do begin
		readln(s);
		for j := 1 to 4 do
			desk[i,j] := strtok(s)[1];
	end;
	close(input);	
end;

procedure WriteOutput;
begin
	rewrite(output, OUT_FILE);
	writeln(answer:0:6);
	close(output);
end;

procedure Propagate(num : integer);
var
	pos : TPos;
	count, i, j : integer;
	num2 : integer;
	delta : real;
	
	function Check(i, j : integer) : boolean;
	begin
		result := (pos[i] > 0) and (pos[j] > 0) and (desk[i, pos[i]] = desk[j, pos[j]]);
	end;
	
begin
	if prob[num] < EPSILON then
		exit;
		
	pos := Decode(num);
    
	count := 0;
	for i := 1 to 9 do
		for j := i + 1 to 9 do
			if Check(i, j) then
				inc(count);
				
	if count = 0 then
		exit;
	delta := 1.0 / count;
	
	for i := 1 to 9 do
		for j := i + 1 to 9 do
			if Check(i, j) then begin
				num2 := num - pow[i] - pow[j];
				prob[num2] := prob[num2] + delta * prob[num];
			end;
end;

procedure Solve;
var
	i : integer;
		
begin
	for i := 0 to MAX_P - 1 do
		prob[i] := 0;
	prob[MAX_P - 1] := 1.0;
	for i := MAX_P - 1 downto 0 do
		Propagate(i);
	answer := prob[0];
end;

begin
	ReadInput;
	Solve;
	WriteOutput;
end.
