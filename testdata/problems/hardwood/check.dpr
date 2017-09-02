program check;

uses
	testlib, sysutils;

var
	ja, pa, m, n: longint;

begin
	ja := ans.readlongint;
	pa := ouf.readlongint;

	if (ja <> pa) then
		quit(_wa, format('expected: %d, found: %d', [ja, pa]));

    m := inf.readlongint;
    n := inf.readlongint;

	quit(_ok, format('%dx%d, ans = %d', [m, n, ja]));
end.