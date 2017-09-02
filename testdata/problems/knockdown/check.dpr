uses
    testlib, sysutils;

const
    eps = 1.1e-6;
var
    ja, pa: double;
begin
    ja := ans.readReal();
    pa := ouf.readReal();

    if abs(ja - pa) > eps then begin
        quit(_wa, format('%.7f instead of %.7f', [pa, ja]));
    end;

    quit(_ok, format('%.7f', [ja]));
end.