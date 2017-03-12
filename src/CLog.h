//**********************************************************************************************
//                                        CLog.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CLog_H
#define CLog_H

#include "wx/wx.h"
#include "wx/log.h"

class CProject;


class CLog : public wxLog
{
public:
	CLog(wxTextCtrl*);

	void DoLogText  (const wxString&);
	bool Open ();
	void Clear();

private:
	CProject*   m_pProject;
	wxTextCtrl* m_pTextCtrl;
	wxString    m_strRep;
	wxString    m_strFileName;
};

#endif