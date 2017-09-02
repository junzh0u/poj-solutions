uses
    sysutils;

const
    max_n = 20;

var
    i, j, n, m: longint;
    s: array [1..max_n, 0..max_n] of char;
    cd, cr: array [0..max_n + 1, 0..max_n + 1] of boolean;
    c: array [0..max_n + 1, 0..max_n + 1] of char;
    f: boolean;
    u1, u2: array [char] of boolean;

procedure dfs(i, j: longint; ch: char);
begin
    c[i][j] := ch;
    if (not cd[i][j]) and (c[i + 1][j] = #0) then dfs(i + 1, j, ch);
    if (not cd[i - 1][j]) and (c[i - 1][j] = #0) then dfs(i - 1, j, ch);
    if (not cr[i][j]) and (c[i][j + 1] = #0) then dfs(i, j + 1, ch);
    if (not cr[i][j - 1]) and (c[i][j - 1] = #0) then dfs(i, j - 1, ch);
end;

procedure dfs2(i, j: longint; ch: char);
begin
    c[i][j] := ch;
    if (s[i][j] = s[i + 1][j]) and (c[i + 1][j] = #0) then dfs2(i + 1, j, ch);
    if (s[i][j] = s[i - 1][j]) and (c[i - 1][j] = #0) then dfs2(i - 1, j, ch);
    if (s[i][j] = s[i][j + 1]) and (c[i][j + 1] = #0) then dfs2(i, j + 1, ch);
    if (s[i][j] = s[i][j - 1]) and (c[i][j - 1] = #0) then dfs2(i, j - 1, ch);
end;

procedure fill();
var
    i, j: longint;
begin
    fillchar(u1, sizeof(u1), 0);
    fillchar(u2, sizeof(u2), 0);
    fillchar(c, sizeof(c), 0);
    for i := 1 to n do begin
        for j := 1 to m do begin
            u1[s[i][j]] := true;
            if c[i][j] = #0 then begin
                dfs(i, j, s[i][j]);
                u2[s[i][j]] := true;
            end;
        end;
    end;
end;

procedure cutV(i, j, d: longint);
begin
    while (s[i][j] <> s[i][j + 1]) and not cr[i][j] do begin
        cr[i][j] := true;
        i := i + d;
        f := true;
    end;
end;

procedure cutH(i, j, d: longint);
begin
    while (s[i][j] <> s[i + 1][j]) and not cd[i][j] do begin
        cd[i][j] := true;
        j := j + d;
        f := true;
    end;
end;

var
    ch: char;
    q: longint;
begin
    assign(input, 'hardwood.in'); reset(input);
    assign(output, 'hardwood.out'); rewrite(output);

    read(n, m);
    assert((1 <= n) and (n <= max_n));
    assert((1 <= m) and (m <= max_n));
    assert(eoln);
    readln;

    for i := 1 to n do begin
        for j := 1 to m do begin
            read(s[i][j]);
            assert((s[i][j] in ['A'..'Z']) or (s[i][j] in ['a'..'z']) or (s[i][j] in ['0'..'9']));
        end;
        assert(eoln);
        readln;
    end;
    assert(eof);

    fillchar(u1, sizeof(u1), 0);
    for i := 1 to n do begin
        for j := 1 to m do begin
            if c[i][j] = #0 then begin
                assert(not(u1[s[i][j]]));
                dfs2(i, j, s[i][j]);
            end;
        end;
    end;

    for i := 1 to n do begin
        cr[i][0] := true;
        cr[i][m] := true;
    end;

    for j := 1 to m do begin
        cd[0][j] := true;
        cd[n][j] := true;
    end;

    repeat
        fill();

        f := false;
        for i := 1 to n do begin
            for j := 1 to m do begin
                if (c[i][j] <> c[i + 1][j]) or (c[i][j + 1] <> c[i + 1][j + 1]) then begin
                    cutV(i, j, -1);
                end;
                if (c[i - 1][j] <> c[i][j]) or (c[i - 1][j + 1] <> c[i][j + 1]) then begin
                    cutV(i, j, 1);
                end;
                if (c[i][j] <> c[i][j + 1]) or (c[i + 1][j] <> c[i + 1][j + 1]) then begin
                    cutH(i, j, -1);
                end;
                if (c[i][j - 1] <> c[i][j]) or (c[i + 1][j - 1] <> c[i + 1][j]) then begin
                    cutH(i, j, 1);
                end;
            end;
        end;
    until not f;

    q := 0;
    for ch := low(ch) to high(ch) do begin
        if u1[ch] and u2[ch] then begin
            inc(q);
        end;
    end;

    writeln(q);

    close(input);
    close(output);
end.