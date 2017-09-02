uses
    sysutils;

const
    value: array [1..9] of char = ('6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A');
    suit: array [1..4] of char = ('S', 'C', 'D', 'H');

var
    deck: array [1..36] of string;
    i, j: longint;
    p: array [1..36] of longint;
begin
    randseed := strToInt(paramstr(1));

    for i := 1 to 9 do begin
        for j := 1 to 4 do begin
            deck[(i - 1) * 4 + j] := value[i] + suit[j];
        end;
    end;

    for i := 1 to 36 do begin
        j := random(i) + 1;
        p[i] := p[j];
        p[j] := i;
    end;

    for i := 1 to 9 do begin
        for j := 1 to 4 do begin
            if j <> 1 then write(' ');
            write(deck[p[(i - 1) * 4 + j]]);
        end;
        writeln;
    end;
end.