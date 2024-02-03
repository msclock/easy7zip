#pragma once

#include <cstdio>
#define INITGUID
#include <7zip/Archive/HandlerCont.h>
#include <7zip/Archive/IArchive.h>
#include <7zip/Archive/XzHandler.h>

enum {
    kId_Zip = 1,
    kId_BZip2 = 2,
    kId_7z = 7,
    kId_Xz = 0xC,
    kId_Tar = 0xEE,
    kId_GZip = 0xEF,
};

STDAPI CreateObject(const GUID *clsid, const GUID *iid, void **outObject);

#define DEFINE_GUID_ARC(CLSID_Format, id) \
    Z7_DEFINE_GUID(CLSID_Format, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, id, 0x00, 0x00);

DEFINE_GUID_ARC(CLSID_7z, kId_7z)

bool t() {
    CMyComPtr<IInArchive> archive;
    if (CreateObject(&CLSID_7z, &IID_IInArchive, (void **)&archive) != S_OK) {
        return false;
    }
    return true;
}
