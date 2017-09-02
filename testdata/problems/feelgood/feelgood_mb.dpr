{$apptype CONSOLE}

uses Math;

const
    IN_FILE = 'feelgood.in';
    OUT_FILE = 'feelgood.out';

    MAX_N = 100000;

var
    n : integer;
    a : array[1..MAX_N] of integer;
    perm : array[1..MAX_N] of integer;
    root : array[1..MAX_N] of integer;
    min, max : array[1..MAX_N] of integer;
    sum : array[1..MAX_N] of int64;
    ansvalue : int64;
    ansl, ansr : integer;
                
procedure ReadInput;
var
    i : integer;
    
begin
    reset(input, IN_FILE);
    read(n);
    for i := 1 to n do
        read(a[i]);
    close(input);   
end;

procedure WriteOutput;
begin
    rewrite(output, OUT_FILE);
    writeln(ansvalue);
    writeln(ansl, ' ', ansr);
    close(output);
end;

procedure Sort;

    procedure QSort(l, r : integer);
    var
        i, j : integer;
        x, t : integer;

    begin
        i := l;
        j := r;
        x := a[perm[l + random(r - l)]];
        while i <= j do begin
            while a[perm[i]] < x  do
                inc(i);
            while x < a[perm[j]] do
                dec(j);
            if i <= j then begin
                t := perm[i];
                perm[i] := perm[j];
                perm[j] := t;
                inc(i);
                dec(j);
            end;
        end;
        if l < j then
            QSort(l, j);
        if i < r then
            QSort(i, r);
    end;
    
var
	i : integer;

begin
	for i := 1 to n do
    	perm[i] := i;
    QSort(1, n);
end;

procedure InitDSU;
begin
    fillchar(root, sizeof(root), -1);
end;

procedure MakeSet(i, value : integer);
begin
    root[i] := i;
    sum[i] := value;
    min[i] := i;
    max[i] := i;
end;

function GetRoot(i : integer) : integer;
begin
    if (root[i] = - 1) or (root[i] = i) then begin
        result := root[i];
        exit;
    end;
    
    root[i] := GetRoot(root[i]);
    result := root[i];
end;

function GetSum(i : integer) : int64;
begin
    result := sum[GetRoot(i)];
end;

function GetMin(i : integer) : integer;
begin
    result := min[GetRoot(i)];
end;

function GetMax(i : integer) : integer;
begin
    result := max[GetRoot(i)];
end;

procedure Unite(i, j : integer);
var
    tmp : integer;
    
begin
    i := GetRoot(i);
    j := GetRoot(j);
    if random < 0.5 then begin
        tmp := i;
        i := j;
        j := tmp;
    end;
    root[j] := i;
    inc(sum[i], sum[j]);
    min[i] := math.min(min[i], min[j]);
    max[i] := math.max(max[i], max[j]);
end;
    
procedure Solve;
var 
    i, j : integer;
    curvalue : int64;
    
begin
    Sort;
    InitDSU;
    ansvalue := -1;
    for i := n downto 1 do begin
        j := perm[i];
        MakeSet(j, a[j]);
        if (j > 1) and (root[j - 1] <> -1) then
            Unite(j - 1, j);
        if (j < n) and (root[j + 1] <> -1) then
            Unite(j, j + 1);
        curvalue := int64(a[j]) * GetSum(j);
        if curvalue > ansvalue then begin
            ansvalue := curvalue;
            ansl := GetMin(j);
            ansr := GetMax(j);
        end;
    end;
end;

begin
    ReadInput;
    Solve;
    WriteOutput;
end.
