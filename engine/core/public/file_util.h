#ifndef MELON_FILE_SYSTEM_H
#define MELON_FILE_SYSTEM_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "types.h"

enum EFileOpType {
    opCreateNew,
    opCreateAlways,
    opOpenExisting,
    opOpenAlways,
    opTrunc
};

namespace File {

    class FileOp {
    public:
        explicit FileOp() = default;
        ~FileOp();

        bool open(const std::string& filename, EFileOpType opType = opCreateAlways);
        void write(const void* data, size_t size) const;
        void write_str(const std::string& str) const;
        void trunc(const std::string& filename);

        s64 size() const;

    private:
        static DWORD ToCreationDisposition(EFileOpType opType);
        HANDLE file = INVALID_HANDLE_VALUE;
    };

    class DirOp {
    public:
        static void set_working(const std::string& dir_path);
        static std::string get_working();
    };
}
#endif // _WIN32

#endif // MELON_FILE_SYSTEM_H