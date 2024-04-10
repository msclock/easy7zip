from __future__ import annotations

from pysmooth7zip import _core


def test_core_version():
    assert _core.version() != ""
