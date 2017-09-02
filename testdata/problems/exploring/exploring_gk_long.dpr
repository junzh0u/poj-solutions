{$q+,r+}
const
    max_n = 300;
    max_l = 90;

type    
    tlong = array [1..max_l] of longint;

procedure addmul(var a, b, c: tlong);
var
    i, j: longint;
    p: longint;
begin
    for i := 1 to max_l do begin
        for j := 1 to max_l - i do begin
            a[i + j - 1] := a[i + j - 1] + b[i] * c[j];
        end;
    end;

    p := 0;
    for i := 1 to max_l do begin
        p := p div 10 + a[i];
        a[i] := p mod 10;
    end;
end;

var
    s: string;
    u: array [1..max_n, 1..max_n] of boolean;
    v: array [1..max_n, 1..max_n] of tlong;

procedure f(i, j: longint);
var
    k: longint;
begin
    if not u[i][j] then begin
        u[i][j] := true;
        fillchar(v[i][j], sizeof(v[i][j]), 0);
        if (i < j) then begin
            if (s[i] = s[j]) and not odd(j - i) then begin
                for k := i + 2 to j do begin
                	if not odd(k - i) then begin
                         f(i + 1, k - 1);
                         f(k, j);
                         addmul(v[i][j], v[i + 1][k - 1], v[k][j]);
                    end;
                end;
            end;
        end else begin
            v[i][j][1] := 1;
        end;
    end;
end;

procedure o(var a: tlong);
var
    i: longint;
    f: boolean;
begin
    f := false;
    for i := max_l downto 2 do begin
        f := f or (a[i] <> 0);
        if f then write(a[i]);
    end;
    writeln(a[1]);
end;

begin
    assign(input, 'exploring.in'); reset(input);
    assign(output, 'exploring.out'); rewrite(output);

    readln(s);
    f(1, length(s));
    o(v[1][length(s)]);

    close(input);
    close(output);
end.
