{$o-}
uses
    math;

const
    max_n = 100000;
var
    n: longint;
    g, a, k, p, l, r: array [1..max_n] of longint;
    s: array [1..max_n] of int64;
    
procedure qsort(l, r: longint);
var
    i, j: longint;
    x, t: longint;
begin
    x := a[(7 * l + 13 * r) div 20];
    i := l;
    j := r;
    while (i <= j) do begin
        while (a[i] > x) do inc(i);
        while (a[j] < x) do dec(j);
        if (i <= j) then begin
            t := a[i]; a[i] := a[j]; a[j] := t;
            t := k[i]; k[i] := k[j]; k[j] := t;
            inc(i);
            dec(j);
        end;
    end;
    if (l < j) then qsort(l, j);
    if (i < r) then qsort(i, r);
end;

function find(i: longint): longint;
begin
    if p[i] <> i then begin
        p[i] := find(p[i]);
    end;
    find := p[i];
end;

procedure union(i, j: longint);
begin
    i := find(i);
    j := find(j);

    if i <> j then begin
        p[j] := i;
        l[i] := min(l[i], l[j]);
        r[i] := max(r[i], r[j]);
        s[i] := s[i] + s[j];
    end;
end;

var
    i, j: longint;
    best: int64;
    bestl, bestr: longint;
begin
    assign(input, 'feelgood.in'); reset(input);
    assign(output, 'feelgood.out'); rewrite(output);
       
    read(n);
    for i := 1 to n do begin
        read(g[i]);
    end;

    for i := 1 to n do begin
        a[i] := g[i];
        k[i] := i;
    end;
    qsort(1, n);

    best := -1;
    bestl := 0;
    bestr := 0;
    for i := 1 to n do begin
        p[i] := i;
        l[i] := i;
        r[i] := i;
        s[i] := g[i];

        if best < g[i] then begin
            best := g[i];
            bestl := i;
            bestr := i;
        end;
    end;

    for i := 1 to n do begin
        if (k[i] > 1) and (g[k[i] - 1] >= a[i]) then begin
            union(k[i] - 1, k[i]);
        end;
        if (k[i] < n) and (g[k[i] + 1] >= a[i]) then begin
            union(k[i] + 1, k[i]);
        end;
        j := find(k[i]);

        if best < s[j] * a[i] then begin
            best := s[j] * a[i];
            bestl := l[j];
            bestr := r[j];
        end;
    end;

    writeln(best);
    writeln(bestl, ' ', bestr);

    close(input);
    close(output);
end.