#include <7zip/7zip.h>
#include <gtest/gtest.h>

#include <7zip/CPP/7zip/Archive/HandlerCont.h>
#include <7zip/CPP/7zip/Archive/IArchive.h>
#include <7zip/CPP/7zip/ICoder.h>
#include <7zip/CPP/Common/MyCom.h>

#include <filesystem>
#include <random>

constexpr auto IdFormatZip = GUID{0x23170F69, 0x40C1, 0x278A, {0x10, 0x00, 0x00, 0x01, 0x10, 0x01, 0x00, 0x00}};
constexpr auto IdFormat7z = GUID{0x23170F69, 0x40C1, 0x278A, {0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00}};

class TempDirTest : public ::testing::Test {
protected:
    std::filesystem::path temp_dir;

    void SetUp() override {
        std::random_device rd;
        std::mt19937_64 gen(rd());

        std::uniform_int_distribution<> dis(1000, 9999);
        auto suffix_dir_name = std::to_string(dis(gen)) + '_' + std::to_string(dis(gen));
        temp_dir = std::filesystem::temp_directory_path() / suffix_dir_name;
        std::filesystem::create_directory(temp_dir);
    }

    void TearDown() override { std::filesystem::remove_all(temp_dir); }
};

TEST(verify, CreateObject) {
    CMyComPtr<IInArchive> ia;
    EXPECT_EQ(CreateObject(&IdFormatZip, &IID_IInArchive, (void **)&ia), S_OK);
    EXPECT_EQ(CreateObject(&IdFormat7z, &IID_IInArchive, (void **)&ia), S_OK);

    CMyComPtr<IOutArchive> oa;
    EXPECT_EQ(CreateObject(&IdFormatZip, &IID_IOutArchive, (void **)&oa), S_OK);
    EXPECT_EQ(CreateObject(&IdFormat7z, &IID_IOutArchive, (void **)&ia), S_OK);
}

TEST(verify, GetNumberOfFormats) {
    uint32_t number;
    EXPECT_EQ(GetNumberOfFormats(&number), S_OK);
    EXPECT_EQ(number, 55); // Default to 55 formats

    for (size_t i = 0; i < number; ++i) {
        PROPVARIANT prop{};
        std::string format;
        GUID id;
        EXPECT_EQ(GetHandlerProperty2(static_cast<UInt32>(i), NArchive::NHandlerPropID::kName, &prop), S_OK);
        ASSERT_TRUE(prop.bstrVal);
        EXPECT_EQ(GetHandlerProperty2(static_cast<UInt32>(i), NArchive::NHandlerPropID::kExtension, &prop), S_OK);
        EXPECT_EQ(GetHandlerProperty2(static_cast<UInt32>(i), NArchive::NHandlerPropID::kUpdate, &prop), S_OK);
        EXPECT_EQ(GetHandlerProperty2(static_cast<UInt32>(i), NArchive::NHandlerPropID::kClassID, &prop), S_OK);
        ASSERT_TRUE(prop.bstrVal);
    }
}

TEST(verify, GetNumberOfMethods) {
    uint32_t number;
    EXPECT_EQ(GetNumberOfMethods(&number), S_OK);
    EXPECT_EQ(number, 24); // Default to 24 methods

    for (size_t i = 0; i < number; ++i) {
        PROPVARIANT prop{0};
        std::string name;
        EXPECT_EQ(GetMethodProperty(static_cast<UInt32>(i), NMethodPropID::kName, &prop), S_OK);
        ASSERT_TRUE(prop.bstrVal);
        EXPECT_EQ(GetMethodProperty(static_cast<UInt32>(i), NMethodPropID::kID, &prop), S_OK);
    }
}

#include <7zip/CPP/7zip/Common/FileStreams.h>
#include <7zip/CPP/7zip/IPassword.h>
#include <7zip/CPP/Common/IntToString.h>
#include <7zip/CPP/Common/MyString.h>
#include <7zip/CPP/Common/MyVector.h>
#include <7zip/CPP/Windows/FileFind.h>
#include <7zip/CPP/Windows/FileIO.h>

class CArchiveUpdateCallback Z7_final : public IArchiveUpdateCallback2,
                                        public ICryptoGetTextPassword2,
                                        public CMyUnknownImp {
    Z7_IFACE_COM7_IMP(IProgress);
    Z7_IFACE_COM7_IMP(IArchiveUpdateCallback);
    Z7_IFACES_IMP_UNK_2(IArchiveUpdateCallback2, ICryptoGetTextPassword2);

public:
    const CObjectVector<NWindows::NFile::NFind::CFileInfo> *dirItems{nullptr};
    bool passwordIsDefined{false};
    bool askPassword{false};
    UString password;
    bool needBeClosed{false};
    FString path_prefix;
    FStringVector failedFiles;
    CRecordVector<HRESULT> failedCodes;
    CRecordVector<UInt64> volumesSizes;
    UString volName;
    UString volExt;

    CArchiveUpdateCallback(const CObjectVector<NWindows::NFile::NFind::CFileInfo> *dirItmes, const FString path_prefix)
        : dirItems{dirItmes},
          passwordIsDefined{false},
          askPassword{false},
          needBeClosed{false},
          path_prefix{path_prefix} {}

    ~CArchiveUpdateCallback() { finilize(); }

    HRESULT finilize() {
        if (needBeClosed) {
            needBeClosed = false;
        }
        return S_OK;
    }
};

Z7_COM7F_IMF(CArchiveUpdateCallback::SetTotal(UInt64)) {
    return S_OK;
}

Z7_COM7F_IMF(CArchiveUpdateCallback::SetCompleted(const UInt64 *)) {
    return S_OK;
}

Z7_COM7F_IMF(
    CArchiveUpdateCallback::GetUpdateItemInfo(UInt32 index, Int32 *newData, Int32 *newProps, UInt32 *indexInArchive)) {
    if (newData) {
        *newData = static_cast<Int32>(true);
    }
    if (newProps) {
        *newProps = static_cast<Int32>(true);
    }

    if (indexInArchive) {
        *indexInArchive = static_cast<UInt32>(static_cast<Int32>(-1));
    }
    return S_OK;
}

Z7_COM7F_IMF(CArchiveUpdateCallback::GetProperty(UInt32 index, PROPID propID, PROPVARIANT *value)) {
    NWindows::NCOM::CPropVariant prop;
    if (propID == kpidIsAnti) {
        prop = false;
        prop.Detach(value);
        return S_OK;
    }

    {
        const NWindows::NFile::NFind::CFileInfo &fi = (*dirItems)[index];
        switch (propID) {
            case kpidPath:
                prop = fs2us(fi.Name);
                break;
            case kpidIsDir:
                prop = fi.IsDir();
                break;
            case kpidSize:
                prop = fi.Size;
                break;
            case kpidCTime:
                PropVariant_SetFrom_FiTime(prop, fi.CTime);
                break;
            case kpidATime:
                PropVariant_SetFrom_FiTime(prop, fi.ATime);
                break;
            case kpidMTime:
                PropVariant_SetFrom_FiTime(prop, fi.MTime);
                break;
            case kpidAttrib:
                prop = static_cast<UInt32>(fi.GetWinAttrib());
                break;
            case kpidPosixAttrib:
                prop = static_cast<UInt32>(fi.GetPosixAttrib());
                break;
            default:
                break;
        }
    }
    prop.Detach(value);
    return S_OK;
}

Z7_COM7F_IMF(CArchiveUpdateCallback::GetStream(UInt32 index, ISequentialInStream **inStream)) {
    RINOK(finilize());

    const NWindows::NFile::NFind::CFileInfo &fi = (*dirItems)[index];

    if (fi.IsDir())
        return S_OK;

    {
        CInFileStream *inStreamSpec = new CInFileStream;
        CMyComPtr<ISequentialInStream> inSequentialInStream(inStreamSpec);
        if (!inStreamSpec->Open(path_prefix + fi.Name)) {
            const auto sysError = ::GetLastError();
            failedCodes.Add(HRESULT_FROM_WIN32(sysError));
            failedFiles.Add(path_prefix + fi.Name);
            return S_FALSE;
        }
        *inStream = inSequentialInStream.Detach();
    }

    return S_OK;
}

Z7_COM7F_IMF(CArchiveUpdateCallback::SetOperationResult(Int32 operationResult)) {
    needBeClosed = true;
    return S_OK;
}

Z7_COM7F_IMF(CArchiveUpdateCallback::GetVolumeSize(UInt32 index, UInt64 *size)) {
    if (volumesSizes.Size() == 0)
        return S_FALSE;
    if (index >= static_cast<UInt32>(volumesSizes.Size()))
        index = volumesSizes.Size() - 1;
    *size = volumesSizes[index];
    return S_OK;
}

Z7_COM7F_IMF(CArchiveUpdateCallback::GetVolumeStream(UInt32 index, ISequentialOutStream **volumeStream)) {
    wchar_t temp[16]{};
    ConvertUInt32ToString(index + 1, temp);
    UString res = temp;
    while (res.Len() < 2) {
        res.InsertAtFront(L'0');
    }
    UString fileName = volName;
    fileName.Add_Dot();
    fileName += res;
    fileName += volExt;
    COutFileStream *streamSpec = new COutFileStream;
    CMyComPtr<ISequentialOutStream> outSequentialStream(streamSpec);
    if (streamSpec->Create(us2fs(fileName), false)) {
        return GetLastError_noZero_HRESULT();
    }
    *volumeStream = outSequentialStream.Detach();
    return S_OK;
}

Z7_COM7F_IMF(CArchiveUpdateCallback::CryptoGetTextPassword2(Int32 *passwdIsDefined, BSTR *passwd)) {
    if (!passwordIsDefined) {
        if (askPassword) {
            return E_ABORT;
        }
    }
    *passwdIsDefined = static_cast<Int32>(passwordIsDefined);
    return StringToBstr(password, passwd);
}

TEST_F(TempDirTest, Command_A) {
    const FString &file_to_archive = FString(__FILE__);
    const auto archive_path = temp_dir / "temp.zip";
    NWindows::NFile::NFind::CFileInfo fi;
    ASSERT_TRUE(fi.Find(file_to_archive));
    CObjectVector<NWindows::NFile::NFind::CFileInfo> dir_items;
    dir_items.Add(fi);

    auto archive_name = FString(archive_path.c_str());
    auto out_file_string_spec = new COutFileStream;
    ASSERT_TRUE(out_file_string_spec->Create(archive_name, True));

    CMyComPtr<IOutStream> out_file_stream(out_file_string_spec);

    CMyComPtr<IOutArchive> out_archive;
    EXPECT_EQ(CreateObject(&IdFormatZip, &IID_IOutArchive, (void **)&out_archive), S_OK);

    CMyComPtr<ISetProperties> set_properties;
    EXPECT_EQ(out_archive->QueryInterface(IID_ISetProperties, (void **)&set_properties), S_OK);
    ASSERT_TRUE(set_properties);

    const uint8_t num_prop = 1;
    const wchar_t *names[num_prop] = {L"x"}; // Compress level 9(utral)
    NWindows::NCOM::CPropVariant values[num_prop] = {(UINT32)9};
    EXPECT_EQ(set_properties->SetProperties(names, values, num_prop), S_OK);

    CArchiveUpdateCallback *updateCallbackSpec =
        new CArchiveUpdateCallback(&dir_items, file_to_archive.Left(file_to_archive.ReverseFind_PathSepar() + 1));
    CMyComPtr<IArchiveUpdateCallback2> updateCallback(updateCallbackSpec);
    auto passwordIsDefined = false;
    updateCallbackSpec->passwordIsDefined = passwordIsDefined;

    EXPECT_EQ(out_archive->UpdateItems(out_file_stream, dir_items.Size(), updateCallback), S_OK);

    updateCallbackSpec->finilize();

    EXPECT_TRUE(std::filesystem::exists(archive_path));
}
