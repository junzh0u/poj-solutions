const
    max_n = 1000;

function readInt(): longint;
var
    ch: char;
    r: longint;
begin
    read(ch);
    while not (ch in ['0'..'9']) do begin
        read(ch);
    end;

    r := 0;
    while ch in ['0'..'9'] do begin
        r := r * 10 + ord(ch) - ord('0');
        read(ch);
    end;
    readInt := r;
end;

function readIp(): longint;
begin
    readIp := (readInt() shl 24) + (readInt() shl 16) + (readInt() shl 8) + readInt();
end;

procedure writeIp(n: longint);
begin
    writeln((n shr 24) and $ff, '.', (n shr 16) and $ff, '.', (n shr 8) and $ff, '.', n and $ff);
end;

var
    a: array [1..max_n] of longint;
    i, j, n: longint;
    mask: longint;
    f: boolean;
begin
    assign(input, 'ip.in'); reset(input);
    assign(output, 'ip.out'); rewrite(output);

    read(n);

    for i := 1 to n do begin
        a[i] := readIp();
    end;

    for i := 0 to 32 do begin
        if i <> 0 then begin
            mask := not (1 shl (i - 1) shl 1 - 1);
        end else begin
            mask := -1;
        end;

        f := true;
        for j := 2 to n do begin
            f := f and ((a[1] and mask) = (a[j] and mask));
        end;
        if f then begin
            writeIp(a[1] and mask);
            writeIp(mask);
            break;
        end;
    end;

    close(input);
    close(output);
end.