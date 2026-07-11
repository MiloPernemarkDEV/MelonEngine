#ifndef MELON_FILE_SYSTEM_H
#define MELON_FILE_SYSTEM_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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
        // Optional, same as calling open and passing in the opTrunc enum value
        void trunc(const std::string& filename);

    private:
        static DWORD ToCreationDisposition(EFileOpType opType);
        HANDLE file = INVALID_HANDLE_VALUE;
    };


    class DirOp {
    public:
        explicit DirOp() = default;
        ~DirOp();

        static void set_working(const std::string& dir_path);
        static std::string get_working();

    private:
    };
}
#endif // WIN32

#endif // MELON_FILE_SYSTEM_H