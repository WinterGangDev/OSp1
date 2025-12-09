#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include<Windows.h>
#include<chrono>
#include<string>

using namespace std;

vector<long long int> readData();
vector<int> INDEX;

random_device ran;
mt19937 gen(ran());

int randMaker(int);
bool timer(int);
bool checkUsedIndex(int);
bool findInVector(int);

int main(int argc, char* argv[]) {


	if (argc < 2) {
		cout << "Child received no index!" << endl;
		return -1;
	}

	int resInedex= atoi(argv[1]); 


	TCHAR Name3[] = TEXT("OSpIndex");
	HANDLE memoryIndex = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, Name3);

	int* BufIndex = (int*)MapViewOfFile(memoryIndex, FILE_MAP_ALL_ACCESS, 0, 0, 2 * sizeof(int));
	if (!BufIndex) {
		CloseHandle(memoryIndex);
		exit(-1);

	}

	int logicalProcess = BufIndex[1];
	vector <long long int> loadedData = readData();
	long long int mainNum = loadedData[0];
	vector <long long int> resualt ;
	long long int dataSum = 0;
	long long int prevDist = mainNum;
	long long int dist = 0;
	string sResualt = "";

	TCHAR Name2[] = TEXT("OSpRes");
	HANDLE memoryRes = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, Name2);

	long long int* BufRes = (long long int*)MapViewOfFile(memoryRes, FILE_MAP_ALL_ACCESS, 0, 0, loadedData.size() * logicalProcess * sizeof(long long int));
	if (!BufRes) {
		CloseHandle(memoryRes);
		exit(-1);

	}

	while (timer(BufIndex[0]))
	{
		dataSum = 0;
		vector<long long int> tempResult = { prevDist };

		int index;
		do {
			do {
			 index = randMaker(loadedData.size());

			} while (checkUsedIndex(index));

			long long int randVal = loadedData[index];
			dataSum = dataSum + randVal;
			tempResult.push_back(randVal);

		} while (dataSum <= mainNum);
		//check same index[()]

		dist = dataSum - mainNum;


		if (dist < prevDist)
		{
			prevDist = dist;
			tempResult[0] = prevDist;
			resualt = tempResult;

		}
		//string sresualt;
		//sResualt = to_string(prevDist) + ": ";

		//for (int i = 1;i < resualt.size();i++)
		//{
		//	sResualt += to_string(resualt[i]);
		//	if (i != resualt.size() - 1)
		//		sResualt += " + ";
		//}

		tempResult.clear();
		INDEX.clear();
	}

	//cout << "final>>>>>>>>" << sResualt << "\n\n\n";

	//cout << "My index is: " << resInedex << endl;


	if (!resualt.empty()) 
	for (int i = 0;i < resualt.size();i++) {
		BufRes[(resInedex * loadedData.size()) + i] = resualt[i];
	}

	UnmapViewOfFile(BufIndex);
	CloseHandle(memoryIndex);

	UnmapViewOfFile(BufRes);
	CloseHandle(memoryRes);

	return 0;
}

vector<long long int> readData()
{
	TCHAR Name[] = TEXT("OSp01");
	HANDLE memory = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, Name);

	long long int* Buf = (long long int*)MapViewOfFile(memory, FILE_MAP_ALL_ACCESS, 0, 0, 100 * sizeof(long long int));
	if (!Buf) {
		CloseHandle(memory);
		exit(-1);
	}

	vector<long long int> numbers;

	for (int i = 0; Buf[i];i++) {
		numbers.push_back(Buf[i]);
	}

	return numbers;
}

//vector<long long int> readData()
//{
//	ifstream dataFile("C:\\Users\\SHARIF\\Desktop\\os\\osp01\\childe\\childe\\data.txt");
//	if (!dataFile) {
//		cout << "we have a problem when trying open file....";
//		exit(-1);
//	}
//
//	vector<long long int> numbers;
//	long long int fileMember;
//
//	while (dataFile >> fileMember) {
//		numbers.push_back(fileMember);
//	}
//	return numbers;
//}

int randMaker(int max) {
	
	uniform_int_distribution<int> dist(1, max-1);
	int randNum = dist(gen);
	return randNum;
}

bool timer(int seconds)
{
	static bool started = false;
	static auto start = std::chrono::steady_clock::now();

	if (!started) {
		start = std::chrono::steady_clock::now();
		started = true;
	}

	auto elapsed = std::chrono::steady_clock::now() - start;

	if (elapsed < std::chrono::seconds(seconds))
		return true;

	started = false;
	return false;
}

bool checkUsedIndex(int index)
{
	if (findInVector(index)) {
		return true;
	}
	else {
		INDEX.push_back(index);
		return false;
	}
	
}

bool findInVector(int index)
{
	for (int i = 0;i < INDEX.size();i++) {
		if (INDEX[i] == index) {
			return true;
		}
	}
	return false;
}
