#ifndef MELON_FILE_SYSTEM_H
#define MELON_FILE_SYSTEM_H


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#endif

enum EFileOpType {
    opCreateNew,
    opCreateAlways,
    opOpenExisting,
    opOpenAlways,
    opTruncateExisting
};

class FileOp {
public:
    explicit FileOp() = default;
    ~FileOp();

    HANDLE handle = INVALID_HANDLE_VALUE;

    bool open(const std::string& filename, EFileOpType opType = opCreateAlways);
    void write(const void* data, size_t size) const;
    void write_str(const std::string& str) const;

    HANDLE file = INVALID_HANDLE_VALUE;
private:
    static DWORD ToCreationDisposition(EFileOpType opType);
};
#endif