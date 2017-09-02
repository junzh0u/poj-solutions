program fctest;

  var
    sIn: string;

  begin
  readln;
  readln( sIn );
  if ( sIn = 'FC: no differences encountered' ) then
    halt( 0 )
  else
    halt( 1 );
  end.