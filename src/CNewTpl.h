//**********************************************************************************************
//                                        CNewTpl.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CNewTpl_H
#define CNewTpl_H

#include "wx/wx.h"
#include "wx/wizard.h"

class CProject;


class CNewTpl : public wxWizard
{
public:
	CNewTpl(wxWindow*);

private:
	CProject* m_pProject;
};

#endif