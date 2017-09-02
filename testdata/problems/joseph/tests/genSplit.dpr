uses SysUtils;
var i, cnt:integer;
    s:string;
begin
  reset (input, 'tests.lst');
  cnt:=1;
  repeat
    str (cnt, s); while length (s)<2 do s:='0'+s;
    rewrite (output, s); 
    readln (s); writeln (s);
    close (output);
    inc (cnt);
  until eof;
end.