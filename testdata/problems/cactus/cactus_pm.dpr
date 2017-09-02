{$q+,r+,s+,o-}

uses
  sysutils, math;

const
  maxn = 20000;
  maxm = 1000000;

var
  l: integer;
  ob, ds, nx: array[1..maxm * 2] of integer;
  hd: array[1..maxn] of integer;

procedure add(i, j: integer);
begin
  inc(l);
  ds[l] := j;
  nx[l] := hd[i];
  hd[i] := l;
end;

var
  z: array[1..maxn] of boolean;

procedure dfs1(i: integer);
var
  j, k: integer;
begin
  z[i] := true;
  j := hd[i];
  while j <> 0 do begin
    k := ds[j];
    if not z[k] then dfs1(k);
    j := nx[j];
  end;
end;

var
  c: array[0..maxn] of boolean;
  s: array[1..maxn] of integer;
  size: integer;

procedure push(i: integer);
begin
  c[size] := false;
  inc(size);
  s[size] := i;
end;

procedure pop;
begin
  dec(size);
end;

var
  r: array[1..maxn] of integer;


procedure dfs2(i, q: integer);
var
  j, k, l: integer;
begin
  r[i] := 1;
  push(i);
  j := hd[i];
  while j <> 0 do begin
    if ob[j] <> q then begin
    k := ds[j];
    case r[k] of
      0: dfs2(k, j);
      1: begin
        l := size - 1;
        while s[l] <> k do begin
          if c[l] then break;
          dec(l);
        end;
        if c[l] then begin
          writeln('NO');
          halt;
        end;
        for l := l to size - 1 do
          c[l] := true;
      end;
      2: ;
    end;
    end;
    j := nx[j];
  end;
  r[i] := 2;
  pop;
end;

var
  n, m: integer;
  i, j, k, a, aa: integer;

begin
  reset(input, 'cactus.in');
  rewrite(output, 'cactus.out');

  read(n, m);
  l := 0;
  for i := 1 to m do begin
    read(k);
    read(aa);
    for j := 1 to k - 1 do begin
      read(a);
      if (a <> aa) then begin
        add(aa, a);
        add(a, aa);
        ob[l] := l-1;
        ob[l-1] := l;
      end;
      aa := a;
    end;
  end;

  fillchar(z, sizeof(z), 0);
  dfs1(1);
  for i := 1 to n do if not z[i] then begin
    writeln('NO');
    exit;
  end;

  dfs2(1, 0);
  writeln('YES');
end.