#include "FilePrepairing.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <random>
#include <iostream>

namespace fs = std::filesystem;

std::string defineFilePath(int fileIndex, std::string folder) {
	
	std::string pathFolder;
	if (folder == "n") {
		pathFolder = "../files/natural/";
	}
	else if (folder == "m") {
		pathFolder = "../files/multiphase/primary/";
	}
	else if (folder == "m1") {
		pathFolder = "../files/multiphase/container1/";
	}
	else if (folder == "m2") {
		pathFolder = "../files/multiphase/container2/";
	}
	else if (folder == "m3") {
		pathFolder = "../files/multiphase/container3/";
	}

	std::string pathBase;
	if (fileIndex != 0) {
		pathBase = std::to_string(fileIndex);
	}
	else if (fileIndex == 0) {
		pathBase = "buffer";
	}

	std::string pathDat = ".bin";
	std::string pathNameSort = pathFolder + pathBase + pathDat;
	return pathNameSort;
}

void PrepairFolders() {
	fs::create_directory("../files/");
	fs::create_directory("../files/multiphase/");
	fs::create_directory("../files/multiphase/primary/");
	fs::create_directory("../files/multiphase/container1/");
	fs::create_directory("../files/multiphase/container2/");
	fs::create_directory("../files/multiphase/container3/");
	fs::create_directory("../files/natural/");
}

void CreateUnsortedFile(int count) {
	std::string pathName = "../files/unsorted.bin";

	FILE* createdFile = fopen(pathName.c_str(), "wb");

	for (int i = 0; i < count; ++i) {
		int writeValue = rand() % 100;
		fwrite(&writeValue, sizeof(int), 1, createdFile);
	}

	fclose(createdFile);
}

void ReadUnsortedFile() {
	std::string pathName = "../files/unsorted.bin";

	FILE* createdFile = fopen(pathName.c_str(), "rb");

	int readValue;
	
	while (fread(&readValue, sizeof(int), 1, createdFile)) {
		std::cout << readValue << '/';
	}

	std::cout << std::endl;

	fclose(createdFile);
}

void ReadSortedFile() {
	std::string pathName = "../files/sorted.bin";

	FILE* createdFile = fopen(pathName.c_str(), "rb");

	int readValue = 0;
	int readValueCount = 0;

	while (fread(&readValue, sizeof(int), 1, createdFile)) {
		readValueCount++;
		std::cout << readValue << '/';
	}

	std::cout << std::endl << readValueCount << std::endl;

	fclose(createdFile);
}

void ReadSortedFiles(std::string folder) {

	int fileIndex = 1;
	std::string pathName = defineFilePath(fileIndex, folder);
	FILE* sortedFile;

	while (sortedFile = fopen(pathName.c_str(), "rb")) {
		int readValue;

		while (fread(&readValue, sizeof(int), 1, sortedFile)) {
			std::cout << readValue << '/';
		}
		std::cout << std::endl;

		fclose(sortedFile);
		fileIndex++;
		pathName = defineFilePath(fileIndex, folder);
	}
}

int multMod = 100;

void DivideFile(std::string pathNameUnsort, std::string folder) {
	FILE* unsortedFile = fopen(pathNameUnsort.c_str(), "rb");

	int readValue, bufferValue = 0;
	int fileIndex = 1;

	if (folder == "n") bufferValue = -INT_MIN;
	else if (folder == "m") bufferValue = 1;

	std::string pathNameSort = defineFilePath(fileIndex, folder);

	FILE* sortedFile = fopen(pathNameSort.c_str(), "wb");

	while (fread(&readValue, sizeof(int), 1, unsortedFile)) {
		if ((folder == "n" && readValue >= bufferValue) ||
			(folder == "m" && bufferValue <= multMod)) 
		{
			fwrite(&readValue, sizeof(int), 1, sortedFile);
			if (folder == "n") bufferValue = readValue;
			else if (folder == "m") bufferValue++;
		}
		else 
		{
			fclose(sortedFile);
			fileIndex++;
			pathNameSort = defineFilePath(fileIndex, folder);
			sortedFile = fopen(pathNameSort.c_str(), "wb");

			fwrite(&readValue, sizeof(int), 1, sortedFile);
			if (folder == "n") bufferValue = readValue;
			else if (folder == "m") bufferValue = 1;
		}
	}
	fclose(sortedFile);
}