# POJ Solutions — Agent Instructions

An archive of accepted [POJ](http://poj.org) submissions, one directory per problem ID. Nothing is built or tested by the repo itself; the judge is the test suite.

## Layout

- `<id>/<runId>_AC_<time>MS_<memory>K.<ext>` — one file per accepted submission, named from its POJ status row (time before memory). Several files in a directory means the problem was solved more than once, e.g. in another language.
- `<id>/test_data/<id>.in` / `.out` — sample data, only where it was worth keeping.
- `<id>/tags/<tag>` — empty marker files used as tags; manage them with `./tag.sh` (`tag.sh` lists all tags, `tag.sh <id>` lists a problem's, `tag.sh <id> <tag>` adds one).
- `TODO` — the backlog, nothing but problem ids one per line: those POJ says user `150014` has not solved, ranked by global solve count, most-solved first.
- `_attempts_/<id>.md` — the write-up left behind by a problem that was tried and not accepted: what was understood, what was submitted, the verdicts, and where it stalled. Its existence is what marks the problem as parked.

## Picking what to solve

"Solve N problems" means: take the **top N ids of `TODO`** — the list is already ordered so the most-solved (roughly easiest and best-documented) come first — skipping any that already have an `_attempts_/<id>.md`, and work them. A specific problem id the user names overrides both the ordering and the skip.

To rebuild `TODO`, submit the form at `http://poj.org/moreproblem`: it lists exactly the problems the logged-in user has not solved, each with its global solve count. Only the `ID` and `Solved` columns are wanted, so re-render the table in the page as `<id> <solved>` lines inside a `<pre>` and replace the body with it before calling `get_page_text` — the page as served runs to ~150 KB and would be cut off at the tool's 50 KB default, while two numeric columns fit comfortably. Then sort by solve count descending and write out the ids alone.

## Solving a problem end to end

The whole loop runs through the user's Chrome session, which is already logged in as user `150014`.

### 1. Read the statement

`http://poj.org/problem?id=<id>` — `get_page_text` returns the whole statement including the samples and the limits. Note the time and memory limits; they decide how much the algorithm can afford.

**Exponents lose their superscript.** A bound written `2^54` on the page arrives as `254`, and `10^9` as `109`, because the markup carries the exponent and the text does not. Read any implausible constant that way — a limit of "254" on a problem about factoring large numbers is 2^54 — and check it against the samples and the memory limit before sizing anything to it. Getting this wrong invents a range the problem never had: one solve here chased a 66-second worst case that turned out to sit outside the real bound entirely.

### 2. Write and test locally

Work in the scratchpad, not the repo — only the accepted source gets committed.

- Target **C++98**: POJ's compilers are ancient, so no `auto`, no range-`for`, no `<cstdint>`. Build with `g++ -std=c++98 -pedantic -Wall -Wextra -O2`.
- Reproduce the statement's sample first and `diff` against the expected output verbatim.
- Then go past the sample: a stress case near the stated input limits (`/usr/share/dict/words` is a handy source of real words), the degenerate inputs, and — where the answer is a classification rather than a value — a randomized differential test against a brute-force reference on small instances. POJ reports nothing but the verdict, so anything not caught locally costs a blind submission.
- Judge machines are much slower than a modern laptop; leave several times the stated limit as headroom.

### 3. Submit

The submit form is `document.forms[2]` on `http://poj.org/submit?problem_id=<id>`: fields `problem_id`, `language` (`0=G++ 1=GCC 2=Java 3=Pascal 4=C++ 5=C 6=Fortran`), `source`, and a hidden `encoded=1` because `onsubmit` base64-encodes the textarea. So plant the **plain** source and submit by clicking the real button — `form.submit()` skips `onsubmit` and would post unencoded source under `encoded=1`.

Getting the source into the textarea: gzip+base64 it (`gzip -9nc x.cpp | base64 | tr -d '\n'`) and inflate it inside the page, which keeps the payload about a third of the source:

```js
const bin = Uint8Array.from(atob(B), c => c.charCodeAt(0));
const src = await new Response(new Blob([bin]).stream().pipeThrough(new DecompressionStream('gzip'))).text();
const f = document.forms[2];
f.language.value = '4';
f.source.value = src;
f.elements['submit'].click();
```

Do the whole thing — plant, verify, click — in a **single** JS call, guarded by `f.problem_id.value === '<id>'` and aborting if it does not match. With several agents in one browser a sibling can re-navigate your tab between two calls, and then the click submits *their* form: nothing appears under your problem and the source you planted is gone. The guard is a line long and the failure mode is otherwise silent.

Check `f.source.value.length` against the file before clicking, and `Code Length` in the status row after — that is the proof the source arrived intact. Serving the file from a local HTTP server and `fetch`ing it from the page does **not** work: Chrome's private-network access blocks a public page from reaching `127.0.0.1`.

### 4. Check the verdict

`http://poj.org/status?problem_id=<id>&user_id=150014`, first row of `table.a`, cells `Run ID | User | Problem | Result | Memory | Time | Language | Code Length | Submit Time`. Poll until the result leaves `Waiting` / `Compiling` / `Running & Judging`.

A clean click is not evidence the submission landed, either. POJ drops a submission arriving within ~10s of any other — from a sibling agent as much as from you — and says nothing: the guard passes, the source length checks out, and no status row ever appears. So confirm a **new** row before believing anything, and a submission that produced no row is not an attempt and must not count against the cap.

A browser error *after* the submit click is not evidence the submission was lost — with several agents in one browser, a sibling's navigation can take the tab out from under you, so the JS that was going to confirm the click fails even though the post went through. Always look at the status page before resubmitting, or phantom retries eat the submission cap.

### 5. Iterate

Report each verdict as it arrives rather than silently resubmitting.

- **Compile Error** — a C++98 slip; check `Compile Error` on the status page for the message, or retry under language `0` (G++) which is a different compiler from `4` (C++).
- **Wrong Answer** — re-read the statement for the case that was missed, reproduce it locally, then resubmit.
- **Time Limit Exceeded** — profile the local stress case; POJ is slow, so an algorithmic fix usually beats micro-optimisation.
- **Runtime Error** — usually an out-of-bounds index or recursion depth, both reproducible locally under `-fsanitize=address,undefined`.

### 6. Commit

Only after **Accepted**. Copy the exact accepted source to `<id>/<runId>_AC_<time>MS_<mem>K.<ext>`, prefixed with a `// POJ <id> - <Title>` comment block explaining the approach and any ambiguity in the statement.

Commit subject is `<id> <Title>` — plain, no Conventional Commits prefix. The body explains the algorithm and the decisions behind it, not the code. One problem per commit.

Do not push unless asked.

## Solving several at once

Spawn **one agent per problem**, all in a single message so they run concurrently, and keep the statements, the test scaffolding, and the failed attempts out of the main context — the parent only ever sees a verdict per problem.

Each agent owns steps 1–5 for its problem, ending at **Accepted**, and writes the annotated source to `<id>/<runId>_AC_<time>MS_<mem>K.<ext>`. Two things it must **not** do, because they are shared state:

- **Do not commit.** Concurrent `git add`/`git commit` in one worktree races on `index.lock` and can sweep another agent's files into the wrong commit. The parent commits afterwards, one problem per commit, in TODO order.
- **Do not edit `TODO`.** The parent strikes each accepted problem's id once its commit lands.

Each agent should also open its own tab (`tabs_create_mcp`) and close it when done. If the browser tools report that several Chrome browsers are connected and demand a choice, an agent must **not** stop to ask — it cannot reach the user, and the whole fan-out stalls behind it. Any of them works: `select_browser` with any deviceId and carry on.

POJ rejects submissions that arrive within ~10s of the previous one, so an agent that gets turned away should wait and resubmit rather than treat it as a verdict. Past three agents this stops being rare — they finish local testing at similar times and collide, each collision costing a poll-and-retry cycle. Give each agent a **submit slot**: agent k waits 25*k seconds before its first click. The stagger costs one agent a minute or two and buys back more than that in avoided collisions.

Tell the agent how to wait, though, or the instruction backfires: an agent that starts a *background* sleep and ends its turn to await the notification never wakes, and the task dies silently mid-run with no report. Waits go in the foreground — `python3 -c "import time; time.sleep(N)"` — and the agent continues in the same turn.

State it as the general rule, **never end a turn waiting for anything**, because the stall does not only come from the stagger: an agent that backgrounds a long differential test and ends its turn to await the result dies exactly the same way. A test too slow to sit through in the foreground should be made smaller — a few hundred trials whose result is actually collected beat thousands that are not. When resuming an agent that stalled, have it read the status page first: it may already have a submission judged, and resubmitting on top of it wastes an attempt.

Cap an agent at **5 submissions** for its problem. Iterating past that means the approach is wrong rather than buggy, and each blind retry costs judge time — better to hand the problem back than to grind. An agent that hits the cap, or that gets stuck before submitting at all, reports the last verdict, what it tried, and what it thinks the problem actually needs.

An agent that dies to a usage or rate limit has not attempted anything — no write-up, no park, and the id keeps its place at the top of `TODO`. Wait for the reset and rerun it.

The parent turns that report into `_attempts_/<id>.md` — the reading of the statement, the algorithm tried, the verdict of each submission, and the failing case if one was found — and commits it as `<id> attempt notes`. The id stays where it is in `TODO`, but a problem carrying an attempt file is skipped when picking the next N; solve it only when asked for by id, and hand the file to the agent so it starts where the last one stopped instead of re-deriving the dead end.
