#include "file_util.h"
#include "logger.h"
#include "string_util.h"

constexpr size_t FILE_LOCAL_BUFFER_SIZE = 128;

#ifdef WIN32

FileOp::~FileOp() {
    if (handle != INVALID_HANDLE_VALUE) {
        CloseHandle(handle);
    }
}

bool FileOp::open(const std::string& filename, const EFileOpType opType) {
    
    const std::wstring wideFilename = StringUtil::utf8_to_wide(filename);
    const DWORD creationDisp{ToCreationDisposition(opType)};

    handle = CreateFileW(
        wideFilename.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        creationDisp,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (handle == INVALID_HANDLE_VALUE) {
        ME_LOG(Error, "Failed to open file {0}", StringUtil::wide_to_utf8(wideFilename));
        return false;
    }

    return true;
}

void FileOp::write(const void* data, const size_t size) const {
    DWORD bytesWritten = 0;

    const BOOL result = WriteFile(
        handle,
        data,
        static_cast<DWORD>(size),
        &bytesWritten,
        nullptr
    );

    if (!result || bytesWritten != size)
    {
        ME_LOG(Error, "Failed to write to file");
    }
}

void FileOp::write_str(const std::string& str) const {
    DWORD bytesWritten = 0;

    const BOOL result = WriteFile(
        handle,
        str.data(),
        static_cast<DWORD>(str.size()),
        &bytesWritten,
        nullptr
    );

    if (!result || bytesWritten != str.size())
    {
        ME_LOG(Error, "Failed to write to file");
    }
}

DWORD FileOp::ToCreationDisposition(const EFileOpType opType) {
    DWORD creationDisp{};

    switch (opType) {
        case opCreateNew: creationDisp = CREATE_NEW; break;
        case opCreateAlways: creationDisp = CREATE_ALWAYS; break;
        case opOpenExisting: creationDisp = OPEN_EXISTING; break;
        case opOpenAlways: creationDisp = OPEN_ALWAYS; break;
        case opTruncateExisting: creationDisp = TRUNCATE_EXISTING; break;
        default: creationDisp = CREATE_NEW; break;
    }
    return creationDisp;
}

#endif