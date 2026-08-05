---
name: backlog-loop
description: Run the POJ TODO backlog unattended in Codex Goal mode, with concurrent solve agents, a bounded stop condition, model escalation, and rate-limit recovery. Use when the user asks Codex to work or keep solving the backlog without supervision, run overnight, solve problems until progress stalls, or invokes $backlog-loop.
---

# Work the backlog unattended

Read [references/policy.md](references/policy.md) completely before starting. Follow `AGENTS.md` for problem selection, solving, submission, parking, and commits; this skill only adds unattended orchestration.

## Start or continue the goal

Use Goal mode, not a fixed-interval scheduler. If this skill was not invoked from `/goal`, create a goal from the user's request and the policy below. Keep the assembled goal text complete enough to survive context compaction.

Default to batches of three, which fills the three solve-agent slots available alongside the parent, and stop after two consecutive confirmed barren cycles unless the user specifies otherwise. A complete baseline goal is:

```text
Use $backlog-loop to solve the POJ backlog in batches of 3, following AGENTS.md and spawning one solve subagent per problem concurrently. Before spawning, preflight one working, authorized browser submission path. Stop after two consecutive confirmed cycles produce no Accepted solution. Use gpt-5.6-terra solve agents until the first model park. Starting with the following batch, use gpt-5.6-sol and rerun that parked problem with its _attempts_ notes before continuing. A submission-control interruption is not a park: do not create attempt notes or advance the barren-cycle counter; keep the same problem ids eligible after the interruption clears. On a Codex usage or rate limit, abort the run safely without notes, replacement ids, or barren-cycle progress; do not wait for a five-hour reset. Report each verdict as it arrives. Do not push.
```

Adapt the batch size, stop condition, and model policy to the user's request without dropping the safety rules in the shared policy.

## Preflight submission control

Do this once in the parent before spawning a cycle; do not make every solve agent rediscover the same missing capability after it has finished a solution.

1. Inspect the enabled tool inventory for `tabs_create_mcp`, `select_browser`, and browser JavaScript execution. Tool instructions may describe these names even when the current runtime does not actually expose them, so verify availability rather than assuming it from `AGENTS.md`.
2. If the browser tools are exposed, open a POJ submit page and verify that `document.forms[2]` exists in a logged-in Chrome session. Close the preflight tab without changing the form.
3. If the browser tools are absent, the supported fallback is Chrome AppleScript. Before using it, the user must enable Chrome's View > Developer > Allow JavaScript from Apple Events setting and explicitly authorize both AppleScript control and submitting solutions to the user's POJ account. Enabling the Chrome setting alone is not submission authorization. Spawn submission agents only after the authorization so their inherited user context contains it directly; relaying the authorization to an older agent is insufficient for the external-action reviewer.
4. Preflight AppleScript read-only without interrupting the user: choose an existing normal Chrome window, save its window id and active-tab index, create the submit page as a tab at the end, retain the new tab's unique id, and restore the saved active-tab index immediately. Verify the logged-in form by addressing that exact background tab id, then close only that tab. Do not create or activate a new window, and do not rely on the frontmost tab because concurrent agents can change it.
5. If neither path passes preflight, do not spawn the cycle. A missing browser tool, disabled Chrome setting, logged-out session, or rejected external-action authorization is an infrastructure interruption: it creates no `_attempts_` note, does not advance the barren-cycle counter, and leaves the same problem ids eligible to resume.

Tell every solve agent which preflighted control path to use. For AppleScript, tell it to retain an existing window id plus its own background tab's unique id, restore the user's prior active-tab index, execute the guarded JavaScript against that exact tab specifier, and close only that tab after the click/status check.

## Give every solve agent the submission recipe

Every solve-agent task must tell the agent to read `AGENTS.md` and must relay its §3 guarded JavaScript and §3–4 submission/confirmation rules **verbatim** — `AGENTS.md` is the canonical copy of the snippet, the form facts, and the dropped-click rule; do not paraphrase them here or in task prompts. What this skill adds is the Codex transport mapping:

1. Fail-visible curl for statements, discuss pages, and verdicts stays exactly as `AGENTS.md` specifies. Chrome is submit-only. If curl cannot resolve POJ inside the sandbox, retry the same read-only curl with sandbox escalation; use a retained-ID background Chrome status tab only when escalated curl is unavailable, or stop as an infrastructure interruption.
2. Open `http://poj.org/submit?problem_id=<id>` through the preflighted path: the browser MCP tools if exposed, otherwise a retained-ID background tab in an existing Chrome window via AppleScript.
3. Run the canonical guarded JS in one call against that exact tab/window specifier. Never split planting and clicking across calls. Do **not** interpolate the canonical JavaScript directly into an AppleScript string: AppleScript consumes the snippet's `\r` and `\n` escapes, silently corrupts the regular expression, and `execute javascript` returns `missing value` without ever reaching `.click()`. Run `scripts/build-submit-js <id> <source-file>` and give AppleScript its output only through the escape-safe wrapper `execute <tab> javascript "eval(atob('<helper-output>'))"`. The helper substitutes the id and source into the canonical snippet, then base64-encodes that entire JavaScript payload. Do not hand-build this payload or rerun its browser probe for every problem; verify the helper once after it changes.
4. Prepare the retained tab, guarded payload, and browser action before seeking permission to click; do not use fixed per-agent delays. On the AppleScript path, run the single `osascript` command that executes the guarded JavaScript through `scripts/with-submit-lock <osascript-command> [args ...]` for the first attempt and every retry. The helper takes an advisory lock shared by every local solve agent, waits until 15 seconds after the last attempted click, and uses the shared timestamp file's mtime to reserve and record this attempt. Its lock is released by the OS if an agent dies. Keep only the browser execution inside the helper; the landing wait and status polling happen after it releases the lock. `POJ_SUBMIT_COORDINATOR_DIR` and `POJ_SUBMIT_MIN_INTERVAL` exist for isolated helper tests, not normal runs. Browser MCP calls cannot run as a child of the helper: on that path, the ready agent asks the parent for permission, the parent grants one agent at a time, and the parent does not grant the next click until 15 seconds after the prior agent reports its attempt. Do not reserve with `with-submit-lock true` and then make an MCP call outside the lock.
5. Keep the retained submission tab alive for a full five-second foreground landing window after `execute javascript` returns, even when it returns `missing value`; only then close that tab. The working Chrome transport leaves the tab alive while the POST is handed off, and closing immediately adds a separate cancellation risk.
6. After the landing window, close the retained submission tab and poll status per `AGENTS.md` §4 with curl. Do not keep or create a browser status tab merely for convenience.
7. Treat AppleScript's `missing value` after a click as ambiguous navigation, not proof that a submission landed or failed. Likewise, cancelling the five-second landing window does not prove it stopped before the click; leave the tab alone and recheck authoritative curl status before retrying or closing it.
8. The dropped-click bound (30 seconds, one clean retry, then infrastructure interruption) is `AGENTS.md` §4's rule; the Codex-specific detail is that the retry starts from a fresh retained-ID background tab and also runs through `scripts/with-submit-lock`.

Track every retained submit-tab id in the parent. After each solve agent exits, verify its tracked id is closed; an agent can finish before a queued cleanup message reaches it. The owning agent closes its exact retained tab after the landing window even when a successful click has navigated it from the problem's submit page to POJ's status page. For parent cleanup, require the retained window/tab id plus either the original problem submit URL or the expected POJ status landing URL; never close every POJ or submit tab indiscriminately. At the end of a cycle, require zero tracked tabs left open.

## Run each cycle

Select the next batch exactly as `AGENTS.md` specifies, then spawn all solve agents in one concurrent batch. Use `gpt-5.6-terra` until its first model park; from the next batch onward, use `gpt-5.6-sol` and include the parked id for its required stronger-model retry.

Keep the goal active between successful cycles. Do not mark it complete until the stop condition is satisfied, and do not mark it blocked merely because work is slow or a subagent is still running. On resumption, inspect status and the worktree before retrying so an already-judged submission or completed agent is not duplicated.

Codex has no five-hour reset workflow to wait through. If a solve agent or the parent hits a usage or rate limit, stop spawning and retrying, confirm any in-flight submission from authoritative status when possible, preserve scratch sources and the same problem ids, leave `TODO` and `_attempts_` unchanged for unfinished work, do not advance the barren-cycle counter, and abort the current run with a concise handoff.

Report verdicts as they arrive. After all agents finish, make the parent-only commits and `TODO` edits required by `AGENTS.md`, update the run state, and immediately begin the next cycle unless the goal is complete.
