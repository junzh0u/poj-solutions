{$o-,r+,q+}
uses
    sysutils, math;

const
    max_n = 20;
    eps = 1e-10;

type
    tvect = record
        x, y, z: double;
    end;

var
    lat, lon: array [1..max_n] of double;
    v: array [1..max_n] of tvect;
    n: longint;

function d(x, y, z: double): double;
begin
    d := sqrt(x * x + y * y + z * z);
end;

function dist(var v1, v2: tvect): double;
begin
    dist := d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
end;

function d2l(d: double): double;
begin
    d2l := 2 * arcsin(d / 2);
end;

procedure solve1();
begin
    writeln(pi);
end;

procedure solve2();
begin
    writeln(pi - d2l(dist(v[1], v[2])) / 2:0:9);
end;

function scale(var v: tvect; d: double): tvect;
begin
    result.x := v.x * d;
    result.y := v.y * d;
    result.z := v.z * d;
end;

function norm(v: tvect): tvect;
begin
    norm := scale(v, 1 / d(v.x, v.y, v.z));
end;

function add(v1, v2: tvect): tvect;
begin
    result.x := v1.x + v2.x;
    result.y := v1.y + v2.y;
    result.z := v1.z + v2.z;
end;

function sub(v1, v2: tvect): tvect;
begin
    result.x := v1.x - v2.x;
    result.y := v1.y - v2.y;
    result.z := v1.z - v2.z;
end;

function cross(v1, v2: tvect): tvect;
begin
    result.x := v1.y * v2.z - v2.y * v1.z;
    result.y := v1.z * v2.x - v2.z * v1.x;
    result.z := v1.x * v2.y - v2.x * v1.y;
end;

function dot(v1, v2: tvect): double;
begin
    dot := v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
end;

function intersect(var v1, v2, r1, r2: tvect; r: double): boolean;
var
    vx, vy, vz: tvect;
    xn, p: double;
    x, z: double;
begin
    if dist(v1, v2) < 2 - eps then begin
        intersect := true;

        vx := norm(add(v1, v2));
        vy := norm(sub(v1, v2));
        vz := cross(vx, vy);

        xn := dot(vx, v1);
        p := 1 - r * r / 2;

        x := p / xn;
        if (abs(x) >= 1) then begin
            intersect := false;
            exit;
        end;
        z := sqrt(1 - x * x);

        r1 := add(scale(vx, x), scale(vz, +z));
        r2 := add(scale(vx, x), scale(vz, -z));
    end else begin
        intersect := false;
    end;
end;

procedure solve3();
var
    l, r, m: double;
    i, j, k: longint;
    iv1, iv2: tvect;
    f, fi: boolean;
    f1, f2: boolean;
begin
    l := 0;
    r := 2 - 2 * eps;
    while true do begin
        m := (l + r) / 2;
        if (l = m) or (r = m) then break;

        f := true;
        fi := false;
        for i := 1 to n do begin
            for j := i + 1 to n do begin
                if intersect(v[i], v[j], iv1, iv2, m + eps) then begin
                    fi := true;
                    f1 := false;
                    f2 := false;
                    for k := 1 to n do begin
                        f1 := f1 or (dist(v[k], iv1) < m - eps);
                        f2 := f2 or (dist(v[k], iv2) < m - eps);
                    end;
                    f := f and f1 and f2;
                end;
            end;
        end;

        if f and fi then begin
            r := m;
        end else begin
            l := m;
        end;
    end;
    writeln(d2l(l):0:9);
end;

var
    i: longint;
begin
    assign(input, 'knockdown.in'); reset(input);
    assign(output, 'knockdown.out'); rewrite(output);

    read(n);

    for i := 1 to n do begin
        read(lat[i], lon[i]);
        lat[i] := lat[i] / 180 * pi;
        lon[i] := lon[i] / 180 * pi;
    end;

    for i := 1 to n do begin        
        v[i].x := cos(lon[i]) * cos(lat[i]);
        v[i].y := sin(lon[i]) * cos(lat[i]);
        v[i].z := sin(lat[i]);
    end;

    if (n = 1) then begin
        solve1();
    end else if (n = 2) then begin
        solve2();
    end else begin
        solve3();
    end;

    close(input);
    close(output);
end.