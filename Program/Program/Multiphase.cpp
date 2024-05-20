#include "FilePrepairing.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::pair<int, int> GetFibonnachiNumbers(int numberToCompareTo, std::pair<int, int> PreviousTwoNumbers)
{
	int currentNumber = PreviousTwoNumbers.first + PreviousTwoNumbers.second;
	std::pair <int, int> PairToReturn;

	if (currentNumber < numberToCompareTo)
		PairToReturn = GetFibonnachiNumbers(numberToCompareTo, { PreviousTwoNumbers.second, currentNumber });
	else
		PairToReturn = PreviousTwoNumbers;

	return PairToReturn;
}

std::pair<int, int> GetFibonnachiNumbers(int numberToCompareTo)
{
	return GetFibonnachiNumbers(numberToCompareTo, { 0, 1 });
}

std::pair<int, int> SortMultiphasePrepairGroups() {
	
	int inIndex = 1;

	std::string path = defineFilePath(inIndex, "m");

	while (fs::exists(path)) {
		
		std::vector<int> vect;
		FILE* inFile = fopen(path.c_str(), "rb");
		int outvalue;

		while (fread(&outvalue, sizeof(int), 1, inFile)) {
			vect.push_back(outvalue);
		}
		fclose(inFile);
		fs::remove(path);

		sort(vect.begin(), vect.end());

		FILE* outFile = fopen(path.c_str(), "wb");

		while (!vect.empty()) {
			outvalue = vect[0];
			fwrite(&outvalue, sizeof(int), 1, outFile);
			vect.erase(vect.begin());
		}
		fclose(outFile);

		inIndex++;
		path = defineFilePath(inIndex, "m");
	}

	std::pair<int, int> groupCount = GetFibonnachiNumbers(inIndex-1);
	
	while (inIndex <= groupCount.first + groupCount.second) {
		std::string path = defineFilePath(inIndex, "m");
		FILE* inFile1 = fopen(path.c_str(), "wb");
		fclose(inFile1);
		inIndex++;
	}

	inIndex = 1;

	int newIndex = 1;
	for (int i = 0; i < groupCount.first; i++) {
		std::string path1 = defineFilePath(inIndex, "m");
		std::string path2 = defineFilePath(newIndex, "m1");
		FILE* inFile = fopen(path1.c_str(), "rb");
		FILE* outFile = fopen(path2.c_str(), "wb");
		int outvalue;
		while (fread(&outvalue, sizeof(int), 1, inFile)) {
			fwrite(&outvalue, sizeof(int), 1, outFile);
		}
		fclose(inFile);
		fclose(outFile);

		fs::remove(path1);

		inIndex++;
		newIndex++;
	}

	newIndex = 1;
	for (int i = 0; i < groupCount.second; i++) {
		std::string path1 = defineFilePath(inIndex, "m");
		std::string path2 = defineFilePath(newIndex, "m2");
		FILE* inFile = fopen(path1.c_str(), "rb");
		FILE* outFile = fopen(path2.c_str(), "wb");
		int outvalue;
		while (fread(&outvalue, sizeof(int), 1, inFile)) {
			fwrite(&outvalue, sizeof(int), 1, outFile);
		}
		fclose(inFile);
		fclose(outFile);

		fs::remove(path1);

		inIndex++;
		newIndex++;
	}

	return groupCount;

}

void SortMultiphaseSingleIteration(int fIndex, int from1, int from2, int to1) {
	std::string path1;
	std::string path2;
	std::string path3;
	
	switch (from1)
	{
	case 1:
		path1 = defineFilePath(fIndex, "m1");
		break;
	case 2:
		path1 = defineFilePath(fIndex, "m2");
		break;
	case 3:
		path1 = defineFilePath(fIndex, "m3");
		break;
	}
	switch (from2)
	{
	case 1:
		path2 = defineFilePath(fIndex, "m1");
		break;
	case 2:
		path2 = defineFilePath(fIndex, "m2");
		break;
	case 3:
		path2 = defineFilePath(fIndex, "m3");
		break;
	}
	switch (to1)
	{
	case 1:
		path3 = defineFilePath(fIndex, "m1");
		break;
	case 2:
		path3 = defineFilePath(fIndex, "m2");
		break;
	case 3:
		path3 = defineFilePath(fIndex, "m3");
		break;
	}

	FILE* inFile1 = fopen(path1.c_str(), "rb");
	FILE* inFile2 = fopen(path2.c_str(), "rb");
	FILE* outFile = fopen(path3.c_str(), "wb");

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
}

void SortMultiphase() {
	int biggerPath, lesserPath, targetPath;
	std::string path1;
	std::string path2;
	
	std::pair<int, int> currentCount = SortMultiphasePrepairGroups();

	biggerPath = 2;
	lesserPath = 1;
	targetPath = 3;

	while (currentCount.first + currentCount.second > 1) {
		for (int i = 1; i <= currentCount.first; i++) {
			SortMultiphaseSingleIteration(i, biggerPath, lesserPath, targetPath);
		}
		for (int i = currentCount.first + 1; i <= currentCount.second; i++) {
			switch (biggerPath)
			{
			case 1:
				path1 = defineFilePath(i, "m1");
				path2 = defineFilePath(i - currentCount.first, "m1");
				break;
			case 2:
				path1 = defineFilePath(i, "m2");
				path2 = defineFilePath(i - currentCount.first, "m2"); 
				break;
			case 3:
				path1 = defineFilePath(i, "m3");
				path2 = defineFilePath(i - currentCount.first, "m3"); 
				break;
			}
			rename(path1.c_str(), path2.c_str());
		}
		int temp = lesserPath;
		lesserPath = biggerPath;
		biggerPath = targetPath;
		targetPath = temp;

		currentCount.second -= currentCount.first;
		std::swap(currentCount.first, currentCount.second);
	}

	switch (biggerPath)
	{
	case 1:
		path1 = defineFilePath(1, "m1");
		break;
	case 2:
		path1 = defineFilePath(1, "m2");
		break;
	case 3:
		path1 = defineFilePath(1, "m3");
		break;
	}

	FILE* inFile = fopen(path1.c_str(), "rb");
	FILE* outFile = fopen("../files/sorted.bin", "wb");

	int outvalue;

	while (fread(&outvalue, sizeof(int), 1, inFile)) {
		fwrite(&outvalue, sizeof(int), 1, outFile);
	}

	fclose(inFile);
	fclose(outFile);

	remove(path1.c_str());
}