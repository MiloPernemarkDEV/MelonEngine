#ifndef MELON_FILE_SYSTEM_H
#define MELON_FILE_SYSTEM_H

#include <filesystem>
#include <fstream>


class FileSystem
{
public:

	FileSystem();
	~FileSystem();


	// File Manipulation

	template<typename T>
	static void write_object(const std::filesystem::path& path, const T& object)
	{
		std::fstream f(path, std::ios::binary | std::ios::out);

		if (f.is_open())
		{
			f.write(
				reinterpret_cast<const char*>(&object),
				sizeof(T)
			);
		}

		f.close();
	}


	template<typename T>
	static T read_object(const std::filesystem::path& path)
	{
		T object{};

		std::fstream f(path, std::ios::binary | std::ios::in);

		if (f.is_open())
		{
			f.read(
				reinterpret_cast<char*>(&object),
				sizeof(T)
			);
		}

		f.close();

		return object;
	}


	static void clear_file(const std::filesystem::path& path);


	// Directories and paths

	static void set_working_directory(
		const std::filesystem::path& path);


	static std::string get_exe_path();


	static std::filesystem::path& get_working_directory()
	{
		return current_working_directory;
	}


private:

	inline static std::filesystem::path current_working_directory;

};


#endif