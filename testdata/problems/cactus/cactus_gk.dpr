{$q-,r-}
const
    max_n = 20000;
    max_m = 1000;
    max_k = 1000;
    max_l = 7000;
    max   = 100000000;

var
    res: array [1..max_l] of longint;

procedure mul2(b: longint);
var
    i: longint;
    p: longint;
begin
    p := 0;
    for i := 1 to max_l do begin
        p := res[i] * b + p div 10;
        res[i] := p mod 10;
    end;
end;

var
    z: longint = 1;

procedure mul(b: longint);
begin
    if (max div z) > b then begin
        z := z * b;
    end else begin
        mul2(z);
        z := b;
    end;
end;

procedure o();
var
    i: longint;
    f: boolean;
begin
    f := false;
    for i := max_l downto 2 do begin
        f := f or (res[i] <> 0);
        if f then write(res[i]);
    end;
    writeln(res[1]);
end;

type
    plist = ^tlist;
    tlist = record
        value: longint;
        next: plist;
    end;
    tstate = (white, grey, black);

var
    i, j, l, n, m: longint;
    b, e: array[1..max_m * max_k] of longint;
    u: array [1..max_m * max_k] of boolean;
    ce: longint;
    r: array [1..max_n] of plist;
    s: array [1..max_n] of tstate;
    c: array [1..max_n] of boolean;
    ine: array [1..max_n] of longint;
    f: boolean;

function other(edge, v: longint): longint;
begin
    if b[edge] = v then begin
        other := e[edge];
    end else begin
        other := b[edge];
    end;
end;

procedure add(v, e: longint);
var
    p: plist;
begin
    new(p);
    p.value := e;
    p.next := r[v];
    r[v] := p;
end;

procedure dfs(i, j: longint);
var
    p: plist;
    k: longint;
    q: longint;
begin
    ine[i] := j;
    s[i] := grey;
    p := r[i];
    while p <> nil do begin
        j := other(p.value, i);
        if s[j] = white then begin
            dfs(j, p.value);
        end else if (s[j] = grey) and (j <> other(ine[i], i)) then begin
            f := f and not u[p.value];
            u[p.value] := true;

            k := i;
            q := 1;
            while k <> j do begin
                f := f and not u[ine[k]];
                u[ine[k]] := true;
                k := other(ine[k], k);
                inc(q);
            end;
            mul(q + 1);
        end;
        p := p.next;
    end;
    s[i] := black;
end;

var
    tb, te: longint;
begin
    assign(input, 'cactus.in'); reset(input);
    assign(output, 'cactus.out'); rewrite(output);

    ce := 1;
    b[ce] := 1;
    e[ce] := 1;

    read(n, m);
    for i := 1 to m do begin
        read(l);
        read(tb);
        for j := 2 to l do begin
            read(te);
            inc(ce);
            b[ce] := tb;
            e[ce] := te;
            add(tb, ce);
            add(te, ce);
            tb := te;
        end;
    end;

    for i := 1 to n do begin
        s[i] := white;
        c[i] := false;
    end;

    res[1] := 1;
    f := true;
    dfs(1, 0);

    if f then begin
        for i := 1 to n do begin
            f := f and (s[i] = black);
        end;
    end;

    mul(max);
    if f then begin
        o();
    end else begin
        writeln(0);
    end;

    close(input);
    close(output);
end.