---
name: backlog-loop
description: Run the POJ TODO backlog unattended in Claude Code — compose a self-paced /loop prompt with a bounded stop condition, model escalation, and usage-limit recovery. Use when the user asks Claude to work or keep solving the backlog without supervision, run overnight, solve problems until progress stalls, or invokes /backlog-loop.
---

# Work the backlog unattended

Read `../../../.agents/skills/backlog-loop/references/policy.md` completely before starting. Follow `AGENTS.md` for problem selection, solving, submission, parking, and commits; this skill only adds Claude-specific unattended orchestration.

## Compose the loop

Invoke `/loop` with no interval so agent completions re-invoke the parent. The prompt must carry the complete run state because it is passed back verbatim on every firing and survives context compaction.

Default to a pool of five kept full by refill, and stop after ten consecutive resolutions with no accept, unless the user specifies otherwise.

Keep run-specific state **out** of this baseline. A model name or a particular problem id baked into copy-paste text goes stale without anyone noticing, and the prompt beats the prose every time, because the prompt is what actually runs. The previous baseline hardcoded a `sonnet` trial and kept spawning `sonnet` for a whole commit after the paragraph below had declared that trial over. Say what the run should *do*; let it read what is currently *true* from the repo.

```text
/loop keep 5 solve agents in flight, refilling each slot from the top of TODO as soon as an agent reports rather than waiting for its siblings. Stop when 10 consecutive resolved problems produce no accept. Spawn subagents with no model override so they inherit the session model. Before the first spawn, run `.agents/skills/backlog-loop/scripts/park-notes list --for-model <the model id the solve agents will run>` and put every id it prints in the earliest free slots, triaging anything it reports on stderr instead of ignoring it. Park with `park-notes record` so the parking model and park kind are recorded. A usage or rate limit is not a park: do not create attempt notes or advance the no-accept counter; wait for the reset and retry the same ids. Report each verdict as it arrives and commit each accept in the same turn. Do not push.
```

Inheriting the session model rather than naming one is what makes that prompt durable: switch the session to a different model and the agents follow, with nothing to update here.

The refill needs no scheduling machinery: a finishing agent already re-invokes the parent, so the turn that reads a verdict is the turn that commits it, strikes the id, and spawns its replacement. Do that in the same turn — a slot left empty until the next wakeup is the idleness this design exists to remove. Spawn nothing when `TODO` is empty or the stop condition has been met, and let the remaining agents drain.

Adapt the pool size, stop condition, and model policy to the user's request without dropping the safety rules in the shared policy. A user who asks for a number of *solves* rather than a barren-stretch stop ("stop after 10 solved") is setting a target, so keep the barren-stretch condition as a safety net underneath it and follow `AGENTS.md` for the endgame: keep the pool full until committed accepts reach the target, then drain rather than kill. Put both conditions and the drain rule in the loop prompt, since that is what survives compaction — this run's amendments to its own stop condition were made by re-arming `ScheduleWakeup` with an updated prompt, which is the mechanism for changing run state mid-run.

Preflight the submit path yourself before spawning, per `AGENTS.md`. It is the one failure that would otherwise hit every agent in the batch at once, and it costs one tab — keep that tab rather than closing it as `AGENTS.md` says, since on this harness it becomes the keepalive tab below. Loaded browser tools with no connected Chrome are not a submission path either — check that a browser is actually attached before reading the form. A failed preflight is an infrastructure stop: no attempt notes, no movement of the no-accept counter, the ids stay eligible.

Preflight is also where the browser gets chosen. With several connected Chromes, probe them for the logged-in form and record the `deviceId` of the one that has it, per `CLAUDE.md`; that id is run-scoped state the agents cannot rediscover for themselves, so it goes in every task prompt — including every agent spawned into a refilled slot — and stays fixed for the whole run.

## Cheaper models and owed retries

Agents inherit the session model. `sonnet`'s trial ended on 2026-08-06 with its first genuine model park — 2125, five Wrong Answers on a min-weight vertex cover — which is where the shared policy retires a cheap-model trial.

It ended on a strong record rather than a weak one: 37 problems off the top of the backlog produced 35 Accepted, and no accept ever needed a second submission — every one landed on its first click that reached the judge. The other miss was 1112, where POJ's own judge is broken and no model would have helped. So nothing observed says `sonnet` was struggling; the trial ended because one park ends it. That is worth knowing before treating the retirement as a verdict on the model.

To trial a cheap model again, add to the loop prompt: `Spawn subagents with model "<name>" until the first model park; after that spawn with no model override.` Keep a trial in the prompt, where it expires with the run — not here, where it outlives its evidence.

Any model new to the fleet has to be ranked in `.agents/skills/backlog-loop/references/model-ranks.txt` before it can park anything; `park-notes record` refuses an unranked one. That is deliberate, and it is the one piece of setup a new model needs: an unranked park asks a question — is this owed a retry? — that nothing downstream can answer. Ranking it against the models already there is a judgment call to make once, and equal ranks are available for "no claim either way".

Two consequences of running with no override:

- **A park counts against this run as soon as this run cannot escalate it.** With agents on the session model there is nothing stronger to spawn, so the park resolves the problem for this run and advances the no-accept counter straight away. Under a declared cheap-model trial the counter moves more slowly, because parks stay provisional until their retry lands.
- **What the *next* run is owed is a separate question, and the park record answers it.** Whether the session model was the strongest one is a fact about the run, not about the problem: `park-notes record` writes down which model actually parked it, and `park-notes list --for-model` hands the id back to any run whose solve model outranks that. So a run held on a cheaper session model no longer buries a retryable problem behind a park that reads as final — which is what happened before the record existed, since "no override" and "the strongest model" were being treated as the same thing. An owed retry is then ordinary work: a normal spawn into a free slot, which is why the baseline prompt runs the scan before its first spawn rather than carrying ids itself.

The escalation earned its keep the first time it was exercised. 2125, the only owed retry as of 2026-08-06, was Accepted on the retry's first submission, and the park was retired the same day it came due. The defect was a swapped pair of statement lines that the official sample cannot distinguish — see `AGENTS.md` §1 — so this is evidence for the escalation rule specifically: a second reading of the statement is worth more than a sixth submission of the first one. When a retry does clear a park, delete `attempts/<id>.md` in the same commit as the solution.

That run is also the default model's own baseline: 13 problems off the top of the backlog, 13 Accepted, every one on its first submission, no park and no dropped click.

## Re-pin a browser that dies mid-run

`AGENTS.md` makes this a recoverable path rather than a stop; the Claude-side mechanics are:

- Re-run `list_connected_browsers`, `select_browser`, then navigate and read the form exactly as in preflight. A crashed-and-restarted Chrome comes back under a **new `deviceId`**, so the old id is gone even though it is the same browser and the same profile — do not read its absence as the session being lost. On 2026-08-06 the pinned id vanished and the replacement was still logged in, with the parent's keepalive tab and its tab group both intact across the crash.
- Push the new id to **every live agent** with `SendMessage`, and record it as the pin for every agent spawned afterwards. An agent that stopped and reported the loss is resumed from its transcript with its finished solve still in context and needs only the new id to continue at step 3; an agent still working gets the message queued to its next tool round and picks the id up before it reaches submit. Both paths were exercised in one batch — two resumed, three queued, all five submitted normally.
- Say in the message that the loss was infrastructure and that no attempt was consumed, so the agent does not spend its own reasoning re-deriving whether its cap or its solve is intact.
- Re-state the keepalive tab id and that it must not be closed. A resumed agent re-reads the tab group and will otherwise see a tab it did not create.

## What every solve-agent task must carry

`AGENTS.md` is the canonical copy of the guarded snippet, the form facts, and the dropped-click rule; relay its §3–4 text **verbatim** in the task prompt rather than paraphrasing it here. On top of that, each task prompt must carry:

- The **exact model identifier** assigned to that agent, and the requirement to report its finalized `// Model:` line alongside the verdict. The transport refuses to build a payload from a source that does not declare it exactly once, and a provenance error found after Accepted cannot be fixed without either another submission or a falsified archive.
- The claude-in-chrome transport rules in `CLAUDE.md` — in particular that every click, first attempt and retry alike, is reserved through `.agents/skills/backlog-loop/scripts/with-submit-lock` as the tool call immediately preceding it. Agents that space their clicks by their own finishing times are not coordinating.
- The **preflighted `deviceId`**, with the requirement to `select_browser` it before opening any tab and to keep it for every retry. An agent handed no device picks one, and a run spread across two Chromes submits half its solutions from a logged-out one.
- The requirement to report **every tab id it created and every one it closed**, not just the tab that carried the successful click. Agents that recover from tab-group churn create replacement tabs, and only their own report distinguishes a tab they closed from one the group teardown stranded.
- The requirement to pass **`--html-out <path>`** on every `status-via-curl` baseline and poll, writing each to a distinct path. Relaying §4's "take a baseline" without this satisfies only half of what the shared policy asks for: it requires *preserved, distinct before/after evidence per attempt*, and the parsed JSON an agent keeps in context is neither preserved across compaction nor sufficient to re-diagnose an ambiguous click. The raw response is the only artifact that survives to tell a dropped click apart from a parser miss after the fact. This was under-relayed in the 2026-08-06 batch and cost nothing only because no click was ambiguous.

## Own the tab group's lifetime

The parent holds a **keepalive tab** in the MCP tab group: open it before spawning the first agent, keep it for the whole run, close it only after the pool has drained and the last agent has reported. A refilling pool makes this more load-bearing, not less — slots turn over continuously, so there are far more moments where an agent's close would otherwise be the group's last. Everything in the run shares that one group, and the moment any agent closes what the extension counts as its last tab the group is auto-removed — which ungroups every sibling tab still inside rather than closing it, stranding each one beyond the reach of `tabs_close_mcp` and leaving the user to close it by hand. The keepalive tab is the whole fix: the group never reaches zero, so it is never removed. See `CLAUDE.md` for the mechanism and the measurement.

A refilling run of 13 agents confirmed it end to end: 13 tabs created, 13 closed, and the group auto-removed exactly once — on the parent's own final close, where the removal is harmless because nothing is left inside. A last `tabs_context_mcp` before that close is the cheap confirmation, and it should show the keepalive tab alone.

Agents will report tabs they did not create. Sibling churn means a `navigate` can spawn a tab another agent then adopts, and stale tabs disappear from the group on their own — both were reported this run and both were benign, reconciling against the sibling's own created/closed pair. Treat such a report as an observation to match up in the final ledger, not as a leak.

Do not audit agent tabs from the parent by id while the pool is running. Membership of the shared group flickers as agents create and close tabs, and a parent reading it mid-run can see the full set, a stale subset, or no group at all — none of which is evidence about any particular agent. Audit at the end of the run instead: require every agent to report the tab ids it created and closed, accumulate those reports as they arrive, and treat created-minus-closed as the leak count. Report any leak to the user with the ids, since only they can clear it.

## Recover from stalls

Use `ScheduleWakeup` only as a fallback for a hung agent or rate-limit reset, never as the refill mechanism — refill happens in the turn a verdict arrives, not on a timer. For a normal hung-agent fallback, schedule 1800 seconds.

A hung agent is more expensive under refill than it was under batching, because it holds a slot open indefinitely instead of merely delaying a boundary. When the fallback fires and one agent is still out, check whether it is actually working before treating it as hung: `status-via-curl <id>` shows any submissions it has already made, and its scratchpad subdirectory shows whether artifacts are still being written. On 2026-08-06 an agent that looked stalled at the 30-minute mark had four Wrong Answers and a fresh status baseline three minutes old — it was mid-solve on its final attempt, and killing it would have thrown away a completed write-up.

On a usage limit, arm the wakeup before refining the reset time because the environment may be partially unavailable. `ScheduleWakeup` caps at one hour, so chain wakeups for a longer reset. When the loop resumes, inspect status before resubmitting; the interrupted turn may already have landed a submission.

Report verdicts as they arrive rather than saving them for a summary, and commit each accept in the same turn you report it — a verdict held for a tidier batch summary is a verdict the user did not get. Under refill this is also what keeps the pool full, so batching up the bookkeeping stalls the run itself.
