{$q+,r+,o-}
uses
    math;
var
    n1, n2, n3, m1, m2, m3, m4: longint;
    best: longint;
    bm1, bm2, bm3, bm4: longint;
begin
    assign(input, 'guards.in'); reset(input);
    assign(output, 'guards.out'); rewrite(output);

    read(n1, n2, n3);

    best := 10000;
    for m1 := 0 to 1000 do begin
        if 3 * m1 >= best then break;
        for m3 := 0 to 1000 do begin
            if 3 * m1 + 2 * m3 >= best then break;
            if m1 + 2 * m3 < n2 then continue;
            for m4 := 0 to 1000 do begin
                if 3 * m1 + 4 * m3 + 5 * m4 >= best then break;
                if m1 + m3 + 2 * m4 < n3 then continue;
                m2 := max(0, n1 - (m1 + 2 * m3 + 3 * m4));
                if 3 * m1 + m2 + 4 * m3 + 5 * m4 < best then begin
                    best := 3 * m1 + m2 + 4 * m3 + 5 * m4;
                    bm1 := m1;
                    bm2 := m2;
                    bm3 := m3;
                    bm4 := m4;
                end;
            end;
        end;
    end;

    writeln(3 * bm1, ' ', bm2, ' ', 4 * bm3, ' ', 5 * bm4);

    close(input);
    close(output);
end.