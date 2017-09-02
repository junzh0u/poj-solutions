uses
    math;

function sum(a, d, n: int64): int64;
begin
    result := (2 * a - d * n) * (n + 1) div 2;
end;

var
    i, n, k, p, q, a: longint;
    r: int64;
begin
    assign(input, 'joseph.in'); reset(input);
    assign(output, 'joseph.out'); rewrite(output);

    read(n, k);

    i := 1;
    r := 0;
    while (i <= n) do begin
        q := k mod i;
        p := k div i;
        if p > 0 then a := q div p else a := maxlongint;
        r := r + sum(q, p, min(a, n - i));
        i := i + min(a, n - i) + 1;
    end;

    writeln(r);

    close(input);
    close(output);
end.