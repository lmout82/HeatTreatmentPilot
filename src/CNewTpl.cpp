//**********************************************************************************************
//                                        CNewTpl.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "wx/stdpaths.h"

#include "Cst.h"
#include "CNewTpl.h"
#include "CProject.h"
#include "CAskNameDlg.h"
#include "NewAcq.h"


CNewTpl::CNewTpl(wxWindow* parent)
	:wxWizard(parent, wxID_ANY, "Setup a project template")
{
	m_pProject = CProject::Instance();
	m_pProject->Reset();

	CAskNameDlg dlg(this, "Name of the template project (no special characters .\\/:*?\"<>|)", "New template project");

	if(dlg.ShowModal() == wxID_OK)
	{
		if(!wxDirExists(m_pProject->GetRep()+"\\"+dlg.GetValue()))
		{
			wxMkdir(m_pProject->GetRep()+"\\"+dlg.GetValue());
		}
		else
		{
			int rep = wxMessageBox("This project already exists. Overwrite it?", "Warning", wxYES_NO);
			if(rep == wxYES)
			{
				if(wxFileExists(m_pProject->GetRep()+"\\"+dlg.GetValue()+"\\"+dlg.GetValue()+".tpl"))
				{
					wxRemoveFile(m_pProject->GetRep()+"\\"+dlg.GetValue()+"\\"+dlg.GetValue()+".tpl");
				}
			}
			else
			{
				return;
			}
		}
	}
	else
	{
		return;
	}

	m_pProject->SetTpl(dlg.GetValue());

	// wizard
	this->SetPageSize(wxSize(470,350)); 

	CReguSetupPage   *page1 = new CReguSetupPage(this, false);
	CAcqSetupPage    *page2 = new CAcqSetupPage(this);
	wxWizardPageSimple::Chain(page1,page2);
	
	if(this->RunWizard(page1) == true)
	{
		wxFile prj_file;
		prj_file.Open(m_pProject->GetRep()+"\\"+m_pProject->GetTpl()+"\\"+m_pProject->GetTpl()+".tpl", wxFile::write);

		if(m_pProject->Save(&prj_file) == false)
		{
			this->Destroy();
			wxMessageBox("Unable to save the template.", "Error", wxICON_ERROR|wxOK);
			return;
		}

		wxMessageBox("The setup of the template is finished.", "Information");
	}

	this->Destroy();
}