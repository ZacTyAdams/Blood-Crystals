/// \file main.cpp 
/// \brief Main file for Ian Parberry's game engine.

/// \mainpage Game Engine
/// This code was written by Ian Parberry to accompany his book
/// "Introduction to Game Physics with Box2D", published by CRC Press in 2013.
///
/// This basic and elementary single-screen 2D sprite-based fixed-function graphics 
/// game engine uses Microsoft DirectX 9.0 for graphics,
/// some Windows API functions, and TinyXML for reading XML files. It shouldn't
/// be necessary for you to drill down into the code, but you should probably
/// know about the interfaces. This book is not about writing game engine code,
/// it's about incorporating game physics into an existing game engine.

#include <windows.h>
#include <windowsx.h>
#include <process.h>

#include "defines.h"
#include "abort.h"
#include "tinyxml2.h"
#include "debug.h"
//#include "..\My Game\resource.h"
#include "Sound.h"

//globals
BOOL g_bActiveApp;  ///< TRUE if this is the active application
HWND g_HwndApp; ///< Application window handle.
HINSTANCE g_hInstance; ///< Application instance handle.
extern char g_szGameName[]; ///< Name of this game

//graphics settings
int g_nScreenWidth; ///< Screen width.
int g_nScreenHeight; ///< Screen height.

//XML settings
const char* g_xmlFileName="gamesettings.xml"; ///< Settings file name.
tinyxml2::XMLDocument g_xmlDocument; ///< TinyXML document for settings.
XMLElement* g_xmlSettings = nullptr; ///< TinyXML element for settings tag.

//debug variables
#ifdef DEBUG_ON
  CDebugManager g_cDebugManager; //< The debug manager.
#endif //DEBUG_ON

extern CSoundManager* g_pSoundManager; //< The sound manager.
extern CTimer g_cTimer; ///< The game timer.

void ProcessFrame();
void InitGame();
void EndGame();
BOOL KeyboardHandler(WPARAM keystroke); //keyboard handler
BOOL MouseHandler(WPARAM click); //Zac: Im gonna try and make a mouse handler cross your fingers
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void LoadGameSettings();

/// Initialize XML settings.
/// Opens an XML file and prepares to read settings from it. Settings
/// tag is loaded to XML element g_xmlSettings for later processing. Aborts if it
/// cannot load the file or cannot find settings tag in loaded file.

void InitXMLSettings(){
  //open and load XML file
  const char* xmlFileName = "gamesettings.xml"; //Settings file name.
  if(g_xmlDocument.LoadFile(xmlFileName) != 0)
    ABORT("Cannot load settings file %s.", g_xmlFileName);

  //get settings tag
  g_xmlSettings = g_xmlDocument.FirstChildElement("settings"); //settings tag
  if(!g_xmlSettings) //abort if tag not found
    ABORT("Cannot find <settings> tag in %s.", g_xmlFileName);
} //InitXMLSettings

//windows functions: default window procedure and winmain

/// Default window procedure.
/// Handler for messages from the Windows API. 
///  \param hwnd window handle
///  \param message message code
///  \param wParam parameter for message 
///  \param lParam second parameter for message
///  \return 0 if message is handled

LRESULT CALLBACK DefaultWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){ //Zac: tried a WPARAM clicker in here
  
  switch(message){ //handle message

    case WM_ACTIVATEAPP: 
      g_bActiveApp = (BOOL)wParam; 
      break; 

    case WM_KEYDOWN: //keyboard hit
      if(KeyboardHandler(wParam))DestroyWindow(hwnd);
      break;

	case WM_LBUTTONDOWN: //Handler for the mouse :Zac
		if (MouseHandler(wParam)){
			SetCapture(hwnd);
			DestroyWindow(hwnd); //set capture move to mygame
		}
		break; 

    case WM_DESTROY: //on exit
      EndGame();
      PostQuitMessage(0); //this is the last thing to do on exit
      break;

    default: //default window procedure
      return DefWindowProc(hwnd, message, wParam, lParam);
  } //switch(message)
  return 0;
} //WindowProc

/// Create a default window.
/// Registers and creates a full screen window.
///  \param name the name of this application
///  \param hInstance handle to the current instance of this application
///  \param nCmdShow specifies how the window is to be shown
///  \return handle to the application window

HWND CreateDefaultWindow(char* name, HINSTANCE hInstance, int nCmdShow){
  WNDCLASS wc; //window registration info
  //fill in registration information wc
  wc.style = CS_HREDRAW|CS_VREDRAW; //style
  wc.lpfnWndProc = WindowProc; //window message handler
  wc.cbClsExtra = wc.cbWndExtra = 0;
  wc.hInstance = hInstance; //instance handle
  wc.hIcon = LoadIcon(hInstance, 0); //MAKEINTRESOURCE(IDI_ICON1));
  wc.hCursor = LoadCursor(NULL,IDC_HAND); //no cursor
  wc.hbrBackground = nullptr; //we will draw background
  wc.lpszMenuName = nullptr; //no menu
  wc.lpszClassName = name; //app name provided as parameter

  //register window
  RegisterClass(&wc);

  //create and set up window 
  HWND hwnd;
  RECT r;  
  r.left = 0; r.right = g_nScreenWidth; 
  r.top = 0; r.bottom = g_nScreenHeight;

  DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_SYSMENU; 
  DWORD dwStyleEx = WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME;

  AdjustWindowRectEx(&r, dwStyle, FALSE, dwStyleEx); 

  hwnd = CreateWindowEx(dwStyleEx, name, name, dwStyle, 0, 0,
    r.right-r.left, r.bottom-r.top, nullptr, nullptr, hInstance, nullptr);

  	//center window on screen
	  int x = (GetSystemMetrics(SM_CXSCREEN) - g_nScreenWidth)/2;
	  int y = (GetSystemMetrics(SM_CYSCREEN) - g_nScreenHeight)/2;
	  ::SetWindowPos(hwnd, nullptr, x, y, r.right - r.left, r.bottom - r.top,
	    SWP_NOZORDER | SWP_SHOWWINDOW);

  if(hwnd){ //if successfully created window
    ShowWindow(hwnd, nCmdShow); UpdateWindow(hwnd); //show and update
    SetFocus(hwnd); //get input from keyboard
  }

  return hwnd; //return window handle
} //CreateDefaultWindow

/// Load game settings.
/// Loads essential game settings from an xml file.

void LoadGameSettings(){
  if(!g_xmlSettings)return; //bail and fail

  //get game name
  XMLElement* ist = g_xmlSettings->FirstChildElement("game"); 
  if(ist){
    int len = strlen(ist->Attribute("name")); //length of name string
    strncpy_s(g_szGameName, len+1, ist->Attribute("name"), len); 
  } //if

  //get renderer settings
  XMLElement* renderSettings = 
    g_xmlSettings->FirstChildElement("renderer"); //renderer tag
  if(renderSettings){ //read renderer tag attributes
    g_nScreenWidth = renderSettings->IntAttribute("width");
    g_nScreenHeight = renderSettings->IntAttribute("height");
  } //if

  //get debug settings
  #ifdef DEBUG_ON
    g_cDebugManager.GetDebugSettings(g_xmlSettings);
  #endif //DEBUG_ON
} //LoadGameSettings
              
/// Default WinMain.  
/// The main entry point for this application should call this function first. 
///  \param hInstance handle to the current instance of this application
///  \param hPrevInstance unused
///  \param lpCmdLine unused 
///  \param nCmdShow specifies how the window is to be shown
///  \return TRUE if application terminates correctly

int DefaultWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow){
  #ifdef DEBUG_ON
    g_cDebugManager.open(); //open debug streams, settings came from XML file
  #endif //DEBUG_ON

  g_hInstance = hInstance;
  InitXMLSettings(); //initialize XML settings reader
  LoadGameSettings();

  g_HwndApp = CreateDefaultWindow(g_szGameName, hInstance, nCmdShow); //create fullscreen window
  if(!g_HwndApp)return FALSE; //bail if problem creating window

  g_cTimer.start(); //start game timer

  InitGame();

  g_pSoundManager = new CSoundManager(); //create sound manager
  g_pSoundManager->Load(); //load the sounds for this game

  //message loop
  MSG msg; //current message
  while(TRUE)
    if(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)){ //if message waiting
      if(!GetMessage(&msg, nullptr, 0, 0))return (int)msg.wParam; //get it
      TranslateMessage(&msg); DispatchMessage(&msg); //translate it
    }
    else 
      if(g_bActiveApp)ProcessFrame();
      else WaitMessage();
} //WinMain