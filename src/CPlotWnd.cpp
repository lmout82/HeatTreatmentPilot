//**********************************************************************************************
//                                        CPlotWnd.cpp
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
#include "CPlotWnd.h"
#include "CProject.h"

#include "mathplot.h"


CPlotWnd::CPlotWnd(wxWindow* parent, wxWindowID id)
	:mpWindow(parent, id)
{
	m_pMeasurements = CMeasurements::Instance();
	m_pProject      = CProject::Instance();

	m_pVec = NULL;
	m_IdSerie = 0;

	this->EnableDoubleBuffer(true);

	m_pScaleX = new mpScaleX("Time(min)", mpALIGN_BOTTOM, true);
	m_pScaleY = new mpScaleY(wxEmptyString, mpALIGN_LEFT, true);

	m_pMPVecteur = new mpFXYVector(wxEmptyString);
	m_pMPVecteur ->SetContinuity(true);
	wxPen style_trace(*wxRED, 2, wxSOLID);
    m_pMPVecteur->SetPen(style_trace);

	this->AddLayer(m_pScaleX);
    this->AddLayer(m_pScaleY);
	this->AddLayer(m_pMPVecteur);
}

CPlotWnd::~CPlotWnd()
{
	delete m_pScaleX;
	delete m_pScaleY;
	delete m_pMPVecteur;
}

void CPlotWnd::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.Clear();

	if(m_pProject->m_bIsReadOnly != true && m_pProject->m_bIsAcquiring != true)
		return;

	size_t m_NbVec = m_pMeasurements->GetNbVec();
	m_pVec = new CVector*[m_NbVec];
	for(size_t i=0; i<m_NbVec; i++)
	{
		m_pVec[i] = m_pMeasurements->GetVec(i);
	}

	std::vector <float> v_time;
	std::vector <float> v_meas;
	double time, measure;

	for(int i=0; i<m_pVec[0]->GetSize(); i++)
	{
		m_pVec[0]->Get(i, &time);
		m_pVec[m_IdSerie+1]->Get(i, &measure);
		v_time.push_back(time);
		v_meas.push_back(measure);
	}

	m_pMPVecteur->SetData(v_time, v_meas);
	this->Fit();
	mpWindow::OnPaint(event);

	m_pMPVecteur->Clear();
	v_time.clear();
	v_meas.clear();
	delete[] m_pVec;
}

void CPlotWnd::OnGraphChanged(wxCommandEvent& event)
{
	if(event.GetInt()>=0)
		m_IdSerie = event.GetInt();
	else
		m_IdSerie = 0;
	
	this->UpdateAll();
}

DEFINE_EVENT_TYPE(wxEVT_COMMAND_GRAPH_SELECTION_CHANGED)

BEGIN_EVENT_TABLE(CPlotWnd, wxWindow)
    EVT_PAINT  (CPlotWnd::OnPaint)
	EVT_COMMAND(800, wxEVT_COMMAND_GRAPH_SELECTION_CHANGED, CPlotWnd::OnGraphChanged)
END_EVENT_TABLE()