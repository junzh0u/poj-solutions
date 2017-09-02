program check;

uses
	testlib, sysutils;

var
	ja, pa: string;

function compress(s: string): string;
begin
	if length(s) < 100 then
		compress := s
	else
		compress := copy(s, 1, 50) + '..' + copy(s, length(s) - 50 + 1, 50);
end;

begin
	ja := ans.readstring;
	pa := ouf.readstring;

	if ja <> pa then 
		quit(_wa, format('expected: %s (%d), found: %s (%d)', [compress(ja), length(ja), compress(pa), length(pa)]));
	
	quit(_ok, '');
end.

