uses
    testlib, sysutils;

const
    eps = 1.1e-6;
    max_n = 100;
    inf = 1 / 0;

function parseDouble(s: string): double;
var
    code: integer;
begin
    if s <> '*' then begin
        val(s, result, code);
        if code <> 0 then quit(_pe, format('Expected real instead of "%s"', [s]));
    end else begin
        parseDouble := inf;
    end;
end;

function eq(j, p: double): boolean;
begin
    eq := (j = p) or (abs(j - p) <= eps);
end;

var
    ja, pa: longint;
    jl, jr, pl, pr: double;
    i: longint;
begin
    ja := ans.readLongint();
    pa := ouf.readLongint();

    if ja <> pa then begin
        quit(_wa, format('%d segments instead of %d', [pa, ja]));
    end;

    for i := 1 to ja do begin
        jl := parseDouble(ans.readWord(blanks, blanks));
        jr := parseDouble(ans.readWord(blanks, blanks));
        pl := parseDouble(ouf.readWord(blanks, blanks));
        pr := parseDouble(ouf.readWord(blanks, blanks));
        if (not eq(jl, pl) or not eq(jr, pr)) then begin
            quit(_wa, format('Segment %d: (%.10f, %.10f) instead of (%.10f, %.10f)', [i, pl, pr, jl, jr]));
        end;
    end;

    quit(_ok, format('%d segments', [ja]));
end.