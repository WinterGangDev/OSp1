#include<iostream>
#include<Windows.h>
#include<vector>
using namespace std;


int main() {
	SYSTEM_INFO core;
	GetSystemInfo(&core);
	int logicalProcess = core.dwNumberOfProcessors;

	STARTUPINFO s;
	PROCESS_INFORMATION p;

	ZeroMemory(&s, sizeof(s));
	s.cb = sizeof(s);
	ZeroMemory(&p, sizeof(p));

	TCHAR Name[] = TEXT("OSp01");
	HANDLE memory= CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 100 * sizeof(long long int), Name);


	long long int* Buf = (long long int*)MapViewOfFile(memory, FILE_MAP_ALL_ACCESS, 0, 0, 100 * sizeof(long long int));
	if (!Buf) {
		CloseHandle(memory);
		return 1;
	}

	long long int finalResualt[100] = { 0 };
	long long int minDist = 8e17;
	
	for (int i = 0;i < logicalProcess;i++) {
		ZeroMemory(Buf, 100 * sizeof(long long int));

		bool b = CreateProcess(TEXT("C:\\Users\\SHARIF\\Desktop\\os\\osp01\\childe\\x64\\Release\\childe.exe"),
			NULL, NULL, NULL, FALSE, 0, NULL, NULL, &s, &p);

		if (!b)
		{
			cout << "Create Process failed" << endl;
			return 1;
		}
		
		
		// Wait until child process exits.
		
		WaitForSingleObject(p.hProcess, INFINITE);
		cout << "-------------------------------------------------\n";

		if (Buf[0] < minDist) {
			minDist = Buf[0];
			
			for (int i = 0;i<100;i++) {
				finalResualt[i] = 0;
			}
			for (int j = 1;Buf[j] != 0;j++) {
				finalResualt[j-1]=Buf[j];
			}
		}
	}
	cout << "-------------------------------------------------\n";
	cout << ">>min<< ( " << minDist << " )\n";
	for (int j = 0;finalResualt[j]!=0;j++) {
		if ( finalResualt[j+1]==0)
			cout << finalResualt[j] << '\n';
		else
			cout << finalResualt[j] << " + ";

	}
}
