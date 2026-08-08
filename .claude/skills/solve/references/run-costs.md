# Solve-run cost baselines

Dollar accounting of /solve runs, one section per run, measured identically so runs stay comparable across models. The measurement is `scripts/run-cost`: it reads the parent session transcript and the solve agents' task transcripts, dedupes assistant messages by id, and prices input / output / cache-write (5m and 1h) / cache-read tokens at the model's list rates. Billed tokens are dominated by cache traffic, so harness-reported `subagent_tokens` figures (which ignore it) are not comparable to anything here.

To measure the next batch, run it as its parent reports the last verdict (or later — the transcripts persist):

```sh
.claude/skills/solve/scripts/run-cost --session <session-id> --model <opus-5|fable-5|sonnet-5> \
  --solved <accepts> --agents <solver-agent-ids, comma-separated>
```

`--agents` keeps non-solver transcripts (search agents, etc.) out of the tally; the default parent window (the `/solve` invocation through the loop stop) keeps unrelated same-session work out. Sonnet-5 preset uses list rates ($3/$15); through 2026-08-31 intro pricing ($2/$10) actually bills — scale by 2/3 if measuring a sonnet run before then.

## Run 1 — claude-opus-5, 2026-08-08 (session `6e1fcce7-4388-438e-9c4e-81516b4ae662`)

Pool of five, refilling. 13 problems attempted over ~72 minutes (19:32–20:44 UTC), 13 submissions: **12 accepts, all first-submission**, plus 3003 (Wrong Answer, killed, later gated under special-judge). 17 agent transcripts (includes resumed/killed agents). Run under the **pre-refactor docs** — the parent and every agent carried the full old AGENTS.md + CLAUDE.md as shared context.

```
SOLVE AGENTS (17 transcripts)                 PARENT (orchestration)
  input               1,012   $  0.01          input                 214   $  0.00
  output             96,375   $  2.41          output             85,477   $  2.14
  cache write     2,175,830   $ 13.60          cache w(1h)       612,083   $  6.12
  cache read     37,640,305   $ 18.82          cache read     47,990,201   $ 24.00
  TOTAL          39,913,522   $ 34.83          TOTAL          48,687,975   $ 32.25
                 (506 assistant msgs)                         (108 assistant msgs)

RUN TOTAL: $67.09   (agents 52%, parent 48%)      PER ACCEPT (12): $5.59
```

- 88.6M tokens billed; **7.38M billed tokens per accept**.
- Cache reads were 64% of spend ($42.82). The parent alone cost as much as all thirteen solves combined — it re-reads a growing conversation on every verdict at 1-hour cache TTL (2× write price).
- Per-agent range $1.38–$6.27; the killed 3003 agent was the most expensive at $6.27 with nothing to show.
- Throughput: ~10 accepts/hour.
- Scope note: measured over the whole session transcript mid-session ($67.09); re-measured after the session closed it reads $69.26 / $5.77 per accept, the delta being the session's own closing turns. The recorded $67.09 is the run figure.

## Run 2 — claude-fable-5, 2026-08-08 (session `ffbe951d-3217-4cd1-bb0c-aca9453397d7`)

Pool of five, refilling, target 5 with drain overshoot. 9 problems attempted over ~26 minutes (21:07–21:33 UTC), 10 submissions: **9 accepts (8 first-submission; 2836 took two)**. 3103 and 2837 gated by `spawn-precheck` at no agent cost. First run under the **post-refactor docs** (audience-split: solver manual in the agent definition, parent manual in the skill, 38-line AGENTS.md).

```
SOLVE AGENTS (9 transcripts)                  PARENT (run window only)
  input                 472   $  0.00          input                 111   $  0.00
  output             50,765   $  2.54          output             34,931   $  1.75
  cache write     1,096,950   $ 13.71          cache w(1h)       546,098   $ 10.92
  cache read     16,596,075   $ 16.60          cache read     11,755,129   $ 11.76
  TOTAL          17,744,262   $ 32.85          TOTAL          12,336,269   $ 24.42
                 (236 assistant msgs)                          (56 assistant msgs)

RUN TOTAL: $57.28   (agents 57%, parent 43%)      PER ACCEPT (9): $6.36
```

Per-agent (problem, cost, messages): 1677 $3.05/20 · 2795 $3.24/25 · 3710 $3.31/23 · 2044 $3.59/24 · 2836 $6.12/40 · 3528 $3.96/28 · 3622 $3.41/28 · 3393 $3.26/23 · 2415 $2.91/25.

- 30.1M tokens billed; **3.34M billed tokens per accept** — 2.2× fewer than run 1.
- Throughput: ~21 accepts/hour, double run 1.
- The one two-submission solve (2836) again cost roughly double a clean solve ($6.12 vs ~$3.3 median) — a Wrong Answer iteration is the single biggest per-problem cost driver in both runs.
- Scope note: parent window includes two mid-run user Q&A exchanges (about `with-submit-lock`) and the post-run doc-improvement commit; solver transcripts are exactly the nine `poj-solver` agents.

## Run 1 vs run 2

| | Run 1 (opus-5) | Run 2 (fable-5) |
|---|---|---|
| Accepts | 12 (of 13 attempted) | 9 (of 9) |
| First-submission rate | 12/12 accepts (1 problem failed outright) | 8/9 |
| Total cost | $67.09 | $57.28 |
| **Per accept** | **$5.59** | **$6.36 (+14%)** |
| Billed tokens per accept | 7.38M | 3.34M (−55%) |
| Wall clock per accept | ~6.0 min | ~2.9 min |
| Agents : parent split | 52 : 48 | 57 : 43 |

Reading: at 2× the per-token price, fable landed within 14% of opus per accept because it billed 2.2× fewer tokens per accept, and it ran twice as fast with a cleaner record (no failed problem; opus's one failure alone cost $6.27). **Confound:** run 2 is also the first run on the refactored docs, which shrank every context (solver manual no longer duplicated in shared context; parent no longer carries solve-procedure text), so the token drop is model + docs together — not attributable to the model alone. Run 3 on a different model under the same docs will separate the two. Problem sets also differ (consecutive slices of `TODO`, so roughly comparable difficulty, run 2's marginally less-solved).

Prices used (list, $/MTok in/out): opus-5 $5/$25, fable-5 $10/$50, sonnet-5 $3/$15.
