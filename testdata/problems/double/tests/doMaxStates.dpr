{$apptype console}

uses
    sysutils;

const
    value: array [1..9] of char = ('6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A');
    suit: array [1..4] of char = ('S', 'C', 'D', 'H');

type
    tpos = array [1..9] of longint;

var
    v: array [1..9, 0..4] of char;
    c: array [0..1953125] of double;
    u: array [0..1953125] of boolean;
    z: longint;

function encode(var p: tpos): longint;
var
    i: longint;
    q: longint;
begin
    result := 0;
    q := 1;
    for i := 1 to 9 do begin
        result := result + p[i] * q;
        q := q * 5;
    end;
end;

function pr(var p: tpos): double;
var
    code: longint;
    i, j: longint;
    q: longint;
begin
    code := encode(p);
    if not u[code] then begin
        inc(z);
        u[code] := true;
        c[code] := 0;
        q := 0;
        for i := 1 to 9 do begin
            for j := i + 1 to 9 do begin
                if v[i][p[i]] = v[j][p[j]] then begin
                    dec(p[i]);
                    dec(p[j]);
                    c[code] := c[code] + pr(p);
                    inc(p[i]);
                    inc(p[j]);

                    inc(q);
                end;
            end;
        end;
        if q <> 0 then begin
            c[code] := c[code] / q;
        end;
    end;
    pr := c[code];
end;

var
    deck: array [1..36] of string;
    i, j, m, n: longint;
    p, q: array [1..36] of longint;
    pz, pq: longint;
    pos: tpos;
    t: longint;
    u2: array [1..36, 1..36] of boolean;
    a: longint;
    cv, pv: double;
begin
    randseed := strToInt(paramstr(1));

    for i := 1 to 9 do begin
        for j := 1 to 4 do begin
            deck[(i - 1) * 4 + j] := value[i] + suit[j];
        end;
    end;

    for i := 1 to 9 do begin
        v[i][0] := chr(i);
    end;

    for i := 1 to 9 do begin
        pos[i] := 4;
    end;

    for i := 1 to 36 do begin
        j := random(i) + 1;
        p[i] := p[j];
        p[j] := i;
    end;

    pz := 0;
    while true do begin
        fillchar(u2, sizeof(u2), 0);
        a := 0;
        pq := pz;
        while true do begin
            repeat
                n := random(36) + 1;
                m := random(36) + 1;
            until not u2[n][m];
            u2[n][m] := true;
            inc(a);

            if a = 36 * 36 then break;

            if deck[p[n]][1] <> deck[p[m]][1] then begin
                t := p[n]; p[n] := p[m]; p[m] := t;

                for i := 1 to 9 do begin
                    for j := 1 to 4 do begin
                        v[i][j] := deck[p[(j - 1) * 9 + i]][1];
                    end;
                end;

                fillchar(u, sizeof(u), 0);
                fillchar(c, sizeof(c), 0);
                z := 0;

                u[0] := true;
                c[0] := 1;

                pv := pr(pos);
                writeln(a:4, ' ', n:2, ' ', m:2, ' ', pv:0:10, ' ', z:5, ' ', pq:5, ' ', pz:5);

                if pq < z then begin
                    //writeln(z, ' ', pz, ' ', exp((z - pz) / temperature));
                    //readln;
                    pq := z;
                    q := p;
                    break;
                end;
                {
                if (pq < z) then begin
                    pq := z;
                    q := p;
                    break;
                end;
                }

                t := p[n]; p[n] := p[m]; p[m] := t;
            end;
            //p := s;
            {
            if n < m + 1 then begin
                for i := 0 to (m - n) div 2 do begin
                    t := p[n + i]; p[n + i] := p[m - i]; p[m - i] := t;
                end;

                for i := 1 to 9 do begin
                    for j := 1 to 4 do begin
                        v[i][j] := deck[p[(j - 1) * 9 + i]][1];
                    end;
                end;

                fillchar(u, sizeof(u), 0);
                fillchar(c, sizeof(c), 0);
                z := 0;

                u[0] := true;
                c[0] := 1;

                writeln(a:4, ' ', n:2, ' ', m:2, ' ', pr(pos):0:10, ' ', z:5, ' ', pq:5, ' ', pz:5);

                if (pq < z) then begin
                    pq := z;
                    q := p;
                    break;
                end;
                

                for i := 0 to (m - n) div 2 do begin
                    t := p[n + i]; p[n + i] := p[m - i]; p[m - i] := t;
                end;
            end;
            }
        end;
        {
        temperature := temperature * 0.99;
        writeln(temperature:0:10);
        pz := pq;
        p := q;
        }
        if pz < pq then begin
            pz := pq;
            p := q;
            continue;
        end;
        break;
    end;

    for i := 1 to 9 do begin
        for j := 1 to 4 do begin
            if j <> 1 then write(' ');
            write(deck[p[(j - 1) * 9 + i]]);
        end;
        writeln;
    end;
end.