//**********************************************************************************************
//                                        CMeasurementsGrid.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CMesuresGrid_H
#define CMesuresGrid_H

#include "wx/wx.h"
#include "wx/grid.h"

class CMeasurements;
class CProject;


class CMeasurementsGrid : public wxGrid
{
public:
	CMeasurementsGrid(wxWindow*, wxWindowID);
	~CMeasurementsGrid();

	void OnCellLeftDClick(wxGridEvent&);

	void Clear();

private:
	CMeasurements *m_pMeasurements;
	CProject      *m_pProject;
	size_t m_NbLine;
	size_t m_NbCol;

	DECLARE_EVENT_TABLE()
};

#endif