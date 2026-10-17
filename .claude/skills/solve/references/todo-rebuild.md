# Rebuilding TODO

`TODO` is the backlog: the problems POJ says user `150014` has not solved and that are not parked, ranked by global solve count, most-solved first. Rebuilding it is a parent/user task, run rarely.

The source is the form at `http://poj.org/moreproblem`: a POST of `user_ids=150014&submit=Submit` lists exactly the problems that user has not solved, each with its global solve count. The `user_ids` field is not optional — posted empty, the form returns every problem on the site, solved ones included, and nothing on the page says so. Run it from the logged-in pinned Chrome, since `userstatus` returns 403 to non-browser requests: load `http://poj.org/` (navigating the tab straight to `moreproblem` or other non-homepage URLs can land on a Chrome error page) and `fetch` the POST from there.

The rows are not an HTML table in the response but inline script calls, one per problem: `s(<id>,"<title>",<solved>,"<source>")`. Extract `<id> <solved>` pairs with a regex over the response text — `\bs\((\d+),"(?:[^"\\]|\\.)*",(\d+),` — and check the match count against the number of `s(<digits>,` calls. Re-render them as lines inside a `<pre>` that replaces the body before extracting the page text, rather than returning them from the script call, which can be truncated.

Then sort by solve count descending, subtract every id that has an `attempts/<id>.md` — POJ still lists parked problems as unsolved, and without the subtraction every rebuild would silently un-park them — and write out the ids alone.

Compare the other direction too: a parked id that POJ no longer lists as unsolved was accepted outside the solve workflow. Its source is at `showsource?solution_id=<run id>` (find the run with `scripts/status-via-curl <id>`); archive it and commit with `commit-accept`, which deletes the stale note.
