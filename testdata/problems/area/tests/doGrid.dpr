uses
    tools, sysutils;

var
    v1x, v2x, v1y, v2y: longint;
    i: longint;
    t: string;
    x, y: longint;
    n, c: longint;
begin
    n := nextInt();
    v1x := nextInt();
    v1y := nextInt();
    v2x := nextInt();
    v2y := nextInt();
    c := nextInt();
    t := nextString();
    randseed := nextInt();

    for i := 1 to n do begin
        repeat
            x := random(2 * max_c + 1) - max_c;
            y := random(2 * max_c + 1) - max_c;
        until add(v1x * x + v2x * y, v1y * x + v2y * y, chr(ord('A') + random(c)));
    end;

    sight(t);
    done();
end.