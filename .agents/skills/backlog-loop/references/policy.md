# Unattended backlog policy

- Treat one batch of problems as one cycle. Follow `AGENTS.md` for selection, concurrent ownership, submission limits, attempt notes, commits, and `TODO` edits.
- Require a finite stop condition. Default to stopping after two consecutive confirmed cycles with no Accepted solution; reset the counter whenever a cycle accepts anything.
- Count only genuine problem parks toward a barren cycle. A cheaper model failing is a model park, not a problem park: retry that id on the stronger model with its `_attempts_` notes before retiring or counting it.
- Treat usage and rate limits as interruptions, not verdicts or parks. Do not create attempt notes, advance the barren-cycle counter, or select replacement ids. Resume the same ids after the reset.
- Confirm external state before retrying after any interruption. A submit click, browser error, or lost agent response can hide a submission that already landed.
- Report every verdict when it arrives instead of batching updates at the end of a cycle.
- Keep shared-state work in the parent. Solve agents must not commit or edit `TODO`.
- Never push unless the user explicitly asks for that push.
