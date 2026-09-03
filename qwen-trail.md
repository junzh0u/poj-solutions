# Qwen POJ live trials

## Trial 1: qwen3.6 through Ollama

### Outcome

On 2026-08-12/13, `qwen3.6:latest` running locally through Ollama solved POJ 2380, Sales Report. Its first and only judge submission was Accepted as run `25197306` in 381 ms and 2520 KB with a code length of 1892 bytes.

The trial succeeded as a model benchmark but exposed substantial latency, workflow-compliance, and Codex/Ollama integration problems. Qwen derived a viable algorithm and corrected a sample-visible implementation bug, but it did not independently complete the repository's submission workflow safely. The parent had to interrupt an unsafe browser attempt, constrain a restart, perform the authorized submission fallback, and feed the Accepted row back for archival.

The accepted solution is `solutions/2380/25197306_AC_381MS_2520K.cpp`, committed with the `TODO` strike as `addfb81`. The model rank change is committed separately as `d95379c`.

### Trial configuration

- Machine: Apple M1 Max MacBook Pro with 10 CPU cores and 64 GB unified memory.
- Local service: Ollama installed and run as a Homebrew service.
- Model: `qwen3.6:latest`, manifest ID `07d35212591f`, 23 GB on disk.
- Loaded footprint: 26 GB, reported by `ollama ps` as 100% GPU.
- Codex CLI: `0.147.0`.
- Repository rank: 5, below every previously ranked solver so every established model can retry a Qwen park.
- Trial shape: one fresh backlog problem, no refill, five-submission cap, no push.
- Selected problem: POJ 2380, the first fresh ID in `TODO` at trial start.
- Preflight: repository clean, park notes valid, no Qwen-owed retries, problem not Special Judge, pinned Chrome submit form initially logged in and valid.
- Scratch root: a fresh `/tmp/poj-solve-*` directory, with all Qwen work confined to its problem subdirectory until Accepted.

### Useful launch configuration

Codex has native Ollama support, so no OpenAI-compatible proxy was needed:

```sh
codex exec \
  --oss \
  --local-provider ollama \
  --model qwen3.6:latest \
  --ignore-user-config \
  --sandbox workspace-write \
  -c 'approval_policy="never"' \
  -c 'sandbox_workspace_write.network_access=true' \
  -c 'model_context_window=131072' \
  -C /Users/junz/Workspace/poj-solutions \
  '<solver task>'
```

`--approve-for-me` cannot be combined with an explicit `--sandbox` on this CLI build. The equivalent nested automatic reviewer was also unusable in this trial, so the working configuration retained the workspace sandbox, enabled network inside it, and used `approval_policy="never"` to avoid escalation requests.

When resuming an Ollama session, `--model qwen3.6:latest` alone did not preserve the provider and Codex attempted to send the model name to OpenAI. The resume had to pin the provider explicitly:

```sh
codex exec resume \
  --model qwen3.6:latest \
  -c 'model_provider="ollama"' \
  '<session-id>' \
  '<follow-up>'
```

### Measured inference performance

The model loaded in 10.33 seconds. Ollama allocated a 262,144-token context because Codex could not obtain model metadata and fell back to generic values. A later restart overrode the context to 131,072 tokens, but full-context reprocessing remained the dominant cost.

The first prompt contained 31,718 tokens. Ollama reported 112.38 seconds of prompt evaluation at 282.23 tokens/second, followed by 213 generated tokens in 16.68 seconds at 12.77 tokens/second. Total time for that first model turn was 129.06 seconds.

Later generation generally ran around 12-13.6 tokens/second. The raw generation rate was usable; latency was dominated by two other factors:

- Qwen often generated thousands of reasoning tokens before a simple tool action. One observed turn exceeded 3,700 generated tokens while planning an elementary map-based solution.
- Ollama repeatedly logged `forcing full prompt re-processing due to lack of cache data`, attributed by llama.cpp to SWA or hybrid/recurrent memory. Each tool result expanded the conversation and caused another large prefill instead of cheap prefix reuse.

One later handoff turn reprocessed roughly 46,000 prompt tokens before recovering from a failed file copy. The model remained fully GPU-loaded, so this was not CPU fallback.

Codex reported these terminal cumulative token counts for the three distinct Qwen sessions used by the trial:

- Initial approval-reviewer session: 137,357 tokens.
- Main solving session interrupted after the unsafe browser attempt: 402,309 tokens.
- Constrained solver and Accepted-handoff session: 857,739 tokens.

The sum is 1,397,405 Codex-reported tokens. These totals include repeatedly ingested prompt and tool context, not just visible generation. The trial took roughly 34.5 minutes from the first Qwen launch to the Accepted row and roughly 43 minutes through archival, including restarts, infrastructure handling, and the wait for POJ re-login.

### Codex and Ollama compatibility findings

Every local launch emitted a model-catalog error because Codex expected a response with a top-level `models` field while Ollama returned an OpenAI-style object with a `data` array:

```text
failed to decode models response: missing field `models`
body: {"object":"list","data":[{"id":"qwen3.6:latest",...}]}
```

Inference still worked, but Codex warned that model metadata was unavailable and used fallback metadata. The warning repeated approximately every three minutes as the model manager refreshed.

The first agent launch used automatic approval review. A live POJ read was rejected before execution because the reviewer request failed with `unknown input item type: "additional_tools"`. Qwen retried and received the same rejection. No network request or submission occurred.

Workspace sandboxing with `sandbox_workspace_write.network_access=true` allowed ordinary POJ `curl` reads without escalation. With `approval_policy="never"`, Qwen first tried the manual's escalation path, was correctly denied, then retried the ordinary curl and succeeded.

AppleScript did not work from the nested workspace sandbox. The exact `open-submit-tab` helper failed with `Connection Invalid error for service com.apple.hiservices-xpcservice` and an AppleScript syntax error before creating a tab. The same helper worked when the parent ran it through the approved external execution path.

### Solver behavior

Qwen correctly read the live raw HTML and identified the problem as Sales Report. It chose sparse aggregation keyed by item and salespoint, collected sorted row and column IDs, and emitted every required table cell.

Its first implementation grouped the aggregated pairs under the item ID instead of the salespoint ID, producing zeros in all sample data cells. Qwen diagnosed the transposed key from the sample failure, changed the grouping direction, and reproduced the sample output exactly.

Verification was much weaker than the solver contract requires:

- It compiled with plain `g++ -O2`, not `g++ -std=c++98 -pedantic -Wall -Wextra -O2`.
- It ran only the statement sample and a `diff`.
- It did not inspect the discussion board.
- It did not build a stress case or independent reference.
- It captured no test data beyond scratch-only sample files.

The accepted source uses C++11 constructs including `auto` and braced `push_back`, even though the solver manual requires C++98. POJ accepted this exact source under language ID 4, shown as `C++` in the authoritative status row. This proves that those constructs worked for this exact submission on 2026-08-13; it does not establish the full compiler version or invalidate the conservative repository-wide C++98 policy by itself.

A final mechanical check with `g++ -std=c++98 -pedantic -Wall -Wextra -O2` confirmed the contract violation: it emitted three C++11-extension warnings and failed on nested closing angle brackets, braced initialization, and a nested template reference. The sample-only compile had used the compiler's default language mode, which hid all of these failures.

Qwen's memory analysis was optimistic. The program holds a tree map of aggregated pairs, two tree sets, a second map of vectors duplicating the aggregated data, and row/column vectors under a 64 MB limit. It was Accepted on the judge data, but the trial did not establish worst-case memory safety from the published bounds.

The Qwen-authored commit message also overstates two claims: the two-pointer output still performs one iteration per required table cell, and no evidence was gathered that POJ language IDs 0 and 4 compile identically. Accepted certifies the submitted program for POJ's tests, not those broader explanations.

### Browser safety and submission handling

The initial network-enabled Qwen process did not follow the retained-ID transport contract. After solving locally, it wrote an ad hoc scratch `submit.sh` and attempted to address Chrome's active tab directly with custom AppleScript. The malformed command failed with a syntax error before changing Chrome. The parent interrupted the process immediately, deleted the unsafe scratch script, verified the pinned window's active tab and tab list, and confirmed authoritative status remained empty.

A constrained restart explicitly prohibited direct `osascript`, active-tab addressing, direct POSTs, and custom submission scripts. It required only the repository helpers and an immediate infrastructure report if an exact helper failed. Qwen then followed that restriction, invoked `open-submit-tab`, received the nested-sandbox AppleScript failure, and handed back an infrastructure report with zero submissions and no tab IDs.

The parent-owned fallback first confirmed an empty authoritative baseline. Its initial submit-form check then found `forms.length === 1`, meaning the POJ session had expired. The helper closed its temporary tab and the run paused for the user to log back in.

After login, the parent revalidated the exact pinned window, closed the preflight tab, took another empty status baseline, generated the guarded payload from Qwen's unchanged 1892-byte source, opened a just-in-time retained tab, submitted under `with-submit-lock`, closed the exact tab after completed `/status` navigation, and polled authoritative status after ten seconds.

The accepted attempt's tab ledger was `created=1542221067, closed=1542221067`. The preflight tab was also closed. No untracked POJ tab remained.

### Authoritative result and repository changes

```text
Problem: 2380
Run ID: 25197306
Verdict: Accepted
Time: 381MS
Memory: 2520K
Language: C++
Code length: 1892B
Submit time: 2026-08-13 08:04:59
```

The authoritative code length exactly matched the local source size. Qwen copied the source byte-for-byte to `solutions/2380/25197306_AC_381MS_2520K.cpp` and verified its hash. The solve helper committed that archive together with the `TODO` strike as `addfb81`.

The Accepted archival handoff exposed more unnecessary agent work: Qwen first tried to copy into the nonexistent `solutions/2380` directory, then issued two checks against the nonexistent destination before eventually creating the directory and succeeding. Resuming without an explicit provider also briefly targeted OpenAI and failed before model execution; pinning `model_provider="ollama"` corrected it.

### Conclusions and next-trial recommendations

The core answer to the benchmark question is yes: this local Qwen model can solve at least an easy-to-moderate backlog POJ problem, debug a sample-visible mistake, and produce code that passes the live judge on its first submission.

The stronger conclusion is that `qwen3.6:latest` should not yet be trusted as an unattended end-to-end POJ solver under the current Codex/Ollama integration. The biggest issues were not algorithm quality but repeated full-context cost, excessive reasoning, incomplete verification, failure to honor the browser transport contract, and inability to run AppleScript inside the nested sandbox.

For another trial:

1. Keep Qwen at rank 5 until several problems establish a record.
2. Use Qwen only for statement reading, solution generation, and local verification; keep all Chrome, submission, status confirmation, archival, and commits parent-owned.
3. Give Qwen a narrow output contract such as tested source plus a structured verification report, with no browser tools available.
4. Pin `model_provider="ollama"` on every fresh or resumed local session.
5. Use workspace sandboxing with explicit network access rather than automatic escalation review.
6. Set a smaller explicit context window if the model and manual fit, and investigate Codex/Ollama prefix-cache compatibility before running a multi-tool benchmark.
7. Enforce the exact C++98 compile command mechanically before accepting a source handoff.
8. Require at least sample, degenerate cases, near-limit stress, and an independently derived checker or differential test where practical.
9. Record wall time, prompt-evaluation tokens/second, generation tokens/second, total ingested tokens, number of tool turns, and judge result for comparison across local models.
10. Prefer a second problem with more algorithmic substance and an independent brute-force oracle; POJ 2380 establishes feasibility, not broad solver competence.

## Trial 2: qwen3.8-27b-mlx@4bit through LM Studio

### Outcome

On 2026-09-02/03, `qwen3.8-27b-mlx@4bit` running locally through LM Studio solved POJ 3802, Cubist Artwork. The exact 1729-byte source was Accepted on its first actual judge submission as run `25201875` in 14 ms and 256 KB under POJ language `C++`.

The trial was a stronger algorithm-and-verification success than Trial 1. Qwen derived the correct closed form, produced C++98-compatible source, found and repaired two independent-oracle bugs after the differential harness failed, and ultimately verified the solution against samples, exhaustive search, min-cost flow, edge cases, random full-size cases, and a throughput stress. It also obeyed the browser transport boundary: after nested AppleScript failed, it reported infrastructure instead of inventing another submission path.

The end-to-end workflow was still not autonomous. The parent had to start LM Studio's API server, preflight Chrome, perform the actual submission, recover a logged-out POJ session, archive through a separate narrow Qwen handoff, correct one false explanatory sentence in the Qwen-authored commit message, and commit the result. The accepted solution and `TODO` strike are commit `1898df0`.

### Trial configuration and selection

- Model identifier: `qwen3.8-27b-mlx@4bit`.
- LM Studio loaded footprint: 16.08 GB, 208384-token context, parallelism 4, local device, initially `IDLE`.
- Codex CLI: `0.152.1`.
- Trial shape: one fresh backlog problem, no refill, five-submission cap, no push.
- Selected problem: POJ 3802, the first ID in `TODO` at trial start.
- Eligibility: `spawn-precheck 3802` returned `ok`; it was not Special Judge.
- Browser target: pinned normal Chrome window `1542232492`; preflight tab `1542233192` was created and closed without changing the active tab.
- Scratch root: `/tmp/poj-solve-FUzC9I`, with all solver work under its `3802` subdirectory.
- Model rank: absent from `model-ranks.txt`. `park-notes list --for-model qwen3.8-27b-mlx@4bit` correctly refused to infer a rank, so the trial could accept but could not create a model park. Acceptance made that limitation moot; no rank was added from a single result.

The collaboration runtime could not select an arbitrary LM Studio model for its built-in child-agent slot, so the solve agent ran as a native child `codex exec` process with the exact model pinned. This preserved the solver context boundary but exposed several differences from the ordinary custom-agent path.

### LM Studio discovery and launch

The model was loaded in the LM Studio application, but the OpenAI-compatible API server was not running. A sandboxed and then externally permitted request to `127.0.0.1:1234/v1/models` both failed to connect. `lsof` showed an internal LM Studio listener on the run-specific port `41343`, but `/v1/models` there returned 404; it was not the inference API.

`lms ps` confirmed the requested model was loaded. `lms server status` reported that the server was stopped, and `lms server start` started it successfully on port 1234. The server remained running on port 1234 at the end of the trial.

The main solve used:

```sh
codex exec \
  --oss \
  --local-provider lmstudio \
  --model qwen3.8-27b-mlx@4bit \
  --approve-for-me \
  -c 'sandbox_workspace_write.network_access=true' \
  -c 'model_context_window=208384' \
  -C /Users/junz/Workspace/poj-solutions \
  -o /tmp/poj-solve-FUzC9I/3802/final-report.txt \
  '<solver task>'
```

Codex reported `approval: on-request`, a workspace-write sandbox with network enabled, and `reasoning effort: high`. This inherited high effort was a poor fit for the local model: nearly every nontrivial turn spent several minutes generating hidden reasoning before a short tool action.

Resuming the same session required pinning the provider explicitly, just as the Ollama trial did:

```sh
codex exec resume \
  01a0649c-956d-7840-9414-e2ff18eb3ad2 \
  --model qwen3.8-27b-mlx@4bit \
  -c 'model_provider="lmstudio"' \
  -c 'sandbox_workspace_write.network_access=true' \
  -c 'approval_policy="never"' \
  '<follow-up>'
```

Fresh narrow handoffs added `-c 'model_reasoning_effort="low"'`. Low effort reached its first tool call much faster, but the model still over-analyzed simple archival and editorial decisions.

### Codex and LM Studio compatibility findings

Every launch and periodic model-manager refresh emitted the same catalog decoding error. Codex expected a top-level `models` field, while LM Studio returned an OpenAI-style `data` array containing the 4-bit model, an 8-bit sibling, and an embedding model:

```text
failed to decode models response: missing field `models`
body: {"data":[{"id":"qwen3.8-27b-mlx@4bit",...}],"object":"list"}
```

Inference continued with fallback metadata, so the error was noisy but non-fatal. It recurred roughly every four and a half minutes and repeatedly occupied the live output stream.

Nested automatic approval review was unusable. Multiple escalation requests for both a public read-only POJ fetch and the documented AppleScript helper failed before execution with:

```text
invalid_request_error: Invalid type for 'input'
param: input
code: invalid_union
```

The local solver retried variations with and without optional approval fields even after the failure text advised against workarounds. By the solver's count, five escalation attempts failed identically. This was wasted time and an instruction-compliance weakness, although no external command ran.

The workspace sandbox's explicitly enabled network did reach `poj.org`, despite the Codex adapter's normal assumption that POJ DNS is unavailable in a restricted solve agent. A direct sandboxed `curl` fetched the 9327-byte statement in 767 ms. This does not invalidate the adapter for ordinary Codex children; the local process was itself launched from an externally permitted parent process with network enabled, so its effective environment was different.

Apple Events remained blocked inside that nested workspace sandbox. Running the exact `open-submit-tab` helper there failed before creating a tab with `Connection Invalid error for service com.apple.hiservices-xpcservice` and AppleScript error `-2741`. The identical helper worked from the parent through the approved external execution path.

### Inference performance and token use

The trial did not capture LM Studio's per-request prompt-evaluation or generation rates, so no precise tokens-per-second claim is available. The visible behavior was nevertheless clear: first-token and inter-tool latency dominated execution, and the delay repeated on most turns rather than only during initial ingestion.

The main high-effort session ran from approximately 17:12 PDT until its infrastructure report around 18:16 PDT and reported 104040 cumulative tokens. Several individual reasoning turns took roughly five to twelve minutes before one tool action. The source was Accepted around 18:17 PDT, about 65 minutes after launch.

The first same-session Accepted-handoff resume replayed the approximately 104000-token history, disconnected twice, and was interrupted without completing a response. Reusing the verbose solve history was operationally worse than a fresh handoff.

A fresh low-effort archival session still reported 52318 tokens and took roughly 28 minutes. It reread the full solver manual in chunks after one-shot tool output truncation, inspected scratch files and repository examples, repeatedly debated whether routine edge data should be archived, briefly misread the mangled HTML constraints as 100 before recovering the raw `w,d <= 10` and height `<= 20` bounds, and eventually completed the byte-identical archive and commit message.

A final fresh low-effort correction session reported 3768 tokens but spent roughly six minutes after reading a 30-line commit message without emitting the requested edit. It was interrupted, and the parent made the minimal factual correction directly. The three distinct sessions therefore reported 160126 cumulative tokens in total; this excludes useful accounting for the failed high-context resume, whose terminal counter did not advance despite substantial wall time and server work.

From initial launch at approximately 17:12 PDT to the accepted commit at `2026-09-02T19:05:40-07:00`, the full solve, recovery, archival, correction, and commit took about 1 hour 53 minutes. The model's raw ability was adequate, but its current Codex/LM Studio turn economics were poor.

### Problem solving and algorithm quality

Qwen identified POJ 3802 as Cubist Artwork, with `w,d <= 10`, heights up to 20, a 1000 ms limit, and a 65536 KB limit. It correctly modeled the grid as cell heights whose column maxima equal the front view and whose row maxima equal the side view.

After some repeated manual constructions and recounting mistakes, it reached the correct invariant: count every row-maximum and column-maximum requirement separately, then subtract the maximum overlap. One cell can be the chosen maximum for both a row and a column only when their required heights are equal. Requirements of height `v` therefore save `v * min(countFront[v], countSide[v])`, giving:

```text
sum(front) + sum(side)
  - sum over v of v * min(countFront[v], countSide[v])
```

The model supplied both a lower-bound argument through distinct row/column maximum cells and an achievable construction using equal-height pairs plus global-maximum lines for unmatched requirements. It also noticed that the formula is symmetric in the two views, so front/side orientation in the illustrations cannot affect the numeric result.

The final solution is small, uses only C++98-compatible language features, and runs in constant bounded work per dataset. Qwen's own compile command was `c++ -std=c++98 -O2 -Wall -Wextra`, not the procedure's exact `g++ -std=c++98 -pedantic -Wall -Wextra -O2`. After acceptance, the parent ran the exact required command against the byte-identical archive and reproduced every official sample output with an empty diff.

Qwen did not inspect the POJ discuss board, despite the solver procedure requiring it. The eventual commit message initially said no board input was needed; a more precise description is that the board was not checked. The statement, independent proofs, differential references, and Accepted verdict were sufficient for this problem, but success does not excuse the procedural miss.

### Verification behavior

Qwen extracted one official sample input containing nine datasets and the corresponding nine output lines. The raw HTML used CRLF and omitted the final output newline; it normalized both files to LF and added a conventional trailing newline to the output before exact diffing. All nine numeric answers matched.

The model built two independent references: a min-cost-flow maximum-weight bipartite matching and an exhaustive placement search for small dimensions. Their first build/run was usefully fail-visible rather than falsely green:

- The flow reference first failed to compile because it used `Edge(...)` without a matching constructor. Qwen changed this scratch-only reference to brace initialization under the modern default compiler mode.
- After compilation, the flow reference returned no savings because `addEdge` never incremented the forward adjacency count `gc[a]`, so forward edges overwrote one another.
- The brute reference used a 32-bit bitmask for up to 36 row/column positions; shifts beyond bit 31 invoked undefined behavior and produced an impossible result below the proven lower bound.

Qwen treated both disagreements as findings, diagnosed them correctly, replaced the brute mask with per-row maxima, fixed flow edge bookkeeping, rebuilt, and reran the suite. This was the best behavior of the trial: the model did not dismiss rare or inconvenient oracle failures and did not submit on sample-only confidence.

Final verification reported:

- All nine official datasets matched expected output.
- Small official datasets matched the exhaustive reference.
- 400 randomized feasible small cases matched the closed form, exhaustive search, and min-cost-flow reference.
- 300 randomized feasible full-size cases matched the closed form and min-cost-flow reference.
- Hand-constructed 1x1, 2x1, and 2x2 cases matched all three programs.
- 10000 generated 10x10 datasets produced 10000 lines in about 12 ms locally, leaving ample headroom under 1000 ms.

Only the official sample pair was archived under `solutions/3802/test_data/`. The edge, random, and stress data remained scratch-only because they did not catch a candidate bug or settle an ambiguity. The buggy reference programs also remained scratch-only.

### Explanatory-quality findings

The Accepted source's leading comment contains one false sentence: `an optimum has exactly one non-zero cell per column`. An unmatched row maximum can require an additional nonzero cell in a taller column that already has its own maximum; front `[5,1]` and side `[5,5]` is a counterexample. The formula and executable code do not depend on that sentence and were Accepted, but the source cannot be edited after the verdict without breaking the archive's byte-identical guarantee.

The fresh handoff repeated the same false claim in its first commit-message draft even after its earlier solve reasoning had encountered the counterexample shape. Before commit, the parent replaced only that sentence with the correct requirement-counting lower bound: every column and row needs a maximum cell, equal-valued row/column requirements may share, and requirements that cannot share contribute separately. The accepted source itself was not changed.

The archival handoff also violated a narrow `do not use git` directive by running read-only `git log` to inspect recent commit-message style. No repository state changed, but it was unnecessary and should count as an instruction-compliance miss. It also spent disproportionate time inspecting statement text and reference files after being told not to re-solve.

### Browser safety, submission, and recovery

The solver took an empty authoritative baseline for user `150014` and problem 3802, then attempted only the documented retained-ID helper. When nested approval review failed and in-sandbox AppleScript was blocked, it reported an infrastructure interruption with zero submissions, no repository writes, and a tab ledger of none created/none closed. Unlike Trial 1, it did not write an ad hoc browser script, target the active tab, or improvise another transport.

The parent confirmed a second empty baseline and generated the guarded payload from Qwen's unchanged source. The first parent-owned `open-submit-tab` returned `submit readiness failed: 1|missing`, meaning the POJ session was logged out; the helper closed that exact failed tab before returning.

The documented one-time login bootstrap opened and closed background tab `1542233215`, waited for the auto-login userscript, and restored the active tab. The retry opened submission tab `1542233221`, the guarded payload completed navigation to `http://poj.org/status` under `with-submit-lock`, and the parent immediately closed that exact tab. After the required ten-second delay, authoritative status returned Accepted with code length `1729B`, exactly matching the local file.

All tracked tabs were reconciled:

- Preflight: `1542233192` created and closed.
- Login bootstrap: `1542233215` created and closed.
- Accepted submission: `1542233221` created and closed.
- Local solver: no tabs created or closed.

### Archival and repository result

The original high-context session could not complete the Accepted handoff because replaying its large history caused two stream reconnects. A fresh low-effort Qwen handoff copied the exact source to `solutions/3802/25201875_AC_14MS_256K.cpp`, copied the official sample pair to `solutions/3802/test_data/3802.in` and `3802.out`, and wrote the commit-message file.

`cmp` succeeded for the source and both sample files. The source hash matched on both sides:

```text
76705aaf07967b6785a7e657d6bcb4ae41a204d396211f25bbf8fef5d050c473
```

The parent corrected the one false explanatory sentence in the scratch commit message, validated its final model trailer, and ran `commit-accept`. Commit `1898df0` contains exactly four state changes: the solution source, two official sample files, and the removal of `3802` from `TODO`. Nothing was pushed.

The authoritative result was:

```text
Problem: 3802
Run ID: 25201875
Verdict: Accepted
Time: 14MS
Memory: 256K
Language: C++
Code length: 1729B
POJ submit time: 2026-09-03 09:17:37
```

### Conclusions and recommendations

The core benchmark result is positive: `qwen3.8-27b-mlx@4bit` solved a nontrivial optimization problem, produced portable accepted code, and demonstrated materially better verification discipline than the earlier `qwen3.6:latest` trial. Its response to broken oracles was especially good.

The model is still unsuitable for an unattended end-to-end POJ pool in this configuration. The limiting factors are repeated multi-minute reasoning turns, enormous accumulated context, expensive resume behavior, periodic catalog errors, broken nested approval review, unavailable nested AppleScript, missed instructions, skipped discussion-board review, and excessive analysis of deterministic handoff tasks.

For future LM Studio trials:

1. Start and verify `lms server` before launching Codex; a loaded model does not imply the API is running.
2. Pin `--local-provider lmstudio` on fresh sessions and `model_provider="lmstudio"` on resumes.
3. Use low reasoning effort from the first turn unless the problem demonstrably needs more; high effort added large latency without a corresponding correctness benefit here.
4. Keep the local model's role narrow: statement, algorithm, source, and verification report. Keep Chrome, authoritative status, archival, and git parent-owned until nested approval and Apple Events are reliable.
5. Avoid resuming a verbose solve history for deterministic handoff work. Start a fresh narrow context containing only the authoritative row, exact source path, algorithm summary, and verification facts.
6. Mechanically enforce the exact C++98 compile command before submission rather than relying on the model's remembered command.
7. Require the discuss-board step explicitly and verify that it happened in the report.
8. Preserve fail-visible independent checks; the two broken references proved the value of treating differential disagreement as evidence rather than noise.
9. Add a compact structured output contract and hard turn/token limits for planning. The model should act after a bounded analysis rather than spending minutes re-litigating archive policy.
10. Keep `qwen3.8-27b-mlx@4bit` absent from `model-ranks.txt` until a park requires an explicit comparative rank or more solves provide a defensible record.
11. Treat Accepted as proof of the submitted program, not of every explanatory comment. Review model-authored prose for claims not exercised by the judge.
