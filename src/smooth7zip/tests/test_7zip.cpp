#include <gtest/gtest.h>
#include "smooth7zip.hpp"

#ifndef _7ZIP_STATIC_DEFINE
#include <7zip/CPP/Common/MyInitGuid.h>
#include <7zip/guids.h>
#endif // _7ZIP_STATIC_DEFINE

#include <7zip/CPP/7zip/Archive/HandlerCont.h>
#include <7zip/CPP/Common/MyCom.h>

TEST(verify, CreateObject) {
    CMyComPtr<IInArchive> ia;
    EXPECT_EQ(CreateObject(&CLSID_CFormatZip, &IID_IInArchive, (void**)&ia), S_OK);

    CMyComPtr<IOutArchive> oa;
    EXPECT_EQ(CreateObject(&CLSID_CFormatZip, &IID_IOutArchive, (void**)&oa), S_OK);
}

// TODO GetMethodProperty

TEST(verify, GetNumberOfFormats) {
    uint32_t number;
    EXPECT_EQ(GetNumberOfFormats(&number), S_OK);
    EXPECT_GE(number, 0);
}

// TODO GetHandlerProperty2

TEST(verify, GetNumberOfMethods) {
    uint32_t number;
    EXPECT_EQ(GetNumberOfMethods(&number), S_OK);
    EXPECT_GE(number, 0);
}
