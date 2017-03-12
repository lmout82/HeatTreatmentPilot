//**********************************************************************************************
//                                        NewAcq.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "wx/filename.h"
#include "wx/file.h"
#include "wx/utils.h"
#include "wx/busyinfo.h"
#include "wx/stdpaths.h"

#include "Cst.h"
#include "NewAcq.h"
#include "CProject.h"
#include "CAcqSetup.h"
#include "COvenSetup.h"
#include "CRegu.h"
#include "CAskNameDlg.h"

enum
{
	ID_ACQ_DURATION = 7000,
	ID_ACQ_PERIOD,
	ID_REGU_GRID
};


CAcqSetupPage::CAcqSetupPage(wxWizard *parent)
	:wxWizardPageSimple(parent)
{
	m_pProject = CProject::Instance();
	CAcqSetup* acq_setup = m_pProject->GetAcqSetup();
	m_dAcqDuration       = acq_setup->GetDuration();
	m_dAcqPeriod         = acq_setup->GetPeriod();

	this->CreateCtrl();
}

void CAcqSetupPage::CreateCtrl()
{
	new wxStaticBox (this, wxID_ANY, wxT("Acquisition"), wxPoint(9,8), wxSize(359,160));
	new wxStaticText(this, wxID_ANY, wxT("Duration (h)"), wxPoint(27,48));
	new wxTextCtrl  (this, ID_ACQ_DURATION, wxEmptyString, wxPoint(146,49), wxSize(98,16));
	new wxStaticText(this, wxID_ANY, wxT("Period (>=1 min)"), wxPoint(28,72));
	new wxTextCtrl  (this, ID_ACQ_PERIOD, "1", wxPoint(146,73), wxSize(98,16));
}

bool CAcqSetupPage::TransferDataToWindow()
{
	wxTextCtrl* PeriodCtrl     = (wxTextCtrl*)FindWindow(ID_ACQ_PERIOD);
	wxTextCtrl* DurationCtrl   = (wxTextCtrl*)FindWindow(ID_ACQ_DURATION);
	
	wxString strPeriod, strDuration;
	strPeriod  .Printf("%4.1f", m_dAcqPeriod);
	strDuration.Printf("%4.2f", m_pProject->GetTotalDuration());

	PeriodCtrl  ->SetValue(strPeriod);
	DurationCtrl->SetValue(strDuration);

	COvenSetup* oven_setup = m_pProject->GetOvenSetup();

	return true;
}

bool CAcqSetupPage::TransferDataFromWindow()
{
	wxTextCtrl* PeriodCtrl   = (wxTextCtrl*)FindWindow(ID_ACQ_PERIOD);
	wxTextCtrl* DurationCtrl = (wxTextCtrl*)FindWindow(ID_ACQ_DURATION);
	
	wxString strPeriod, strDuration;
	strPeriod   = PeriodCtrl  ->GetValue();
	strDuration = DurationCtrl->GetValue();

	COvenSetup* oven_setup = m_pProject->GetOvenSetup();

	if(strPeriod.ToDouble(&m_dAcqPeriod) == false || strDuration.ToDouble(&m_dAcqDuration) == false)
	{
		wxMessageDialog(this, "Unable de convert <wxString> to <double>.", "Error", wxOK);
		return false;
	}

	if(m_dAcqPeriod>=1 && m_dAcqDuration>0)
	{
		CAcqSetup* acq_setup = m_pProject->GetAcqSetup();
		acq_setup->SetDuration(m_dAcqDuration);
		acq_setup->SetPeriod  (m_dAcqPeriod);
	}
	else
	{
		wxMessageDialog(this, "The parameters are not incorrect.", "Error", wxOK);
		return false;
	}

	return true;
}

CAcqSetupPage::~CAcqSetupPage()
{	
}


CReguSetupPage::CReguSetupPage(wxWizard *parent, bool readonly)
	:wxWizardPageSimple(parent)
{
	m_pProject  = CProject::Instance();
	m_bReadOnly = readonly;

	this->CreateCtrl();
}

void CReguSetupPage::CreateCtrl()
{
	m_pGrid = new wxGrid(this, ID_REGU_GRID);
	m_pGrid->CreateGrid(16,4);

	m_pGrid->SetColLabelValue(0, "    Type    ");
	m_pGrid->SetColLabelValue(1, "Temperature (°C)");
	m_pGrid->SetColLabelValue(2, "Gradient (°C/min)");
	m_pGrid->SetColLabelValue(3, "Duration (h)");

	m_pGrid->SetCellValue(0, 0, "Hold");
	m_pGrid->SetCellValue(0, 2, "0");
	m_pGrid->SetCellValue(0, 3, "0");
	m_pGrid->SetReadOnly (0, 0);
	m_pGrid->SetReadOnly (0, 2);
	m_pGrid->SetReadOnly (0, 3);
	m_pGrid->SetCellBackgroundColour(0, 0, *wxLIGHT_GREY);
	m_pGrid->SetCellBackgroundColour(0, 2, *wxLIGHT_GREY);
	m_pGrid->SetCellBackgroundColour(0, 3, *wxLIGHT_GREY);

	wxString step_name[] = {"", "Hold", "Gradient"};
	for(int i=0; i<=15; i++)
		m_pGrid->SetCellEditor(i, 0, new wxGridCellChoiceEditor(3, step_name));
	
	for(int j=1; j<4; j++)
	{
		m_pGrid->SetColFormatFloat(j, -1, 2);
	}

	if(m_bReadOnly == false)
	{
		for(int i=1; i<=15; i++)
			for(int j=1; j<4; j++)
				m_pGrid->SetReadOnly(i,j,true);
	}
	else
	{
		for(int i=0; i<=15; i++)
			for(int j=0; j<4; j++)
				m_pGrid->SetReadOnly(i,j,true);
	}

	m_pGrid->AutoSizeColumns();

	wxBoxSizer *grid_sizer = new wxBoxSizer(wxVERTICAL);
	grid_sizer->Add(m_pGrid, 1, wxEXPAND);
	SetSizer(grid_sizer);
	grid_sizer->Fit(this);
}

void CReguSetupPage::OnCellChange(wxGridEvent& event)
{
	int Col = event.GetCol();
	int Row = event.GetRow();

	if( Col == 0)
	{
		wxString type = m_pGrid->GetCellValue(Row, Col);

		for(int i=1; i<4; i++)
		{
			m_pGrid->SetCellBackgroundColour(Row, i, *wxWHITE);
			m_pGrid->SetReadOnly (Row, i, true);
			m_pGrid->SetCellValue(Row, i, "");
		}
		
		if(type == "Hold")
		{
			m_pGrid->SetCellBackgroundColour(Row, 2, *wxLIGHT_GREY);
			m_pGrid->SetReadOnly (Row, 1, false);
			m_pGrid->SetReadOnly (Row, 3, false);
			m_pGrid->SetCellValue(Row, 2, "0");

			if(m_pGrid->GetCellValue(Row-1, 0) == "Gradient")
			{
				wxString strTemp = m_pGrid->GetCellValue(Row-1, 1);
				m_pGrid->SetCellValue(Row, 1, strTemp);
				m_pGrid->SetReadOnly(Row, 1, true);
			}
		}

		if(type == "Gradient")
		{
			m_pGrid->SetCellBackgroundColour(Row, 3, *wxLIGHT_GREY);
			m_pGrid->SetReadOnly (Row, 1, false);
			m_pGrid->SetReadOnly (Row, 2, false);
			m_pGrid->SetCellValue(Row, 2, "");
		}
	}

	if(Col == 1)
	{
		double dT;
		wxString strT = m_pGrid->GetCellValue(Row, 1);

		if(!strT.ToDouble(&dT))
		{
			wxMessageBox("The temperature is not correct.", "Error", wxOK | wxICON_ERROR);
			m_pGrid->SetCellBackgroundColour(Row, 1, *wxRED);
		}
		else
		{
			if(dT<15 || dT>300)
			{
				wxMessageBox("The temperature is not correct. It needs to be between 15 and 300°C", "Error", wxOK | wxICON_ERROR);
				m_pGrid->SetCellBackgroundColour(Row, 1, *wxRED);
			}
			else
			{
				m_pGrid->SetCellBackgroundColour(Row, 1, *wxWHITE);

				if(m_pGrid->GetCellValue(Row, 0) == "Gradient" && m_pGrid->GetCellValue(Row+1, 1) != "")
				{
					wxString strTemp = m_pGrid->GetCellValue(Row, 1);
					m_pGrid->SetCellValue(Row+1, 1,strTemp);
				}

				wxString strGrad = m_pGrid->GetCellValue(Row, 2);
				if(strGrad != "")
					Col = 2;
			}
		}
	}

	if(Col == 2 && Row >= 1)
	{
		double deltaT, dT1, dT2, dGrad, dDuration;
		wxString strT1 = m_pGrid->GetCellValue(Row-1, 1);
		wxString strT2 = m_pGrid->GetCellValue(Row, 1);

		if(strT1.ToDouble(&dT1) && strT2.ToDouble(&dT2))
		{
			deltaT = dT2-dT1;
			wxString strGrad = m_pGrid->GetCellValue(Row, 2);

			if(strGrad.ToDouble(&dGrad))
			{ 
				if(dGrad>0)
				{
					m_pGrid->SetCellBackgroundColour(Row, 2, *wxWHITE);

					dDuration = deltaT/dGrad;
					dDuration /= 60;

					if(dDuration>0 && dDuration<=99.59)
					{
						m_pGrid->SetCellBackgroundColour(Row, 3, *wxLIGHT_GREY);
					}
					else
					{
						wxMessageBox("Some parameters are not correct.\nThe duration of a step needs to be between 0 and 99,59h.", "Error", wxOK | wxICON_ERROR);
						m_pGrid->SetCellBackgroundColour(Row, 3, *wxRED);
					}
						
					wxString strDuration;
					strDuration.sprintf("%f", dDuration);
					m_pGrid->SetCellValue(Row, 3, strDuration);
				}
				else
				{
					m_pGrid->SetCellBackgroundColour(Row, 2, *wxRED);
					wxMessageBox("A gradient can not be negative or equal to 0.", "Error", wxOK | wxICON_ERROR);
				}

			}
			else
			{
				m_pGrid->SetCellBackgroundColour(Row, 2, *wxRED);
				wxMessageBox("The gradient is incorrect.", "Error", wxOK | wxICON_ERROR);
			}

		}
		else
		{
			wxMessageBox("Some temperatures are incorrects.", "Error", wxOK | wxICON_ERROR);
		}
	}

	if(Col == 3)
	{
		wxString strDuration = m_pGrid->GetCellValue(Row, 3);
		double dDuration;
		if(strDuration.ToDouble(&dDuration))
		{
			if(dDuration<=0 || dDuration>99.59)
			{
				wxMessageBox("The duration of a step needs to be between 0 and 99,59h.", "Error", wxOK | wxICON_ERROR);
				m_pGrid->SetCellBackgroundColour(Row, 3, *wxRED);
			}
			else
			{
				m_pGrid->SetCellBackgroundColour(Row, 3, *wxWHITE);
			}
		}
		else
		{
			wxMessageBox("The duration is incorrect.", "Error", wxOK | wxICON_ERROR);
			m_pGrid->SetCellBackgroundColour(Row, 3, *wxRED);
		}
	}

	event.Skip();
}

bool CReguSetupPage::TransferDataToWindow()
{
	COvenSetup* oven_setup = m_pProject->GetOvenSetup();
	long nb_step           = oven_setup->GetNbStep();

	COvenStep* step = new COvenStep[nb_step];
	oven_setup->GetSteps(step);

	for(long i=0; i<nb_step; i++)
	{
		wxString strType, strTemp, strDuration, strGrad;

		long lType = step[i].GetType();
		if(lType == 0)
		{
			strType = "Hold";

			strTemp    .Printf("%f", step[i].GetTemp());
			strDuration.Printf("%f", step[i].GetDuration());

			m_pGrid->SetCellValue(i, 0, strType);
			m_pGrid->SetCellValue(i, 1, strTemp);
			m_pGrid->SetCellValue(i, 2, "0");
			m_pGrid->SetCellValue(i, 3, strDuration);
		}
		else if(lType == 1)
		{
			wxString strGrad;

			strType = "Gradient";

			strTemp    .Printf("%f", step[i].GetTemp());
			strDuration.Printf("%f", step[i].GetDuration());
			strGrad    .Printf("%f", step[i].GetGradient());
			
			m_pGrid->SetCellValue(i, 0, strType);
			m_pGrid->SetCellValue(i, 1, strTemp);
			m_pGrid->SetCellValue(i, 2, strGrad);
			m_pGrid->SetCellValue(i, 3, strDuration);
		}
	}

	oven_setup->SetSteps(step);

	delete[] step;

	return true;
}

bool CReguSetupPage::TransferDataFromWindow()
{
	long lNbStep = 0;
	for(int i=0; i<=15; i++)
	{ 
		if(m_pGrid->GetCellValue(i, 1) != "")
			lNbStep++;
		else
			break;
	}

	if(lNbStep <= 1)
		return false;

	bool bErr        = false;
	COvenStep *pStep = new COvenStep[lNbStep];

	double dTotalDuration = 0;
	for(int i=0; i<lNbStep; i++)
	{
		wxString strType = "";
		long lType;
		strType = m_pGrid->GetCellValue(i, 0);

		if(strType == "Hold")
			lType = 0;
		else if(strType == "Gradient")
			lType = 1;

		wxString strTemp     = m_pGrid->GetCellValue(i, 1);
		wxString strGradient = m_pGrid->GetCellValue(i, 2);
		wxString strDuration = m_pGrid->GetCellValue(i, 3);

		double dDuration = -1, dTemp = -1, dGradient = -1;
		strTemp.ToDouble(&dTemp);
		strDuration.ToDouble(&dDuration);
		strGradient.ToDouble(&dGradient);
		
		pStep[i].SetType(lType);
		pStep[i].SetDuration(dDuration);
		pStep[i].SetTemp(dTemp);
		pStep[i].SetGradient(dGradient);

		if(dDuration < 0 || dDuration > 99.59 || dTemp < 15 || dTemp > 300)
		{
			bErr = true;
			break;
		}

		dTotalDuration += dDuration;
	}

	for(int i=1; i<lNbStep; i++)
	{
		// Gradient to Gradient forbidden 
		if(m_pGrid->GetCellValue(i, 0) == "Gradient" && m_pGrid->GetCellValue(i-1, 0) == "Gradient")
		{
			bErr = true;
			break;
		}

		// Gradient-hold with same temperature
		if(m_pGrid->GetCellValue(i, 0) == "Hold" && m_pGrid->GetCellValue(i-1, 0) == "Gradient")
		{
			wxString strT1 = m_pGrid->GetCellValue(i, 1);
			wxString strT2 = m_pGrid->GetCellValue(i-1, 1);

			double dT1, dT2;
			strT1.ToDouble(&dT1);
			strT2.ToDouble(&dT2);

			if(dT1 != dT2)
			{
				bErr = true;
				break;
			}
		}

		// Hold->Gradient & Gradient->Gradient increasing temp
		if((m_pGrid->GetCellValue(i, 0) == "Gradient" && m_pGrid->GetCellValue(i-1, 0) == "Hold")||(m_pGrid->GetCellValue(i, 0) == "Gradient" && m_pGrid->GetCellValue(i-1, 0) == "Gradient"))
		{
			wxString strT1 = m_pGrid->GetCellValue(i, 1);
			wxString strT2 = m_pGrid->GetCellValue(i-1, 1);

			double dT1, dT2;
			strT1.ToDouble(&dT1);
			strT2.ToDouble(&dT2);

			if(dT1 <= dT2)
			{
				bErr = true;
				break;
			}
		}
	}

	m_pProject->SetTotalDuration(dTotalDuration);

	COvenSetup* pOvenSetup = m_pProject->GetOvenSetup(); 
	pOvenSetup->SetNbStep(lNbStep);
	pOvenSetup->SetSteps(pStep);

	delete[] pStep;

	return !bErr;
}

CReguSetupPage::~CReguSetupPage()
{
}

BEGIN_EVENT_TABLE(CReguSetupPage, wxWizardPageSimple)
	EVT_GRID_CELL_CHANGED(CReguSetupPage::OnCellChange)
END_EVENT_TABLE()


CNewAcq::CNewAcq(wxWindow* parent, bool tab_readonly)
	:wxWizard(parent, wxID_ANY, "Setup of the acquisition")
{
	m_pProject = CProject::Instance();
	wxString project_path;

	CAskNameDlg dlg(this, "Name of the project (no special characters .\\/:*?\"<>|)", "New project");
	if(dlg.ShowModal() == wxID_OK)
	{
		m_pProject->SetFileName(dlg.GetValue());

		if(m_pProject->GetTpl() == "__NO_TPL__")
			project_path = m_pProject->GetRep()+"\\"+FOLDER_NAME_WITHOUT_TPL+"\\"+dlg.GetValue();
		else
			project_path = m_pProject->GetRep()+"\\"+m_pProject->GetTpl()+"\\"+dlg.GetValue();
		
		if(!wxDirExists(project_path))
		{
			wxMkdir(project_path);
		}
		else
		{
			int rep = wxMessageBox("This project already exists. Overwrite it?", "Information", wxYES_NO);
			if(rep == wxYES)
			{
				if(wxFileExists(project_path+"\\"+dlg.GetValue()+".prj"))
				{
					wxRemoveFile(project_path+"\\"+dlg.GetValue()+".prj");
					wxRemoveFile(project_path+"\\"+dlg.GetValue()+".dat");
					wxRemoveFile(project_path+"\\"+dlg.GetValue()+".log");
					wxRemoveFile(project_path+"\\"+dlg.GetValue()+".csv");
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
		this->Destroy();
		return;
	}

	this->SetPageSize(wxSize(470,350));

	CReguSetupPage   *page1 = new CReguSetupPage(this, tab_readonly);
	CAcqSetupPage    *page2 = new CAcqSetupPage(this);
	wxWizardPageSimple::Chain(page1,page2);
	
	if(this->RunWizard(page1) == true)
	{
		wxFile prj_file;
		prj_file.Open(project_path+"\\"+m_pProject->GetFileName()+".prj", wxFile::write);

		if(m_pProject->Save(&prj_file) == false)
		{
			this->Destroy();
			wxMessageBox("Unable to save the parameters.", "Error", wxICON_ERROR|wxOK);
			return;
		}
		
		wxWindowDisabler disableall;
		wxBusyInfo info("Setting up the temperature regulator...", this);

#ifndef _NO_PERIPH

		CRegu e5ck;
		if(e5ck.Reboot())  
		{
			wxSleep(7);
			e5ck.StopProgramReset();
			e5ck.Level1(); 
			
			COvenSetup* oven_setup = m_pProject->GetOvenSetup();
			long nb_step           = oven_setup->GetNbStep();

			if(!e5ck.Command(5,60,nb_step))
			{
				wxMessageBox("An error occured when setting up the regulator.", "Error");
				this->Destroy();
				return;
			}

			if(!e5ck.Command(5,56,0))
			{
				wxMessageBox("An error occured when setting up the regulator.", "Error");
				this->Destroy();
				return;
			}

			if(!e5ck.Command(5,51,0))
			{
				wxMessageBox("An error occured when setting up the regulator.", "Error");
				this->Destroy();
				return;
			}

			COvenStep* step = new COvenStep[nb_step];
			oven_setup->GetSteps(step);

			int param_no_step, param_no_steptime;
			double Temp, Duration;

			for(int i=0; i<nb_step; i++)
			{
				wxTheApp->Yield();

				if(i<8)
				{
					param_no_step = 5+3*i;
					param_no_steptime = param_no_step+2;
				}
				else
				{
					param_no_step = 2*i+13;
					param_no_steptime = param_no_step+1;
				}

				Temp     = step[i].GetTemp();
				Duration = step[i].GetDuration();

				if(!e5ck.Command(5,param_no_step,(int)(Temp*10)))
				{
					delete[] step;
					wxMessageBox("An error occured when setting up the regulator.", "Error");
					this->Destroy();
					return;
				}

				double duration_unit, duration_dec;
				duration_unit = (int)Duration;
				duration_dec  = Duration-duration_unit;
				duration_dec *= 60;
				duration_dec /= 100;
				Duration = duration_unit+duration_dec;

				if(!e5ck.Command(5,param_no_steptime,(int)(Duration*100)))
				{
					delete[] step;
					wxMessageBox("An error occured when setting up the regulator.", "Error");
					this->Destroy();
					return;
				}
			}
	
			e5ck.Command(5,4,1);
			e5ck.Reboot();

			delete[] step;
		}
		else
		{
			wxMessageBox("An error occured when setting up the regulator.", "Error");
			this->Destroy();
			return;
		}
		
		wxSleep(7);
		wxMessageBox("The setup is finished!\n Click on OK to start data acquisition.","Acquisition setup", wxICON_INFORMATION|wxOK);
		e5ck.StartProgram();
#endif 

		m_pProject->m_bIsAcquiring = true;
		m_pProject->m_bIsReadOnly  = false;

		wxLogMessage("Settings of the acquisition and of oven regulator:");

		COvenSetup *oven_setup = m_pProject->GetOvenSetup();
		CAcqSetup *acq_setup   = m_pProject->GetAcqSetup();

		wxLogMessage("Total duration: %3.2f h", acq_setup->GetDuration());
		wxLogMessage("Period between two acquisitions: %3.2f min", acq_setup->GetPeriod());

		long lNbStep     = oven_setup->GetNbStep();
		COvenStep* pStep = new COvenStep[lNbStep];
		oven_setup->GetSteps(pStep);
		for(long i=0; i<lNbStep; i++)
		{
			if(pStep[i].GetType() == 0)
			{
				wxLogMessage("Step #%1d: type 'hold', temperature: %3.1f °C, duration: %2.2f h", i+1, pStep[i].GetTemp(), pStep[i].GetDuration()); 
			}
			else if(pStep[i].GetType() == 1)
			{
				wxLogMessage("Step #%1d: type 'gradient', temperature: %3.1f °C, gradient: %2.2f °C/min, duration %2.2f h", i+1, pStep[i].GetTemp(), pStep[i].GetGradient(), pStep[i].GetDuration()); 
			}
		}

		delete[] pStep;

		wxLogMessage("Beginning of the acquisition");
	}

	this->Destroy();
}


CNewAcq::~CNewAcq()
{
}