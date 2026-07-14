#include "file_util.h"
#include "logger.h"
#include "string_util.h"

constexpr size_t FILE_LOCAL_BUFFER_SIZE = 128;

#ifdef _WIN32

File::FileOp::~FileOp() {
    if (file != INVALID_HANDLE_VALUE) {
        CloseHandle(file);
    }
}

bool File::FileOp::open(const std::string& filename, const EFileOpType opType) {
    
    const std::wstring wideFilename = StringUtil::utf8_to_wide(filename);
    const DWORD creationDisp{ToCreationDisposition(opType)};
    file = CreateFileW(
        wideFilename.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        creationDisp,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    if (file == INVALID_HANDLE_VALUE) {
        ME_LOG(Error, "Failed to open file {0}", StringUtil::wide_to_utf8(wideFilename));
        return false;
    }
    return true;
}

void File::FileOp::write(const void* data, const size_t size) const {
    DWORD bytesWritten = 0;

    const BOOL result = WriteFile(
        file,
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

void File::FileOp::write_str(const std::string& str) const {
    DWORD bytesWritten = 0;

    const BOOL result = WriteFile(
        file,
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

void File::FileOp::trunc(const std::string& filename) {
    const std::wstring wideFilename = StringUtil::utf8_to_wide(filename);
    const DWORD creationDisp{ToCreationDisposition(opTrunc)};

    file = CreateFileW(
        wideFilename.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        creationDisp,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE) {
        ME_LOG(Error, "Failed to open file {0}", StringUtil::wide_to_utf8(wideFilename));
    }
}

s64 File::FileOp::size() const {
    LARGE_INTEGER fileSize;
    const BOOL result = GetFileSizeEx(file, &fileSize);

    return (result == FALSE) ? 0 : fileSize.QuadPart;
}


DWORD File::FileOp::ToCreationDisposition(const EFileOpType opType) {
    DWORD creationDisp{};

    switch (opType) {
        case opCreateNew: creationDisp = CREATE_NEW; break;
        case opCreateAlways: creationDisp = CREATE_ALWAYS; break;
        case opOpenExisting: creationDisp = OPEN_EXISTING; break;
        case opOpenAlways: creationDisp = OPEN_ALWAYS; break;
        case opTrunc: creationDisp = TRUNCATE_EXISTING | CREATE_ALWAYS; break;
        default: creationDisp = CREATE_NEW; break;
    }
    return creationDisp;
}

void File::DirOp::set_working(const std::string& dir_path) {
    const BOOL result = SetCurrentDirectoryW(StringUtil::utf8_to_wide(dir_path).c_str());
    if (result == FALSE) {
       ME_LOG(Error, "Failed to set new working directory!");
    }
}

std::string File::DirOp::get_working() {
    const DWORD length = GetCurrentDirectoryW(0, nullptr);
    if (length == 0) {
        ME_LOG(Error, "Failed to get current working directory!");
        return "";
    }

    std::wstring path(length, L'\0');
    const DWORD written = GetCurrentDirectoryW(length, path.data());
    if (written == 0) {
        ME_LOG(Error, "Failed to get current working directory!");
        return "";
    }

    path.resize(written);
    return StringUtil::wide_to_utf8(path);
}

#endif