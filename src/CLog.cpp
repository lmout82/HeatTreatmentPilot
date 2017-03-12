//**********************************************************************************************
//                                        CLog.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "wx/textfile.h"

#include "Cst.h"
#include "CProject.h"
#include "CLog.h"


CLog::CLog(wxTextCtrl *pTextCtrl)
{
	m_pProject = CProject::Instance();
	m_strRep        = wxEmptyString;
	m_strFileName   = wxEmptyString;
	m_pTextCtrl     = pTextCtrl;

	this->DisableTimestamp();
}

void CLog::Clear()
{
	m_pTextCtrl->Clear();
}

// Add only a new line in the log file if data aquisition is launched.
void CLog::DoLogText(const wxString &msg)
{
	wxString project_path, str_line;

	if (m_pProject->GetTpl() == "__NO_TPL__")
		project_path = m_pProject->GetRep()+"\\"+FOLDER_NAME_WITHOUT_TPL+"\\"+m_pProject->GetFileName();
	else
		project_path = m_pProject->GetRep()+"\\"+m_pProject->GetTpl()+"\\"+m_pProject->GetFileName();

	wxDateTime date   = wxDateTime::Now();
	wxString str_date = date.Format("%d/%m/%Y %Hh%M : ");

	str_line = str_date+msg+wxT("\r\n");
	m_pTextCtrl->AppendText(str_line);

	if (m_pProject->m_bIsAcquiring == true)
	{
		wxTextFile file(project_path+"\\"+m_pProject->GetFileName()+".log");

		if (file.Exists())
			file.Open();
		else
			file.Create();

		file.AddLine(str_line, wxTextFileType_None);
		file.Write();
		file.Close();
	}
}

bool CLog::Open()
{
	m_strRep        = m_pProject->GetRep();
	m_strFileName   = m_pProject->GetFileName();

	m_pTextCtrl->Clear();

    wxTextFile log_file(m_strRep+"\\"+m_strFileName+".log");
	if(log_file.Exists() == false)
		return false;
	
	log_file.Open();
	for(size_t i=0; i<log_file.GetLineCount(); i++)
	{
		wxString str = log_file[i]+"\r\n";
		m_pTextCtrl->WriteText(str);
	}
	log_file.Close();

	return true;
}