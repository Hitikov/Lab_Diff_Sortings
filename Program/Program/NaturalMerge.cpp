#include "FilePrepairing.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

void SortNaturalMergePair(int inIndex, int outIndex) {

	std::string path1 = defineFilePath(inIndex, "n");
	std::string path2 = defineFilePath(inIndex + 1, "n");
	std::string pathb = defineFilePath(0, "n");

	FILE* inFile1 = fopen(path1.c_str(), "rb");
	FILE* inFile2 = fopen(path2.c_str(), "rb");
	FILE* outFile = fopen(pathb.c_str(), "wb");
	
	int inputValue1;
	int inputValue2;
	bool isAddedValue1 = false;
	bool isAddedValue2 = false;

	fread(&inputValue1, sizeof(int), 1, inFile1);
	fread(&inputValue2, sizeof(int), 1, inFile2);

	while (!feof(inFile1) && !feof(inFile2)) {
		
		if (inputValue1 < inputValue2) {
			fwrite(&inputValue1, sizeof(int), 1, outFile);
			isAddedValue1 = true;
			isAddedValue2 = false;
		}
		else {
			fwrite(&inputValue2, sizeof(int), 1, outFile);
			isAddedValue2 = true;
			isAddedValue1 = false;
		}

		if (isAddedValue1) {
			fread(&inputValue1, sizeof(int), 1, inFile1);
		}
		if (isAddedValue2) {
			fread(&inputValue2, sizeof(int), 1, inFile2);
		}
	}

	if (!feof(inFile1)) {
		fwrite(&inputValue1, sizeof(int), 1, outFile);
		while (fread(&inputValue1, sizeof(int), 1, inFile1)) {
			fwrite(&inputValue1, sizeof(int), 1, outFile);
		}
	}
	else if (!feof(inFile2)) {
		fwrite(&inputValue2, sizeof(int), 1, outFile);
		while (fread(&inputValue2, sizeof(int), 1, inFile2)) {
			fwrite(&inputValue2, sizeof(int), 1, outFile);
		}
	}

	fclose(inFile1);
	fclose(inFile2);
	fclose(outFile);

	remove(path1.c_str());
	remove(path2.c_str());

	inFile1 = fopen(pathb.c_str(), "rb");

	path1 = defineFilePath(outIndex, "n");
	outFile = fopen(path1.c_str(), "wb");

	while (fread(&inputValue1, sizeof(int), 1, inFile1)){
		fwrite(&inputValue1, sizeof(int), 1, outFile);
	}

	fclose(outFile);
	fclose(inFile1);
	remove(pathb.c_str());
}

void SortNaturalMerge() {
	int inIndex = 1;
	int outIndex = 1;

	std::string path1 = defineFilePath(inIndex, "n");
	std::string path2 = defineFilePath(inIndex + 1, "n");

	if (!fs::exists(path1)) {
		return;
	}

	while (!(inIndex == 1 && !fs::exists(path2)))
	{
		while (fs::exists(path1))
		{
			if (!fs::exists(path2)) {
				FILE* inFile2 = fopen(path2.c_str(), "wb");
				fclose(inFile2);
			}
			SortNaturalMergePair(inIndex, outIndex);
			inIndex += 2;
			outIndex++;

			path1 = defineFilePath(inIndex, "n");
			path2 = defineFilePath(inIndex + 1, "n");
		}
		inIndex = 1;
		outIndex = 1;

		path1 = defineFilePath(inIndex, "n");
		path2 = defineFilePath(inIndex + 1, "n");
	}
	
	std::string path = defineFilePath(1, "n");
	FILE* inFile = fopen(path.c_str(), "rb");
	FILE* outFile = fopen("../files/sorted.bin", "wb");

	int outvalue;

	while (fread(&outvalue, sizeof(int), 1, inFile)) {
		fwrite(&outvalue, sizeof(int), 1, outFile);
	}

	fclose(inFile);
	fclose(outFile);

	remove(path.c_str());
}

/*
while (!(inIndex == 1 && !inFile2))
	{
		while (inFile1)
		{
			fclose(inFile1);
			if (!inFile2) {
				inFile2 = fopen(path2.c_str(), "wb");
			}
			fclose(inFile2);

			SortNaturalMergePair(inIndex, outIndex);
			inIndex += 2;
			outIndex++;

			path1 = defineFilePath(inIndex, "n");
			path2 = defineFilePath(inIndex + 1, "n");
			inFile1 = fopen(path1.c_str(), "rb");
			inFile2 = fopen(path2.c_str(), "rb");
		}
		inIndex = 1;
		outIndex = 1;

		path1 = defineFilePath(inIndex, "n");
		path2 = defineFilePath(inIndex + 1, "n");
		inFile1 = fopen(path1.c_str(), "rb");
		inFile2 = fopen(path2.c_str(), "rb");
	}

	fclose(inFile1);
*/