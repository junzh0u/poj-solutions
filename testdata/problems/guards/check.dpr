uses Testlib, SysUtils;

const
	MAX_V = 10000;
	
var
	alpha, beta, gamma : integer;
	px, py, pz, pt : integer;
	jcost, pcost : integer;
	
begin
	alpha := inf.ReadInteger;
	beta  := inf.ReadInteger;
	gamma := inf.ReadInteger;
	
	px := ouf.ReadInteger;
	if (px < 0) or (px > MAX_V) then
		Quit(_WA, 'x is out of range');
	if px mod 3 <> 0 then
		Quit(_WA, 'x is not divisible by 3');
	py := ouf.ReadInteger;
	if (py < 0) or (py > MAX_V) then
		Quit(_WA, 'y is out of range');
	pz := ouf.ReadInteger;
	if (pz < 0) or (pz > MAX_V) then
		Quit(_WA, 'z is out of range');
	if pz mod 4 <> 0 then
		Quit(_WA, 'z is not divisible by 4');
	pt := ouf.ReadInteger;
	if (pt < 0) or (pt > MAX_V) then
		Quit(_WA, 't is out of range');
	if pt mod 5 <> 0 then
		Quit(_WA, 't is not divisible by 5');
		
	pcost := px + py + pz + pt;
	
	px := px div 3;
	pz := pz div 4;
	pt := pt div 5;
		
	if px + py + 2 * pz + 3 * pt < alpha then
		Quit(_WA, 'Workdays daylight requirement is not satisfied');	
	if px + 2 * pz < beta then
		Quit(_WA, 'Weekends daylight requirement is not satisfied');	
	if px + pz + 2 * pt < gamma then
		Quit(_WA, 'Nighttime requirement is not satisfied');	

	jcost := ans.ReadInteger + ans.ReadInteger + ans.ReadInteger + ans.ReadInteger;

	if pcost > jcost then
		Quit(_WA, format('Not an optimal solution: %d > %d', [pcost, jcost]));	
	if pcost < jcost then
		Quit(_FAIL, format('Better solution that jury: %d < %d', [pcost, jcost]));	
	Quit(_OK, format('%d guard(s)', [pcost]));	
end.
