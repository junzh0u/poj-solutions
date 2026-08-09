# Solve-run cost baselines

Dollar accounting of /solve runs, one section per run, normalized to the same token categories and list-rate pricing so runs stay comparable across models. For Claude runs, `scripts/run-cost` reads the parent session transcript and the solve agents' task transcripts, dedupes assistant messages by id, and prices input / output / cache-write (5m and 1h) / cache-read tokens. For Codex runs, take the final `event_msg.payload.info.total_token_usage` from the parent and each solver transcript, subtract `cached_input_tokens` from `input_tokens` to get uncached input, and price the three categories separately. Billed tokens are dominated by cache traffic, so harness-reported agent or goal token figures without the cache split are not comparable to anything here.

Two rules keep runs comparable. **Start the run from a clean session** (`/clear`, then `/solve`): the parent window filters which messages count, but every counted turn's cache read re-bills the entire conversation prefix, so pre-run work inflates the parent tally on every verdict (run 2 paid ≈$7.7 for a prefix it never used). **Measure as the parent reports the last verdict** (or later — the transcripts persist):

```sh
.claude/skills/solve/scripts/run-cost --session <session-id> --model <opus-5|fable-5|sonnet-5> \
  --parent-model <the session model, when it differs from the agents'> \
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
- **Dirty-context caveat** (found after run 3): this session had 110 assistant messages of docs-refactor work before the `/solve` invocation, so the parent entered the run on a 159k-token context — a clean start reads ~22k (run 3) or 0 (run 1, fresh session). Window filtering picks which *messages* count, but every counted turn's cache read re-bills the whole prefix, so the 56 parent turns carried ≈7.7M pre-run tokens ≈ **$7.68 that belongs to the earlier conversation**. Corrected: **≈$49.6 total, ≈$5.51 per accept, ≈2.5M billed tokens per accept**. The block above is the raw measurement; the corrected figures are the comparable ones.

## Run 3 — claude-sonnet-5 agents, claude-fable-5 parent, 2026-08-08 (session `d467ba35-01ca-4c8a-9bc4-42fa534e3d5d`)

Pool of five, refilling, target 10 with drain overshoot. 15 problems attempted over ~37 minutes (21:38–22:15 UTC), 16 judged submissions: **14 accepts, all first-submission**, plus 3801 (two System Errors across two compilers, verified and parked as `judge` — POJ's judge for that problem has accepted nobody since 2025-11-27). 2568 gated by `spawn-precheck` at no agent cost. Second run under the post-refactor docs — same docs as run 2, different model, which is the comparison run 2 asked for.

**Mixed-model run**: agents on sonnet-5, parent on fable-5 (the session model). Measured before `run-cost` grew its `--parent-model` flag, so the parent below was re-priced at fable rates by hand; mixed runs are now the norm — always pass `--parent-model` with the session model when it differs from the agents'.

```
SOLVE AGENTS (15 transcripts, sonnet-5)       PARENT (run window only, fable-5 rates)
  input               1,120   $  0.00          input                 114   $  0.00
  output             98,084   $  1.47          output             32,581   $  1.63
  cache write     2,189,845   $  8.21          cache w(1h)       130,915   $  2.62
  cache read     44,244,419   $ 13.27          cache read      5,081,881   $  5.08
  TOTAL          46,533,468   $ 22.96          TOTAL           5,245,491   $  9.33
                 (560 assistant msgs)                          (58 assistant msgs)

RUN TOTAL: $32.29 at list rates   (agents 71%, parent 29%)   PER ACCEPT (14): $2.31
INTRO-PRICED (agents ×2/3 through 2026-08-31; fable parent unchanged): $24.64 total, $1.76 per accept
```

Per-agent at list rates (problem, cost, messages): 2951 $1.04/31 · 2400 $2.39/48 · 3801 $2.70/73 (park) · 3484 $1.43/39 · 3654 $1.05/30 · 2464 $1.95/41 · 2705 $1.11/27 · 1838 $0.99/33 · 3645 $0.84/25 · 3283 $0.98/28 · 2463 $1.01/23 · 2003 $2.43/50 · 1987 $1.80/42 · 3339 $1.71/39 · 3328 $1.53/31.

- 51.8M tokens billed; **3.70M billed tokens per accept** — apparently matching run 2's 3.34M, but that match is an artifact of run 2's dirty-context inflation (see its caveat). The clean comparison is agents-only, which pre-run context cannot touch: under identical docs, fable agents billed **1.97M tokens per accept** to sonnet's **3.32M** (1.7×). So the run 1→2 token drop was docs *and* model — fable is genuinely more token-frugal, and sonnet wins on price, not frugality.
- Throughput: ~23 accepts/hour, the fastest run yet, with the cleanest record (14/14 first-submission; the only non-accept was a broken judge, not a failed solve).
- The parent's token count dropped to 5.2M (vs 48.7M run 1, 12.3M run 2) — shorter run, leaner reports — but at fable rates it still carried 29% of spend. A cheaper parent model is the obvious next lever: the same 5.2M parent tokens at sonnet rates would have been $2.80, saving $6.53 (~20% of the run).
- The expensive agents were the ones that iterated on context, not verdicts: 3801 ($2.70, judge park with two submissions), 2003 ($2.43, deep discuss-board archaeology), 2400 ($2.39, statement-swap investigation). No Wrong Answer iterations at all this run.

## Run 4 — gpt-5.6-terra agents, gpt-5.6-sol parent, 2026-08-08 (session `019fe3e9-d52b-7421-84a8-ac564fbf21fe`)

Pool of three, refilling, target 10 with drain overshoot. 12 problems attempted over ~14 minutes, 12 judged submissions: **12 accepts, all first-submission**. 3566 was gated by `spawn-precheck` at no agent cost. The clean session used the post-refactor docs, persistent solver threads, retained-ID Chrome submission, Terra agents, and a Sol parent.

Codex's three solver threads were reused through `followup_task`, so their final cumulative usage covers 4, 3, and 5 problems rather than one transcript per problem. The transcripts record no cache-write tokens; uncached input is total input minus cached input. Pricing uses the model pages' rates recorded on the run date: [Terra](https://developers.openai.com/api/docs/models/gpt-5.6-terra) at $2/M uncached input, $0.20/M cached input, and $12/M output; [Sol](https://developers.openai.com/api/docs/models/gpt-5.6-sol) at $5/M, $0.50/M, and $30/M respectively.

```
SOLVE AGENTS (3 transcripts, terra)           PARENT (sol)
  uncached input      330,031   $ 0.66          uncached input      117,689   $ 0.59
  cached input      9,906,432   $ 1.98          cached input      4,206,848   $ 2.10
  output               61,709   $ 0.74          output               11,857   $ 0.36
  TOTAL             10,298,172   $ 3.38          TOTAL              4,336,394   $ 3.05

RUN TOTAL: $6.43 at list rates   (agents 53%, parent 47%)   PER ACCEPT (12): $0.54
```

Per-thread at list rates (problems, cost): 3761/1231/2323/3715 $1.10 · 1174/3449/3109 $1.19 · 3327/2005/1794/3782/3073 $1.09.

- 14.63M tokens billed; **1.22M billed tokens per accept**, 67% fewer than run 3.
- Agents billed **0.86M tokens per accept**, 74% fewer than run 3's 3.32M.
- Throughput: ~50 accepts/hour, about 2.2× run 3; the narrower three-agent pool still finished 12 accepts in ~14 minutes.
- Every attempted problem accepted on its first submission. The only other backlog resolution was the pre-spawn Special Judge gate, which consumed no solver transcript or submission.
- The parent was 30% of tokens but 47% of spend because Sol cost 2.5× Terra across every token category.
- Goal mode reported 99,799 execution tokens over 14m17s. That counter is retained as an operational metric but not used for pricing because it does not expose the cache split; the cache-aware transcript total is the comparable number.

## Cross-run comparison

| | Run 1 (opus-5) | Run 2 (fable-5) | Run 3 (sonnet-5) | Run 4 (terra/sol) |
|---|---|---|---|---|
| Docs | pre-refactor | post-refactor | post-refactor | post-refactor |
| Clean parent context | yes (fresh session) | **no — 110 prior msgs, see caveat** | yes (right after /clear) | yes (fresh session) |
| Accepts | 12 (of 13 attempted) | 9 (of 9) | 14 (of 15; the 15th was a broken judge) | 12 (of 12; one additional id gated) |
| First-submission rate | 12/12 accepts (1 problem failed outright) | 8/9 | 14/14 | 12/12 |
| Total cost | $67.09 | $57.28 raw / ≈$49.6 corrected | $32.29 list / $24.64 intro (fable parent) | $6.43 list |
| **Per accept** | **$5.59** | **$6.36 raw / ≈$5.51 corrected** | **$2.31 list / $1.76 intro** | **$0.54** |
| Billed tokens per accept | 7.38M | 3.34M raw / ≈2.5M corrected | 3.70M | 1.22M |
| Agents-only tokens per accept | 3.33M | 1.97M | 3.32M | 0.86M |
| Wall clock per accept | ~6.0 min | ~2.9 min | ~2.6 min | ~1.2 min |
| Agents : parent split (of spend) | 52 : 48 | 57 : 43 | 71 : 29 (by tokens 90 : 10 — the fable parent is 10% of tokens, 29% of cost) | 53 : 47 (by tokens 70 : 30) |

Reading: with run 2's dirty-context carriage removed (≈$7.7 of parent cache reads re-billing the pre-run conversation), fable lands at ≈$5.51 per accept — **parity with opus at 2× the per-token price** — and it ran twice as fast with a cleaner record (no failed problem; opus's one failure alone cost $6.27). **Confound:** run 2 is also the first run on the refactored docs, which shrank every context, so its token drop vs run 1 is model + docs together. Run 3's clean head-to-head on the agents side (identical docs, and pre-run context can't touch agent transcripts): fable 1.97M tokens per accept vs sonnet 3.32M — so the drop was docs *and* model, fable being the most token-frugal of the three. Sonnet's advantage is price, not frugality: 1.7× fable's agent tokens at 30% of fable's rates still lands 58% cheaper per accept. Problem sets also differ (consecutive slices of `TODO`, so roughly comparable difficulty).

Run 4 sets the new cost and throughput baseline: versus run 3 it used 67% fewer billed tokens and cost 77% less per accept while running about 2.2× faster. Its perfect first-submission record helped, but run 3's accepted problems were also all first-submission; the remaining confounds are model/provider accounting, a three-wide rather than five-wide pool, persistent multi-problem Codex solver threads, and the next consecutive slice of `TODO`.

Prices used (list, $/MTok uncached input/output): opus-5 $5/$25, fable-5 $10/$50, sonnet-5 $3/$15, gpt-5.6-terra $2/$12, gpt-5.6-sol $5/$30. Terra cached input was $0.20/MTok; Sol cached input was $0.50/MTok.
