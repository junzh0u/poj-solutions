{$apptype CONSOLE}
{$Q+,R+}

uses
	Testlib, Sysutils;
	
var
	a, b : integer;
		
begin
	a := ouf.Readlongint;
	b := ans.Readlongint;
	if a <> b then
		Quit(_WA, format('%d instead of %d', [a,b]))
	else
		Quit(_OK, inttostr(a));		
end.
