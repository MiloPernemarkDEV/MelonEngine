#include "file_system.h"

#include "logger.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <fstream>


FileSystem::FileSystem()
{
	current_working_directory = std::filesystem::current_path();
}


void FileSystem::set_working_directory(const std::filesystem::path& path)
{
	try
	{
		std::filesystem::current_path(path);
		current_working_directory = path;
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		ME_LOG(Error, "Failed to set working directory: {0}", e.what());
	}
}


std::string FileSystem::get_exe_path()
{
	wchar_t buffer[MAX_PATH]{};

	DWORD size = GetModuleFileNameW(nullptr, buffer, MAX_PATH);

	if (size == 0)
	{
		return {};
	}

	std::filesystem::path path(buffer);

	return path.parent_path().string();
}


void FileSystem::clear_file(const std::filesystem::path& path)
{
	std::fstream file(path, std::ios::out | std::ios::trunc);

	if (!file.is_open())
	{
		ME_LOG(Warning, "Failed to open file {0} for truncation", path.string());
	}
}