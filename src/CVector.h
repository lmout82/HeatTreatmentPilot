//**********************************************************************************************
//                                        CVector.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CVector_H
#define CVector_H

#include <vector>
#include "wx/wx.h"


class CVector
{
public:
	CVector();
	~CVector();

	void Add(double);
	bool Get(size_t, double*);
	void   GetMinMax(double*, double*);
	size_t GetSize();

private:
	double m_dMin;
	double m_dMax;
	std::vector <double> m_Measurements;
};

#endif