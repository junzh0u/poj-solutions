{$apptype CONSOLE}
{$assertions ON}
{$O-,R+}

const
	IN_FILE = 'brackets.in';
	OUT_FILE = 'brackets.out';

type
	PNode = ^TNode;

	TNode = record
		left, right : PNode;
		op : char;
	end;

var
	expr : string;

function MakeNode(left, right : PNode; op : char) : PNode;
begin
	new(result);
	result.left := left;
	result.right := right;
	result.op := op;
end;
	
function ParseExpr(s : string; var pos : integer) : PNode; forward;

function ParseFactor(s : string; var pos : integer) : PNode;
begin
	if s[pos] = '(' then begin
		inc(pos);
		result := ParseExpr(s, pos);
		assert(s[pos] = ')');
		inc(pos);
	end else if s[pos] in ['a'..'z'] then begin
		result := MakeNode(nil, nil, s[pos]);
        inc(pos);
	end else
		assert(false);
end;

function ParseTerm(s : string; var pos : integer) : PNode;
var
	op : char;
	
begin
	result := ParseFactor(s, pos);
	while s[pos] in ['*', '/'] do begin
		op := s[pos];
		inc(pos);
		result := MakeNode(result, ParseFactor(s, pos), op);
	end;
end;

function ParseExpr(s : string; var pos : integer) : PNode;
var
	op : char;

begin
	result := ParseTerm(s, pos);
	while s[pos] in ['+', '-'] do begin
		op := s[pos];
		inc(pos);
		result := MakeNode(result, ParseTerm(s, pos), op);
	end;
end;

function Parse(s : string) : PNode;
var
	pos : integer;

begin
	pos := 1;
	s := s + '$';
	result := ParseExpr(s, pos);
	assert(s[pos] = '$');
end;

function GetPri(op : char) : integer;
begin
	case op of
        '+', '-': result := 0;
        '*', '/': result := 1;
    	'a'..'z': result := 2;
        else assert(false);
    end;
end;

function GetInverse(op : char) : char;
begin
	case op of
        '+' : result := '-';
        '-' : result := '+';
        '*' : result := '/';
        '/' : result := '*';
        else assert(false);
    end;
end;

function IsInversing(op : char) : boolean;
begin
	result := op in ['-', '/'];
end;

procedure Print(node : PNode; inverse : boolean);
var
	linverse, rinverse : boolean;
    
begin
	case node^.op of
    	'a'..'z':
        	write(node^.op);

        '+', '-', '*', '/' : begin
        	linverse := inverse;
			if GetPri(node^.left^.op) < GetPri(node^.op) then
            	write('(');
			if GetPri(node^.left^.op) <> GetPri(node^.op) then
            	linverse := false;
            Print(node^.left, linverse);
			if GetPri(node^.left^.op) < GetPri(node^.op) then
            	write(')');

        	if inverse then
            	write(GetInverse(node^.op))
            else
            	write(node^.op);

			rinverse := inverse xor IsInversing(node^.op);

			if GetPri(node^.right^.op) < GetPri(node^.op) then
            	write('(');
			if GetPri(node^.right^.op) <> GetPri(node^.op) then
            	rinverse := false;
            Print(node^.right, rinverse);
			if GetPri(node^.right^.op) < GetPri(node^.op) then
            	write(')');
        end;

        else assert(false);
    end;
end;

procedure ReadInput;
begin
	reset(input, IN_FILE);
	readln(expr);
	close(input);
end;

procedure WriteOutput;
begin
	rewrite(output, OUT_FILE);
	Print(Parse(expr), false);
	close(output);
end;

begin
	ReadInput;
	WriteOutput;
end.
