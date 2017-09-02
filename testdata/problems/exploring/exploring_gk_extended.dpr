const
    max_n = 500;
var
    s: string;
    u: array [1..max_n, 1..max_n] of boolean;
    v: array [1..max_n, 1..max_n] of extended;

function f(i, j: longint): extended;
var
    k: longint;
begin
    if not u[i][j] then begin
        u[i][j] := true;
        if (i < j) then begin
            v[i][j] := 0;
            if (s[i] = s[j]) and not odd(j - i) then begin
                for k := i + 2 to j do begin
                    v[i][j] := v[i][j] + f(i + 1, k - 1) * f(k, j);
                end;
            end;
        end else begin
            v[i][j] := 1;
        end;
    end;
    f := v[i][j];
end;

begin
    assign(input, 'exploring.in'); reset(input);
    assign(output, 'exploring.out'); rewrite(output);

    readln(s);
    writeln(f(1, length(s)):0:0);

    close(input);
    close(output);
end.
