{ X, Y }

program puzgen;

  uses crt;

  const
    MAXPUZSIZE = 10;
    MAXSIDETYPES = 5;
    ROWDIFF = 1;
    COLDIFF = 2;

  type
    TSide = -5..5;
    TPiece = record
      Image: array [ 1..25, 1..12 ] of char;
      Top, Left, Bottom, Right: TSide;
      end;
    TPuzzle = array [ 1..MAXPUZSIZE, 1..MAXPUZSIZE ] of TPiece;

  var
    iPuzSize: integer;

function ArePiecesTheSame( var piece1, piece2: TPiece ): boolean;

  begin
  if ( piece1.top = piece2.top )
    and ( piece1.left = piece2.left )
    and ( piece1.bottom = piece2.bottom )
    and ( piece1.right = piece2.right ) then
    ArePiecesTheSame := true
  else
    ArePiecesTheSame := false;
  end;

function IsInPuzzle( var puz: TPuzzle; piece: TPiece; iMaxCol, iMaxRow: integer ): boolean;

  var
    iRow, iCol: integer;

  begin
  IsInPuzzle := false;
  for iRow := 1 to iMaxRow do
    for iCol := 1 to iMaxCol do
      if ( iRow <> iMaxRow ) and ( iCol <> iMaxCol ) then
        if ( ArePiecesTheSame( puz[ iCol, iRow ], piece ) ) then
          begin
          IsInPuzzle := true;
          exit;
          end;
  end;

function RandSign: integer;

  begin
  if ( trunc( random( 2 ) ) = 1 ) then
    RandSign := 1
  else
    RandSign := -1;
  end;

procedure AssignSides( var Puzzle: TPuzzle );

  var
    iCol, iRow: integer;
    piece: TPiece;
    iPic: longint;

  begin
  clrscr;
  gotoxy( 1, 1 );
  write( 'Generating puzzle:  row      col  ' );
  for iRow := 1 to iPuzSize do
    begin
    gotoxy( 25, 1 ); write( iRow:1, ' ' );
    for iCol := 1 to iPuzSize do
      begin
      piece := Puzzle[ iCol, iRow ];
      gotoxy( 34, 1 ); write( iCol:1, ' ' );
      while ( true ) do
        begin
        if ( iRow = 1 ) then
          piece.top := 0
        else
          piece.top := -Puzzle[ iCol, iRow - 1 ].bottom;

        if ( iCol = 1 ) then
          piece.left := 0
        else
          piece.left := -Puzzle[ iCol - 1, iRow ].right;

        if ( iRow = iPuzSize ) then
          piece.bottom := 0
        else
          piece.bottom := trunc( random( MAXSIDETYPES ) + 1 ) * RandSign;

        if ( iCol = iPuzSize ) then
          piece.right := 0
        else
          piece.right := trunc( random( MAXSIDETYPES ) + 1 ) * RandSign;

        if not IsInPuzzle( Puzzle, piece, iCol, iRow ) then
          begin
          Puzzle[ iCol, iRow ] := piece;
          break;
          end;
        end;
      end;
    end;
  end;

procedure ScramblePuzzle( var puz: TPuzzle );

  var
    iRow, iCol: integer;
    iRandRow, iRandCol: integer;
    piece: TPiece;

  begin
  for iRow := 1 to iPuzSize do
    for iCol := 1 to iPuzSize do
      begin
      piece := puz[ iCol, iRow ];
      iRandRow := trunc( random( iPuzSize ) ) + 1;
      iRandCol := trunc( random( iPuzSize ) ) + 1;
      puz[ iCol, iRow ] := puz[ iRandCol, iRandRow ];
      puz[ iRandCol, iRandRow ] := piece;
      end;
  end;

  var
    puz: TPuzzle;
    fin: text;
    fout: text;
    sLine: string;
    iHeight: integer;
    iWidth: integer;
    iPieceWidth: integer;
    iPieceHeight: integer;
    i, j, k, l, iPuzX, iPuzY: integer;
    iLeftDiff, iRightDiff, iBottomDiff, iTopDiff: integer;
    sFileIn: string;

  begin
  write( 'What is the puzzle file? ' );
  readln( sFileIn );
  write( 'What is the puzzle size? ' );
  readln( iPuzSize );

  randomize;
  clrscr;

  assign( fin, sFileIn );
  reset( fin );
  assign( fout, 'puzzle.in' );
  rewrite( fout );

  iHeight := 1;
  sLine := '';
  while ( sLine = '' ) and ( not eof( fin ) ) do
    readln( fin, sLine );

  iWidth := length( sLine );
  while ( sLine <> '' ) and ( not eof( fin ) ) do
    begin
    iHeight := iHeight + 1;
    readln( fin, sLine );
    end;

  {writeln( fout, iWidth:1, ' ', iHeight:1 );}

  reset( fin );
  sLine := '';
  while ( sLine = '' ) and ( not eof( fin ) ) do
    readln( fin, sLine );

  { okay, now we're ready to read the puzzle in and generate pieces }
  iPieceWidth := iWidth div iPuzSize;
  iPieceHeight := iHeight div iPuzSize;

  i := 1;
  j := 1;
  iPuzX := 1;
  iPuzY := 1;
  k := 1;
  while ( sLine <> '' ) do
    begin
    iPuzX := 1;
    l := 1;
    for j := 1 to iWidth do
      begin
      puz[ iPuzX, iPuzY ].Image[ l, k ] := sLine[ j ];
      if ( l = iPieceWidth ) then
        begin
        iPuzX := iPuzX + 1;
        l := 1;
        end
      else
        l := l + 1;
      end;
    i := i + 1;
    if ( k = iPieceHeight ) then
      begin
      iPuzY := iPuzY + 1;
      k := 1;
      end
    else
      k := k + 1;
    if ( eof( fin ) ) then
      break;
    readln( fin, sLine );
    end;

  AssignSides( puz );
  ScramblePuzzle( puz );

  writeln( fout, iPuzSize, ' ', iPieceHeight, ' ', iPieceWidth );
  for i := 1 to iPuzSize do
    begin
    for j := 1 to iPuzSize do
      begin
      for k := 1 to iPieceHeight do
        begin
        for l := 1 to iPieceWidth do
          write( fout, puz[ j, i ].Image[ l, k ] );
        writeln( fout );
        end;
      writeln( fout, puz[ j, i ].Top:1, ' ', puz[ j, i ].Left:1, ' ',
        puz[ j, i ].Bottom:1, ' ', puz[ j, i ].Right:1 );
      writeln( fout );
      end;
    end;

  close( fin );
  close( fout );
  end.