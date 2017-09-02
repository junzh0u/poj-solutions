program check;

uses
	testlib, sysutils;

var
	ja, pa: string;

begin
	ja := ans.readstring;
	pa := ouf.readstring;
	if ja <> pa then
		quit(_wa, format('Wrong address, expected: %s, found: %s', [ja, pa]));
	ja := ans.readstring;
	pa := ouf.readstring;
	if ja <> pa then
		quit(_wa, format('Wrong mask, expected: %s, found: %s', [ja, pa]));

    quit(_ok, '');
end.