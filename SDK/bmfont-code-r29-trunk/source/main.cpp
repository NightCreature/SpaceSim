/*
   AngelCode Bitmap Font Generator
   Copyright (c) 2004-2019 Andreas Jonsson
  
   This software is provided 'as-is', without any express or implied 
   warranty. In no event will the authors be held liable for any 
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any 
   purpose, including commercial applications, and to alter it and 
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you 
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product 
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and 
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source 
      distribution.
  
   Andreas Jonsson
   andreas@angelcode.com
*/

#include <windows.h>
#include <crtdbg.h>
#include <fcntl.h>
#include <stdio.h>
#include <io.h>
#include <string>
#include <iostream>

#include "dynamic_funcs.h"
#include "charwin.h"

using namespace std;

// from https://stackoverflow.com/questions/311955/redirecting-cout-to-a-console-in-windows
void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr)
{
	AttachConsole( ATTACH_PARENT_PROCESS);

	// Re-initialize the C runtime "FILE" handles with clean handles bound to "nul". We do this because it has been
	// observed that the file number of our standard handle file objects can be assigned internally to a value of -2
	// when not bound to a valid target, which represents some kind of unknown internal invalid state. In this state our
	// call to "_dup2" fails, as it specifically tests to ensure that the target file number isn't equal to this value
	// before allowing the operation to continue. We can resolve this issue by first "re-opening" the target files to
	// use the "nul" device, which will place them into a valid state, after which we can redirect them to our target
	// using the "_dup2" function.
	if (bindStdIn)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "nul", "r", stdin);
	}
	if (bindStdOut)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "nul", "w", stdout);
	}
	if (bindStdErr)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "nul", "w", stderr);
	}

	// Redirect unbuffered stdin from the current standard input handle
	if (bindStdIn)
	{
		HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
		if(stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if(fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "r");
				if(file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stdin));
					if (dup2Result == 0)
					{
						setvbuf(stdin, NULL, _IONBF, 0);
					}
				}
			}
		}
	}

	// Redirect unbuffered stdout to the current standard output handle
	if (bindStdOut)
	{
		HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if(stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if(fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "w");
				if(file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stdout));
					if (dup2Result == 0)
					{
						setvbuf(stdout, NULL, _IONBF, 0);
					}
				}
			}
		}
	}

	// Redirect unbuffered stderr to the current standard error handle
	if (bindStdErr)
	{
		HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);
		if(stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if(fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "w");
				if(file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stderr));
					if (dup2Result == 0)
					{
						setvbuf(stderr, NULL, _IONBF, 0);
					}
				}
			}
		}
	}

	// Clear the error state for each of the C++ standard stream objects. We need to do this, as attempts to access the
	// standard streams before they refer to a valid target will cause the iostream objects to enter an error state. In
	// versions of Visual Studio after 2005, this seems to always occur during startup regardless of whether anything
	// has been read from or written to the targets or not.
	if (bindStdIn)
	{
		std::wcin.clear();
		std::cin.clear();
	}
	if (bindStdOut)
	{
		std::wcout.clear();
		std::cout.clear();
	}
	if (bindStdErr)
	{
		std::wcerr.clear();
		std::cerr.clear();
	}
}

const char *getArgValue(const char *cmdLine, string &value)
{
	cmdLine += strspn(cmdLine, " \t");
	size_t end;
	if( *cmdLine == '"' )
	{
		end = strcspn(++cmdLine, "\"");
		if( cmdLine[end] == '"' )
			value.assign(cmdLine, end);
		end++;
	}
	else
	{
		end = strcspn(cmdLine, " ");
		value.assign(cmdLine, end);
	}
	cmdLine += end;
	return cmdLine;
}

// Returns true if the GUI should be opened
bool processCmdLine(const char *cmdLine, string &configFile, int &result)
{
	string outputFile;
	string textFile;

	configFile = CCharWin::GetDefaultConfig(); // Use the last configuration from the GUI as default

	bool hasError = false;

	size_t start = strspn(cmdLine, " \t");
	if( start == 0 && *cmdLine == 0 )
	{
		// No arguments, open the GUI
		return true;
	}

	cmdLine += start;
	if( *cmdLine == '-' )
	{
		while( *cmdLine == '-' )
		{
			cmdLine++;
			if( *cmdLine == 'o' )
				cmdLine = getArgValue(++cmdLine, outputFile);
			else if( *cmdLine == 'c' )
				cmdLine = getArgValue(++cmdLine, configFile);
			else if( *cmdLine == 't' )
				cmdLine = getArgValue(++cmdLine, textFile);
			else
			{
				hasError = true;
				break;
			}
	
			cmdLine += strspn(cmdLine, " \t");
		}
	}
	else 
	{
		// It may be a bmfc file, in which case we assume the app is being 
		// opened with the intention to show the GUI and load that file
		cmdLine = getArgValue(cmdLine, configFile);
		return true;
	}

	BindCrtHandlesToStdHandles(false, true, true);

	if( hasError || outputFile == "" )
	{
		cerr << "Incorrect arguments. See documentation for instructions." << endl;
		return false;
	}

	CFontGen *fontGen = new CFontGen();

	cout << "Loading config." << endl;
	fontGen->LoadConfiguration(configFile.c_str());

	if( textFile != "" )
	{
		cout << "Selecting characters from file." << endl;
		fontGen->SelectCharsFromFile(textFile.c_str());
	}

	cout << "Generating pages." << endl;
	fontGen->GeneratePages(false);

	cout << "Saving font." << endl;
	fontGen->SaveFont(outputFile.c_str());
	
	int countFailed = fontGen->GetNumFailedChars();
	if( countFailed )
	{
		if (fontGen->GetAutoFitNumPages() > 0)
		{
			cout << countFailed << " characters did not fit within " << fontGen->GetAutoFitNumPages() << " pages." << endl;
		}
		else
		{
			cout << countFailed << " characters did not fit within the textures." << endl;
		}
		result = 1;
	}
	else if (fontGen->GetAutoFitNumPages() > 0)
	{
		cout << "AutoFit results: " << fontGen->GetAutoFitNumPages() << " pages filled by font size " << fontGen->GetFontSize() << endl;
	}

	delete fontGen;
	cout << "Finished." << endl;

	return false;
}

int WINAPI WinMain(HINSTANCE hInst, 
				   HINSTANCE hPrevInst, 
				   LPSTR     cmdLine, 
				   int       showFlag)
{
	// Turn on memory leak detection
	// Find the memory leak with _CrtSetBreakAlloc(n) where n is the number reported
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDERR);

	Init();

	string configFile;
	int result = 0;
	bool openGui = processCmdLine(cmdLine, configFile, result);

	if( openGui )
	{
		CCharWin *wnd = new CCharWin;
		wnd->Create(512, 512, configFile);
		while( !acWindow::CWindow::CheckMessages(true) ) {}
		delete wnd;
	}

	Uninit();

	return result;
}
