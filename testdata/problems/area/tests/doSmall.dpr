uses
    tools, sysutils;

var
    dx, dy, c: longint;
    i, n: longint;
    t: string;
    x1, y1, x2, y2, x3, y3, x4, y4: longint;

function q(x1, y1, x2, y2: longint): double;
begin
    if (y1 <> y2) then begin
        q := (x2 * y1 - x1 * y2) / (y1 - y2);
    end else begin
        q := random * 1e100;
    end;
end;

begin
    n := nextInt();
    dx := nextInt();
    dy := nextInt();
    c := nextInt();
    t := nextString();
    randseed := nextInt();

    repeat
        x1 := random(dx * 2 + 1) - dx;
        y1 := random(random(dy) + 1);
        x2 := random(dx * 2 + 1) - dx;
        y2 := random(random(dy) + 1);
        x3 := random(dx * 2 + 1) - dx;
        y3 := random(random(dy) + 1);
        x4 := random(dx * 2 + 1) - dx;
        y4 := random(random(dy) + 1);
    until abs(q(x1, y1, x2, y2) - q(x3, y3, x4, y4)) < 1e-3;

    add(x1, y1, chr(ord('A') + random(c)));
    add(x2, y2, chr(ord('A') + random(c)));
    add(x3, y3, chr(ord('A') + random(c)));
    add(x4, y4, chr(ord('A') + random(c)));

    for i := 5 to n do begin
        while not add(random(dx * 2 + 1) - dx, random(dy) + 1, chr(ord('A') + random(c))) do;
    end;

    sight(t);
    done();
end.