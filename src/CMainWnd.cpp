//**********************************************************************************************
//                                        CMainWnd.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "wx/dialog.h"
#include "wx/utils.h"
#include "wx/busyinfo.h"
#include "wx/stdpaths.h"
#include "wx/filename.h"
#include "wx/dirdlg.h"
#include "wx/statusbr.h"
#include "wx/gauge.h"
#include "wx/sizer.h"

#include "Cst.h"
#include "CMainWnd.h"
#include "NewAcq.h"
#include "CProject.h"
#include "CLog.h"
#include "CPlotWnd.h"
#include "CMeasurements.h"
#include "CMeasurementsGrid.h"
#include "CAcqSetup.h"
#include "CRegu.h"
#include "CBalance.h"
#include "CAskNameDlg.h"
#include "CNewTpl.h"

#include "maj.xpm"


CMainWnd::CMainWnd(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCAPTION|wxCLOSE_BOX|wxCLIP_CHILDREN), m_pTimer(this, TIMER)
{
	m_pProject		= CProject::Instance();
	m_pMeasurements = CMeasurements::Instance();

	m_nIdTab	   = 0;
	m_DateStartAcq = 0;

    m_pMenuProg        = new wxMenu;
	m_pMenuAcq         = new wxMenu;
	m_pMenuMeas        = new wxMenu;
	m_pMenuLog         = new wxMenu;
	m_pMenuTpl		   = new wxMenu;
	m_pMenuTools       = new wxMenu;
	m_pMenuToolsSubReg = new wxMenu;
    
    m_pMenuProg ->Append(PROG_QUIT,       "Quit", "Quit the program");
	m_pMenuTpl  ->Append(TPL_NEW,         "New",  "New project template");
	m_pMenuAcq  ->Append(PJT_OPEN,        "Open", "Open a project");
	m_pMenuAcq  ->Append(PJT_NEW,         "New",  "Setup and start a new project");
	m_pMenuAcq  ->Append(PJT_NEW_TPL,     "New with a template", "New project from a template");
	m_pMenuAcq  ->Append(PJT_STOP,        "Stop", "Stop the data acquisition in progress");
	m_pMenuAcq  ->Enable(PJT_STOP, false);
	m_pMenuMeas ->Append(MEAS_EXPORT_CSV, "Export in CSV", "Export in a CSV text file format");
	m_pMenuMeas ->Enable(MEAS_EXPORT_CSV, false);
	m_pMenuLog  ->Append(LOG_ADD,         "Add a comment", "Add a comment in the log");
	m_pMenuLog  ->Enable(LOG_ADD, false);

	m_pMenuToolsSubReg ->Append(REGU_REBOOT,    "Reboot",   "Restart the oven temperature regulator");
	m_pMenuToolsSubReg ->Append(REGU_EXE,       "Execute",  "Start the program in the regulator memory");
	m_pMenuTools       ->Append(TOOLS_SUB_REG,  "Regulator", m_pMenuToolsSubReg);

    wxMenuBar *menu = new wxMenuBar();
    menu->Append(m_pMenuProg , "Program");
	menu->Append(m_pMenuTpl  , "Project template");
	menu->Append(m_pMenuAcq  , "Project");
	menu->Append(m_pMenuMeas , "Measurements");
	menu->Append(m_pMenuLog  , "Log");
	menu->Append(m_pMenuTools, "Tools");

    SetMenuBar(menu);

	m_pToolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER);

	wxBitmap bitmap_bu(maj_xpm);
	m_pButtonUpdate = new wxBitmapButton(m_pToolbar, BUTTON_UPDATE, bitmap_bu);
	m_pButtonUpdate->Disable();

	wxArrayString strListGraph;
	m_pMeasurements->GetVecName(&strListGraph);
	strListGraph.RemoveAt(0, 1);
	
	m_pListGraph = new wxComboBox(m_pToolbar, LIST_GRAPH, wxEmptyString, wxDefaultPosition, wxDefaultSize, strListGraph, wxCB_READONLY);
	m_pListGraph->Disable();
	m_pListGraph->SetValue(strListGraph[0]);

	m_pToolbar->AddControl(m_pButtonUpdate);
	m_pToolbar->AddControl(m_pListGraph);
	m_pToolbar->Realize();
	this->SetToolBar(m_pToolbar);

	m_pNotebook = new wxNotebook(this, NOTEBOOK, wxDefaultPosition, wxDefaultSize);

	m_pLogTextCtrl = new wxTextCtrl(m_pNotebook, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxSUNKEN_BORDER|wxTE_RICH|wxTE_READONLY);
	m_pLog         = new CLog(m_pLogTextCtrl);
    delete wxLog::SetActiveTarget(m_pLog);
	m_pNotebook->AddPage(m_pLogTextCtrl, "Log", true, 0);

	m_pMeasurementsGrid = new CMeasurementsGrid(m_pNotebook, NOTEBOOK_GRID);
	m_pNotebook->AddPage(m_pMeasurementsGrid, "Measurements", false, 1);

	m_pPlotWnd = new CPlotWnd(m_pNotebook, NOTEBOOK_PLOT);
	m_pNotebook->AddPage(m_pPlotWnd, "Plots", false, 2);

	m_pStatusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP, "statusBar");
	int widths[] = {300, 300, -1};
	m_pStatusBar->SetFieldsCount(3, widths);

	wxRect rect;
	m_pStatusBar->GetFieldRect(1, rect);
	m_pProBar = new wxGauge(m_pStatusBar, wxID_ANY, 100);
	m_pProBar->SetSize(rect.x+2, rect.y+2, 300-4, rect.height-4);
	m_pProBar->SetValue(0);

	this->SetStatusBar(m_pStatusBar);
}


void CMainWnd::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void CMainWnd::OnClose(wxCloseEvent& WXUNUSED(event))
{
	if(m_pProject->m_bIsAcquiring == true)
		if(wxMessageBox("Data acquisition is in progress. Do you want to continue?", "Confirmation", wxYES_NO|wxICON_QUESTION) != wxYES)
		{
			return;
		}
		else
		{
			wxWindowDisabler disableall;
			wxBusyInfo info("Stopping the temperature regulator...", this);

#ifndef _NO_PERIPH
			CRegu e5ck;
			if(!e5ck.StopProgramReset())
				wxMessageBox("Operation Failed", "Error");
#endif
		}

	m_pTimer.Stop();
	Destroy();
}

bool CMainWnd::Acq()
{
	// 0 :mass  1: PT100
	if(m_pProject->m_bIsAcquiring == true)
	{
		CVector **vec = new CVector*[3];
		vec[0] = m_pMeasurements->GetVec(0);
		vec[1] = m_pMeasurements->GetVec(1);
		vec[2] = m_pMeasurements->GetVec(2);
		
		wxLogMessage("Acquiring");

		wxDateTime date = wxDateTime::Now();
		double ElapsedTime = (date.GetTicks()-m_DateStartAcq)/60;
		
		double mass = 0;
#ifndef _NO_PERIPH
		CBalance kern;
		if(!kern.GetMass(&mass))
			mass = 0;
#else
		mass = ElapsedTime * 2;
#endif

		vec[0]->Add(ElapsedTime);
		vec[1]->Add(mass); 

		wxSleep(1);

		double PT100 = 0;
#ifndef _NO_PERIPH
		CRegu e5ck;
		int data_regu;
		if(e5ck.Command(1,0,0, &data_regu))
			PT100 = (double)data_regu/10;
		else
			PT100 = 0;
#else
		PT100 = ElapsedTime * 3;
#endif

		vec[2]->Add(PT100); 
		m_pMeasurements->Save();

		// End of data acquisition ?
		size_t nb_meas = vec[0]->GetSize();

		CAcqSetup* acq_cfg  = m_pProject->GetAcqSetup();
		double acq_duration = acq_cfg->GetDuration(); // H
		double acq_period   = acq_cfg->GetPeriod();   // min

		if(nb_meas >= (size_t)(acq_duration*60/acq_period))
		{
			m_pProBar->SetValue(100);

			wxString strProgress;
			strProgress.Printf("100 %c", 37);
			m_pStatusBar->SetStatusText(strProgress, 2);	

			wxCommandEvent evt;
			this->OnExportCVS(evt);

			this->OnPjtStop(evt);
			delete[] vec;

			return true;
		}

		// show the progress
		int iProgress = (int)((nb_meas*acq_period*100)/(acq_duration*60));
		m_pProBar->SetValue(iProgress);

		wxString strProgress;
		strProgress.Printf("%d %c", iProgress, 37);
		m_pStatusBar->SetStatusText(strProgress, 2);		

		delete[] vec;
	}

	return true;
}


void CMainWnd::OnTimer(wxTimerEvent& WXUNUSED(event))
{

	wxWindowDisabler disableall;
	wxBusyInfo info("Data acquisition is in progress...", this);

	this->Acq();

	wxCommandEvent event;
	this->OnUpdateData(event);
}

void CMainWnd::OnLogAdd(wxCommandEvent& WXUNUSED(event))
{
	wxTextEntryDialog dlg(this, "Please enter your comment", "Add a comment", wxEmptyString, wxOK);
	if(dlg.ShowModal() == wxID_OK)
	{
		wxLogMessage(dlg.GetValue());
	}
}

void CMainWnd::OnTplNew(wxCommandEvent& WXUNUSED(event))
{
	m_pProject->Reset();
	CNewTpl dlg(this);
}

void CMainWnd::OnPjtOpen(wxCommandEvent& WXUNUSED(event))
{
	m_pProject->Reset();

	wxFileDialog dlg(this, "Open a project", m_pProject->GetRep(), wxEmptyString, "*.prj", wxFD_OPEN);
	if(dlg.ShowModal() == wxID_OK)
	{
		wxFileName file_name(dlg.GetPath());
	
		m_pProject->SetRep(file_name.GetPath());
		m_pProject->SetFileName(file_name.GetName());

		wxFile file;
		if(!file.Open(dlg.GetPath(), wxFile::read))
		{
			wxMessageBox("Unable to open the project.", "Error");
			return;
		}

		if(m_pProject->Open(&file) != true)
		{
			wxMessageBox("Unable to open the project.", "Error", wxOK|wxICON_ERROR);
			return;
		}

		if(m_pMeasurements->Open() != true)
		{
			wxMessageBox("Unable to read the measurements.", "Error", wxOK|wxICON_ERROR);
			return;
		}

		if(m_pLog->Open() != true)
			wxMessageBox("Unable to open the log file.", "Error", wxOK|wxICON_ERROR);

		m_pMenuMeas->Enable(MEAS_EXPORT_CSV, true);

		m_pProject->m_bIsReadOnly = true;
		m_pProject->m_bIsAcquiring = false;

		m_pMeasurementsGrid->Clear();
		m_pNotebook->ChangeSelection(0);
	}
}

void CMainWnd::OnPjtNew(wxCommandEvent& WXUNUSED(event))
{
	m_pLog->Clear();
	m_pMeasurements->Reset();
	m_pProject     ->Reset();

	CNewAcq dlg(this, false);
	if(m_pProject->m_bIsAcquiring == true)
	{
		CAcqSetup *acq_setup = m_pProject->GetAcqSetup();
		int millisec         = acq_setup ->GetPeriod()*60*1000;

		if(m_pTimer.Start(millisec) != true)
		{
			m_pProject->m_bIsAcquiring = false;
			wxMessageBox("Unable to start the timer.", "Error");
			return;
		}

		wxDateTime date = wxDateTime::Now();
		m_DateStartAcq  = date.GetTicks();
		this->Acq();

		m_pMenuTpl  ->Enable(TPL_NEW,         false);
		m_pMenuAcq  ->Enable(PJT_NEW_TPL,     false);
		m_pMenuAcq  ->Enable(PJT_OPEN,        false);
		m_pMenuAcq  ->Enable(PJT_NEW,         false);
		m_pMenuAcq  ->Enable(PJT_STOP,        true);
		m_pMenuLog  ->Enable(LOG_ADD,         true);
		m_pMenuMeas ->Enable(MEAS_EXPORT_CSV, true);
		m_pMenuTools->Enable(TOOLS_SUB_REG,   false);

		m_pProBar->SetValue(0);

		m_pMeasurementsGrid->Clear();
		m_pNotebook->ChangeSelection(0);
	}
}

void CMainWnd::OnPjtNewTpl(wxCommandEvent& WXUNUSED(event))
{
	m_pLog->Clear();
	m_pMeasurements->Reset();
	m_pProject     ->Reset();

	wxDirDialog dlg(this, "Template folder", m_pProject->GetRep()+"\\"+FOLDER_NAME_WITHOUT_TPL, wxDD_DEFAULT_STYLE |wxDD_DIR_MUST_EXIST);
	if(dlg.ShowModal() == wxID_OK)
	{
		wxString file = wxFindFirstFile(dlg.GetPath()+"\\*.tpl");
		if(file.empty())
		{
			wxMessageBox("The file template is missing.", "Error");
			return;
		}
		
		wxFileName file_name(file);

		wxFile file_tpl;
		if(!file_tpl.Open(file, wxFile::read))
		{
			wxMessageBox("Unable to open the project template.", "Error");
			return;
		}

		m_pProject->Open  (&file_tpl);
		m_pProject->SetTpl(file_name.GetName());

		CNewAcq dlg(this, true);
		if(m_pProject->m_bIsAcquiring == true)
		{
			CAcqSetup *acq_setup = m_pProject->GetAcqSetup();
			int millisec         = acq_setup ->GetPeriod()*60*1000;

			if(m_pTimer.Start(millisec) != true)
			{
				m_pProject->m_bIsAcquiring = false;
				wxMessageBox("Unable to start the timer.", "Error");
				return;
			}

			wxDateTime date = wxDateTime::Now();
			m_DateStartAcq  = date.GetTicks();
			this->Acq();

			m_pMenuTpl  ->Enable(TPL_NEW,     false);
			m_pMenuAcq  ->Enable(PJT_NEW_TPL, false);
			m_pMenuAcq  ->Enable(PJT_OPEN,    false);
			m_pMenuAcq  ->Enable(PJT_NEW,     false);
			m_pMenuAcq  ->Enable(PJT_STOP,    true);
			m_pMenuLog  ->Enable(LOG_ADD,     true);
			m_pMenuMeas ->Enable(MEAS_EXPORT_CSV, true);
			m_pMenuTools->Enable(TOOLS_SUB_REG,   false);

			m_pProBar->SetValue(0);

			m_pMeasurementsGrid->Clear();
			m_pNotebook->ChangeSelection(0);
		}
	}
}

void CMainWnd::OnPjtStop(wxCommandEvent& WXUNUSED(event))
{
	wxLogMessage("Data acquisition stopped/finished.");

	m_pProject->m_bIsAcquiring = false;
	m_pProject->m_bIsReadOnly  = true;

	m_pMenuTpl  ->Enable(TPL_NEW,         true);
	m_pMenuAcq  ->Enable(PJT_NEW_TPL,     true);
	m_pMenuAcq  ->Enable(PJT_OPEN,        true);
	m_pMenuAcq  ->Enable(PJT_NEW,         true);
	m_pMenuAcq  ->Enable(PJT_STOP,        false);
	m_pMenuLog  ->Enable(LOG_ADD,         false);
	m_pMenuMeas ->Enable(MEAS_EXPORT_CSV, false);
	m_pMenuTools->Enable(TOOLS_SUB_REG,   true);

	m_pTimer.Stop();

#ifndef _NO_PERIPH
	CRegu e5ck;
	if(!e5ck.StopProgramReset())
		wxMessageBox("Unable to stop the temperature regulator.");
#endif
}

void CMainWnd::OnRegReboot(wxCommandEvent& WXUNUSED(event))
{
#ifndef _NO_PERIPH
	CRegu e5ck;
	if(!e5ck.Reboot())
		wxMessageBox("Unable to restart the temprature regulator.", "Error");
#endif
}

void CMainWnd::OnRegExe(wxCommandEvent& WXUNUSED(event))
{
#ifndef _NO_PERIPH
	CRegu e5ck;
	if(!e5ck.StartProgram())
		wxMessageBox("Unable to start the temperature program.", "Error");
#endif
}

void CMainWnd::OnExportCVS(wxCommandEvent& WXUNUSED(event))
{
	if(m_pMeasurements->ExportCSV() != true)
		wxMessageBox("Unable to export the measurements.", "Error", wxICON_ERROR|wxOK);
}

void CMainWnd::OnGraphChange(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString ListGraph;
	m_pMeasurements->GetVecName(&ListGraph);

	for(size_t i=1; i<ListGraph.Count(); i++)
	{
		if(ListGraph[i] == m_pListGraph->GetValue())
		{
			wxCommandEvent evt(wxEVT_COMMAND_GRAPH_SELECTION_CHANGED, 800);
			evt.SetInt((int)(i-1));
			wxPostEvent(m_pPlotWnd, evt);
		}
	}
}

void CMainWnd::OnTabChanged(wxNotebookEvent& event)
{
	if(m_pProject->m_bIsAcquiring != true && m_pProject->m_bIsReadOnly != true)
		return;

	int IdTab = event.GetSelection();
	switch(IdTab)
	{
	case 0: 
		m_pListGraph   ->Disable();
		m_pButtonUpdate->Disable();
		m_nIdTab = 0;
		break;
	case 1:
		m_pListGraph   ->Disable();
		m_pButtonUpdate->Enable();
		m_nIdTab = 1;
		break;
	case 2:
		m_pListGraph   ->Enable();
		m_pButtonUpdate->Enable();
		m_nIdTab = 2;
		break;
	}
}

void CMainWnd::OnUpdateData(wxCommandEvent& WXUNUSED(event))
{
	if(m_nIdTab == 1)
	{
		wxGridEvent evt;
		m_pMeasurementsGrid->OnCellLeftDClick(evt);
	}
	else if(m_nIdTab == 2)
	{
		m_pPlotWnd->UpdateAll();
	}
}

BEGIN_EVENT_TABLE(CMainWnd, wxFrame)
	EVT_TIMER(TIMER,          CMainWnd::OnTimer)
    EVT_MENU(PROG_QUIT,       CMainWnd::OnQuit)
	EVT_MENU(LOG_ADD,         CMainWnd::OnLogAdd)
	EVT_MENU(TPL_NEW,         CMainWnd::OnTplNew)
	EVT_MENU(PJT_OPEN,        CMainWnd::OnPjtOpen)
	EVT_MENU(PJT_NEW,         CMainWnd::OnPjtNew)
	EVT_MENU(PJT_NEW_TPL,     CMainWnd::OnPjtNewTpl)
	EVT_MENU(PJT_STOP,        CMainWnd::OnPjtStop)
	EVT_MENU(MEAS_EXPORT_CSV, CMainWnd::OnExportCVS)
	EVT_MENU(REGU_REBOOT,     CMainWnd::OnRegReboot)
	EVT_MENU(REGU_EXE,        CMainWnd::OnRegExe)
	EVT_NOTEBOOK_PAGE_CHANGED(NOTEBOOK, CMainWnd::OnTabChanged)
	EVT_BUTTON(BUTTON_UPDATE, CMainWnd::OnUpdateData)
	EVT_COMBOBOX(LIST_GRAPH, OnGraphChange)
	EVT_CLOSE(CMainWnd::OnClose)
END_EVENT_TABLE()