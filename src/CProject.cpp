//**********************************************************************************************
//                                        CProject.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "wx/wx.h"
#include "wx/log.h"
#include "wx/file.h"
#include "wx/textfile.h"
#include "wx/filename.h"
#include "wx/stdpaths.h"

#include "Cst.h"
#include "COvenSetup.h"
#include "CAcqSetup.h"
#include "CMeasurements.h"
#include "CProject.h"


CProject* CProject::m_Singleton = NULL;

CProject* CProject::Instance()
{
	if (NULL == m_Singleton)
    {
		m_Singleton = new CProject;
    }
    
    return m_Singleton;
}

void CProject::Destroy()
{
	if (NULL != m_Singleton)
    {
		delete m_Singleton;
        m_Singleton = NULL;
    }
}

CProject::CProject()
{
	wxStandardPathsBase &stdPath = wxStandardPaths::Get();
	if(!wxDirExists(stdPath.GetDocumentsDir()+"\\"+FOLDER_NAME_BASE))
	{
		wxMkdir(stdPath.GetDocumentsDir()+"\\"+FOLDER_NAME_BASE);
	}

	this->SetRep(stdPath.GetDocumentsDir()+"\\"+FOLDER_NAME_BASE);

	if(!wxDirExists(stdPath.GetDocumentsDir()+"\\"+FOLDER_NAME_BASE+"\\"+FOLDER_NAME_WITHOUT_TPL))
	{
		wxMkdir(stdPath.GetDocumentsDir()+"\\"+FOLDER_NAME_BASE+"\\"+FOLDER_NAME_WITHOUT_TPL);
	}

	m_strFileName  = wxEmptyString;
	m_strTpl       = "__NO_TPL__";
	m_bIsAcquiring = false;
	m_bIsReadOnly  = false;
	m_pAcqSetup    = new CAcqSetup();
	m_pOvenSetup   = new COvenSetup();
}

void CProject::Reset()
{
	wxStandardPathsBase &stdPath = wxStandardPaths::Get();
	this->SetRep(stdPath.GetDocumentsDir()+"\\"+FOLDER_NAME_BASE);

	m_strFileName    = wxEmptyString;
	m_strTpl         = "__NO_TPL__";
	m_bIsAcquiring   = false;
	m_bIsReadOnly    = false;
	m_dTotalDuration = 0;

	m_pAcqSetup ->Reset();
	m_pOvenSetup->Reset();
}

CProject::~CProject()
{
	delete m_pAcqSetup;
	delete m_pOvenSetup; 
}

double CProject::GetTotalDuration()
{
	return m_dTotalDuration;
}

void CProject::SetTotalDuration(double duration)
{
	if(duration>0)
		m_dTotalDuration = duration;
}

void CProject::SetTpl(wxString tpl_name)
{
	m_strTpl = tpl_name;
}

wxString CProject::GetTpl()
{
	return m_strTpl;
}

bool CProject::Open(wxFile* prj_file)
{
	if(prj_file->IsOpened() == false)
		return false;
	
	long Version;
	prj_file->Read(&Version, sizeof(long));

	// Version 2 : add the name of the project template
	if(Version == 2)
	{
		size_t len = 0;
		prj_file->Read(&len, sizeof(size_t));
		char *pstrMod = new char[len+1];
		prj_file->Read(pstrMod, sizeof(char)*(len+1));
		m_strTpl.Printf("%s", pstrMod);
		delete[] pstrMod;
	}

	prj_file->Read(m_pAcqSetup,  sizeof(CAcqSetup));

	if(Version == 1)
	{
		long lRefTime = 0;
		prj_file->Read(&lRefTime, sizeof(long));
		m_pOvenSetup->SetRefTime(lRefTime);
	}

	long lNbStep = 0;
	prj_file->Read(&lNbStep, sizeof(long));
	COvenStep *pStep = new COvenStep[lNbStep];
	for(long i=0; i<lNbStep; i++)
	{
		prj_file->Read(&pStep[i], sizeof(COvenStep));
	}

	double dTotalDuration;
	prj_file->Read(&dTotalDuration, sizeof(double));
	this->SetTotalDuration(dTotalDuration);

	m_pOvenSetup->SetNbStep(lNbStep);
	m_pOvenSetup->SetSteps(pStep);
	delete[] pStep;

	prj_file->Close();

	return true;
}


bool CProject::Save(wxFile *file)
{	
	if(file->IsOpened() == false)
		return false;

	long version = 2;
	file->Write(&version, sizeof(version));

	size_t len = m_strTpl.Len();
	file->Write(&len, sizeof(size_t));
	file->Write(m_strTpl.c_str(), sizeof(char)*(len+1));

	file->Write(m_pAcqSetup, sizeof(CAcqSetup));

	long lNbStep = m_pOvenSetup->GetNbStep();
	file->Write(&lNbStep, sizeof(long));
	COvenStep* pStep = new COvenStep[lNbStep];
	m_pOvenSetup->GetSteps(pStep);
	for(long i=0; i<lNbStep; i++)
	{
		file->Write(&pStep[i], sizeof(COvenStep));
	}

	file->Write(&m_dTotalDuration, sizeof(double));

	file->Close();
	delete[] pStep;

	return true;
}

void CProject::SetRep(wxString rep)
{
	m_strRep = rep;
}

wxString CProject::GetRep()
{
	return m_strRep;
}

CAcqSetup* CProject::GetAcqSetup()
{
	return m_pAcqSetup;
}

COvenSetup* CProject::GetOvenSetup()
{
	return m_pOvenSetup;
}

void CProject::SetFileName(wxString name)
{
	m_strFileName = name;
}

wxString CProject::GetFileName()
{
	return m_strFileName;
}