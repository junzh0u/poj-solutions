# POJ Solutions — Agent Instructions

An archive of accepted [POJ](http://poj.org) submissions, one directory per problem ID under `solutions/`. Nothing is built or tested by the repo itself; the judge is the test suite.

This file is the shared contract: the layout, the backlog's semantics, and the interface between the orchestrating parent and its solve agents. The two roles keep their own tool-specific manuals — the solve procedure lives in `.claude/agents/poj-solver.md` or `.agents/skills/solve/references/solver.md` (loaded only by solve agents), and orchestration lives in the matching `solve` skill (loaded only by the parent). Any solving, including a single problem the user names, goes through that skill; the parent stays a coordinator that handles ids, verdicts, and file paths, never problem content.

## Layout

- `solutions/<id>/<runId>_AC_<time>MS_<memory>K.<ext>` — one file per accepted submission, named from its POJ status row (time before memory). Several files in a directory means the problem was solved more than once, e.g. in another language.
- `solutions/<id>/test_data/` — test data that was worth keeping: the statement's sample, and any data found online or constructed during a solve that provided real value (official contest data, a separating case that settled an ambiguity, a stress case that caught an actual bug). `<id>.in` / `<id>.out` for the primary set; further files carry a label (`<id>-<label>.in` / `.out`).
- `solutions/<id>/tags/<tag>` — empty marker files used as tags; manage them with `./tag.sh` (`tag.sh` lists all tags, `tag.sh <id>` lists a problem's, `tag.sh <id> <tag>` adds one).
- `TODO` — the backlog, nothing but problem ids one per line: those POJ says user `150014` has not solved and that are not parked, ranked by global solve count, most-solved first. Rebuilding it is the matching solve skill's `references/todo-rebuild.md`.
- `attempts/<id>.md` — the write-up left behind by a problem that was tried and not accepted: what was understood, what was submitted, the verdicts, and where it stalled, under machine-readable park front matter. Parking also strikes the id from `TODO` (same commit); the file is the record and the retry starting point. It must be self-contained: never refer to scratchpad, temporary, private, or other uncommitted local files.

## Parks

`attempts/<id>.md` front matter records who parked the problem and why; the matching solve skill's `scripts/park-notes` is the only way to write or read it (`record`, `gate`, `list`, `check`). `park` is the whole classification — `model` (a stronger model plausibly does better; the only kind re-picked automatically, via `park-notes list --for-model`), `judge` (POJ's judge or data is broken; gated on the judge recovering), `infeasible` (neither will change the outcome), and `gated` (a standing policy hold applied before any attempt; the only gate in force is `special-judge`). `references/model-ranks.txt` next to the scripts is the whole definition of model strength. The full parking policy — evidence, escalation, and what comes back when — is the matching solve skill's `references/orchestration.md`.

## The interface

Every solve runs in a **poj-solver subagent** of an orchestrating parent — the statement, the test scaffolding, and the failed attempts stay out of the parent's context, which only ever sees the report. The solver owns one problem from statement to Accepted or hand-back and never touches shared state: no git, no `TODO`, no browser preflight. The parent owns everything shared and commits what the solver's report names.

The **task prompt** carries exactly the run-scoped state the solver cannot discover, and the solver refuses to proceed without it:

- The **problem id**, plus its `attempts/<id>.md` path when the spawn is a parked-problem retry.
- The **exact model identifier** assigned to that agent, for the source's `// Model:` line.
- The preflighted submission target: browser **`deviceId`** for Claude or pinned Chrome **window id** for Codex.
- The harness's **keepalive tab id**, or an explicit not-applicable marker when its retained tabs do not share a tab group.
- The agent's **scratchpad subdirectory** (the shared scratchpad plus the problem id).
- The **submission cap**: 5 for a fresh solve; a stronger-model retry of a park runs under 10, extendable to 20 only while each further submission tests a genuinely distinct hypothesis.
- Any run-specific directive in force, since the definition cannot know it.

The **solver writes**, besides its scratchpad: `solutions/<id>/` (archived source and `test_data/`, only after Accepted); `<scratch>/<id>/commit-msg.txt` on an accept — subject `<id> <Title>` (plain, no Conventional Commits prefix), a body of plain prose about the algorithm and the decisions, ending with `Model: <model-id>`; and the prose body of `attempts/<id>.md` on a park (no front matter — the parent stamps the record). On an infrastructure interruption it writes nothing.

The **report** is everything the parent acts on without redoing any part of the solve, around a dozen lines: the outcome; the `// Model:` line verbatim; one line per submission (run id, verdict, code length, what changed); the files written (archive path with time and memory, `test_data/` files, commit-msg path or park note); the park-kind suggestion with its evidence counts when parking; and the tab ledger (every tab id created and closed). All prose stays in the files.

The **parent acts** with the helpers in its solve skill's `scripts/`: `spawn-precheck` before every spawn, `commit-accept` / `commit-park` for the commits (each commit is the whole state change for one problem — the solution or note plus its `TODO` strike), `verify-judge-park` before believing a judge park.

Do not push unless asked.
