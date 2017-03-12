//**********************************************************************************************
//                                        CProject.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CProject_H
#define CProject_H

#include "wx/wx.h"
#include "wx/file.h"

class CAcqSetup;
class COvenSetup;
class CMeasurements;


class CProject
{
public:
	static CProject* Instance();
	static void Destroy();

	bool Open(wxFile*);
	bool Save(wxFile*);
	
	CAcqSetup*  GetAcqSetup();
	COvenSetup* GetOvenSetup();
	void     SetRep(wxString);
	wxString GetRep();
	void     SetFileName(wxString);
	wxString GetFileName();
	double GetTotalDuration();
	void   SetTotalDuration(double);
	void     SetTpl(wxString);
	wxString GetTpl();

	void Reset();

	bool m_bIsAcquiring;
	bool m_bIsReadOnly;

private:
	CProject();
	~CProject();

	static CProject* m_Singleton;
	wxString m_strRep;
	wxString m_strFileName;
	wxString m_strTpl;
	CAcqSetup*  m_pAcqSetup;
	COvenSetup* m_pOvenSetup;
	CMeasurements* m_pMeasurements;
	double m_dTotalDuration;
};

#endif