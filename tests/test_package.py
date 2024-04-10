from __future__ import annotations

from importlib_metadata import version

import pysmooth7zip as m


def test_version():
    assert version("pysmooth7zip") == m.__version__
