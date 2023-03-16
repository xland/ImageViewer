#pragma once
#include <filesystem>
class FileHelper
{
public:
	FileHelper();
	~FileHelper();
	void Open();
	void Save();
	void ShowNext();
	void ShowPrev();
public:
	std::filesystem::path currentPath{};
private:
	void showFileDialog(bool isOpen);
	void loopFile(bool isNext);
private:
	
};

