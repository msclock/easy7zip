from __future__ import annotations

import importlib.metadata

import pysmooth7zip as m


def test_version():
    assert importlib.metadata.version("pysmooth7zip") == m.__version__
