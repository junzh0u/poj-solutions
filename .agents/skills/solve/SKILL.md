---
name: solve
description: Run one POJ problem or the TODO backlog end to end in Codex with dedicated solve subagents, bounded unattended Goal-mode execution, model-aware parking, parent-owned commits, and retained-ID Chrome AppleScript submission. Use when the user asks Codex to solve a POJ problem, keep working the backlog without supervision, run overnight, solve until progress stalls, or invokes $solve.
---

# Solve POJ through the agent pool

Read [references/orchestration.md](references/orchestration.md) completely before starting. It is the parent policy: selection, spawning, refill, reports, parking, and stop conditions. Do not read or paraphrase the solver manual; `.codex/agents/poj-solver.toml` loads [references/solver.md](references/solver.md) only into solve agents. The parent↔solver contract is `AGENTS.md` § "The interface".

## Choose the run shape

Use the same path for a named problem and the backlog. A named problem is a pool of one. For backlog work, default to three agents kept in flight by immediate refill because this Codex runtime has three child slots alongside the parent; reduce the pool if fewer slots are available.

Require a finite stop condition. Unless the user gives one, stop after three consecutive resolved problems produce no accept, one Codex pool's worth. If the user requests a target number of accepts, retain that barren-stretch safety net, keep the pool full until the committed-accept target is met, then drain live agents without killing them.

Use Goal mode only for an explicitly unattended or multi-turn request. If no goal exists, create one from the user's request plus the durable run state below. A single named solve or other bounded normal-turn request does not need a goal. Keep the goal text complete enough to survive compaction:

```text
Use $solve to run a three-agent refilling POJ solve pool. Resolve and pin one exact solve-agent model for the entire run. Before the first spawn, preflight the retained-ID Chrome AppleScript submission path and run `park-notes list --for-model <model-id>`; retry every owed park in the earliest slots. Run `spawn-precheck` before every spawn and gate Special Judge problems. Give every solve agent exactly the run-scoped state required by AGENTS.md and have it read the Codex solver manual. Act on every report immediately: commit accepts with `commit-accept`, stamp and commit parks with `park-notes` plus `commit-park`, report the verdict, and refill the free slot in the same turn. Stop after three consecutive resolved problems produce no accept unless the user's target or stop condition says otherwise. Infrastructure, usage, and rate-limit interruptions do not create notes, advance the barren counter, or select replacement ids. Confirm external status before any retry. Do not push.
```

Resolve the model exactly once. Use the model the user names; otherwise use `gpt-5.6-terra`, whose rank and backlog record are already encoded in `references/model-ranks.txt`. Pass the exact identifier to `park-notes`, every agent task, and the source/commit attribution. Do not silently switch models mid-run. If the requested model is absent from the ranks file, surface that policy decision before a run that may need to park.

## Preflight retained-ID Chrome submission

Use the battle-proven AppleScript transport for this Codex workflow. Do not substitute the in-app Browser or Chrome extension unless the user asks to change transports.

An explicit `$solve` invocation or request to solve under this repository's documented workflow authorizes submissions for the bounded problems selected by that request to POJ user `150014`; it does not authorize later runs or a push. A request to prepare, inspect, or diagnose without solving does not authorize submission.

Preflight once before spawning. Read-only AppleScript must:

1. List existing Chrome windows and select a normal window without creating or activating a window.
2. Save that window's active-tab index, create `http://poj.org/submit?problem_id=<first-id>` as a tab at the end, retain its tab id, and immediately restore the saved active-tab index.
3. Wait on that exact retained tab until loading is false, then require its URL, `document.forms.length === 3`, and `document.forms[2].problem_id.value === '<first-id>'`.
4. Close only the retained preflight tab and pass the pinned Chrome window id to every solve agent as the run's submission target.

User authorization does not bypass macOS Automation permission or Chrome's View > Developer > Allow JavaScript from Apple Events. If Chrome, either permission, or the logged-in POJ form is unavailable, stop as infrastructure before spawning. Do not create notes or move the barren counter.

The window id is run-scoped and fixed. If it disappears, re-preflight in the parent and send the replacement id to every live agent; if no logged-in normal Chrome window remains, stop the run. AppleScript tabs are independently addressable and have no MCP tab group, so the AGENTS.md keepalive field is the explicit marker `not-applicable: retained-id AppleScript`.

## Spawn and refill

Prefer the project custom agent `poj-solver` when the runtime exposes custom-agent selection. Otherwise spawn a normal child whose first task instruction is: `Read .agents/skills/solve/references/solver.md completely before taking any action and follow it as your whole procedure.` Do not paste, summarize, or leak the solver manual into the parent.

Every task supplies exactly:

- problem id and `attempts/<id>.md` when retrying a park;
- exact model identifier;
- pinned Chrome window id as the submission target;
- keepalive marker `not-applicable: retained-id AppleScript`;
- scratchpad subdirectory `<shared-scratch>/<id>`;
- submission cap: 5 fresh, 10 on a stronger-model retry, extendable to 20 only under the contract;
- any run-specific directive.

Spawn the initial pool concurrently. When a report arrives, act on it and spawn its replacement in the same turn. Never wait for siblings to form a batch boundary. Keep task names problem-specific and preserve enough forked user context to carry the bounded solve/submission request.

## Recover without inventing verdicts

On an agent-reported browser loss, re-preflight and send the new window id to every live agent. On a usage or rate limit, stop spawning, confirm any ambiguous submissions from authoritative status, and resume the same ids when the run can continue. On a suspected hung agent, inspect its scratchpad and `scripts/status-via-curl <id>` before interrupting it.

Report each verdict and commit in the turn it arrives. Close only tracked retained tab ids. At the end, reconcile every agent's created/closed ledger and report any leak; never sweep all POJ or Chrome tabs.
