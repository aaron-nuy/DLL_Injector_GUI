#include "cFrame.h"

cFrame::cFrame()
	: wxFrame(NULL, wxID_ANY, "DLL Injector")
{
	wxIcon mainicon;
	mainicon.LoadFile(L"inject.ico", wxBITMAP_TYPE_ICO);
	SetIcon(mainicon);

	auto style = this->GetWindowStyle();
	this->SetWindowStyle(~wxMAXIMIZE & style);

	wxSize size(350, 280);
	this->SetMaxSize(wxSize(375,600));
	this->SetMinSize(wxSize(375,600));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* topPanel = new wxPanel(this, -1, wxDefaultPosition, size, 0, "topPanel");
	wxPanel* bottomPanel = new wxPanel(this, -1, wxDefaultPosition, size, 0, "bottomPanel");

	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
		listView = new wxListBox(topPanel, 5, wxDefaultPosition, size, 0);
	topSizer->Add(listView, 1, wxEXPAND , 10);
	
	
	wxSize maxBtnSize = wxSize(400,38);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* openSizer = new wxBoxSizer(wxHORIZONTAL);
			openButton = new wxButton(bottomPanel, 11, "Open Process");
			refreshButton = new wxButton(bottomPanel, 12, "Refresh");
			refreshButton->SetMaxSize(maxBtnSize);
			openButton->SetMaxSize(maxBtnSize);
			openSizer->AddStretchSpacer();
			openSizer->Add(openButton, 4, wxEXPAND | wxLEFT, 5);
			openSizer->Add(refreshButton, 4, wxEXPAND | wxRIGHT, 5);
			openSizer->AddStretchSpacer();
		wxBoxSizer* browseSizer = new wxBoxSizer(wxHORIZONTAL);
			browseButton = new wxButton(bottomPanel, 21, "Browse..");
			browseButton->SetMaxSize(maxBtnSize);
			browseSizer->AddStretchSpacer();
			browseSizer->Add(browseButton, 3, wxEXPAND | wxLEFT | wxRIGHT | 0, 5);
			browseSizer->AddStretchSpacer();
		wxBoxSizer* injectSizer = new wxBoxSizer(wxHORIZONTAL);
			injectButton = new wxButton(bottomPanel, 31, "Inject");
			injectButton->SetMaxSize(maxBtnSize);
			injectSizer->AddStretchSpacer();
			injectSizer->Add(injectButton, 3, wxEXPAND | wxLEFT | wxRIGHT, 5);
			injectSizer->AddStretchSpacer();
	bottomSizer->Add(openSizer, 1, wxEXPAND   | wxLEFT | wxRIGHT, 10);
	bottomSizer->Add(browseSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
	bottomSizer->Add(injectSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

	bottomPanel->Center();
	topPanel->Center();
	bottomPanel->SetSizerAndFit(bottomSizer);
	topPanel->SetSizerAndFit(topSizer);

	mainSizer->Add(topPanel, 5, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);
	mainSizer->Add(bottomPanel, 2, wxEXPAND, 0);


	this->SetSizerAndFit(mainSizer);
	this->Bind(wxEVT_BUTTON, &cFrame::onInject, this, 31);
	this->Bind(wxEVT_BUTTON, &cFrame::onBrowse, this, 21);
	this->Bind(wxEVT_BUTTON, &cFrame::onRefresh, this, 12);
	this->Bind(wxEVT_BUTTON, &cFrame::onOpen, this, 11);

	injectButton->Enable(false);

}


void cFrame::onBrowse(wxCommandEvent& event)
{
	wxFileDialog dllFile(this, "Import DLL", "", "", "DLL files (*.dll)|*.dll|*.DLL", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dllFile.ShowModal() == wxID_CANCEL)
		return;

	libraryPath = dllFile.GetPath();
	pathIsLoaded = TRUE;
	
	if(processIsOpened) injectButton->Enable(true);

}

void cFrame::onOpen(wxCommandEvent& event)
{
	int selectedProcess = listView->GetSelection();
	if (selectedProcess >= 0) {
		try {
			Injector::Win32OpenProcess(processes[selectedProcess].ID);
			
			wxMessageBox("Process " + std::to_string(processes[selectedProcess].ID) + " Opened Successfuly!", "Success", wxICON_INFORMATION, this);
			selectedProcessID = selectedProcess;
			processIsOpened = TRUE;
			if (pathIsLoaded) injectButton->Enable(true);
		}
		catch(std::exception& e) {
			wxMessageBox("Process: " + std::to_string(processes[selectedProcess].ID) + " Failed to open!\n\n" + e.what(), "Error", wxICON_ERROR, this);
		}

	}
	else {
		wxMessageBox("Please Select Process", "Error", wxICON_ERROR, this);
	}
}

void cFrame::onRefresh(wxCommandEvent& event)
{
	processes.clear();
	processes = Injector::Win32ReturnRunningProcceses();
	listView->Clear();
	for (auto& process : processes) {
		listView->Append(process.Name);
	}
}

void cFrame::onInject(wxCommandEvent& event)
{
	try {
		Injector::Win32InjectDLLToProcess(libraryPath);
		wxMessageBox("DLL Injecred Successfully!\n", "Success", wxICON_INFORMATION, this);
		if (pathIsLoaded) injectButton->Enable(false);
		processIsOpened = false;
	}
	catch (std::exception& e) {
		wxMessageBox("Could not Inject DLL\n" + std::string(e.what()), "Error", wxICON_ERROR, this);
	}
}

