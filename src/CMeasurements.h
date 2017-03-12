//**********************************************************************************************
//                                        CMeasurements.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CMeasurements_H
#define CMeasurements_H

#include <vector>
#include "wx/wx.h"
#include "CVector.h"

class CProject;


class CMeasurements
{
public:
	static CMeasurements* Instance();
	static void Destroy();

	void AddVec(size_t, wxArrayString);
	void AddVec(size_t);
	void GetVecName(wxArrayString*);
	size_t   GetNbVec();
	CVector* GetVec(size_t);

	bool Open();
	bool Save();
	bool ExportCSV();
	void Reset();

private:
	CMeasurements();
	~CMeasurements();

	wxString m_strRep;
	wxString m_strFileName;
	CProject*              m_pProject;
	static CMeasurements*  m_Singleton;
	wxArrayString*         m_pVecName;
	std::vector <CVector*> m_Data;	
};

#endif