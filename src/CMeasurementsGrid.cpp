//**********************************************************************************************
//                                        CMeasurementsGrid.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "wx/wx.h"

#include "Cst.h"
#include "CMeasurements.h"
#include "CProject.h"
#include "CMeasurementsGrid.h"


CMeasurementsGrid::CMeasurementsGrid(wxWindow *parent, wxWindowID id)
	:wxGrid(parent, id)
{
	m_pMeasurements = CMeasurements::Instance();
	m_pProject      = CProject::Instance();
	m_NbLine = 1;
	
	wxArrayString ListGraph;
	m_pMeasurements->GetVecName(&ListGraph);
	m_NbCol = ListGraph.Count();

	this->CreateGrid((int)m_NbLine,(int)m_NbCol);

	for(size_t i=0; i<ListGraph.Count(); i++)
	{
		this->SetColLabelValue((int)i, ListGraph[i]);
	}

	this->AutoSizeColumns();
}

CMeasurementsGrid::~CMeasurementsGrid()
{
}

void CMeasurementsGrid::Clear()
{
	this->ClearGrid();
}

void CMeasurementsGrid::OnCellLeftDClick(wxGridEvent& WXUNUSED(event))
{
	size_t NbVec = m_pMeasurements->GetNbVec();
	
	if(NbVec>0)
	{
		CVector **pVec = new CVector*[NbVec];
		for(size_t i=0; i<NbVec; i++)
		{
			pVec[i] = m_pMeasurements->GetVec(i);
		}
		
		size_t NbMeas = pVec[0]->GetSize();
		if(m_NbLine<NbMeas)
		{
			this->InsertRows((int)m_NbLine, (int)(NbMeas-m_NbLine));
			m_NbLine += NbMeas-m_NbLine;
		}

		double Val = 0;
		for(size_t i=0; i<NbVec; i++)
		{
			for(size_t j=0; j<NbMeas; j++)
			{
				pVec[i]->Get(j, &Val);

				wxString strVal;
				strVal.Printf("%0.4f", Val);

				this->SetCellValue((int)j, (int)i, strVal);
				this->SetReadOnly((int)j, (int)i);				
			}
		}
		
		delete[] pVec;
	}

	this->AutoSizeColumns();
}

BEGIN_EVENT_TABLE(CMeasurementsGrid, wxGrid)
	EVT_GRID_CELL_LEFT_DCLICK(CMeasurementsGrid::OnCellLeftDClick)
END_EVENT_TABLE()