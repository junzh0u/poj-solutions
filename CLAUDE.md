@AGENTS.md

# Claude Code specifics

## Working the backlog unattended

`/loop` with no interval self-paces: each firing runs one cycle, and the loop continues because the solve agents are harness-tracked, so their completions re-invoke the parent automatically. The `ScheduleWakeup` between cycles is only a fallback for a hung agent — 1800s, not a polling interval.

The prompt carries all the run's state, because it is passed back verbatim on every firing and so is the only thing that survives a context compaction:

```
/loop solve 5 problems, stop when two cycles in a row park without an accept. Spawn subagents with model "sonnet" until the first park happens; from the batch after that first park, go back to the default (opus) model, and re-run the parked id on opus in that batch, handing the agent its _attempts_ notes. If a cycle dies to usage or rate limits, that is not a park: do not write attempt files, do not count it toward the stop condition, wait for the limit to reset and rerun the same ids.
```

Each clause exists for a reason:

- **A stop condition is mandatory.** `TODO` holds thousands of ids, so "solve 5 problems" on a loop never terminates on its own. Two barren cycles is the signal that the backlog has outrun the setup.
- **A usage limit is not a park.** It kills the agent before it says anything about the problem, so parking it would file an empty write-up and skip the problem forever. Wait out the reset and rerun the same ids. `ScheduleWakeup` caps at an hour, so a longer reset needs chained wakeups.
- **A model park is not a problem park.** When a cheaper model is being trialled, a problem it could not finish still deserves a run on the stronger one; the `_attempts_` file goes to that agent as its starting point rather than retiring the problem.

Report each verdict as it lands rather than batching them at the end of a cycle — the user is watching an unattended run and a silent hour is indistinguishable from a stalled one.
