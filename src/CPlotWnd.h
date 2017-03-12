//**********************************************************************************************
//                                        CPlotWnd.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CPlotWnd_H
#define CPlotWnd_H

#include "wx/wx.h"
#include "mathplot.h"

class CMesures;
class CVector;
class CProject;
class mpWindow;


class CPlotWnd : public mpWindow
{
public:
	CPlotWnd(wxWindow*, wxWindowID);
	~CPlotWnd();

	void OnPaint       (wxPaintEvent&);
	void OnGraphChanged(wxCommandEvent&);

private:
	size_t m_NbSerie;
	size_t m_IdSerie;
	CMeasurements* m_pMeasurements;
	CVector**      m_pVec;
	CProject*      m_pProject;
	mpScaleX*    m_pScaleX;
	mpScaleY*    m_pScaleY;
	mpFXYVector* m_pMPVecteur;
	
	DECLARE_EVENT_TABLE()
};

#endif