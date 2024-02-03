from __future__ import annotations

import importlib.metadata

import py7zip as m


def test_version():
    assert importlib.metadata.version("py7zip") == m.__version__
