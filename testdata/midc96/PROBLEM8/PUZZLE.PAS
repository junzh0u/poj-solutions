program puzzle;

  {uses crt;}

  const
    MAXPUZSIZE = 10;
    MAXSIDETYPES = 5;

  type
    TSide = -5..5;
    TPiece = record
      Image: array [ 1..25, 1..12 ] of char;
      Top, Left, Bottom, Right: TSide;
      end;
    TPuzzle = array [ 1..MAXPUZSIZE, 1..MAXPUZSIZE ] of integer;

  var
    Pieces: array [ 1..(MAXPUZSIZE * MAXPUZSIZE) ] of TPiece;
    Pile: array [ 1..(MAXPUZSIZE * MAXPUZSIZE) ] of integer;
    iPileCount: integer;
    iPuzSize: integer;

procedure RemovePieceFromPile( k: integer );

  var
    i, l: integer;
    piece: TPiece;
    j: integer;

  begin
  i := 0;
  for l := 1 to iPileCount do
    if Pile[ l ] = k then
      begin
      i := l;
      break;
      end;
  if ( i <> 0 ) then
    begin
    for j := i + 1 to iPileCount do
      Pile[ j - 1 ] := Pile[ j ];
    iPileCount := iPileCount - 1;
    end;
  end;

procedure AddPieceToPile( iPiece: integer );

  begin
  { add to bottom of pile }
  iPileCount := iPileCount + 1;
  Pile[ iPileCount ] := iPiece;
  end;

function Unscramble( var puz: TPuzzle; iCol, iRow: integer ): boolean;

  var
    iTop, iLeft, iRight, iBottom: integer;
    iPiece: integer;
    i, iCandidates: integer;
    Candidates: array [1..10] of integer;

  begin
  if ( iCol = iPuzSize + 1 ) then
    begin
    iRow := iRow + 1;
    iCol := 1;
    end;
  if ( iCol = 1 ) and ( iRow = iPuzSize+1 ) then
    begin
    Unscramble := true;
    exit;
    end;

  {gotoxy( 27, 1 );
  write( iRow:1, ' ' );
  gotoxy( 35, 1 );
  write( iCol:1, ' ' );}

  if ( iRow = 1 ) then
    iTop := 0
  else
    iTop := -Pieces[ Puz[ iCol, iRow - 1 ] ].bottom;

  if ( iCol = 1 ) then
    iLeft := 0
  else
    iLeft := -Pieces[ Puz[ iCol - 1, iRow ] ].right;

  if ( iCol = iPuzSize ) then
    iRight := 0
  else
    iRight := -1;

  if ( iRow = iPuzSize ) then
    iBottom := 0
  else
    iBottom := -1;

  iCandidates := 0;
  for i := 1 to iPileCount do
    begin
    if ( Pieces[ Pile[ i ] ].left = iLeft ) then
      if ( Pieces[ Pile[ i ] ].top = iTop ) then
        if ( iRight = -1 ) or ( Pieces[ Pile[ i ] ].right = iRight ) then
          if ( iBottom = -1 ) or ( Pieces[ Pile[ i ] ].bottom = iBottom ) then
            begin
            iCandidates := iCandidates + 1;
            Candidates[ iCandidates ] := Pile[ i ];
            end;
    end;

  { loop through all the candidates for this position }
  for i := 1 to iCandidates do
    begin
    puz[ iCol, iRow ] := Candidates[ i ];
    { take the piece we're trying off the pile so it can't be reused }
    RemovePieceFromPile( Candidates[ i ] );
    if Unscramble( puz, iCol + 1, iRow ) then
      begin
      Unscramble := true;
      exit;
      end
    else
      { This piece didn't work, so put it back on the pile }
      AddPieceToPile( Candidates[ i ] );
    end;

  Unscramble := false;
  end;

  var
    puz: TPuzzle;
    fin, fout: text;
    sLine: string;
    i, j, k, l: integer;
    iPieceHeight: integer;
    iPieceWidth: integer;
    piece: TPiece;

  begin
  assign( fin, 'puzzle.in' );
  reset( fin );
  assign( fout, 'puzzle.out' );
  rewrite( fout );

  iPileCount := 0;
  readln( fin, iPuzSize, iPieceHeight, iPieceWidth );

  for k := 1 to iPuzSize * iPuzSize do
    begin
    for i := 1 to iPieceHeight do
      begin
      readln( fin, sLine );
      for j := 1 to length( sLine ) do
        piece.Image[ j, i ] := sLine[ j ];
      end;
    readln( fin, piece.top, piece.left, piece.bottom, piece.right );
    readln( fin );
    iPileCount := iPileCount + 1;
    Pile[ iPileCount ] := iPileCount;
    Pieces[ iPileCount ] := piece;
    end;

  {clrscr;
  gotoxy( 1, 1 );
  writeln( 'Unscrambling puzzle:  row     col ' );}
  if Unscramble( puz, 1, 1 ) then
    begin
    for j := 1 to iPuzSize do
      begin
      for k := 1 to iPieceHeight do
        begin
        for i := 1 to iPuzSize do
          begin
          for l := 1 to iPieceWidth do
            write( fout, Pieces[ puz[ i, j ] ].Image[ l, k ] );
          end;
        writeln( fout );
        end;
      end;
    end
  else
    Writeln( fout, 'Solution not found' );

  Close( fin );
  Close( fout );
  end.