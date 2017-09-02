uses
    tools, sysutils;

var
    dx, dy, c: longint;
    i, n: longint;
    tx, ty: longint;
    i1, i2: longint;
    t: string;
begin
    n := nextInt();
    dx := nextInt();
    dy := nextInt();
    c := nextInt();
    t := nextString();
    randseed := nextInt();

    for i := 1 to n div 5 do begin
        while not add((random(dx * 2 + 1) - dx) div 10, random(dy) div 10 + 1, chr(ord('A') + random(c))) do;
    end;

    for i := n div 5 + 1 to n do begin
        repeat
            i1 := random(i - 2) + 1;
            i2 := random(i - 2) + 1;
            tx := 2 * x[i1] - x[i2];
            ty := 2 * x[i1] - x[i2];
        until add(tx, ty, chr(ord('A') + random(c)));
    end;

    sight(t);
    done();
end.