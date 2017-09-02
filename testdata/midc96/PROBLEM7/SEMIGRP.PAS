Program Semigrp(input,output);
const
  OUTPUT_FILENAME = 'semigrp.out';
  INPUT_FILENAME  = 'semigrp.in';
var
  infile,outfile: text;
  i, j, k, n: integer;
  set_no: integer;
  s: array [1..26] of char;
  op: array ['a'..'z','a'..'z'] of char;

function element(c:char):boolean;
  var i: integer;
      done: boolean;
  begin
     element := false;
     done := false;
     i := 1;
     while (i <= n) and (not done) do begin
        if (s[i] = c) then begin
           element := true;
           done := true;
        end;
        i := i + 1;
     end;
  end;


function closed(var bad_i,bad_j:integer):boolean;
  var i,j: integer;
      done: boolean;
  begin
    closed := true;
    done := false;
    i := 1;
    while (i <= n) and (not done) do begin
       j := 1;
       while (j <= n) and (not done) do begin
         if not element(op[s[i],s[j]]) then begin
            bad_i := i;
            bad_j := j;
            closed := false;
            done := true;
         end;
         j := j + 1;
       end;
       i := i + 1;
    end;
  end;

function associative(var bad_i, bad_j, bad_k: integer):boolean;
   var
      i,j,k: integer;
      done : boolean;
   begin
      associative := true;
      done := false;
      i := 1;
      while (i <= n) and (not done) do begin
         j := 1;
         while (j <= n) and (not done) do begin
            k := 1;
            while (k <= n) and (not done) do begin
               if (op[op[s[i],s[j]],s[k]]) <> (op[s[i],op[s[j],s[k]]]) then begin
                 bad_i := i;
                 bad_j := j;
                 bad_k := k;
                 done  := true;
                 associative := false;
               end;
               k := k + 1;
            end;
            j := j + 1;
         end;
         i := i+ 1;
      end;
   end;

function commutative(var bad_i, bad_j: integer):boolean;
   var
      i,j: integer;
      done : boolean;
   begin
      commutative := true;
      done := false;
      i := 1;
      while (i <= n) and (not done) do begin
         j := 1;
         while (j <= n) and (not done) do begin
            if (op[s[i],s[j]] <> op[s[j],s[i]]) then begin
               bad_i := i;
               bad_j := j;
               done  := true;
               commutative := false;
            end;
            j := j + 1;
         end;
         i := i + 1;
      end;
   end;
   
begin
   assign(infile,INPUT_FILENAME);
   reset(infile);
   assign(outfile,OUTPUT_FILENAME);
   rewrite(outfile);
   set_no := 0;
   while not eof(infile) do begin
      readln(infile,n);
      if (n > 0) then begin
        set_no := set_no + 1;
        for i:= 1 to n do read(infile,s[i]);
        readln(infile);
        for i := 1 to n do begin
          for j := 1 to n do begin
             read(infile,op[s[i],s[j]]);
          end;
          readln(infile);
        end;

        write(outfile,'S = {');
        for i := 1 to n-1 do write(outfile,s[i],',');
        writeln(outfile,s[n],'}');
        write(outfile,' #|');
        for i := 1 to n do write(outfile,s[i]);
        writeln(outfile);
        write(outfile,' -+');
        for i := 1 to n do write(outfile,'-');
        writeln(outfile);
        for i := 1 to n do begin
           write(outfile,' ',s[i],'|');
           for j := 1 to n do begin
              write(outfile,op[s[i],s[j]]);
           end;
        writeln(outfile);
        end;
        writeln(outfile);

        if not closed(i,j)
        then begin
           write   (outfile,'NOT A SEMIGROUP: ');
           write   (outfile,s[i]:1,'#',s[j]:1,' = ',op[s[i],s[j]]:1);
           writeln (outfile,'  WHICH IS NOT AN ELEMENT OF THE SET')
        end else begin
           if not associative(i,j,k)
           then begin
              write   (outfile,'NOT A SEMIGROUP: ');
              write   (outfile,'(',s[i]:1,'#',s[j]:1,')#',s[k]:1, ' IS NOT EQUAL TO ');
              writeln (outfile,s[i]:1,'#(',s[j]:1,'#',s[k]:1,')');
           end else begin
              if not commutative(i,j)
              then begin
                 write   (outfile,'SEMIGROUP BUT NOT COMMUTATIVE');
                 write   (outfile,'  (',s[i]:1,'#',s[j]:1,' IS NOT EQUAL TO ');
                 writeln  (outfile,s[j]:1,'#',s[i]:1,')');
              end  else
                 writeln (outfile,'COMMUTATIVE SEMIGROUP');
           end;
        end;

        writeln(outfile,'------------------------------');
        writeln(outfile);
      end;
   end;
   close(infile);
   close(outfile);
end.
