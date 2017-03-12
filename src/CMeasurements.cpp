//**********************************************************************************************
//                                        CMeasurements.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include <vector>

#include "wx/wx.h"
#include "wx/file.h"
#include "wx/textfile.h"

#include "Cst.h"
#include "CMeasurements.h"
#include "CProject.h"


CMeasurements *CMeasurements::m_Singleton = NULL;

CMeasurements::CMeasurements()
{
	m_pProject    = CProject::Instance();
	m_strRep      = wxEmptyString;
	m_strFileName = wxEmptyString;
	m_pVecName    = new wxArrayString;
}

CMeasurements::~CMeasurements()
{
	for(size_t i=0; i<m_Data.size(); i++)
	{
		delete m_Data[i];
	}
	m_Data.clear();

	delete m_pVecName;
}

CMeasurements* CMeasurements::Instance()
{
	if (NULL == m_Singleton)
    {
		m_Singleton = new CMeasurements();
    }
    
    return m_Singleton;
}

void CMeasurements::Destroy()
{
	if (NULL != m_Singleton)
    {
		delete m_Singleton;
        m_Singleton = NULL;
    }
}

void CMeasurements::AddVec(size_t nb_serie, wxArrayString list)
{
	for(size_t i=0; i<nb_serie; i++)
	{
		m_Data.push_back(new CVector());
		*m_pVecName = list;
	}
}

void CMeasurements::AddVec(size_t nb_serie)
{
	for(size_t i=0; i<nb_serie; i++)
	{
		m_Data.push_back(new CVector());
	}
}

void CMeasurements::GetVecName(wxArrayString* list)
{
	*list = *m_pVecName;
}

size_t CMeasurements::GetNbVec()
{
	return m_Data.size();
}

CVector* CMeasurements::GetVec(size_t id)
{
	if(id<0 || id>=m_Data.size())
		return NULL;

	return m_Data[id];
}

void CMeasurements::Reset()
{
	size_t NbVec = this->GetNbVec();

	for(size_t i=0; i<m_Data.size(); i++)
	{
		delete m_Data[i];
	}
	m_Data.clear();

	this->AddVec(NbVec);
}

bool CMeasurements::Open()
{
	m_strRep      = m_pProject->GetRep();
	m_strFileName = m_pProject->GetFileName();

	wxFile file;
	file.Open(this->m_strRep+"\\"+this->m_strFileName+".dat", wxFile::read);

	if(file.IsOpened() == false)
		return false;
	
	size_t NbMeas;
	file.Read(&NbMeas, sizeof(size_t));

	// depuis la sup des series -> vecteur, le nb de vec est egal au nb serie +1
	size_t NbVec;
	file.Read(&NbVec, sizeof(size_t));
	NbVec += 1;

	this->Reset();

	CVector **ListVec;
	ListVec = new CVector*[NbVec];
	for(size_t i=0; i<NbVec; i++)
	{
		ListVec[i] = this->GetVec(i);
	}

	double X, Y;
	for(size_t i=0; i<NbMeas; i++)
	{
		file.Read(&X, sizeof(double));
		ListVec[0]->Add(X);

		for(size_t j=1; j<NbVec; j++)
		{
			file.Read(&Y, sizeof(double));
			ListVec[j]->Add(Y);
		}
	}

	file.Close();
	delete[] ListVec;

	return true;
}

bool CMeasurements::Save()
{
	wxString project_path;

	if(m_pProject->GetTpl() == "__NO_TPL__")
		project_path = m_pProject->GetRep()+"\\"+FOLDER_NAME_WITHOUT_TPL+"\\"+m_pProject->GetFileName();
	else
		project_path = m_pProject->GetRep()+"\\"+m_pProject->GetTpl()+"\\"+m_pProject->GetFileName();

	size_t NbMeas  = m_Data[0]->GetSize();
	size_t NbSerie = m_Data.size();

	if(!wxFile::Exists(project_path+"\\"+m_pProject->GetFileName()+".dat"))
	{
		wxFile file;
		bool res = file.Create(project_path+"\\"+m_pProject->GetFileName()+".dat");

		if(res == false)
			return false;

		file.Write(&NbMeas, sizeof(size_t));

		NbSerie -= 1;
		file.Write(&NbSerie, sizeof(size_t));
		NbSerie += 1;

		double X, Y;
		for(size_t i=0; i<NbMeas; i++)
		{
			m_Data[0]->Get(i, &X);

			file.Write(&X, sizeof(double));
			for(size_t j=1; j<NbSerie; j++)
			{
				m_Data[j]->Get(i, &Y); 
				file.Write(&Y, sizeof(double));
			}
		}

		file.Close();
	}
	else
	{
		wxFile file;
		bool res = file.Open(project_path+"\\"+m_pProject->GetFileName()+".dat", wxFile::read_write);
		
		if(res == false)
			return false;

		size_t NbMeasF;
		file.Read(&NbMeasF, sizeof(size_t));

		if(NbMeas>NbMeasF)
		{
			file.Seek(0);
			file.Write(&NbMeas, sizeof(size_t));
			file.SeekEnd();

			double X, Y;
			for(size_t i=NbMeasF; i<NbMeas; i++)
			{
				m_Data[0]->Get(i, &X);
				file.Write(&X, sizeof(double));

				for(size_t j=1; j<NbSerie; j++)
				{
					m_Data[j]->Get(i, &Y);
					file.Write(&Y, sizeof(double));
				}
			}
		}

		file.Close();
	}

	return true;
}

bool CMeasurements::ExportCSV()
{
	wxString project_path;

	if(m_pProject->m_bIsAcquiring == true)
	{
		if(m_pProject->GetTpl() == "__NO_TPL__")
			project_path = m_pProject->GetRep()+"\\"+FOLDER_NAME_WITHOUT_TPL+"\\"+m_pProject->GetFileName();
		else
			project_path = m_pProject->GetRep()+"\\"+m_pProject->GetTpl()+"\\"+m_pProject->GetFileName();
	}
	else if(m_pProject->m_bIsReadOnly == true)
	{
		project_path = m_pProject->GetRep();
	}

	wxTextFile file(project_path+"\\"+m_pProject->GetFileName()+".csv");
	if(file.Exists())
		file.Clear();
	else
		file.Create();

	size_t NbVec = m_Data.size();
	size_t NbMeas = m_Data[0]->GetSize();
	
	wxString str1Line;
	
	for(size_t i=0; i<m_pVecName->Count(); i++)
	{
		str1Line += (*m_pVecName)[i]+';';
	}
	file.AddLine(str1Line);

	for(size_t i=0; i<NbMeas; i++)
	{
		wxString X0, Y0;
		double dX0 = 0, dY0 = 0;

		m_Data[0]->Get(i, &dX0);
		m_Data[1]->Get(i, &dY0);
		X0.Printf("%lf", dX0);
		Y0.Printf("%lf", dY0);
		str1Line = X0+';'+Y0+';';

		for(size_t j=2; j<NbVec; j++)
		{
			wxString Xn, Yn;
			double dXn = 0, dYn = 0;

			m_Data[j]->Get(i, &dYn);
			Yn.Printf("%lf", dYn);
			str1Line += Yn+';';
		}

		file.AddLine(str1Line);
		str1Line = wxEmptyString;
	}

	file.Write();
	file.Close();

	return true;
}