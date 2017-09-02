const
    max_n = 20000;

type
    plist = ^tlist;
    tlist = record
        value: longint;
        next: plist;
    end;
    tstate = (white, grey, black);

var
    i, j, l, n, m: longint;
    b, e: longint;
    r: array [1..max_n] of plist;
    s: array [1..max_n] of tstate;
    c: array [1..max_n] of boolean;
    parent: array [1..max_n] of longint;
    f: boolean;

procedure add(b, e: longint);
var
    p: plist;
begin
    new(p);
    p.value := e;
    p.next := r[b];
    r[b] := p;
end;

procedure dfs(i, j: longint);
var
    p: plist;
    k: longint;
begin
    parent[i] := j;
    s[i] := grey;
    p := r[i];
    while p <> nil do begin
        j := p.value;
        if s[j] = white then begin
            dfs(j, i);
        end else if (s[j] = grey) and (j <> parent[i]) then begin
            k := i;
            while true do begin
                f := f and not c[k];
                c[k] := true;
                if (k = j) then break;
                k := parent[k];
            end;
        end;
        p := p.next;
    end;
    s[i] := black;
end;

begin
    assign(input, 'cactus.in'); reset(input);
    assign(output, 'cactus.out'); rewrite(output);

    read(n, m);
    for i := 1 to m do begin
        read(l);
        read(b);
        for j := 2 to l do begin
            read(e);
            add(b, e);
            add(e, b);
            b := e;
        end;
    end;

    for i := 1 to n do begin
        s[i] := white;
        c[i] := false;
    end;

    f := true;
    dfs(1, 1);

    if f then begin
        for i := 1 to n do begin
            f := f and (s[i] = black);
        end;
    end;

    if f then begin
        writeln('YES');
    end else begin
        writeln('NO');
    end;

    close(input);
    close(output);
end.