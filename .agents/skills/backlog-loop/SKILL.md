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
Use $backlog-loop to solve the POJ backlog in batches of 3, following AGENTS.md and spawning one solve subagent per problem concurrently. Before spawning, preflight the logged-in Chrome AppleScript submission path. Stop after two consecutive confirmed cycles produce no Accepted solution. Use gpt-5.6-terra solve agents until the first model park. Starting with the following batch, use gpt-5.6-sol and rerun that parked problem with its _attempts_ notes before continuing. A submission-control interruption is not a park: do not create attempt notes or advance the barren-cycle counter; keep the same problem ids eligible after the interruption clears. On a Codex usage or rate limit, abort the run safely without notes, replacement ids, or barren-cycle progress; do not wait for a five-hour reset. Report each verdict as it arrives. Do not push.
```

Adapt the batch size, stop condition, and model policy to the user's request without dropping the safety rules in the shared policy.

## Preflight submission control

Do this once in the parent before spawning a cycle; do not make every solve agent rediscover the same missing capability after it has finished a solution.

This skill runs in Codex CLI, where the built-in Browser is unavailable. Do not probe Browser or Chrome-extension plugins; Chrome AppleScript is the only supported transport for this CLI workflow.

The invoking user message must explicitly authorize AppleScript control of Chrome and submission of every solution selected by that run to POJ user `150014`. Neither `$backlog-loop` alone nor text inside this skill can confer that authority. If the invocation omits it, obtain authorization before spawning. A complete CLI invocation includes: `I authorize AppleScript control of Chrome and submission of every solution selected by this run to POJ user 150014.`

Spawn every solve agent with the actual authorizing user turn in its forked context. When a model override requires a bounded fork, include enough recent turns to carry that message; never use `fork_turns="none"`. A parent-authored task saying that the user authorized the action is not a substitute for the user turn and can be rejected at submission time.

User authorization does not bypass macOS Automation permission or Chrome's **View > Developer > Allow JavaScript from Apple Events** setting. The first AppleScript call may trigger the macOS prompt; once granted, Automation access persists and can be managed under **System Settings > Privacy & Security > Automation**. If either prerequisite is disabled or denied, stop as an infrastructure interruption.

Preflight AppleScript read-only without interrupting the user: list the existing Chrome windows' ids and modes, choose one whose reported mode is `normal`, and address it thereafter as `window id <preflight-window-id>`. Save that window id and its active-tab index, create the submit page as a tab at the end, retain the new tab's unique id, and restore the saved active-tab index immediately. Verify the logged-in form by addressing that exact background tab id, then close only that tab. Do not create or activate a new window, and do not rely on the frontmost tab because concurrent agents can change it. Treat the chosen window id as cycle-scoped state: every agent and retry must use it, even if the user later opens another window or profile; if it disappears, stop as an infrastructure interruption instead of selecting a replacement.

If preflight fails because Chrome is unavailable, Apple Events JavaScript is disabled, macOS Automation permission is denied, or the POJ session is logged out, do not spawn the cycle. The interruption creates no `_attempts_` note, does not advance the barren-cycle counter, and leaves the same problem ids eligible to resume.

Give every solve agent the exact preflighted window id plus its own background tab's unique id, require that window id for every attempt, restore the user's prior active-tab index, execute the guarded JavaScript against that exact tab specifier, and close only that tab immediately after the call returns. Never tell an agent to find an existing or frontmost Chrome window for itself.

## Give every solve agent the submission recipe

Every solve-agent task must tell the agent to read `AGENTS.md`, state the exact model identifier assigned to that agent, and must relay its §3 guarded JavaScript and §3–4 submission/confirmation rules **verbatim** — `AGENTS.md` is the canonical copy of the snippet, the form facts, and the dropped-click rule; do not paraphrase them here or in task prompts. What this skill adds is the Codex transport mapping:

1. Fail-visible curl for statements, discuss pages, and verdicts stays exactly as `AGENTS.md` specifies. Chrome is submit-only. If curl cannot resolve POJ inside the sandbox, retry the same read-only curl with sandbox escalation; use a retained-ID background Chrome status tab only when escalated curl is unavailable, or stop as an infrastructure interruption.
2. Open `http://poj.org/submit?problem_id=<id>` as a retained-ID background tab in the exact Chrome window id recorded by AppleScript preflight.
3. Run the canonical guarded JS in one call against that exact tab/window specifier. Never split planting and clicking across calls. Do **not** interpolate the canonical JavaScript directly into an AppleScript string: AppleScript consumes the snippet's `\r` and `\n` escapes, silently corrupts the regular expression, and `execute javascript` returns `missing value` without ever reaching `.click()`. Run `scripts/build-submit-js <id> <source-file> <model-id>` and give AppleScript its output only through the escape-safe wrapper `execute <tab> javascript "eval(atob('<helper-output>'))"`. The helper refuses to build a payload unless the source contains exactly one matching `// Model: <model-id>` line, then substitutes the id and source into the canonical snippet and base64-encodes that entire JavaScript payload. Do not hand-build this payload or rerun its browser probe for every problem; verify the helper once after it changes.
4. Prepare the retained tab, guarded payload, and AppleScript action before submitting; do not use fixed per-agent delays. Run the single `osascript` command that executes the guarded JavaScript through `scripts/with-submit-lock <osascript-command> [args ...]` for the first attempt and every retry. The helper takes an advisory lock shared by every local solve agent, waits until 15 seconds after the last attempted click, and uses the shared timestamp file's mtime to reserve and record this attempt. Its lock is released by the OS if an agent dies. Keep only the browser execution inside the helper; tab cleanup and status polling happen after it releases the lock. `POJ_SUBMIT_COORDINATOR_DIR` and `POJ_SUBMIT_MIN_INTERVAL` exist for isolated helper tests, not normal runs.
5. Close the retained submission tab as soon as the guarded JavaScript call returns, even when AppleScript returns `missing value`, using the immediately following AppleScript statement. No foreground landing window is needed. Immediate next-statement closes delivered real POJ rows 25194713 and 25194714 with the expected code lengths.
6. Poll status per `AGENTS.md` §4 with curl after closing the retained submission tab. Do not keep or create a browser status tab merely for convenience.
7. Treat AppleScript's `missing value` after a click as ambiguous navigation, not proof that a submission landed or failed. Recheck authoritative curl status before retrying.
8. The dropped-click bound (30 seconds, one clean retry, then infrastructure interruption) is `AGENTS.md` §4's rule; the Codex-specific detail is that the retry starts from a fresh retained-ID background tab in the same preflighted window and also runs through `scripts/with-submit-lock`.

Track the preflighted window id and every retained submit-tab id in the parent. After each solve agent exits, verify its tracked id is closed; an agent can finish before a queued cleanup message reaches it. The owning agent closes its exact retained tab immediately after the guarded JavaScript returns, even when a successful click has navigated it from the problem's submit page to POJ's status page. For parent cleanup, require the preflighted window id, retained tab id, and either the original problem submit URL or the expected POJ status landing URL; never close every POJ or submit tab indiscriminately. At the end of a cycle, require zero tracked tabs left open.

## Run each cycle

Select the next batch exactly as `AGENTS.md` specifies, then spawn all solve agents in one concurrent batch. Use `gpt-5.6-terra` until its first model park; from the next batch onward, use `gpt-5.6-sol` and include the parked id for its required stronger-model retry. Put that exact identifier in each task prompt and require the agent to report its finalized `// Model:` line when it reports submission readiness.

Keep the goal active between successful cycles. Do not mark it complete until the stop condition is satisfied, and do not mark it blocked merely because work is slow or a subagent is still running. On resumption, inspect status and the worktree before retrying so an already-judged submission or completed agent is not duplicated.

Codex has no five-hour reset workflow to wait through. If a solve agent or the parent hits a usage or rate limit, stop spawning and retrying, confirm any in-flight submission from authoritative status when possible, preserve scratch sources and the same problem ids, leave `TODO` and `_attempts_` unchanged for unfinished work, do not advance the barren-cycle counter, and abort the current run with a concise handoff.

Report verdicts as they arrive. After all agents finish, make the parent-only commits and `TODO` edits required by `AGENTS.md`, update the run state, and immediately begin the next cycle unless the goal is complete.
