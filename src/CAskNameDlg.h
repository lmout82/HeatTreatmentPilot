//**********************************************************************************************
//                                        CAskNameDlg.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CAskNameDlg_H
#define CAskNameDlg_H

#include "wx/wx.h"


class CAskNameDlg : public wxTextEntryDialog
{
public:
	CAskNameDlg(wxWindow*, const wxString&, const wxString&);
	wxString GetValue() const;
};

#endif