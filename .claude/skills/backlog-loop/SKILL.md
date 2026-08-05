---
name: backlog-loop
description: Run the POJ TODO backlog unattended — compose a /loop prompt that carries the run's full state (stop condition, model policy, usage-limit handling), start the self-paced loop, and report verdicts as they land. Use whenever the user wants problems solved without supervision, e.g. "work the backlog", "solve N problems on a loop", "run overnight", "keep solving until it stalls", or "/backlog-loop" — even if they don't mention /loop by name.
---

# Working the backlog unattended

An unattended run is the `loop` skill with **no interval**, so it self-paces: each firing runs one "solve N problems" cycle per AGENTS.md (fan out one agent per problem, park failures as `_attempts_/<id>.md`, commit accepts one per problem). The loop continues because the solve agents are harness-tracked, so their completions re-invoke the parent automatically. The `ScheduleWakeup` between cycles is only a fallback for a hung agent — 1800s, not a polling interval.

## Composing the loop prompt

The prompt carries all the run's state, because it is passed back verbatim on every firing and so is the only thing that survives a context compaction. Baseline:

```
/loop solve 5 problems, stop when two cycles in a row park without an accept. Spawn subagents with model "sonnet" until the first park happens; from the batch after that first park, go back to the default (opus) model, and re-run the parked id on opus in that batch, handing the agent its _attempts_ notes. If a cycle dies to usage or rate limits, that is not a park: do not write attempt files, do not count it toward the stop condition, wait for the limit to reset and rerun the same ids.
```

Adapt the batch size and model policy to what the user asked for, but keep every clause — each exists for a reason:

- **A stop condition is mandatory.** `TODO` holds thousands of ids, so "solve 5 problems" on a loop never terminates on its own. Two barren cycles is the signal that the backlog has outrun the setup.
- **A usage limit is not a park.** It kills the agent before it says anything about the problem, so parking it would file an empty write-up and skip the problem forever. Wait out the reset and rerun the same ids. `ScheduleWakeup` caps at an hour, so a longer reset needs chained wakeups.

  Arm that wakeup **first**, before working out exactly when the reset falls. The limit takes out every agent in the cycle at once and can take the environment part-way down with it — a `date` call to compute a precise delay went out with a classifier outage during one such stall. `ScheduleWakeup` needs no shell, the error text states the reset time, and the delay only has to be an over-estimate because the wake can re-check the clock. Fix the loop, then refine the timing.
- **A model park is not a problem park** — the cheap-model-first escalation rule from AGENTS.md, which is tool-neutral there. In Claude Code the concrete names are: spawn solve agents with model `"sonnet"` until the first park, then revert to the default (`opus`) and re-run the parked id on it with its `_attempts_` notes.

## During the run

Report each verdict as it lands rather than batching them at the end of a cycle — the user is watching an unattended run and a silent hour is indistinguishable from a stalled one.
