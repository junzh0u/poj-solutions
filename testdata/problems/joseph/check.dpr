// long numbers compare
uses
	testlib, sysutils;

var
	ja, pa: string;

function trimLZ(s: string): string;
var
  i: integer;
begin
	i := 1;
	while (i < length(s)) and (s[i] = '0') do
  		inc(i);
	result := copy(s, i, length(s) - i + 1);
end;

function isNumber(s: string): boolean;
var
	i: integer;
begin
	if length(s) = 0 then
		result := false
	else begin
		result := s[1] in ['-', '0'..'9'];
		for i := 2 to length(s) do
			if not (s[i] in ['0'..'9']) then
				result := false;
	end;
end;

function compact(s: string): string;
begin
	if length(s) < 40 then
		result := s
	else
		result := copy(s, 1, 10) + '...' + copy(s, length(s) - 9, 10) + ' [' + inttostr(length(s)) + ' digits]';
end;

begin
	ja := ans.readstring;
	pa := trimLZ(trim(ouf.readstring));

	if not isNumber(pa) then
		quit(_pe, format('number is expected, "%s" found', [pa]));

	if ja <> pa then
		quit(_wa, format('expected: %s, found %s', [compact(ja), compact(pa)]));

	quit(_ok, compact(ja));
end.