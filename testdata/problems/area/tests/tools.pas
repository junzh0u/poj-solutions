unit tools;

interface
const
    max_c = 100;
    max_n = 100;
    eps = 1e-10;

var
    u: array [-max_c..max_c, 1..max_c] of boolean;
    n: longint;
    x, y: array [1..max_n] of longint;
    c: array [1..max_n] of char;
    s: string;
    sx: array [1..max_n * max_n] of double;
    cs: longint;

function nextString(): string;
function nextInt(): longint;

function add(nx, ny: integer; nc: char): boolean;
procedure done();
function see(cx: double): string;
procedure randomSight();
procedure maxSight();
procedure minSight();
procedure impossibleSight();
procedure sight(t: string);

implementation

uses
    math, sysutils;

var
    paramIndex: longint;
    b: array [1..max_n] of double;
    d: array [1..max_n] of char;
    v: array [1..max_n * max_n] of string;
    cv: longint;

function nextInt(): longint;
begin
    inc(paramIndex);
    nextInt := strToInt(paramStr(paramIndex));
end;

function nextString(): string;
begin
    inc(paramIndex);
    nextString:= paramStr(paramIndex);
end;

function add(nx, ny: integer; nc: char): boolean;
begin
    if not ((abs(nx) <= max_c) and (0 < ny) and (ny <= max_c)) then begin
        add := false;
        exit;
    end;
    assert(n < max_n);
    assert(abs(nx) <= max_c);
    assert((0 < ny) and (ny <= max_c));
    assert(('A' <= nc) and (nc <= 'Z'));

    if not u[nx][ny] then begin
        u[nx][ny] := true;
        inc(n);
        x[n] := nx;
        y[n] := ny;
        c[n] := nc;

        add := true;
    end else begin
        add := false;
    end;
end;

procedure done();
var
    i, j: longint;
    p: array[1..max_c] of longint;
    q: array['A'..'Z'] of char;
begin
    for i := 1 to n do begin
        j := random(i) + 1;
        p[i] := p[j];
        p[j] := i;
    end;

    for i := 0 to 25 do begin
        j := random(i + 1);
        q[chr(ord('A') + i)] := q[chr(ord('A') + j)];
        q[chr(ord('A') + j)] := chr(ord('A') + i);
    end;

    writeln(n);
    for i := 1 to n do begin
        write(q[s[i]]);
    end;
    writeln;
    for i := 1 to n do begin
        writeln(q[c[p[i]]], ' ', x[p[i]], ' ', y[p[i]]);
    end;
end;

procedure qsort(l, r: longint);
var
    i, j: longint;
    x, t: double;
begin
    x := sx[(l * 13 + r * 7) div 20];
    i := l;
    j := r;
    while i <= j do begin
        while sx[i] < x do inc(i);
        while x < sx[j] do dec(j);
        if i <= j then begin
            t := sx[i]; sx[i] := sx[j]; sx[j] := t;
            inc(i);
            dec(j);
        end;
    end;
    if (l < j) then qsort(l, j);
    if (i < r) then qsort(i, r);
end;

procedure qsortv(l, r: longint);
var
    i, j: longint;
    x, t: string;
begin
    x := v[(l * 13 + r * 7) div 20];
    i := l;
    j := r;
    while i <= j do begin
        while v[i] < x do inc(i);
        while x < v[j] do dec(j);
        if i <= j then begin
            t := v[i]; v[i] := v[j]; v[j] := t;
            inc(i);
            dec(j);
        end;
    end;
    if (l < j) then qsortv(l, j);
    if (i < r) then qsortv(i, r);
end;

procedure calcSegments();
var
    i, j: longint;
begin
    cs := 0;
    for i := 1 to n do begin
        for j := i + 1 to n do begin
            if (y[i] <> y[j]) then begin
                inc(cs);
                sx[cs] := x[i] - (x[j] - x[i]) / (y[j] - y[i]) * y[i];
            end;
        end;
    end;

    qsort(1, cs);
end;

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

function see(cx: double): string;
var
    i: longint;
    s: string;
begin
    for i := 1 to cs do begin
        assert(abs(sx[i] - cx) > 1e-9);
    end;

    for i := 1 to n do begin
        b[i] := arctan2(x[i] - cx, y[i]);
        d[i] := c[i];
    end;
    qsort2(1, n);

    s := '';
    for i := 1 to n do begin
        s := s + d[i];
    end;
    see := s;
end;

procedure randomSight();
var
    r: longint;
    x: double;
begin
    calcSegments();
    if cs <> 0 then begin
        repeat 
            r := random(cs + 1);
        until (r = 0) or (r = cs) or (sx[r] <> sx[r + 1]);
        if (r = 0) then begin
            x := sx[1] - 1;
        end else if (r = cs) then begin
            x := sx[n] + 1;
        end else begin
            x := (sx[r] + sx[r + 1]) / 2;
        end;
    end else begin
        x := 0;
    end;    
    s := see(x);
end;

procedure maxSight();
var
    i: longint;
    best: longint;
    c: longint;
begin
    calcSegments();
    cv := 0;
    if cs <> 0 then begin
        inc(cv);
        v[cv] := see(sx[1] - 1);
        for i := 2 to cs do begin
            if (sx[i] - sx[i - 1]) > eps then begin
                inc(cv);
                v[cv] := see((sx[i] + sx[i - 1]) / 2);
            end;
        end;
        inc(cv);
        v[cv] := see(sx[cs] + 1);
        qsortv(1, cv);
        c := 1;
        best := 0;
        for i := 2 to cv do begin
            if (v[i] = v[i - 1]) then begin
                inc(c);
            end else begin
                c := 1;
            end;
            if (best < c) then begin
                best := c;
                s := v[i];
            end;
        end;
    end else begin
        s := see(0);
    end;
end;

procedure minSight();
var
    i: longint;
    best, x: double;
begin
    calcSegments();

    best := 1e10;
    x := 0;

    for i := 2 to cs do begin
        if (best > (sx[i] - sx[i - 1])) and ((sx[i] - sx[i - 1]) > 2 * eps) then begin
            best := (sx[i] - sx[i - 1]);
            x := (sx[i] + sx[i - 1]) / 2;
        end;
    end;
    s := see(x);
end;

procedure infSight();
var
    x: double;
begin
    calcSegments();

    if cs <> 0 then begin
        if random > 0.5 then begin
            x := sx[1] - 1;
        end else begin
            x := sx[cs] + 1;
        end;
    end else begin
        x := 0;
    end;

    s := see(x);
end;

procedure impossibleSight();
var
    p: array [1..max_n] of longint;
    i, j: longint;
    f: boolean;
begin
    calcSegments();

    assert(cs >= 2);
    cv := 0;
    inc(cv);
    v[cv] := see(sx[1] - 1);
    for i := 2 to cs do begin
        if (sx[i] - sx[i - 1]) > eps then begin
            inc(cv);
            v[cv] := see((sx[i] + sx[i - 1]) / 2);
        end;
    end;
    inc(cv);
    v[cv] := see(sx[cs] + 1);
    repeat 
        s := '';
        for i := 1 to n do begin
            j := random(i) + 1;
            p[i] := p[j];
            p[j] := i;
        end;
        for i := 1 to n do begin
            s := s + c[i];
        end;
        f := true;
        for i := 1 to cv do begin
            f := f and (v[i] <> s);
        end;
    until f;
end;

procedure sight(t: string);
begin
    if t = 'max' then maxSight()
    else if t = 'min' then minSight()
    else if t = 'random' then randomSight()
    else if t = 'impossible' then impossibleSight()
    else if t = 'inf' then infSight()
    else assert(false);
end;

begin
    fillchar(u, sizeof(u), 0);
    n := 0;
end.