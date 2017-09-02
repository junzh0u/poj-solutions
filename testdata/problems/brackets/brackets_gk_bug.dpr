const
    max_n = 1000;

var
    s: string;
    p: longint;
    ops: array [1..max_n] of record
        o1: longint;
        o2: longint;
        op: char;
        pr: longint
    end;
    cops: longint;

function createOp(o1, o2: longint; op: char; pr: longint): longint;
begin
    inc(cops);
    ops[cops].o1 := o1;
    ops[cops].o2 := o2;
    ops[cops].op := op;
    ops[cops].pr := pr;
    createOp := cops;
end;

function parseExpr(): longint; forward;

function parseFactor(): longint;
begin
    if s[p] = '(' then begin
        inc(p);
        parseFactor := parseExpr();
        assert(s[p] = ')');
        inc(p);
    end else begin
        assert(s[p] in ['a'..'z']);
        parseFactor := createOp(0, 0, s[p], 3);
        inc(p);
    end;
end;

function parseTerm(): longint;
var
    o1, o2: longint;
    op: char;
begin
    o1 := parseFactor();
    if (s[p] in ['*', '/']) then begin
        op := s[p];
        inc(p);
        o2 := parseTerm();
        parseTerm := createOp(o1, o2, op, 2);
    end else begin
        parseTerm := o1;
    end;
end;

function parseExpr(): longint;
var
    o1: longint;
    o2: longint;
    op: char;
begin
    o1 := parseTerm();
    if (s[p] in ['+', '-']) then begin
        op := s[p];
        inc(p);
        o2 := parseExpr();
        parseExpr := createOp(o1, o2, op, 1);
    end else begin
        parseExpr := o1;
    end;
end;

procedure print(n: longint; inverse: boolean);
begin
    if ops[n].op in ['a'..'z'] then begin
        write(ops[n].op);
    end else begin
        //write('{');
        if ops[ops[n].o1].pr > ops[n].pr then begin
            print(ops[n].o1, false);
        end else if (ops[ops[n].o1].pr < ops[n].pr) then begin
            write('(');
            print(ops[n].o1, inverse);
            write(')');
        end else begin
            print(ops[n].o1, false);
        end;

        if not inverse then begin
            write(ops[n].op);
        end else if ops[n].op = '+' then begin
            write('-') 
        end else if ops[n].op = '-' then begin
            write('+') 
        end else if ops[n].op = '*' then begin
            write('/') 
        end else if ops[n].op = '/' then begin
            write('*') 
        end;

        if ops[ops[n].o2].pr > ops[n].pr then begin
            print(ops[n].o2, false);
        end else if (ops[ops[n].o2].pr < ops[n].pr) then begin
            write('(');
            print(ops[n].o2, false);
            write(')');
        end else begin
            if (ops[n].op in ['-', '/']) then inverse := not inverse;
            print(ops[n].o2, inverse);
        end;
        //write('}');
    end;
end;

begin
    assign(input, 'brackets.in'); reset(input);
    assign(output, 'brackets.out'); rewrite(output);

    readln(s);
    s := s + '#';

    p := 1;
    print(parseExpr(), false);

    close(input);
    close(output);
end.