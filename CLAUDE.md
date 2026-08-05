@AGENTS.md

## Claude-in-Chrome transport for the submit step

How the harness-neutral submit contract in `AGENTS.md` §3 maps onto claude-in-chrome. Only these lines should need touching when the extension or its tools change.

- Open the tab with `tabs_create_mcp`, run the whole guarded plant-and-click as **one** `javascript_tool` call, and close it with `tabs_close_mcp` as soon as the click is confirmed.
- If the tools report several connected Chromes and demand a choice, do not stop to ask — a subagent cannot reach the user; `select_browser` with any deviceId. Only one Chrome may hold the POJ login: if the submit form is missing, try the other browser before concluding the session is gone.
- The extension blocks `document.cookie` reads — this is why curl cannot borrow the session and the post happens in-page at all.
- Plain `atob` is the whole payload path. The gzip variant (`DecompressionStream`) shrinks long sources to a third but has thrown `Failed to fetch` in this page context; reach for it only when the plain payload is genuinely unwieldy.
- `get_page_text` truncates at 50 KB by default — the reason the `moreproblem` rebuild re-renders the table into a `<pre>` before extraction.
