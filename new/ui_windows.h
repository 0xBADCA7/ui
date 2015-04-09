// 7 april 2015

/*
This file assumes that you have included <windows.h> and "ui.h" beforehand. It provides API-specific functions for interfacing with foreign controls in Windows.
*/

#ifndef __UI_UI_WINDOWS_H__
#define __UI_UI_WINDOWS_H__

// uiWindowsNewControl() creates a new uiControl with the given Windows API control inside.
// You will need to provide the preferredSize() method yourself.
typedef struct uiWindowsNewControlParams uiWindowsNewControlParams;
struct uiWindowsNewControlParams {
	// These match the CreateWindowExW() function.
	DWORD dwExStyle;
	LPCWSTR lpClassName;
	LPCWSTR lpWindowName;
	DWORD dwStyle;		// WS_CHILD and WS_VISIBLE are automatically applied.
	HINSTANCE hInstance;

	// These are called when the control sends a WM_COMMAND or WM_NOTIFY (respectively) to its parent.
	// ui redirects the message back and calls these functions.
	// Store the result in *lResult and return any non-FALSE value (such as TRUE) to return the given result; return FALSE to pass the notification up to your window procedure.
	// Note that these are only issued if they come from the uiControl itself; notifications from children of the uiControl (such as a header control) will be received normally.
	// TODO don't give WPARAM/LPARAM raw
	BOOL (*onWM_COMMAND)(uiControl *c, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
	BOOL (*onWM_NOTIFY)(uiControl *c, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
	// This is called in WM_DESTROY.
	void (*onWM_DESTROY)(uiControl *c);
};
uiControl *uiWindowsNewControl(uiWindowsNewControlParams *);

// This contains the Windows-specific parts of the uiSizing structure.
// baseX and baseY are the dialog base units.
// internalLeading is the standard control font's internal leading; labels in uiForms use this for correct Y positioning.
struct uiSizingSys {
	int baseX;
	int baseY;
	LONG internalLeading;
};
// Use these in your preferredSize() implementation with baseX and baseY.
// TODO rename to DlgUnits?
#define uiDlgUnitToX(dlg, baseX) MulDiv((dlg), baseX, 4)
#define uiDlgUnitToY(dlg, baseY) MulDiv((dlg), baseY, 8)

// and use this if you need the text of the window width
// TODO really export?
extern intmax_t uiWindowsWindowTextWidth(HWND hwnd);

// these functions get and set the window text for such a uiControl
// the value returned should be freed with uiFreeText()
extern char *uiWindowsControlText(uiControl *);
extern void uiWindowsControlSetText(uiControl *, const char *);

#endif
