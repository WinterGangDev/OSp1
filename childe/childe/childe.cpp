#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include<Windows.h>

using namespace std;

vector<long long int> readData();
int randMaker(int);

int main() {

	TCHAR Name[] = TEXT("OSp01");
	//HANDLE memory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 100 * sizeof(long long int), Name);

	HANDLE memory = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, Name);


	long long int *Buf = (long long int*)MapViewOfFile(memory, FILE_MAP_ALL_ACCESS, 0, 0, 100 * sizeof(long long int));
	if (!Buf) {
		CloseHandle(memory);
		return 0;
	}


	vector <long long int> loadedData = readData();
	vector <long long int> resualt = {0};

	long long int mainNum = loadedData[0];
	long long int dataSum = 0;
	int max = 100;
	
	do {
		int index = randMaker(max);
		long long int randVal = loadedData[index];
		dataSum = dataSum + randVal;
		resualt.push_back(randVal);
		loadedData.erase(loadedData.begin() + (index));
		max--;

	} while (dataSum <= mainNum);

	long long int dist = 0;
	dist = dataSum - mainNum;
	resualt[0] = dist;

	for (int i = 0;i < resualt.size();i++) {
		Buf[i] = resualt[i];
	}




	cout << "distance= " << dist<<'\n'<<"values: ";
	for (int j = 1;j < resualt.size();j++) {
		if (j==resualt.size()-1)
		cout << resualt[j] <<'\n';
		else
			cout << resualt[j] << " + ";

	}
	
	CloseHandle(memory);
	UnmapViewOfFile(Buf);
	

	return 0;
}

vector<long long int> readData()
{
	ifstream dataFile("C:\\Users\\SHARIF\\Desktop\\os\\osp01\\childe\\childe\\data.txt");
	if (!dataFile) {
		cout << "we have a problem when trying open file....";
		exit(-1);
	}

	vector<long long int> numbers;
	long long int fileMember;

	while (dataFile >> fileMember) {
		numbers.push_back(fileMember);
	}
	return numbers;
}

int randMaker(int max) {
	random_device ran;
	mt19937 gen(ran());
	uniform_int_distribution<int> dist(1, max);
	int randNum = dist(gen);
	return randNum;
}
