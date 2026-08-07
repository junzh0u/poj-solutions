# POJ Solutions — Agent Instructions

An archive of accepted [POJ](http://poj.org) submissions, one directory per problem ID under `solutions/`. Nothing is built or tested by the repo itself; the judge is the test suite.

## Layout

- `solutions/<id>/<runId>_AC_<time>MS_<memory>K.<ext>` — one file per accepted submission, named from its POJ status row (time before memory). Several files in a directory means the problem was solved more than once, e.g. in another language.
- `solutions/<id>/test_data/<id>.in` / `.out` — sample data, only where it was worth keeping.
- `solutions/<id>/tags/<tag>` — empty marker files used as tags; manage them with `./tag.sh` (`tag.sh` lists all tags, `tag.sh <id>` lists a problem's, `tag.sh <id> <tag>` adds one).
- `TODO` — the backlog, nothing but problem ids one per line: those POJ says user `150014` has not solved and that are not parked, ranked by global solve count, most-solved first.
- `attempts/<id>.md` — the write-up left behind by a problem that was tried and not accepted: what was understood, what was submitted, the verdicts, and where it stalled, over a machine-readable park record in front matter (see "Parking a problem"). Parking also strikes the id from `TODO` (same commit); the file is the record and the retry starting point. It must be self-contained: never refer to scratchpad, temporary, private, or other uncommitted local files; preserve any detail needed for a retry in the write-up itself.

## Picking what to solve

"Solve N problems" means: take the **top N ids of `TODO`** — the list is already ordered so the most-solved (roughly easiest and best-documented) come first — and work them. A specific problem id the user names overrides the ordering; hand its `attempts/<id>.md` to the agent if it has one, so it starts where the last run stopped.

Parked ids are not in `TODO` and come back exactly two ways: the user naming one, or `park-notes list --for-model <model-id>` reporting one as owed a retry on a model stronger than every model that has already tried it. See "Parking a problem".

To rebuild `TODO`, submit the form at `http://poj.org/moreproblem`: it lists exactly the problems the logged-in user has not solved, each with its global solve count. This is the one step besides submitting that must run in the browser — the form posts no user field, so identity comes only from the session cookie, and the curl detour around it is closed too (`userstatus` returns 403 to non-browser requests). Only the `ID` and `Solved` columns are wanted, and the page as served runs to ~150 KB — more than text extraction returns whole — so re-render the table in the page as `<id> <solved>` lines inside a `<pre>` and replace the body with it before extracting the text. Then sort by solve count descending, subtract every id that has an `attempts/<id>.md` — POJ still lists parked problems as unsolved, and without the subtraction every rebuild would silently un-park them — and write out the ids alone.

## Solving a problem end to end

A solve always runs in a **subagent**, even for a single problem — the statement, the test scaffolding, and the failed attempts stay out of the parent's context, which only ever sees the verdict. The agent owns steps 1–5, ending at **Accepted**, and writes the annotated source file; the commit and the `TODO` strike-off are the parent's (step 6), because both are shared state — concurrent `git add`/`git commit` in one worktree races on `index.lock` and can sweep a sibling's files into the wrong commit.

**Never end a turn waiting for anything.** An agent that backgrounds a sleep or a long differential test and ends its turn to await the notification never wakes, and the task dies silently mid-run with no report. Waits go in the foreground — `python3 -c "import time; time.sleep(N)"` — and the agent continues in the same turn. A test too slow to sit through in the foreground should be made smaller: a few hundred trials whose result is actually collected beat thousands that are not. When resuming an agent that stalled this way, have it read the status page first — it may already have a submission judged, and resubmitting on top of it wastes an attempt.

Only the submit itself needs the user's Chrome session (logged in as user `150014`). Everything else — the statement, the samples, the verdict — is plain `curl` with no login, so the browser is touched exactly once per submission: open a fresh tab when ready, close it as soon as the transport has confirmed the POST handoff, and do every status read with `curl`. POST handoff timing is transport-specific and must be measured rather than assumed: claude-in-chrome closes on the next tool call, while the Codex AppleScript helper waits for the retained tab to finish loading the status landing page before the next-statement close. A short browser window means fewer chances for a sibling agent to re-navigate the tab, but the in-call guard in step 3 stays regardless.

### 1. Read the statement

`curl -s 'http://poj.org/problem?id=<id>'` — the raw HTML carries the whole statement, samples, and limits, and needs no login. Note the time and memory limits; they decide how much the algorithm can afford.

Read the HTML itself, not a text extraction — the misreadings that have cost a submission here were extraction artifacts that the raw markup makes unambiguous:

**Exponents.** Text extraction drops superscripts: a bound written `2<sup>54</sup>` in the HTML arrives as `254`, and `10^9` as `109`. The `<sup>` tag is unmissable in the raw markup. Getting this wrong invents a range the problem never had, and sends the solve chasing a worst case that sits outside the real bound entirely. Not every odd constant is a mangled exponent, though — a bound like `cs<=32` may really be a product of two variables, which the statement's own gloss settles.

**Leading whitespace.** Expected output sometimes indents every line, visible in the sample's raw `<pre>` but not in extracted text; missing it turns into a Presentation Error that reads like a wrong answer. In the curl output the bytes are exact (samples also carry `\r\n` line endings; strip them when diffing).

**Prose semantics.** Not every trap is markup. A constraint like "she cannot commence running again until her exhaustion factor reaches 0" forbids more than it first appears — rest, once begun, is forced to the end — and the loose reading still matches the sample, surfacing only as a Wrong Answer. Constraint sentences deserve a second literal read: what exactly do they forbid?

**Figures.** Some statements put load-bearing structure only in an image, and there is no reading of the prose that recovers it. 2286's board is the worked case: which of the 24 cells are numbered in what order, which eight form the centre, and which end of each line wraps to which under moves A–H, all live in `images/2286_1.jpg` and nowhere in the text. Fetch the image and read it — that solve compared the figure's three animation frames pixel by pixel to pin the wrap directions — rather than reconstructing a layout that "looks like" the puzzle. A guessed layout yields a solver that is entirely self-consistent and wrong, and the sample need not catch it, since a wrong labelling can still solve the sample's own board.

Skim the problem's discuss board too — `curl -s 'http://poj.org/bbs?problem_id=<id>'`, the Discuss link on the problem page, no login needed. It usually holds the problem's known pitfalls, often with a concrete failing case, and reading it costs seconds where rediscovering the pitfall costs a submission.

What the board most often carries is not an algorithm but **input framing the statement omits**, which no amount of local reasoning would recover: that the input holds multiple data sets and must be read to EOF (2677, 3764), that blank lines are legal input strings and `scanf("%s")` silently desynchronises the pairing (2629), that there are trailing blank lines after the last case (3764), or that coordinates fall outside the stated range (2451). Close behind it is environment folklore worth honoring on sight — language `4` accepted where `0` gives RE or WA (2429, 2451), `long double` failing where `double` passes (2451), `std::deque` blowing a tight memory limit (1180), an eps of 1e-16 drawing WA where 1e-10 passes (2451). Some of it is wrong (2677's "use `float`, not `double`" is really a `%f`/`%lf` mismatch in the code being copied around), so treat a claim as a hypothesis to check, not as fact — but check it before spending a submission, not after.

The compiler folklore in particular runs **both** directions, so it is a per-problem read of the board rather than a default to flip. One run met both: 2552's board reports RE under `4` for logic Accepted unchanged under `0`, and 3685's reports `long long` arithmetic slow enough to TLE under `4`, while 3686's reports WA and TLE under `0` and it stayed on `4`. All three were Accepted first try on the language the board named. `4` remains the default when the board says nothing.

The board is also where an ambiguity gets settled that the sample cannot settle. Where a statement admits two readings, the official sample often reproduces under both, so matching it is no evidence for either — the same blind spot as a differential test against a reference built from your own reading. When a reading is genuinely open, find the case that separates the candidates before submitting; the board usually has it, because someone already lost a submission to it.

2125 is the worked example, and it is worse than that paragraph implies. Its two cost lines can be read in either order, and the official sample is **invariant** under the swap — both readings cost 5, printing different but equally valid move sets. So is the board's own suggested test case. A first run spent five submissions on the swapped reading with a brute force that shared it, agreeing on 340+ trials. The escape is to construct the separating case yourself and check that the two readings actually disagree on it, rather than assuming any case that exercises the ambiguity will expose it:

```
2 1 / 10 5 / 1 10 / 1 2   ->  answer 1 under the correct reading, 5 under the swap
```

### 2. Write and test locally

Work in the scratchpad, not the repo — only the accepted source gets committed. Use a `<id>/` subdirectory (or prefix every filename with the id): the scratchpad is shared with concurrent sibling agents, and a generic `brute.cpp` has silently collided with a sibling's before.

- Target **C++98**: POJ's compilers are ancient, so no `auto`, no range-`for`, no `<cstdint>`. Build with `g++ -std=c++98 -pedantic -Wall -Wextra -O2`.
- Reproduce the statement's sample first and `diff` against the expected output verbatim.
- Then go past the sample: a stress case near the stated input limits (`/usr/share/dict/words` is a handy source of real words), the degenerate inputs, and — where the answer is a classification rather than a value — a randomized differential test against a brute-force reference on small instances. POJ reports nothing but the verdict, so anything not caught locally costs a blind submission.
- **Enumerate the whole domain when it is small enough** — it retires the question instead of sampling it. 1365's input is one integer below 32768, so all 32765 cases ran against a reference in 0.06s; no randomized test can match that, and it cannot share a misreading it never had to guess at. Check for this before writing a generator.
- A differential test only proves agreement with its reference, and a reference written from the same reading of the statement shares its misreading — hundreds of passing trials have validated the same wrong rule the solution used. Derive the brute force from the statement's own words, not from the solution's model of them.
- **Show that the reference can detect what it certifies**, before trusting a single trial. 1584's brute force searched for protrusions by sampling chords between random interior points, and missed the *sample's own* shallow notch — random chords rarely cross a shallow reflex vertex. It agreed with everything because it found nothing. This is a distinct failure from the shared-misreading trap above: there the reference encodes the wrong rule, here it encodes the right rule and is blind. Both produce a wall of meaningless green. Run the reference against a known-positive case — the sample is usually one — and confirm it fires.
- **Mutate the solution and check whether the sample catches it.** The two traps above are about a blind reference; this one is about a blind *oracle*. Break the rule most likely to be misread — invert the comparison, drop the tie-break, skip the degenerate class — and run the official sample against the mutant. It usually passes. On 2607, five mutants were built and the sample killed exactly one; so did the board's own two posted cases, including the sum-versus-max misreading the board reports as repeatedly costing people submissions, and a sixth mutant passed the board's separating case only by luck. 2540's sample is symmetric under reflection through its square's centre, so even swapping the problem's central Hotter/Colder comparison reproduces it byte-for-byte; 2069's is invariant under dropping any one of its three candidate families; 2556's is passed by the exact bug the board keeps reporting. Six problems in one run. The mutation costs two minutes and tells you whether the sample is evidence or decoration — and when the sample turns out to be strong (2734's killed all four), that is worth knowing too. Where a mutant produces *no* disagreements even against the differential test, work out which it is: 2607's non-station-maximum mutant and 2668's divisor-bound mutant were both genuine equivalences, provable in a line, not blindness.
- **Validate the emitted artifact, not just the objective**, whenever the output is a witness rather than a number. 2125 prints a move set that a special judge checks; every trial of the failed run compared only the cost, which agreed while the move set differed. Assert the properties the judge asserts — well-formed, no duplicates, every arc actually destroyed, printed cost equal to the move set's own cost.
- Where the problem has a dual, **certify rather than cross-check**: 2125's retry proved optimality by building an independently verified feasible flow, so weak duality bounds any valid answer from below. Two implementations of one idea agreeing is much weaker evidence than one result bounded by a different argument.
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

The JavaScript call returning is not by itself proof that the browser handed off the POST — transport notes must cite a submission or reproducible transport test demonstrating their close timing. Follow the harness note rather than guessing, then move all confirmation and verdict reads back to `curl`.

Check `f.source.value.length` before clicking — against the **LF-normalized** source, not the file size: the textarea converts `\r\n` to `\n`, so a 118-byte CRLF file plants (and judges) as 111 bytes. Compare `src.replace(/\r\n/g,'\n').length` inside the same JS call, then `Code Length` in the status row after — that is the proof the source arrived intact. Do not re-read `f.source.value` *after* the click as verification: `onsubmit` runs synchronously inside `.click()` and re-encodes the textarea, so the post-click value is the base64 (about 4/3 the size), not the source. Serving the file from a local HTTP server and `fetch`ing it from the page does **not** work: Chrome's private-network access blocks a public page from reaching `127.0.0.1`.

### 4. Check the verdict

`.agents/skills/backlog-loop/scripts/status-via-curl <id>` — no login, no browser. It makes a fail-visible `curl -fsS` request with a unique query nonce, requires `Problem Status List`, parses every `<tr align=center>` into named JSON fields, and returns `rows` newest first; pass `--html-out <path>` to preserve the raw response for each baseline and poll. Use this helper instead of hand-written row extraction: a parser failure is unknown state, not zero submissions, and a false empty result can trigger a duplicate submission. For a general problem status read, add `--all-users`. Poll until the result leaves `Waiting` / `Compiling` / `Running & Judging`.

A clean click is not evidence the submission landed, either. POJ drops a submission arriving within ~10s of any other — from a sibling agent as much as from you — and says nothing: the guard passes, the source length checks out, and no status row ever appears. So confirm a **new** Run ID from `status-via-curl` before believing anything. No new row in a valid parsed response 30 seconds after the click — three times the window — means the click was dropped: it is not an attempt and does not count against the cap. Wait out the window and make **one** clean retry; a second genuinely rowless click means the problem is structural — an infrastructure interruption to report, not permission to keep clicking.

A browser error *after* the submit click is not evidence the submission was lost — with several agents in one browser, a sibling's navigation can take the tab out from under you, so the JS that was going to confirm the click fails even though the post went through. Always look at the status page before resubmitting, or phantom retries eat the submission cap.

The same churn also drops tabs *before* the click: the JS call fails with `couldn't determine which page this action targets` because the tab group went out from under it. That one is recoverable in place — recreate the tab, re-navigate, run the guarded plant-and-click again — but confirm on the status page that no row exists first, because the error message alone does not distinguish a call that never ran from one that clicked and then lost the tab.

### 5. Iterate

Report each verdict as it arrives rather than silently resubmitting.

- **Compile Error** — a C++98 slip; check `Compile Error` on the status page for the message, or retry under language `0` (G++) which is a different compiler from `4` (C++).
- **Wrong Answer** — re-read the statement for the case that was missed and rebuild the differential reference from that re-reading (a reference sharing the old misreading passes every trial); mine the discuss board for the failing case; reproduce locally, then resubmit.
- **Time Limit Exceeded** — profile the local stress case; POJ is slow, so an algorithmic fix usually beats micro-optimisation.
- **Runtime Error** — usually an out-of-bounds index or recursion depth, both reproducible locally under `-fsanitize=address,undefined`.
- **System Error** — POJ's judge-crash verdict, and the one result that says nothing whatever about the submitted program. Do not debug against it; go straight to the general status page below.

Sometimes the judge is the bug. When an independently-derived known-good solution (a canonical reference implementation submitted as a control) fails the same way as yours, check the problem's **general** status page — `status-via-curl <id> --all-users`: every recent submission from unrelated users failing identically, against the same account accepting other problems the same hour, means the problem's judge or data is broken — this has happened, to a problem that had been accepting solutions of the same shape weeks earlier. That is a park with the evidence recorded, revisited only if the judge recovers — `--kind judge`, so no stronger-model retry is owed and the `recheck` URL carries what would retire it.

A judge park is the one hand-back the parent should **verify rather than take on trust**, and the only one it can: every other park rests on the agent's own reasoning, which the parent cannot re-derive without redoing the solve, but this one rests on a public page one command away. Re-run `status-via-curl <id> --all-users` yourself and confirm three things before writing the notes — unrelated accounts failing the same way, no Accepted anywhere in the window, and this account accepting other problems the same hour. All three held for 1112 (13 `System Error` of the last 20 rows, five unrelated accounts, June–August 2026, four accepts by this account inside the same hour). Record those counts in `attempts/<id>.md`: a later reader deciding whether the judge recovered needs the numbers, not the conclusion.

Cap the run at **5 submissions**. Iterating past that means the approach is wrong rather than buggy, and each blind retry costs judge time — better to hand the problem back than to grind. On hitting the cap, or getting stuck before submitting at all, report the last verdict, what was tried, and what the problem actually needs; the parent turns that report into the `attempts/<id>.md` park.

### 6. Commit

The agent's part ends with the file: only after **Accepted**, copy the exact submitted source byte-for-byte to `solutions/<id>/<runId>_AC_<time>MS_<mem>K.<ext>`. Its title, model, approach, and ambiguity comment block was finalized and tested before submission; do not add or edit it after the verdict.

The commit itself is the parent's, one problem per commit. Subject is `<id> <Title>` — plain, no Conventional Commits prefix; the body explains the algorithm and the decisions behind it, not the code, and ends with `Model: <model-id>` matching the source comment. Once the commit lands, the parent strikes the id from `TODO`.

**The agent writes that body; the parent reads it and commits it.** Composing it in the parent is the one part of the per-verdict work that both scales with the number of solves and runs on the single serialized actor — and it is derived entirely from a report the agent has already written, so the parent is authoring the same content a second time from a worse vantage point. Ask for the finished body in the task prompt, alongside the title and the `// Model:` line, and let it *replace* the corresponding prose in the report rather than sit beside it.

Specify the form, or what comes back is a report: plain text, no markdown headings or tables, no HTML entities, prose wrapped at the usual width, about twenty lines, ending with the `Model:` trailer. Content is the algorithm and the decisions behind it — the statement ambiguity and how it was settled, what the board contributed and whether it was checked or merely believed, what the verification actually established — never a narration of the code.

On the accept path there is no review gate, and adding one back is the mistake this is meant to prevent. The substance was certified by the judge, not by anything the parent could read: POJ returned Accepted, and the archived source is byte-identical to what was judged because its length was checked against the row's `Code Length`. The body is descriptive prose attached to an already-proven fact, in a repository that is not pushed, in git, amendable. So commit it. Add the cross-problem sentence when there is one — the agent cannot know its problem is the fourth this run whose load-bearing detail lived only in a figure — and fix an error you happen to notice, but do not read it as a gate and do not rewrite an accurate body.

If a run does want assurance on an accept, the prose is the weakest test available. Re-read the status page: `status-via-curl <id>` confirms the reported run id and verdict against POJ itself, in one command, and it is the same check §4 already prescribes. Reading the body proves only that the agent can write.

**The park note is the hand-back worth reviewing**, and for the opposite reason: nothing external certifies it. It is the agent's own account of why it failed, and it *gates future work* — `park-notes list --for-model` re-picks an id from the recorded kind, so `model` against `judge` decides whether the problem is ever retried at all. That is why §5 has the parent re-run `--all-users` before writing a judge park rather than taking the agent's word, and why the kind is the parent's call: it is a judgment about the run and the judge's public record, not about one solve.

Do not push unless asked.

## Parking a problem

A hand-back — cap hit, or stuck before submitting — becomes `attempts/<id>.md`, written by the **parent**: the reading of the statement, the algorithm tried, the verdict of each submission, and the failing case if one was found. Over that prose the file carries a machine-readable park record, because "is this problem owed a retry?" used to be answered by a sentence in the body that a later run had to notice and interpret — and a note that simply never mentioned a model was indistinguishable from one with nothing left to try. Two of the five notes standing on 2026-08-06 were in exactly that state, and recovering which model had parked them meant tracing their commits back to the session that wrote them.

Write the prose first, then stamp the record over it:

```sh
.agents/skills/backlog-loop/scripts/park-notes record <id> \
  --model <model-id> --kind model --submissions 5 --verdict 'Wrong Answer'
```

which prepends (or updates) front matter, leaving the body untouched:

```yaml
---
problem: 2125
park: model
parked: 2026-08-06
solvers:
  - claude-sonnet-5 2026-08-06 5 Wrong Answer
---
```

`park` is the whole classification, and only `model` is ever re-picked automatically:

- `model` — the solve failed on reasoning or implementation, and a stronger model plausibly does better. 2125 is the worked example: five Wrong Answers on a statement ambiguity that a second reading resolved on the first retry submission.
- `judge` — POJ's judge or data is broken, on the evidence §5 requires. No model helps, so the retry is gated on the judge recovering instead: `--recheck <status url>` is mandatory and the tool refuses a judge park without it.
- `infeasible` — neither. Nothing about the model or the judge will change the outcome.

Then strike the id from `TODO` and commit both as `<id> attempt notes`.

### What comes back, and when

`park-notes list --for-model <model-id>` prints the ids parked by a strictly weaker model, one per line — that is the whole re-pickup rule, and `.agents/skills/backlog-loop/references/model-ranks.txt` is the whole definition of "weaker". Nothing anywhere infers strength from a model name. A model missing from that file cannot park anything (`record` refuses it), and a note naming one is reported as **needing triage** on stderr with exit 4 rather than skipped — the ids on stdout are still valid, but a silent skip is exactly how an owed retry gets lost, so it is never silent. Equal ranks mean no escalation in either direction, which is the honest encoding for two models whose relative strength has not been established here, notably across vendors.

When the stronger model fails too it does **not** write a second note: it records its own attempt over the existing one with the same command, appending a solver line and leaving `parked` at the original date. The id is then owed nothing until a model outranking *it* exists. If that retry is what uncovers a broken judge, pass `--kind judge --recheck <url>` and the park reclassifies in place.

When a retry does clear a park, delete `attempts/<id>.md` in the same commit as the solution — the note is the record of an *open* park, and a stale one left behind keeps offering a retry for a problem already solved. The escalation has earned this: 2125 was the first id the scan ever handed back, and it was Accepted on the retry's first submission, on a statement ambiguity the official sample cannot distinguish (§1). A second reading of the statement is worth more than a sixth submission of the first one.

`park-notes list` with no model shows every park as `owed` or `final` — whether any ranked model could still be owed it. That is a claim about future runs, not about the current one: a run's own no-accept counter advances unless *that run* will actually retry the id, because a run with nothing stronger to escalate to has genuinely resolved the problem for its own purposes, however `owed` the note is for later. `park-notes check` validates every record; run it before trusting a listing, and in the same commit as any note you hand-edit.

## Solving several at once

A batch adds only orchestration on top of the procedure above: the parent keeps **five solve subagents in flight at once**. Spawn the first five one per problem, all in a single message so they start together; from then on, spawn a replacement from the top of `TODO` as soon as any one agent reports, without waiting for its siblings.

Refill rather than wait, because a batch that waits for its slowest member sits idle for as long as that member runs. Measured on 2026-08-06 on a twelve-problem batch: ten problems had resolved by 22 minutes, but the batch did not end until 36, because one problem needed 22 minutes on its own and another ran to the submission cap. Those freed slots sat empty the whole time. The problems are independent and each accept is committed the moment it lands, so a batch boundary is bookkeeping and nothing else — there is no point in the run where the parent needs all five verdicts in hand at once.

A run with a target — "solve N problems" — needs a rule for when to stop refilling, because the pool can overshoot it. **Keep the pool full until the target is actually met by committed accepts**, then stop spawning and let the remaining agents drain.

Do not taper earlier by spawning only as many agents as the target still needs. That assumes every agent in flight will accept, and one park then empties the pool short of the target and pays a fresh solve's wall clock — five to twenty minutes — to recover, serially, at the very end of the run. The asymmetry decides it: an overshoot is an extra accepted problem off the top of the backlog, committed and kept, and it is bounded by the pool size anyway; an undershoot is dead time and buys nothing. When the pool does empty short, refill it and carry on.

Never kill a live agent to stop on the number, either: it may already have clicked submit, and killing it there strands an Accepted submission with no archived source and no commit, which costs a submission to rediscover.

The run on 2026-08-06 finished at 13 accepts against a target of 10. It stopped spawning with five agents still out and all five accepted — which is exactly the case tapering gets away with, and which a single park would have broken.

Five is a choice about the parent's own attention rather than a throughput ceiling: the parent has to read, commit, and strike each verdict as it lands. Concurrency itself has plenty of headroom — twelve agents ran without saturating anything, and the submit lock's worst observed wait was 15 seconds against solves of 3 to 36 minutes. What does degrade with width is contention for POJ's ~10-second submission window: the only dropped click of that day appeared in the twelve-agent batch, one in thirteen submissions, against none in ten submissions at five. A later refilling run the same day put thirteen submissions through a pool of five with no dropped click either, so the contention scales with width rather than with total volume.

Those small samples read as though a pool of five is clean. It is not — it is merely better. A refilling run on 2026-08-07 put roughly 190 submissions through a pool of five and lost about a dozen clicks to the window, near 6%. So a dropped click is **routine at sustained volume**, not an anomaly: agents should expect one, and the §4 procedure — no row 30 seconds after a valid parse, wait out the window, re-reserve, click once more — is a normal path rather than an alarm. Every agent that hit one on that run diagnosed it correctly and none miscounted its cap. Raising the lock's minimum interval to buy the drops back is not worth it, and the arithmetic is worth recording so nobody re-derives it: about twelve drops at roughly 75 seconds of recovery each is some fifteen minutes, while adding five seconds to each of ~190 clicks costs about sixteen. A wash. Leave the interval at 15 seconds.

Concurrent agents must coordinate the actual submit clicks, not wait fixed offsets from when their independent solves happen to finish. Before the first click and every retry, an agent prepares its tab and payload, then takes an atomic shared lock; while holding it, the agent waits until at least 15 seconds after the last attempted click, performs the guarded click, records the attempt time, and releases the lock. The lock must be process-scoped so it releases automatically if an agent dies. This serializes the ~10-second POJ submission window without serializing statement reading or local testing; harness instructions provide the shared coordinator command.

Before spawning, the parent **preflights the submit path once** — the agents cannot, because a logged-out session looks to each of them like an individual failure and all five burn their run discovering it. Navigate a tab to `http://poj.org/submit?problem_id=<first id>` and read `document.forms.length` and `document.forms[2].problem_id.value`: three forms with a matching id is a live session, one form is the login page. Record the exact browser profile/window selected by preflight and require every agent and retry in the run to use it, including agents spawned later into refilled slots; do not rediscover or switch when the user later opens another window or profile. Close the tab; the agents open their own. A failed preflight or disappearance of the selected browser target is an infrastructure stop, not a park — nothing is attempted and no ids are consumed.

Losing that target mid-run is a stop for the **agents**, not for the run: the parent can re-preflight and re-pin, and the agents resume where they stopped. When the browser dies partway through, each agent's own rule — report, never substitute a target that merely looks available — is what makes the recovery cheap, because none of them has submitted from an unverified session and none has to be unwound. The parent re-preflights exactly as above, then hands every live agent the new target and tells it to resume at step 3, and pins every later spawn to it; agents that had already stopped and reported resume from their transcripts with their finished solve intact, and agents still working pick the new target up before they reach submit. Nothing is re-solved and no attempt is consumed. This ran end to end on 2026-08-06: Chrome crashed mid-batch and was restarted by hand, two of five agents had already stopped and reported the loss, and after one re-preflight all five submitted normally — four Accepted on their first submission.

So an agent reporting a lost browser target is a request for a new one, not a verdict on its problem. Re-preflight first; only when no reachable browser holds the login is the run actually stopped, and that is the point at which the ids stay eligible and the run ends without notes.

The parent commits each accept as its agent reports, one problem per commit — in whatever order the verdicts arrive, not `TODO` order. Ordering the commits means holding a finished solve hostage to a slower sibling, and buys nothing: the commits are independent and `TODO` is a set, not a sequence.

An agent that dies to a usage or rate limit has not attempted anything — no write-up, no park, and the id keeps its place at the top of `TODO`. Wait for the reset and rerun it.

A hand-back report — cap hit, or stuck before submitting — becomes `attempts/<id>.md` per "Parking a problem", written and committed by the parent with the id struck from `TODO`. The agent reports the verdicts; the parent decides the `park` kind, since that is a judgment about the whole run and the judge's public record, not about one solve.

Solve agents may run on a cheaper model than the parent to stretch the budget — most backlog problems don't need the strongest model. Which model they run on is the run's own choice, and the user's whenever they name one; nothing here retires a model mid-run on its own. But **a model park is not a problem park**: a problem the cheap model could not finish still deserves one run on a stronger model, handed its `attempts/<id>.md` as the starting point, before the park is final. Inside a run that retry is just the next agent the parent spawns into a free slot; across runs it is whatever `park-notes list --for-model` reports as owed. Both read the same park record, so the retry survives the run ending, a compaction, or a switch to a different model weeks later.

That record is what makes the model choice free. An earlier version of this file ended a cheap model's run at its first park, on the reasoning that a park nobody revisited was worse than a solve nobody attempted. The record answers that directly — it names the model that parked the id, and the scan hands it back to the first run that outranks it — so stopping the model early buys nothing the record does not already guarantee, and costs the accepts it would have gone on to make. Judge the model on its record, and let a park be a park.

The record is what makes this independent of whether anyone declared a trial. A park by a model that is not the strongest ranked one is provisional whether or not the run knew it was running a cheap model — which is the failure the record exists to close, since a run whose *session* model was the cheap one used to write a final-looking park that no stronger model ever reconsidered.
