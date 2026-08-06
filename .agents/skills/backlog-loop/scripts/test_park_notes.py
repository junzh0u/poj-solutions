from __future__ import annotations

import io
import contextlib
import tempfile
import types
import unittest
from pathlib import Path
from importlib.machinery import SourceFileLoader

SCRIPT = Path(__file__).with_name("park-notes")
MODULE = types.ModuleType("park_notes")
MODULE.__file__ = str(SCRIPT)
SourceFileLoader(MODULE.__name__, str(SCRIPT)).exec_module(MODULE)

ParkNoteError = MODULE.__dict__["ParkNoteError"]
eligibility = MODULE.__dict__["eligibility"]
main = MODULE.__dict__["main"]
parse_note = MODULE.__dict__["parse_note"]
parse_ranks = MODULE.__dict__["parse_ranks"]
park_status = MODULE.__dict__["park_status"]
render_front_matter = MODULE.__dict__["render_front_matter"]

RANKS = {"weak": 10, "mid": 20, "strong": 30, "other": 20}

MODEL_PARK = """---
problem: 2125
park: model
parked: 2026-08-06
solvers:
  - mid 2026-08-06 5 Wrong Answer
---

# POJ 2125

Body text.
"""

JUDGE_PARK = """---
problem: 2096
park: judge
parked: 2026-08-06
solvers:
  - mid 2026-08-06 3 Wrong Answer
recheck: http://poj.org/status?problem_id=2096&result=0
---

Body.
"""


class ParseTest(unittest.TestCase):
    def test_front_matter_round_trips(self) -> None:
        note = parse_note(MODEL_PARK, "2125")
        self.assertEqual(note["park"], "model")
        self.assertEqual(note["solvers"], [
            {"model": "mid", "date": "2026-08-06", "submissions": 5, "verdict": "Wrong Answer"}
        ])
        self.assertEqual(
            render_front_matter(note), MODEL_PARK[: MODEL_PARK.index("---\n\n") + 4]
        )

    def test_missing_front_matter_is_loud(self) -> None:
        with self.assertRaisesRegex(ParkNoteError, "no park record"):
            parse_note("# 2125\n\nprose only\n", "2125")

    def test_unknown_key_is_rejected(self) -> None:
        text = MODEL_PARK.replace("park: model", "park: model\nreason: typo-prone")
        with self.assertRaisesRegex(ParkNoteError, "unknown key"):
            parse_note(text, "2125")

    def test_filename_and_front_matter_must_agree(self) -> None:
        with self.assertRaisesRegex(ParkNoteError, "filename says"):
            parse_note(MODEL_PARK, "2126")

    def test_unknown_park_kind_is_rejected(self) -> None:
        with self.assertRaisesRegex(ParkNoteError, "park must be one of"):
            parse_note(MODEL_PARK.replace("park: model", "park: maybe"), "2125")

    def test_judge_park_must_carry_its_recheck(self) -> None:
        text = "\n".join(
            line for line in JUDGE_PARK.splitlines() if not line.startswith("recheck:")
        )
        with self.assertRaisesRegex(ParkNoteError, "recheck"):
            parse_note(text + "\n", "2096")

    def test_malformed_solver_entry_is_rejected(self) -> None:
        text = MODEL_PARK.replace("mid 2026-08-06 5 Wrong Answer", "mid, five WAs")
        with self.assertRaisesRegex(ParkNoteError, "solver entry must be"):
            parse_note(text, "2125")

    def test_duplicate_rank_is_rejected(self) -> None:
        with self.assertRaisesRegex(ParkNoteError, "listed twice"):
            parse_ranks("10 mid\n20 mid\n")


class EligibilityTest(unittest.TestCase):
    def test_stronger_model_is_owed_the_retry(self) -> None:
        note = parse_note(MODEL_PARK, "2125")
        self.assertEqual(eligibility(note, RANKS, "strong")[0], "retry")

    def test_same_model_is_not_owed_a_retry(self) -> None:
        note = parse_note(MODEL_PARK, "2125")
        self.assertEqual(eligibility(note, RANKS, "mid")[0], "final")

    def test_equal_rank_across_vendors_claims_nothing(self) -> None:
        note = parse_note(MODEL_PARK, "2125")
        self.assertEqual(eligibility(note, RANKS, "other")[0], "final")

    def test_weaker_model_is_not_owed_a_retry(self) -> None:
        note = parse_note(MODEL_PARK, "2125")
        self.assertEqual(eligibility(note, RANKS, "weak")[0], "final")

    def test_strongest_attempt_decides_not_the_last_one(self) -> None:
        note = parse_note(
            MODEL_PARK.replace(
                "  - mid 2026-08-06 5 Wrong Answer",
                "  - strong 2026-08-06 5 Wrong Answer\n  - weak 2026-08-07 5 Wrong Answer",
            ),
            "2125",
        )
        self.assertEqual(eligibility(note, RANKS, "mid")[0], "final")

    def test_judge_park_is_never_offered_to_a_stronger_model(self) -> None:
        note = parse_note(JUDGE_PARK, "2096")
        state, reason = eligibility(note, RANKS, "strong")
        self.assertEqual(state, "final")
        self.assertIn("judge park", reason)

    def test_unranked_model_needs_triage_rather_than_a_silent_skip(self) -> None:
        note = parse_note(MODEL_PARK.replace("mid 2026", "claude-next 2026"), "2125")
        self.assertEqual(eligibility(note, RANKS, "strong")[0], "triage")


class ParkStatusTest(unittest.TestCase):
    def test_park_is_provisional_while_a_stronger_model_exists(self) -> None:
        note = parse_note(MODEL_PARK, "2125")
        self.assertEqual(park_status(note, RANKS), ("owed", "owed a retry on strong"))

    def test_park_is_final_once_the_strongest_model_has_failed(self) -> None:
        note = parse_note(MODEL_PARK.replace("mid 2026", "strong 2026"), "2125")
        self.assertEqual(park_status(note, RANKS)[0], "final")

    def test_judge_park_is_final_from_the_start(self) -> None:
        note = parse_note(JUDGE_PARK, "2096")
        self.assertEqual(park_status(note, RANKS)[0], "final")


class CommandTest(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.attempts = Path(self.tmp.name) / "attempts"
        self.attempts.mkdir()
        self.ranks = Path(self.tmp.name) / "model-ranks.txt"
        self.ranks.write_text("10 weak\n20 mid\n30 strong\n")

    def invoke(self, *arguments: str) -> tuple[int, str, str]:
        out, err = io.StringIO(), io.StringIO()
        argv = [
            "park-notes",
            *arguments,
            "--attempts-dir",
            str(self.attempts),
            "--ranks",
            str(self.ranks),
        ]
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = main(argv)
        return code, out.getvalue(), err.getvalue()

    def test_list_for_model_prints_only_owed_ids(self) -> None:
        (self.attempts / "2125.md").write_text(MODEL_PARK)
        (self.attempts / "2096.md").write_text(JUDGE_PARK)

        code, out, _ = self.invoke("list", "--for-model", "strong")
        self.assertEqual((code, out), (0, "2125\n"))

    def test_unranked_scanning_model_blames_itself_not_the_notes(self) -> None:
        (self.attempts / "2125.md").write_text(MODEL_PARK)
        (self.attempts / "2096.md").write_text(JUDGE_PARK)

        code, out, err = self.invoke("list", "--for-model", "unheard-of")
        self.assertEqual((code, out), (2, ""))
        self.assertIn("unheard-of is not in the model ranks file", err)
        self.assertNotIn("2125", err)
        self.assertNotIn("2096", err)

    def test_list_reports_triage_loudly(self) -> None:
        (self.attempts / "2125.md").write_text("# prose only\n")

        code, _, err = self.invoke("list", "--for-model", "strong")
        self.assertEqual(code, 4)
        self.assertIn("2125", err)

    def test_record_prepends_a_park_record_and_keeps_the_body(self) -> None:
        (self.attempts / "2125.md").write_text("# POJ 2125\n\nBody text.\n")

        code, out, _ = self.invoke(
            "record", "2125", "--model", "mid", "--kind", "model",
            "--submissions", "5", "--verdict", "Wrong Answer", "--date", "2026-08-06",
        )
        self.assertEqual(code, 0)
        self.assertIn("model\towed\towed a retry on strong", out)
        self.assertEqual(self.invoke("list", "--for-model", "strong")[1], "2125\n")
        self.assertTrue(
            (self.attempts / "2125.md").read_text().endswith("# POJ 2125\n\nBody text.\n")
        )

    def test_record_appends_the_retry_and_can_reclassify(self) -> None:
        (self.attempts / "2125.md").write_text(MODEL_PARK)

        self.invoke(
            "record", "2125", "--model", "strong", "--kind", "judge",
            "--submissions", "2", "--verdict", "System Error", "--date", "2026-08-08",
            "--recheck", "http://poj.org/status?problem_id=2125&result=0",
        )
        note = parse_note((self.attempts / "2125.md").read_text(), "2125")
        self.assertEqual([a["model"] for a in note["solvers"]], ["mid", "strong"])
        self.assertEqual(note["park"], "judge")
        self.assertEqual(note["parked"], "2026-08-06")

    def test_record_refuses_a_model_that_is_not_ranked(self) -> None:
        (self.attempts / "2125.md").write_text("# POJ 2125\n")

        code, _, err = self.invoke(
            "record", "2125", "--model", "claude-next", "--kind", "model",
            "--submissions", "5", "--verdict", "Wrong Answer",
        )
        self.assertEqual(code, 2)
        self.assertIn("model ranks file", err)

    def test_record_refuses_a_judge_park_without_a_recheck(self) -> None:
        (self.attempts / "2096.md").write_text("# POJ 2096\n")

        code, _, err = self.invoke(
            "record", "2096", "--model", "mid", "--kind", "judge",
            "--submissions", "3", "--verdict", "Wrong Answer",
        )
        self.assertEqual(code, 2)
        self.assertIn("--recheck", err)

    def test_check_fails_on_a_note_with_no_record(self) -> None:
        (self.attempts / "2125.md").write_text("# prose only\n")

        code, _, err = self.invoke("check")
        self.assertEqual(code, 1)
        self.assertIn("2125", err)


if __name__ == "__main__":
    unittest.main()
