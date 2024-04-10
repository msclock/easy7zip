#include "gtest/gtest.h"

#include <_core.hpp>
#include "utils.hpp"

TEST(_core, version) {
    auto version = _core::ProjectVersion();
    EXPECT_TRUE(!version.empty());
}

using _core_pysmooth7zip_test = test::utils::rc_dir_test;

TEST_F(_core_pysmooth7zip_test, compress) {}

TEST_F(_core_pysmooth7zip_test, archive_writer) {}
