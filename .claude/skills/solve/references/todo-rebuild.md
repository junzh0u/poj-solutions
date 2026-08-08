# Rebuilding TODO

`TODO` is the backlog: the problems POJ says user `150014` has not solved and that are not parked, ranked by global solve count, most-solved first. Rebuilding it is a parent/user task, run rarely.

Submit the form at `http://poj.org/moreproblem`: it lists exactly the problems the logged-in user has not solved, each with its global solve count. This is the one step besides submitting that must run in the browser — the form posts no user field, so identity comes only from the session cookie, and the curl detour around it is closed too (`userstatus` returns 403 to non-browser requests).

Only the `ID` and `Solved` columns are wanted, and the page as served runs to ~150 KB — more than text extraction returns whole, since `get_page_text` truncates at 50 KB by default. So re-render the table in the page as `<id> <solved>` lines inside a `<pre>` and replace the body with it before extracting the text.

Then sort by solve count descending, subtract every id that has an `attempts/<id>.md` — POJ still lists parked problems as unsolved, and without the subtraction every rebuild would silently un-park them — and write out the ids alone.
