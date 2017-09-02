uses
    tools, sysutils;

var
    dx, dy, c: longint;
    i, n: longint;
    t: string;
begin
    n := nextInt();
    dx := nextInt();
    dy := nextInt();
    c := nextInt();
    t := nextString();
    randseed := nextInt();

    for i := 1 to n do begin
        while not add(random(dx * 2 + 1) - dx, random(dy) + 1, chr(ord('A') + random(c))) do;
    end;

    sight(t);
    done();
end.