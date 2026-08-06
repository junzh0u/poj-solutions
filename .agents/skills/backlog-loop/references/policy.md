# Unattended backlog policy

- Keep a fixed number of solve agents in flight and refill each slot as soon as it frees, rather than waiting for a whole batch to finish. Follow `AGENTS.md` for selection, concurrent ownership, submission limits, attempt notes, commits, and `TODO` edits.
- Require a finite stop condition. Because a refilling pool has no batch boundary to count, count resolved problems: default to stopping after **ten consecutive resolutions with no Accepted solution**, which is the same reach as two barren batches of five. Reset the counter on any accept.
- Count only genuine problem parks toward that counter. A cheaper model failing is a model park, not a problem park: retry that id on the stronger model with its `attempts/<id>.md` notes before retiring or counting it.
- Treat usage and rate limits as interruptions, not verdicts or parks. Do not create attempt notes, advance the no-accept counter, or select replacement ids. Resume the same ids after the reset.
- Preflight a working and authorized submission path before spawning solve agents. Submission-control failures are infrastructure interruptions, not verdicts or parks; do not create attempt notes or advance the no-accept counter.
- Pin the exact browser profile and window selected by preflight for the whole run, including agents spawned into refilled slots. Agents must not rediscover or switch to a newly opened window; losing the preflighted target is a submission-control interruption.
- Create each problem's retained submission tab only after its annotated source and guarded payload are ready. Wait for that exact tab to load and verify its URL, logged-in form count, and problem id before every locked click; keep the canonical in-call guard as the atomic final check.
- Validate the assigned model identifier against the finalized source comment before building any submission payload. Provenance errors discovered after Accepted cannot be repaired without either falsifying the archive or spending another submission.
- Serialize every actual submit click, including retries, with a process-scoped shared lock and a last-attempt timestamp. Fixed delays from independently reached readiness points are not submission coordination.
- Keep the exact retained tab alive until the submit transport confirms that its navigation finished on POJ's status page. A synchronous click return or the first URL change to `/submit` is not a completed POST handoff.
- Make status checks fail visibly, use the shared status helper instead of ad hoc HTML parsing, and confirm the expected page before interpreting rows. A transport or parser failure is unknown state, not evidence that no submission landed.
- Confirm external state before retrying after any interruption. A submit click, browser error, or lost agent response can hide a submission that already landed.
- Capture a fail-visible status baseline before every click and preserve distinct before/after evidence for each attempt so a rowless click or ambiguous browser result can be diagnosed without inference.
- Report every verdict when it arrives instead of batching updates until the end of the run.
- Keep shared-state work in the parent. Solve agents must not commit or edit `TODO`.
- Never push unless the user explicitly asks for that push.
