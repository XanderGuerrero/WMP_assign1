//FileName:		assignment1.cpp
//Project:		assignment1
//Author:		Alex Guerrero	
//Date:			Septemeber 15, 2014
//Description:	This windows program creates a main window that contains two listboxes and a
//				button.  The proram fills one listbox with 5 strings while the other is empty.
//				The user can select a string using the mouse and when highlighted, can press the 
//				move button to move the selected strings into the blank listbox. The move button
//				remains disabled until the user selects a name from the main listbox

#include <windows.h>

#define IDC_MAIN_BUTTON		101		// Button identifier
#define IDC_MAIN_LISTBOX	102		// Edit box identifier
#define IDC_MAIN_EDIT		103		// Textbox Identifier
HWND hListBox;
HWND hWndEdit;
HWND hWndButton;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hIcon = NULL;
	wClass.hIconSm = NULL;
	wClass.hInstance = hInst;
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = "Window Class";
	wClass.lpszMenuName = NULL;
	wClass.style = CS_HREDRAW | CS_VREDRAW;

	//create window class
	if (!RegisterClassEx(&wClass))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			"Window class creation failed\r\n",
			"Window Class Failed",
			MB_ICONERROR);
	}

	//sets main window with specified dimensions and class
	//changed the title bar to state my name
	HWND hWnd = CreateWindowEx(NULL,
		"Window Class",
		"Alex Guerrero Assignment 01",//third parameter is used to name the window title bar
		WS_OVERLAPPEDWINDOW,
		 
		200,//how far from the side of the physical screen
		200,////how far from the side of the physical screen
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);

	//if the handle holds nothings, window creation failed
	if (!hWnd)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			"Window creation failed\r\n",
			"Window Creation Failed",
			MB_ICONERROR);
	}

	//show the window
	ShowWindow(hWnd, nShowCmd);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	//infinite while loop to create the window
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			// Create a Listbox

			hListBox = CreateWindowEx(WS_EX_CLIENTEDGE,
				"LISTBOX",
				"",
				WS_CHILD | WS_VISIBLE | LBS_NOTIFY,
				50,//x- where to position the window side to side
				100,//y - where to position the window up and down
				100,//x- size of window
				100,//y-size of window
				hWnd,
				(HMENU)IDC_MAIN_LISTBOX,
				GetModuleHandle(NULL),
				NULL);
			HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hListBox,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE, 0));

			//
			// Instead of setting text, we need to either add a string or insert a string
			// into a Listbox. In this case, adding a string is demonstrated.
			// 6 names added to the original list box
			SendMessage(hListBox,
				LB_ADDSTRING,
				NULL,
				(LPARAM)"John Smith");
			SendMessage(hListBox,
				LB_ADDSTRING,//command used to add a string to a listbox
				NULL,
				(LPARAM)"Mark Ryan");
			SendMessage(hListBox,
				LB_ADDSTRING,
				NULL,
				(LPARAM)"Jerry Hayes");
			SendMessage(hListBox,
				LB_ADDSTRING,
				NULL,
				(LPARAM)"Anthony Hodgins");
			SendMessage(hListBox,
				LB_ADDSTRING,
				NULL,
				(LPARAM)"Bart Simpson");


				// Create a push button called move
				hWndButton = CreateWindowEx(NULL,
				"BUTTON",//create button
				"Move",//call it "move"
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD | BS_DEFPUSHBUTTON,
				175,
				100,
				100,
				24,
				hWnd,
				(HMENU)IDC_MAIN_BUTTON,
				GetModuleHandle(NULL),
				NULL);
			SendMessage(hWndButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE, 0));
			EnableWindow(hWndButton, false);

			// Create a listbox to hold the selected items to move
			//using the move button.  Add room for 5 strings to be added
			hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
				"LISTBOX",
				"",
				WS_CHILD | WS_VISIBLE | LBS_NOTIFY,
				300,
				100,
				125,
				100,
				hWnd,
				(HMENU)IDC_MAIN_EDIT,
				GetModuleHandle(NULL),
				NULL);
			SendMessage(hWndEdit,
				WM_SETFONT,//sets the font to equal that of the previous window
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE, 0));
		}
		break;

		// The LOWORD in wParam indicates the source of the command. (Events generate commands.)
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				//main button has been pressed
				case IDC_MAIN_BUTTON:
				{
					// Since a Listbox has several lines of text, you must first determine if
					// a line of text was selected. This is done with the LB_GETCURSEL command.
					// If nothing is selected, then the return value is LB_ERR, otherwise, the 
					// index (zero based) is returned.
					char buffer[256];
					int indexInList = (int)SendMessage(hListBox,
						LB_GETCURSEL,
						(WPARAM)0,
						(LPARAM)0);

					// If there was something selected, then we get it (using LB_GETTEXT) and
					// display it in the blank listbox using the hWndEdit handle
					if (indexInList != LB_ERR)
					{
						SendMessage(hListBox,
							LB_GETTEXT,
							indexInList,
							reinterpret_cast<LPARAM>(buffer));
						SendMessage(hListBox,//delete the string in the first listbox when you shift
							LB_DELETESTRING,
							indexInList,
							reinterpret_cast<LPARAM>(buffer));
						SendMessage(hWndEdit,////add string to the hWndEdit listbox
							LB_ADDSTRING,
							NULL,
							(LPARAM)buffer);
						EnableWindow(hWndButton, false);
						UpdateWindow(hWndButton);
					}
				}
				break;
				//main listbox has been selected
				case IDC_MAIN_LISTBOX:
				{
					//when listbox is selected, enable button
					if (EnableWindow(hWndButton, true) != 0)
					{
						EnableWindow(hWndButton, true);
						//update window to see changes
						UpdateWindow(hWndButton);
					}
				}
			}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}