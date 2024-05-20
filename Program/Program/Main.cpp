#include "FilePrepairing.h"
#include "NaturalMerge.h"
#include "Multiphase.h"

int main() {
	
	PrepairFolders();

	CreateUnsortedFile(10000);
	
	ReadUnsortedFile();
	
	DivideFile("../files/unsorted.bin", "m");
	
	SortMultiphase(); //SortNaturalMerge();

	ReadSortedFile();
	return 0;
}