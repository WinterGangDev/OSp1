#include<iostream>
#include<Windows.h>
#include<vector>
#include<fstream>
#include<chrono>
#include<string>

using namespace std;

vector<long long int> readData();
vector <long long int> loadedData = readData();
vector<long long int> port;
string values(int);
bool timer(int);

struct bestResult {
	int chlideNum;
	long long int distance;
	string fullResult;
};

int main() {

	SYSTEM_INFO core;
	GetSystemInfo(&core);
	int logicalProcess = core.dwNumberOfProcessors;

	STARTUPINFO s;
	PROCESS_INFORMATION p;

	ZeroMemory(&s, sizeof(s));
	s.cb = sizeof(s);
	ZeroMemory(&p, sizeof(p));
	///////////////////////////////////////////////////////////////memory
	TCHAR Name[] = TEXT("OSp01");
	HANDLE memory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, loadedData.size() * sizeof(long long int), Name);

	TCHAR Name2[] = TEXT("OSpRes");
	HANDLE memoryRes = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, loadedData.size() * logicalProcess * sizeof(long long int), Name2);

	TCHAR Name3[] = TEXT("OSpIndex");
	HANDLE memoryIndex = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1 * sizeof(int), Name3);


	long long int* Buf = (long long int*)MapViewOfFile(memory, FILE_MAP_ALL_ACCESS, 0, 0, loadedData.size() * sizeof(long long int));
	if (!Buf) {
		CloseHandle(memory);
		exit(-1);

	}

	long long int* BufRes = (long long int*)MapViewOfFile(memoryRes, FILE_MAP_ALL_ACCESS, 0, 0, loadedData.size() * logicalProcess * sizeof(long long int));
	if (!BufRes) {
		CloseHandle(memoryRes);
		exit(-1);

	}

	int* BufIndex = (int*)MapViewOfFile(memoryIndex, FILE_MAP_ALL_ACCESS, 0, 0, 2 * sizeof(int));
	if (!BufIndex) {
		CloseHandle(memoryIndex);
		exit(-1);

	}
	///////////////////////////////////////////////////

	for (int i = 0;i < loadedData.size();i++) {
		Buf[i] = loadedData[i];
	}

	long long int minDist = 8e17;
	int timeAsSecond;
	cout << "inpt a time for run (seconds):\n";
	cin >> timeAsSecond;
	system("cls");

	BufIndex[0] = timeAsSecond;
	BufIndex[1] = logicalProcess;

	for (int i = 0;i < logicalProcess;i++) {

		TCHAR cmd[128];
		wsprintf(cmd, TEXT("C:\\Users\\SHARIF\\Desktop\\os\\osp01\\childe\\x64\\Release\\childe.exe %d"), i);

		bool b = CreateProcess(TEXT("C:\\Users\\SHARIF\\Desktop\\os\\osp01\\childe\\x64\\Release\\childe.exe"),
			cmd, NULL, NULL, FALSE, 0, NULL, NULL, &s, &p);

		if (!b)
		{
			cout << "Create Process failed" << endl;
			return 1;
		}
		CloseHandle(p.hProcess);
	}

	while (timer(timeAsSecond + 2)) {
		cout << "loading\r";
		Sleep(500);
		cout << "loading.\r";
		Sleep(500);
		cout << "loading..\r";
		Sleep(500);
		cout << "loading...\r";
		Sleep(500);
		system("cls");

	}
	system("cls");

	bestResult bs;
	bestResult tempp;
	bs.distance = minDist;

	for (int i = 0;i < logicalProcess*loadedData.size();i++) {
		port.push_back(BufRes[i]);
	}
	//////////////////////////////print childe

	//for (int i = 0;i < 2020;i++) {
	//	if (i % 101 == 0) {
	//		cout << "--------------------------------------------------------\n";
	//	}
	//		cout << port[i]<<"\n";
	//}

	for (int i = 0;i < logicalProcess;i++) {

		/*int waitCounter = 0;
		while (BufRes[i * loadedData.size()] == 0 && waitCounter < 5000) {
			Sleep(1);
			waitCounter++;
		}*/

		tempp.distance = port[i * loadedData.size()];
		tempp.chlideNum = i + 1;
		tempp.fullResult = values(tempp.chlideNum);


		//cout << "child <" << i + 1 << ">-------------------------------------\n\n"+ values(i+1);

		if (bs.distance > tempp.distance) {
			bs = tempp;
		}
	}

	cout << "--------------------------------------------------------\n";
	cout << ">>min<< ( " << bs.distance << " )\n\n>>from childe<< ((" << bs.chlideNum << "))\n\n>>with this values<< \n\n" << bs.fullResult<<"\n\n";

	UnmapViewOfFile(Buf);
	CloseHandle(memory);

	UnmapViewOfFile(BufRes);
	CloseHandle(memoryRes);

	UnmapViewOfFile(BufIndex);
	CloseHandle(memoryIndex);
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

string values(int childeNum)
{
	string path = "";
	for (int i = ((childeNum - 1) * loadedData.size()+1);port[i] != 0;i++) {
		if (port[i+1]!=0)
		path += to_string(port[i])+" + ";
		else
		path += to_string(port[i])+"\n\n";

	}
	return path;
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

