//**********************************************************************************************
//                                        CMainWnd.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CMainWnd_H
#define CMainWnd_H

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/statusbr.h"
#include "wx/gauge.h"

class CPlotWnd;
class CProject;
class CLog;
class CMeasurements;
class CMeasurementsGrid;


class CMainWnd : public wxFrame
{
public:
    CMainWnd(const wxString&);

    void OnQuit       (wxCommandEvent&);
	void OnTimer      (wxTimerEvent&);
	void OnLogAdd     (wxCommandEvent&);
	void OnTplNew     (wxCommandEvent&);
	void OnPjtOpen    (wxCommandEvent&);
	void OnPjtNew     (wxCommandEvent&);
	void OnPjtNewTpl  (wxCommandEvent&);
	void OnPjtStop    (wxCommandEvent&);
	void OnExportCVS  (wxCommandEvent&);
	void OnRegReboot  (wxCommandEvent&);
	void OnRegExe     (wxCommandEvent&);
	void OnClose      (wxCloseEvent&);
	void OnGraphChange(wxCommandEvent&);
	void OnTabChanged (wxNotebookEvent&);
	void OnUpdateData (wxCommandEvent&);

	bool Acq();

private:
	wxNotebook *m_pNotebook;
	wxTextCtrl *m_pLogTextCtrl;
	CLog       *m_pLog;
	CPlotWnd   *m_pPlotWnd;
	wxToolBar  *m_pToolbar;
	wxComboBox *m_pListGraph;
	wxBitmapButton *m_pButtonUpdate;
	wxStatusBar    *m_pStatusBar;
	wxGauge        *m_pProBar;
	wxTimer m_pTimer;
	CProject*          m_pProject;
	CMeasurements*     m_pMeasurements;
	CMeasurementsGrid* m_pMeasurementsGrid;
	wxMenu *m_pMenuProg;
	wxMenu *m_pMenuAcq;
	wxMenu *m_pMenuLog;
	wxMenu *m_pMenuMeas;
	wxMenu *m_pMenuTpl;
	wxMenu *m_pMenuTools;
	wxMenu *m_pMenuToolsSubReg;
	int    m_nIdTab;
	time_t m_DateStartAcq;

    DECLARE_EVENT_TABLE()
};

#endif