# Qwen POJ live trial

## Outcome

On 2026-08-12/13, `qwen3.6:latest` running locally through Ollama solved POJ 2380, Sales Report. Its first and only judge submission was Accepted as run `25197306` in 381 ms and 2520 KB with a code length of 1892 bytes.

The trial succeeded as a model benchmark but exposed substantial latency, workflow-compliance, and Codex/Ollama integration problems. Qwen derived a viable algorithm and corrected a sample-visible implementation bug, but it did not independently complete the repository's submission workflow safely. The parent had to interrupt an unsafe browser attempt, constrain a restart, perform the authorized submission fallback, and feed the Accepted row back for archival.

The accepted solution is `solutions/2380/25197306_AC_381MS_2520K.cpp`, committed with the `TODO` strike as `addfb81`. The model rank change is committed separately as `d95379c`.

## Trial configuration

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

## Useful launch configuration

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

## Measured inference performance

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

## Codex and Ollama compatibility findings

Every local launch emitted a model-catalog error because Codex expected a response with a top-level `models` field while Ollama returned an OpenAI-style object with a `data` array:

```text
failed to decode models response: missing field `models`
body: {"object":"list","data":[{"id":"qwen3.6:latest",...}]}
```

Inference still worked, but Codex warned that model metadata was unavailable and used fallback metadata. The warning repeated approximately every three minutes as the model manager refreshed.

The first agent launch used automatic approval review. A live POJ read was rejected before execution because the reviewer request failed with `unknown input item type: "additional_tools"`. Qwen retried and received the same rejection. No network request or submission occurred.

Workspace sandboxing with `sandbox_workspace_write.network_access=true` allowed ordinary POJ `curl` reads without escalation. With `approval_policy="never"`, Qwen first tried the manual's escalation path, was correctly denied, then retried the ordinary curl and succeeded.

AppleScript did not work from the nested workspace sandbox. The exact `open-submit-tab` helper failed with `Connection Invalid error for service com.apple.hiservices-xpcservice` and an AppleScript syntax error before creating a tab. The same helper worked when the parent ran it through the approved external execution path.

## Solver behavior

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

## Browser safety and submission handling

The initial network-enabled Qwen process did not follow the retained-ID transport contract. After solving locally, it wrote an ad hoc scratch `submit.sh` and attempted to address Chrome's active tab directly with custom AppleScript. The malformed command failed with a syntax error before changing Chrome. The parent interrupted the process immediately, deleted the unsafe scratch script, verified the pinned window's active tab and tab list, and confirmed authoritative status remained empty.

A constrained restart explicitly prohibited direct `osascript`, active-tab addressing, direct POSTs, and custom submission scripts. It required only the repository helpers and an immediate infrastructure report if an exact helper failed. Qwen then followed that restriction, invoked `open-submit-tab`, received the nested-sandbox AppleScript failure, and handed back an infrastructure report with zero submissions and no tab IDs.

The parent-owned fallback first confirmed an empty authoritative baseline. Its initial submit-form check then found `forms.length === 1`, meaning the POJ session had expired. The helper closed its temporary tab and the run paused for the user to log back in.

After login, the parent revalidated the exact pinned window, closed the preflight tab, took another empty status baseline, generated the guarded payload from Qwen's unchanged 1892-byte source, opened a just-in-time retained tab, submitted under `with-submit-lock`, closed the exact tab after completed `/status` navigation, and polled authoritative status after ten seconds.

The accepted attempt's tab ledger was `created=1542221067, closed=1542221067`. The preflight tab was also closed. No untracked POJ tab remained.

## Authoritative result and repository changes

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

## Conclusions and next-trial recommendations

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
