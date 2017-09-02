{$apptype CONSOLE}

uses SysUtils;

const
	IN_FILE = 'ip.in';
	OUT_FILE = 'ip.out';
	
	MAX_N = 1000;

var
	n : integer;
	a : array[1..MAX_N] of longword;
	netstart, netmask : longword;
			
function ParseIP(s : string) : longword;
	
	function NextToken : string;
	var
		i : integer;
	begin
		i := pos('.', s);
		if i = 0 then begin
			result := s;
			s := '';
		end else begin
			result := copy(s, 1, i - 1);
			delete(s, 1, i);
		end;
	end;
	
begin
	result :=
		longword(strtoint(NextToken)) shl 24 +	
		longword(strtoint(NextToken)) shl 16 +	
		longword(strtoint(NextToken)) shl  8 +	
		longword(strtoint(NextToken)) shl  0;	
end;

function FormatIP(a : longword) : string;
begin
	result :=
		inttostr((a shr 24) and $ff) + '.' +
		inttostr((a shr 16) and $ff) + '.' +
		inttostr((a shr  8) and $ff) + '.' +
		inttostr((a shr  0) and $ff);
	
end;

procedure ReadInput;
var
	i : integer;
	s : string;
	
begin
	reset(input, IN_FILE);
	readln(n);
	for i := 1 to n do begin
		readln(s);
		a[i] := ParseIP(s);
	end;
	close(input);	
end;

procedure WriteOutput;
begin
	rewrite(output, OUT_FILE);
	writeln(FormatIP(netstart));
	writeln(FormatIP(netmask));
	close(output);
end;

function _shr(a : longword; c : integer) : longword;
begin
	if c >= 32 then
		result := 0
	else
		result := a shr c;	
end;

function _shl(a : longword; c : integer) : longword;
begin
	if c >= 32 then
		result := 0
	else
		result := a shl c;	
end;

procedure Solve;
var
	i : integer;

	function CheckBit(i : integer) : boolean;
	var
		j : integer;
		
	begin
		result := false;
		for j := 2 to n do
			if (a[j] shr i) and 1 <> (a[1] shr i) and 1 then
				exit;
		result := true;
	end;
		
begin
	i := 31;
	while (i >= 0) and CheckBit(i) do
		dec(i);
	inc(i);
	netstart := _shl(_shr(a[1], i), i);
	netmask := _shl(_shr(longword(-1), i), i);
end;

begin
	ReadInput;
	Solve;
	WriteOutput;
end.
