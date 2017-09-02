const MAX = 8;
type poly = array[0..MAX] of integer;
var
  input, output: text;
  p: poly;
  power : integer;

procedure writepoly(var p: poly);
var
  power: integer;
  started: boolean;
begin
  started := false;
  for power := MAX downto 0 do begin
    if (p[power] <> 0) then begin
      if (started) then begin
	if (p[power] < 0) then begin
	  write(output,' - ');
	  p[power] := -p[power];
	end  {if neg}
	else write(output,' + ')
      end {if started}
      else begin
	started := true;
	if (p[power] < 0) then begin
	  write(output,'-');
	  p[power] := -p[power];
	end {if neg}
      end; {else not started}
      if ((p[power] <> 1) or (power = 0)) then write(output,p[power]);
      if (power>0) then write(output,'x');
      if (power>1) then write(output,'^', power);
    end  {if not 0}
  end;  {for power}
  if not started then write(output, 0);
  writeln(output);
end; {writepoly}

begin  { main }
  assign(input,'poly.in');
  reset(input);
  assign(output,'poly.out');
  rewrite(output);
  repeat
    for power:= MAX downto 0 do
      read(input,p[power]);
    readln(input);
    writepoly(p);
  until eof(input);
  close(output);
end.

