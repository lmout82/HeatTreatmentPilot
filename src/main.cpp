//**********************************************************************************************
//                                        main.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "Cst.h"

#include "wx/wx.h"
#include "wx/snglinst.h"
#include "wx/display.h"

#include "CMainWnd.h"
#include "CProject.h"
#include "CMeasurements.h"


#ifndef _DEBUG
	#ifdef _NO_PERIPH
		#error Release build with _NO_PERIPH preprocessor
	#endif
#endif


class CApp : public wxApp
{
public:
    virtual bool OnInit();
	virtual int  OnExit();

#ifndef _DEBUG
	virtual void OnFatalException();
#endif

private:
	wxSingleInstanceChecker* m_pChecker;
};

bool CApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

#ifndef _DEBUG
	wxHandleFatalExceptions(true);
#endif

	// Only one instance of this app.
	m_pChecker = new wxSingleInstanceChecker(MAIN_WND_NAME);
	if(m_pChecker->IsAnotherRunning())
	{
		wxMessageBox("You can't have multiple instances of this application.", "Important!", wxOK|wxICON_ERROR);
		return false;
	}

	// Initialisations  
	CProject::Instance();
	CMeasurements *m_pMeasurements = CMeasurements::Instance();
	wxArrayString SerieName;
	SerieName.Add("Time (min)");
	SerieName.Add("Mass (g)");
	SerieName.Add("Oven temperature (°C) PT100");
	m_pMeasurements->AddVec(3, SerieName);	

	// Main window
#ifndef _NO_PERIPH
	CMainWnd *main_wnd = new CMainWnd(MAIN_WND_NAME);
#else
	CMainWnd *main_wnd = new CMainWnd(MAIN_WND_NAME +" [_NO_PERIPH]");
#endif

	wxDisplay screen;
	wxRect screen_size = screen.GetClientArea();
	main_wnd->SetSize(screen_size);

    main_wnd->Show(true);
    return true;
}

int CApp::OnExit()
{
	delete m_pChecker;
	CProject::Destroy();
	CMeasurements::Destroy();

	return wxApp::OnExit();
}

#ifndef _DEBUG
void CApp::OnFatalException()
{
	wxMessageBox("Your measurements are saved. Click OK to quit the program.", "The program has crashed :-(", wxOK | wxICON_ERROR);
}
#endif

IMPLEMENT_APP(CApp)