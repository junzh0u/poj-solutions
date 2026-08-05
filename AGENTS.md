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

To rebuild `TODO`, submit the form at `http://poj.org/moreproblem`: it lists exactly the problems the logged-in user has not solved, each with its global solve count. This is the one step besides submitting that must run in the browser — the form posts no user field, so identity comes only from the session cookie, and the curl detour around it is closed too (`userstatus` returns 403 to non-browser requests). Only the `ID` and `Solved` columns are wanted, and the page as served runs to ~150 KB — more than text extraction returns whole — so re-render the table in the page as `<id> <solved>` lines inside a `<pre>` and replace the body with it before extracting the text. Then sort by solve count descending and write out the ids alone.

## Solving a problem end to end

A solve always runs in a **subagent**, even for a single problem — the statement, the test scaffolding, and the failed attempts stay out of the parent's context, which only ever sees the verdict. The agent owns steps 1–5, ending at **Accepted**, and writes the annotated source file; the commit and the `TODO` strike-off are the parent's (step 6), because both are shared state — concurrent `git add`/`git commit` in one worktree races on `index.lock` and can sweep a sibling's files into the wrong commit.

**Never end a turn waiting for anything.** An agent that backgrounds a sleep or a long differential test and ends its turn to await the notification never wakes, and the task dies silently mid-run with no report. Waits go in the foreground — `python3 -c "import time; time.sleep(N)"` — and the agent continues in the same turn. A test too slow to sit through in the foreground should be made smaller: a few hundred trials whose result is actually collected beat thousands that are not. When resuming an agent that stalled this way, have it read the status page first — it may already have a submission judged, and resubmitting on top of it wastes an attempt.

Only the submit itself needs the user's Chrome session (logged in as user `150014`). Everything else — the statement, the samples, the verdict — is plain `curl` with no login, so the browser is touched exactly once per submission: open a fresh tab when ready to submit and close it right after the click is confirmed. A shorter browser window means fewer chances for a sibling agent to re-navigate the tab, but the in-call guard in step 3 stays regardless.

### 1. Read the statement

`curl -s 'http://poj.org/problem?id=<id>'` — the raw HTML carries the whole statement, samples, and limits, and needs no login. Note the time and memory limits; they decide how much the algorithm can afford.

Read the HTML itself, not a text extraction — two documented misreadings here were both extraction artifacts that the raw markup makes unambiguous:

**Exponents.** Text extraction drops superscripts: a bound written `2<sup>54</sup>` in the HTML arrives as `254`, and `10^9` as `109`. In the raw HTML the `<sup>` tag is right there (1811's is `2<sup>54</sup>`). Getting this wrong invents a range the problem never had: one solve here chased a 66-second worst case that turned out to sit outside the real bound entirely. Not every odd constant is a mangled exponent, though: 2409's `cs<=32` really is the product c*s, which the statement's own gloss settles.

**Leading whitespace.** 1523's expected output indents every line by two spaces, visible in the sample's raw `<pre>` but not in extracted text — missing it turns into a Presentation Error that reads like a wrong answer. In the curl output the bytes are exact (samples also carry `\r\n` line endings; strip them when diffing).

### 2. Write and test locally

Work in the scratchpad, not the repo — only the accepted source gets committed.

- Target **C++98**: POJ's compilers are ancient, so no `auto`, no range-`for`, no `<cstdint>`. Build with `g++ -std=c++98 -pedantic -Wall -Wextra -O2`.
- Reproduce the statement's sample first and `diff` against the expected output verbatim.
- Then go past the sample: a stress case near the stated input limits (`/usr/share/dict/words` is a handy source of real words), the degenerate inputs, and — where the answer is a classification rather than a value — a randomized differential test against a brute-force reference on small instances. POJ reports nothing but the verdict, so anything not caught locally costs a blind submission.
- Judge machines are much slower than a modern laptop; leave several times the stated limit as headroom.

Before the first submission, finalize the source's leading comment block: `// POJ <id> - <Title>`, `// Model: <model-id>`, the approach, and any statement ambiguity. Use the exact model identifier that authored the solution, then compile and test this annotated file again. Every submission must plant this exact file so the Accepted source can be archived byte-for-byte without post-verdict edits.

### 3. Submit

The one step that needs the browser: curl cannot borrow the session cookie, so the post has to happen from the logged-in page. *How* a tab is opened and JS is run in it is the one harness-specific part of the whole procedure — the transport note lives in `CLAUDE.md` for claude-in-chrome and in the Codex skill for its browser-MCP/AppleScript paths. Everything in this section is the same on every harness.

Check the tab is actually logged in before planting anything: on a logged-out page the login form replaces everything and `document.forms[2]` does not exist (`document.forms.length` is 1). POJ sessions expire; if no reachable browser holds the login, only the user can log back in — agents report back rather than wait.

The submit form is `document.forms[2]` on `http://poj.org/submit?problem_id=<id>`: fields `problem_id`, `language` (`0=G++ 1=GCC 2=Java 3=Pascal 4=C++ 5=C 6=Fortran`), `source`, and a hidden `encoded=1` because `onsubmit` base64-encodes the textarea. So plant the **plain** source and submit by clicking the real button — `form.submit()` skips `onsubmit` and would post unencoded source under `encoded=1`.

Getting the source into the textarea: base64 it locally and decode with `atob` inside the page. This snippet is the **canonical** plant-and-click — every solve agent, on any harness, runs it as written; relay it verbatim rather than paraphrasing, so a behavior change happens here and nowhere else:

```js
const src = atob('<base64>');
const f = document.forms[2];
if (!f || f.problem_id.value !== '<id>') throw new Error('wrong or logged-out submit page');
f.language.value = '4';
f.source.value = src;
if (f.source.value.length !== src.replace(/\r\n/g, '\n').length) throw new Error('source length mismatch');
f.elements['submit'].click();
```

Plain `atob` is the default for any payload; compression variants have proven fragile in page context (see the harness transport notes).

Do the whole thing — plant, verify, click — in a **single** JS call, guarded by `f.problem_id.value === '<id>'` and aborting if it does not match. With several agents in one browser a sibling can re-navigate your tab between two calls, and then the click submits *their* form: nothing appears under your problem and the source you planted is gone. The guard is a line long and the failure mode is otherwise silent.

Check `f.source.value.length` before clicking — against the **LF-normalized** source, not the file size: the textarea converts `\r\n` to `\n`, so a 118-byte CRLF file plants (and judges) as 111 bytes. Compare `src.replace(/\r\n/g,'\n').length` inside the same JS call, then `Code Length` in the status row after — that is the proof the source arrived intact. Serving the file from a local HTTP server and `fetch`ing it from the page does **not** work: Chrome's private-network access blocks a public page from reaching `127.0.0.1`.

### 4. Check the verdict

`curl -fsS 'http://poj.org/status?problem_id=<id>&user_id=150014'` — no login, no browser. Require a successful response containing `Problem Status List` before interpreting its rows; a transport failure or empty response is unknown state, not zero submissions. Each `<tr align=center>` row is a submission, newest first, cells `Run ID | User | Problem | Result | Memory | Time | Language | Code Length | Submit Time`. Poll until the result leaves `Waiting` / `Compiling` / `Running & Judging`.

A clean click is not evidence the submission landed, either. POJ drops a submission arriving within ~10s of any other — from a sibling agent as much as from you — and says nothing: the guard passes, the source length checks out, and no status row ever appears. So confirm a **new** row before believing anything. No new row on a fully loaded status page 30 seconds after the click — three times the window — means the click was dropped: it is not an attempt and does not count against the cap. Wait out the window and make **one** clean retry; a second rowless click means the problem is structural — an infrastructure interruption to report, not permission to keep clicking.

A browser error *after* the submit click is not evidence the submission was lost — with several agents in one browser, a sibling's navigation can take the tab out from under you, so the JS that was going to confirm the click fails even though the post went through. Always look at the status page before resubmitting, or phantom retries eat the submission cap.

The same churn also drops tabs *before* the click: the JS call fails with `couldn't determine which page this action targets` because the tab group went out from under it. That one is recoverable in place — recreate the tab, re-navigate, run the guarded plant-and-click again — but confirm on the status page that no row exists first, because the error message alone does not distinguish a call that never ran from one that clicked and then lost the tab.

### 5. Iterate

Report each verdict as it arrives rather than silently resubmitting.

- **Compile Error** — a C++98 slip; check `Compile Error` on the status page for the message, or retry under language `0` (G++) which is a different compiler from `4` (C++).
- **Wrong Answer** — re-read the statement for the case that was missed, reproduce it locally, then resubmit.
- **Time Limit Exceeded** — profile the local stress case; POJ is slow, so an algorithmic fix usually beats micro-optimisation.
- **Runtime Error** — usually an out-of-bounds index or recursion depth, both reproducible locally under `-fsanitize=address,undefined`.

Cap the run at **5 submissions**. Iterating past that means the approach is wrong rather than buggy, and each blind retry costs judge time — better to hand the problem back than to grind. On hitting the cap, or getting stuck before submitting at all, report the last verdict, what was tried, and what the problem actually needs; the parent turns that report into the `_attempts_/<id>.md` park.

### 6. Commit

The agent's part ends with the file: only after **Accepted**, copy the exact submitted source byte-for-byte to `<id>/<runId>_AC_<time>MS_<mem>K.<ext>`. Its title, model, approach, and ambiguity comment block was finalized and tested before submission; do not add or edit it after the verdict.

The commit itself is the parent's, one problem per commit. Subject is `<id> <Title>` — plain, no Conventional Commits prefix; the body explains the algorithm and the decisions behind it, not the code, and ends with `Model: <model-id>` matching the source comment. Once the commit lands, the parent strikes the id from `TODO`.

Do not push unless asked.

## Solving several at once

A batch adds only orchestration on top of the procedure above: spawn the solve subagents **one per problem, all in a single message** so they run concurrently, and collect a verdict per problem.

Past three concurrent agents, collisions with POJ's ~10s submission window stop being rare — the agents finish local testing at similar times, and each collision costs a poll-and-retry cycle. Give each agent a **submit slot**: agent k waits 25*k seconds before its first click — in the foreground, per the wait rule above, or the stagger backfires into a silent stall. The stagger costs one agent a minute or two and buys back more than that in avoided collisions.

Before spawning, the parent **preflights the submit path once** — the agents cannot, because a logged-out session looks to each of them like an individual failure and all five burn their run discovering it. Navigate a tab to `http://poj.org/submit?problem_id=<first id>` and read `document.forms.length` and `document.forms[2].problem_id.value`: three forms with a matching id is a live session, one form is the login page. Close the tab; the agents open their own. A failed preflight is an infrastructure stop, not a park — nothing is attempted and no ids are consumed.

The parent commits each accept as its agent reports, one problem per commit — in whatever order the verdicts arrive, not `TODO` order. Ordering the commits means holding a finished solve hostage to a slower sibling, and buys nothing: the commits are independent and `TODO` is a set, not a sequence.

An agent that dies to a usage or rate limit has not attempted anything — no write-up, no park, and the id keeps its place at the top of `TODO`. Wait for the reset and rerun it.

A hand-back report — cap hit, or stuck before submitting — becomes `_attempts_/<id>.md`: the reading of the statement, the algorithm tried, the verdict of each submission, and the failing case if one was found. The parent writes it and commits it as `<id> attempt notes`. The id stays where it is in `TODO`, but a problem carrying an attempt file is skipped when picking the next N; solve it only when asked for by id, and hand the file to the agent so it starts where the last one stopped instead of re-deriving the dead end.

Solve agents may run on a cheaper model than the parent to stretch the budget — most backlog problems don't need the strongest model. But **a model park is not a problem park**: a problem the cheap model could not finish still deserves one run on the default (stronger) model, handed the `_attempts_` notes as its starting point, before the park is final. Once the cheap model produces its first park, stop trialling it — later batches go back to the default model.
