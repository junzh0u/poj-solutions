program check;

uses
	testlib, sysutils;

const
	maxn = 100000;

var
	i, l, r, n: longint;
	ja, pa, pc, pm: int64;
	a: array [1..maxn] of int64;

begin
	n := inf.readlongint;
	for i := 1 to n do begin
		a[i] := inf.readlongint;
	end;

	ja := round(ans.readreal);
	pa := round(ouf.readreal);

	if ja > pa then
		quit(_wa, format('expected: %d, found: %d', [ja, pa]));

	l := ouf.readlongint;
	r := ouf.readlongint;
	if (l < 1) or (l > n) or (r < 1) or (r > n) or (l > r) then
		quit(_wa, format('invalid segment: %d - %d', [l, r]));

	pc := 0;
	pm := a[l];
	for i := l to r do begin
		pc := pc + a[i];
		if a[i] < pm then
			pm := a[i];
	end;
	pc := pc * pm;

	if pa <> pc then
		quit(_wa, format('promised: %d, calculated: %d', [pa, pc]));

	if ja < pa then
		quit(_fail, format('expected: %d, found: %d', [ja, pa]));

	quit(_ok, format('%d, [%d - %d]', [pa, l, r]));
end.
