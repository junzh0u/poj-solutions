---
name: backlog-loop
description: Run the POJ TODO backlog unattended in Codex Goal mode, with concurrent solve agents, a bounded stop condition, model escalation, and rate-limit recovery. Use when the user asks Codex to work or keep solving the backlog without supervision, run overnight, solve problems until progress stalls, or invokes $backlog-loop.
---

# Work the backlog unattended

Read [references/policy.md](references/policy.md) completely before starting. Follow `AGENTS.md` for problem selection, solving, submission, parking, and commits; this skill only adds unattended orchestration.

## Start or continue the goal

Use Goal mode, not a fixed-interval scheduler. If this skill was not invoked from `/goal`, create a goal from the user's request and the policy below. Keep the assembled goal text complete enough to survive context compaction.

Default to batches of five and stop after two consecutive confirmed barren cycles unless the user specifies otherwise. A complete baseline goal is:

```text
Use $backlog-loop to solve the POJ backlog in batches of 5, following AGENTS.md and spawning one solve subagent per problem concurrently. Stop after two consecutive confirmed cycles produce no Accepted solution. Use gpt-5.6-terra solve agents until the first model park. Starting with the following batch, use gpt-5.6-sol and rerun that parked problem with its _attempts_ notes before continuing. A usage or rate limit is not a park: do not create attempt notes or advance the barren-cycle counter; keep the goal active and retry the same problem ids after the reset. Report each verdict as it arrives. Do not push.
```

Adapt the batch size, stop condition, and model policy to the user's request without dropping the safety rules in the shared policy.

## Run each cycle

Select the next batch exactly as `AGENTS.md` specifies, then spawn all solve agents in one concurrent batch. Use `gpt-5.6-terra` until its first model park; from the next batch onward, use `gpt-5.6-sol` and include the parked id for its required stronger-model retry.

Keep the goal active between cycles. Do not mark it complete until the stop condition is satisfied, and do not mark it blocked merely because work is slow, a subagent is still running, or a usage reset must pass. On resumption, inspect status and the worktree before retrying so an already-judged submission or completed agent is not duplicated.

Report verdicts as they arrive. After all agents finish, make the parent-only commits and `TODO` edits required by `AGENTS.md`, update the run state, and immediately begin the next cycle unless the goal is complete.
