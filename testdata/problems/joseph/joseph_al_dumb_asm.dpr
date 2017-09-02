{$O-,Q+,R+}
const MaxN=1000000000;
      MaxK=1000000000;

procedure readr (var x:integer; a, b:integer);
begin
  assert (not seekeoln); read (x); assert ((a<=x) and (x<=b));
end;

procedure readl;
begin
  assert (eoln); assert (not eof); readln
end;

var sum:int64;
    n, k:integer;

begin
  reset (input, 'joseph.in'); rewrite (output, 'joseph.out');
  readr (n, 1, MaxN); readr (k, 1, MaxK); readl; assert (eof);
  asm
     mov ecx, n
     mov ebx, k
     xor esi, esi
     xor edi, edi
   @@loop:
     mov eax, ebx
     xor edx, edx
     div ecx
     add edi, edx
     adc esi, 0
     dec ecx
     jnz @@loop
     mov dword ptr sum, edi
     mov dword ptr sum+4, esi
   end;
   writeln (sum);
end.