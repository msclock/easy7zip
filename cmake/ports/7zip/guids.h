//
// Using 7zip as shared library requires GUIDs to be instanciated, they are
// already available with static library.
//
// Add the following in a source file:
//
//
//   #ifndef _7ZIP_STATIC_DEFINE
//     #include <7zip/CPP/Common/MyInitGuid.h>
//     #include <7zip/guids.h>
//   #endif  // _7ZIP_STATIC_DEFINE
//
//
// More details from Microsoft's documentation:
//
// The Initguid.h header file redefines the DEFINE_GUID macro to instantiate
// GUIDs (versus just declaring an EXTERN reference). Include this header
// file in the source file where the GUIDs should be instantiated.
//


// Those id include by issuing the include <7zip/CPP/Common/MyInitGuid.h>

// DEFINE_GUID(IID_ISequentialInStream, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00);

// DEFINE_GUID(IID_ISequentialOutStream, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00);

// DEFINE_GUID(IID_IStreamGetProp, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00);

// DEFINE_GUID(IID_IStreamGetProps, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00);

// DEFINE_GUID(IID_IStreamGetProps2, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00);

// DEFINE_GUID(IID_IInStream, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00);

// DEFINE_GUID(IID_IOutStream, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00);

// DEFINE_GUID(IID_IStreamGetSize, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00);

// DEFINE_GUID(IID_ICompressProgressInfo, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00);

// DEFINE_GUID(IID_ICryptoGetTextPassword, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00);

// DEFINE_GUID(IID_ICryptoGetTextPassword2, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x11, 0x00, 0x00);

// DEFINE_GUID(IID_ISetProperties, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x00, 0x00);

// DEFINE_GUID(IID_IArchiveOpenCallback, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x10, 0x00, 0x00);

// DEFINE_GUID(IID_IArchiveExtractCallback, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x20, 0x00, 0x00);

// DEFINE_GUID(IID_IInArchive, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x60, 0x00, 0x00);

// DEFINE_GUID(IID_IArchiveUpdateCallback, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0x00, 0x00);

// DEFINE_GUID(IID_IArchiveUpdateCallback2, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x82, 0x00, 0x00);

// DEFINE_GUID(IID_IOutArchive, 0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0xA0, 0x00, 0x00);

#ifndef __7ZIP_GUID_H_
#define __7ZIP_GUID_H_

const GUID CLSID_CFormatZip = {0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x01, 0x00, 0x00};

const GUID CLSID_CFormat7z = {0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00};

#endif // _7ZIP_GUID_H_
