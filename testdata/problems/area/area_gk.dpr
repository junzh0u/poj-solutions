uses
    math;
const
    max_n = 100;
    eps = 1e-8;
    inf: double = 1/0;

var
    i, j, n: longint;
    s: string;
    ch: array [1..max_n] of char;
    x, y: array [1..max_n] of longint;
    a: array [1..max_n * max_n] of double;
    b: array [1..max_n] of double;
    d: array [1..max_n] of char;
    l, r: array [1..max_n * max_n] of double;
    c, ca: longint;

procedure qsort2(l, r: longint);
var
    i, j: longint;
    x, t: double;
    c: char;
begin
    x := b[(l * 13 + r * 7) div 20];
    i := l;
    j := r;
    while i <= j do begin
        while b[i] < x do inc(i);
        while x < b[j] do dec(j);
        if i <= j then begin
            t := b[i]; b[i] := b[j]; b[j] := t;
            c := d[i]; d[i] := d[j]; d[j] := c;
            inc(i);
            dec(j);
        end;
    end;
    if (l < j) then qsort2(l, j);
    if (i < r) then qsort2(i, r);
end;


procedure check(lx, rx, cx: double);
var
    i: longint;
    f: boolean;
begin
    for i := 1 to n do begin
        b[i] := arctan2(x[i] - cx, y[i]);
        d[i] := ch[i];
    end;
    qsort2(1, n);

    {
    write(lx, ' ', rx, ' ');
    for i := 1 to n do begin
        write(d[i]);
    end;
    writeln;
    }

    f := true;
    for i := 1 to n do begin    
        f := f and (s[i] = d[i]);
    end;
    if f then begin
        inc(ca);
        l[ca] := lx;
        r[ca] := rx;
    end;
end;

procedure qsort(l, r: longint);
var
    i, j: longint;
    x, t: double;
begin
    x := a[(l * 13 + r * 7) div 20];
    i := l;
    j := r;
    while i <= j do begin
        while a[i] < x do inc(i);
        while x < a[j] do dec(j);
        if i <= j then begin
            t := a[i]; a[i] := a[j]; a[j] := t;
            inc(i);
            dec(j);
        end;
    end;
    if (l < j) then qsort(l, j);
    if (i < r) then qsort(i, r);
end;

begin
    assign(input, 'area.in'); reset(input);
    assign(output, 'area.out'); rewrite(output);

    readln(n);
    readln(s);

    for i := 1 to n do begin
        readln(ch[i], x[i], y[i]);
    end;

    c := 0;
    for i := 1 to n do begin
        for j := i + 1 to n do begin
            if (y[i] <> y[j]) then begin
                inc(c);
                a[c] := (x[j] * y[i] - x[i] * y[j]) / (y[i] - y[j]);
            end;
        end;
    end;

    qsort(1, c);

    if (c <> 0) then begin
        check(-inf, a[1], a[1] - 1);
        for i := 2 to c do begin
            if (a[i] - a[i - 1]) > eps then begin
                check(a[i - 1], a[i], (a[i] + a[i - 1]) / 2);
            end;
        end;
        check(a[c], +inf, a[c] + 1);
    end else begin
        check(-inf, +inf, 0);
    end;

    writeln(ca);
    for i := 1 to ca do begin
        if (l[i] = -inf) then write('*') else write(l[i]:0:9);
        write(' ');
        if (r[i] = +inf) then write('*') else write(r[i]:0:9);
        write(' ');
    end;

    close(input);
    close(output);
end.