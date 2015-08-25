#include <windows.h>

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Configuration
{
	wstring crashWindowCaption;
	wstring pathToApplication; // Path to LEDStudio .exe file, not to executable file of this application
};

Configuration LoadConfiguration(const char* configPath)
{
	wifstream infile;
	infile.open(configPath, ios_base::in);

	if (!infile.good()) {
		throw "Unable to open configuration";
	}

	Configuration result;

	getline(infile, result.crashWindowCaption);
	getline(infile, result.pathToApplication);

	return result;
}

HWND FindCrashWindow(LPCWSTR crashWindowCaption)
{
	return FindWindow(L"#32770", crashWindowCaption);
}

void ExecuteInstanceOfProgram(LPCWSTR pathToApplication)
{
	ShellExecute(NULL, L"open", pathToApplication, NULL, NULL, SW_SHOW);
}

int main(int argc, char **argv)
{
	Configuration config = LoadConfiguration("config.cfg");


	while (true) {
		HWND hCrashWindow = FindCrashWindow(config.crashWindowCaption.c_str());

		if (hCrashWindow) {
			PostMessage(hCrashWindow, WM_QUIT, 0, 0);

			ExecuteInstanceOfProgram(config.pathToApplication.c_str());
		}

		Sleep(1000);
	}

	return 0;
}