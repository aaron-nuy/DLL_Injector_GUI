#pragma once
#include "wx/wx.h"
#include "Injector.h"
#include <string>
#include <wx/dir.h>

enum
{
	ID_Hello = 1
};

class cFrame : public wxFrame
{
public:
	cFrame();
	wxButton* openButton;
	wxButton* refreshButton;
	wxButton* browseButton;
	wxButton* injectButton;
	wxListBox* listView;
	
	BOOL pathIsLoaded = FALSE;
	BOOL processIsOpened = FALSE;

	std::vector<Injector::Process> processes;
	DWORD selectedProcessID;
	std::wstring libraryPath;

private:
	void onOpen(wxCommandEvent& event);
	void onRefresh(wxCommandEvent& event);
	void onBrowse(wxCommandEvent& event);
	void onInject(wxCommandEvent& event);
};

