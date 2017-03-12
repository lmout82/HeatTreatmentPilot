//**********************************************************************************************
//                                        NewAcq.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef NewAcq_H
#define NewAcq_H

#include "wx/wx.h"
#include "wx/grid.h"
#include "wx/wizard.h"

class CProject;


class CAcqSetupPage : public wxWizardPageSimple
{
public:
	CAcqSetupPage(wxWizard*);
	~CAcqSetupPage();

	void CreateCtrl();

	bool TransferDataToWindow();
	bool TransferDataFromWindow();

private:
	CProject* m_pProject;
	wxArrayString* m_pRefTime;
	double m_dAcqDuration;
	double m_dAcqPeriod;
	long   m_lRefTime;
};


class CReguSetupPage : public wxWizardPageSimple
{
public:
	CReguSetupPage(wxWizard*, bool);
	~CReguSetupPage();

	void CreateCtrl();

	bool TransferDataToWindow();
	bool TransferDataFromWindow();

	void OnCellChange(wxGridEvent&);

private:
	CProject *m_pProject;
	wxGrid   *m_pGrid;
	bool      m_bReadOnly;

	DECLARE_EVENT_TABLE()
};


class CNewAcq : public wxWizard
{
public:
	CNewAcq(wxWindow*, bool);
	~CNewAcq();

private:
	CProject* m_pProject;
};

#endif