---
name: backlog-loop
description: Run the POJ TODO backlog unattended in Claude Code — compose a self-paced /loop prompt with a bounded stop condition, model escalation, and usage-limit recovery. Use when the user asks Claude to work or keep solving the backlog without supervision, run overnight, solve problems until progress stalls, or invokes /backlog-loop.
---

# Work the backlog unattended

Read `../../../.agents/skills/backlog-loop/references/policy.md` completely before starting. Follow `AGENTS.md` for problem selection, solving, submission, parking, and commits; this skill only adds Claude-specific unattended orchestration.

## Compose the loop

Invoke `/loop` with no interval so each firing runs one cycle and agent completions re-invoke the parent. The prompt must carry the complete run state because it is passed back verbatim on every firing and survives context compaction.

Default to batches of five and stop after two consecutive confirmed barren cycles unless the user specifies otherwise. Baseline:

```text
/loop solve 5 problems, stop when two confirmed cycles in a row produce no accept. Spawn subagents with model "sonnet" until the first model park. Starting with the following batch, go back to the default model (opus) and rerun that parked id with its _attempts_ notes before continuing. A usage or rate limit is not a park: do not create attempt notes or advance the barren-cycle counter; wait for the reset and retry the same ids. Report each verdict as it arrives. Do not push.
```

Adapt the batch size, stop condition, and model policy to the user's request without dropping the safety rules in the shared policy.

Preflight the submit path yourself before spawning, per `AGENTS.md`. It is the one failure that would otherwise hit every agent in the batch at once, and it costs one tab — keep that tab rather than closing it as `AGENTS.md` says, since on this harness it becomes the keepalive tab below. Loaded browser tools with no connected Chrome are not a submission path either — check that a browser is actually attached before reading the form. A failed preflight is an infrastructure stop: no attempt notes, no barren-cycle progress, the ids stay eligible.

Preflight is also where the browser gets chosen. With several connected Chromes, probe them for the logged-in form and record the `deviceId` of the one that has it, per `CLAUDE.md`; that id is cycle-scoped state the agents cannot rediscover for themselves, so it goes in every task prompt and stays fixed for the whole batch.

`sonnet` is the right default for the top of the backlog — those problems are the most-solved ones and are textbook by construction. Escalate on a park, not on a hunch. A five-problem batch off the top of the backlog has gone five for five, every one Accepted on its first submission.

## What every solve-agent task must carry

`AGENTS.md` is the canonical copy of the guarded snippet, the form facts, and the dropped-click rule; relay its §3–4 text **verbatim** in the task prompt rather than paraphrasing it here. On top of that, each task prompt must carry:

- The **exact model identifier** assigned to that agent, and the requirement to report its finalized `// Model:` line alongside the verdict. The transport refuses to build a payload from a source that does not declare it exactly once, and a provenance error found after Accepted cannot be fixed without either another submission or a falsified archive.
- The claude-in-chrome transport rules in `CLAUDE.md` — in particular that every click, first attempt and retry alike, is reserved through `.agents/skills/backlog-loop/scripts/with-submit-lock` as the tool call immediately preceding it. Agents that space their clicks by their own finishing times are not coordinating.
- The **preflighted `deviceId`**, with the requirement to `select_browser` it before opening any tab and to keep it for every retry. An agent handed no device picks one, and a batch spread across two Chromes submits half its solutions from a logged-out one.
- The requirement to report **every tab id it created and every one it closed**, not just the tab that carried the successful click. Agents that recover from tab-group churn create replacement tabs, and only their own report distinguishes a tab they closed from one the group teardown stranded.

## Own the tab group's lifetime

The parent holds a **keepalive tab** in the MCP tab group: open it before spawning the first agent, keep it for the whole cycle, close it after the last agent has reported. Everything in the batch shares that one group, and the moment any agent closes what the extension counts as its last tab the group is auto-removed — which ungroups every sibling tab still inside rather than closing it, stranding each one beyond the reach of `tabs_close_mcp` and leaving the user to close it by hand. The keepalive tab is the whole fix: the group never reaches zero, so it is never removed. See `CLAUDE.md` for the mechanism and the measurement.

Do not audit agent tabs from the parent by id while the batch runs. Membership of the shared group flickers as agents create and close tabs, and a parent reading it mid-batch can see the full set, a stale subset, or no group at all — none of which is evidence about any particular agent. Audit afterwards instead: require every agent to report the tab ids it created and closed, and treat created-minus-closed as the leak count. Report any leak to the user with the ids, since only they can clear it.

## Recover from stalls

Use `ScheduleWakeup` only as a fallback for a hung agent or rate-limit reset, not as the cycle interval. For a normal hung-agent fallback, schedule 1800 seconds.

On a usage limit, arm the wakeup before refining the reset time because the environment may be partially unavailable. `ScheduleWakeup` caps at one hour, so chain wakeups for a longer reset. When the loop resumes, inspect status before resubmitting; the interrupted turn may already have landed a submission.

Report verdicts as they arrive rather than waiting for the cycle summary, and commit each accept in the same turn you report it — a verdict held for a tidier batch summary is a verdict the user did not get.
