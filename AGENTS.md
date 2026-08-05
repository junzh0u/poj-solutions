# POJ Solutions — Agent Instructions

An archive of accepted [POJ](http://poj.org) submissions, one directory per problem ID. Nothing is built or tested by the repo itself; the judge is the test suite.

## Layout

- `<id>/<runId>_AC_<time>MS_<memory>K.<ext>` — one file per accepted submission, named from its POJ status row (time before memory). Several files in a directory means the problem was solved more than once, e.g. in another language.
- `<id>/test_data/<id>.in` / `.out` — sample data, only where it was worth keeping.
- `<id>/tags/<tag>` — empty marker files used as tags; manage them with `./tag.sh` (`tag.sh` lists all tags, `tag.sh <id>` lists a problem's, `tag.sh <id> <tag>` adds one).

## Solving a problem end to end

The whole loop runs through the user's Chrome session, which is already logged in as user `150014`.

### 1. Read the statement

`http://poj.org/problem?id=<id>` — `get_page_text` returns the whole statement including the samples and the limits. Note the time and memory limits; they decide how much the algorithm can afford.

### 2. Write and test locally

Work in the scratchpad, not the repo — only the accepted source gets committed.

- Target **C++98**: POJ's compilers are ancient, so no `auto`, no range-`for`, no `<cstdint>`. Build with `g++ -std=c++98 -pedantic -Wall -Wextra -O2`.
- Reproduce the statement's sample first and `diff` against the expected output verbatim.
- Then go past the sample: a stress case near the stated input limits (`/usr/share/dict/words` is a handy source of real words), the degenerate inputs, and — where the answer is a classification rather than a value — a randomized differential test against a brute-force reference on small instances. POJ reports nothing but the verdict, so anything not caught locally costs a blind submission.
- Judge machines are much slower than a modern laptop; leave several times the stated limit as headroom.

### 3. Submit

The submit form is `document.forms[2]` on `http://poj.org/submit?problem_id=<id>`: fields `problem_id`, `language` (`0=G++ 1=GCC 2=Java 3=Pascal 4=C++ 5=C 6=Fortran`), `source`, and a hidden `encoded=1` because `onsubmit` base64-encodes the textarea. So plant the **plain** source and submit by clicking the real button — `form.submit()` skips `onsubmit` and would post unencoded source under `encoded=1`.

Getting the source into the textarea: gzip+base64 it (`gzip -9nc x.cpp | base64 | tr -d '\n'`) and inflate it inside the page, which keeps the payload about a third of the source:

```js
const bin = Uint8Array.from(atob(B), c => c.charCodeAt(0));
const src = await new Response(new Blob([bin]).stream().pipeThrough(new DecompressionStream('gzip'))).text();
const f = document.forms[2];
f.language.value = '4';
f.source.value = src;
f.elements['submit'].click();
```

Check `f.source.value.length` against the file before clicking, and `Code Length` in the status row after — that is the proof the source arrived intact. Serving the file from a local HTTP server and `fetch`ing it from the page does **not** work: Chrome's private-network access blocks a public page from reaching `127.0.0.1`.

### 4. Check the verdict

`http://poj.org/status?problem_id=<id>&user_id=150014`, first row of `table.a`, cells `Run ID | User | Problem | Result | Memory | Time | Language | Code Length | Submit Time`. Poll until the result leaves `Waiting` / `Compiling` / `Running & Judging`.

### 5. Iterate

Report each verdict as it arrives rather than silently resubmitting.

- **Compile Error** — a C++98 slip; check `Compile Error` on the status page for the message, or retry under language `0` (G++) which is a different compiler from `4` (C++).
- **Wrong Answer** — re-read the statement for the case that was missed, reproduce it locally, then resubmit.
- **Time Limit Exceeded** — profile the local stress case; POJ is slow, so an algorithmic fix usually beats micro-optimisation.
- **Runtime Error** — usually an out-of-bounds index or recursion depth, both reproducible locally under `-fsanitize=address,undefined`.

### 6. Commit

Only after **Accepted**. Copy the exact accepted source to `<id>/<runId>_AC_<time>MS_<mem>K.<ext>`, prefixed with a `// POJ <id> - <Title>` comment block explaining the approach and any ambiguity in the statement.

Commit subject is `<id> <Title>` — plain, no Conventional Commits prefix. The body explains the algorithm and the decisions behind it, not the code. One problem per commit.

Do not push unless asked.
