from __future__ import annotations

import types
import unittest
from collections.abc import Callable
from importlib.machinery import SourceFileLoader
from pathlib import Path
from typing import TypedDict, cast


class StatusRow(TypedDict):
    run_id: str
    user: str
    problem: str
    result: str
    memory: str
    time: str
    language: str
    code_length: str
    submit_time: str


SCRIPT = Path(__file__).with_name("status-via-curl")
MODULE = types.ModuleType("status_via_curl")
SourceFileLoader(MODULE.__name__, str(SCRIPT)).exec_module(MODULE)
parse_status_html = cast(
    Callable[[bytes], list[StatusRow]], MODULE.__dict__["parse_status_html"]
)


class ParseStatusHtmlTest(unittest.TestCase):
    def test_nested_markup_is_parsed_into_named_cells(self) -> None:
        document = b"""
            <title>Problem Status List</title>
            <tr align=center><td>25194776</td><td><a>150014</a></td>
            <td><a>2127</a></td><td><font color=blue>Accepted</font></td>
            <td>376K</td><td>11MS</td><td>C++</td><td>2160B</td>
            <td>2026-08-06 14:43:12</td></tr>
        """

        self.assertEqual(
            parse_status_html(document),
            [
                {
                    "run_id": "25194776",
                    "user": "150014",
                    "problem": "2127",
                    "result": "Accepted",
                    "memory": "376K",
                    "time": "11MS",
                    "language": "C++",
                    "code_length": "2160B",
                    "submit_time": "2026-08-06 14:43:12",
                }
            ],
        )

    def test_valid_page_can_have_no_rows(self) -> None:
        self.assertEqual(parse_status_html(b"<title>Problem Status List</title>"), [])

    def test_nested_centered_layout_row_is_ignored(self) -> None:
        document = b"""
            <title>Problem Status List</title>
            <tr align=center><td><table>
            <tr align=center><td>25194778</td><td>150014</td><td>2259</td>
            <td>Accepted</td><td>5132K</td><td>331MS</td><td>C++</td>
            <td>1710B</td><td>2026-08-06 14:46:39</td></tr>
            </table></td></tr>
        """

        self.assertEqual(parse_status_html(document)[0]["run_id"], "25194778")

    def test_missing_page_marker_is_unknown_state(self) -> None:
        with self.assertRaisesRegex(ValueError, "not a POJ"):
            parse_status_html(b"<html>temporary proxy response</html>")

    def test_malformed_row_is_unknown_state(self) -> None:
        with self.assertRaisesRegex(ValueError, "malformed"):
            parse_status_html(
                b"<title>Problem Status List</title><tr align=center><td>1</td></tr>"
            )


if __name__ == "__main__":
    unittest.main()
