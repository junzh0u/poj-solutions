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

## Recover from stalls

Use `ScheduleWakeup` only as a fallback for a hung agent or rate-limit reset, not as the cycle interval. For a normal hung-agent fallback, schedule 1800 seconds.

On a usage limit, arm the wakeup before refining the reset time because the environment may be partially unavailable. `ScheduleWakeup` caps at one hour, so chain wakeups for a longer reset. When the loop resumes, inspect status before resubmitting; the interrupted turn may already have landed a submission.

Report verdicts as they arrive rather than waiting for the cycle summary.
