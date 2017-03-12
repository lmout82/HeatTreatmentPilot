//**********************************************************************************************
//                                        CVector.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "CVector.h"

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))


CVector::CVector()
{
	m_dMin = 10000000000000000;
	m_dMax = -10000000000000000;
}

CVector::~CVector()
{
	m_Measurements.clear();
}

void CVector::Add(double val)
{
	m_dMin = MIN(val, m_dMin);
	m_dMax = MAX(val, m_dMax);
	
	size_t NbLine = m_Measurements.size();

	m_Measurements.resize(NbLine+1);
	m_Measurements[NbLine] = val;
}


bool CVector::Get(size_t index, double* val)
{
	if(index>=0 && index<m_Measurements.size())
	{
		*val = m_Measurements[index];
		return true;
	}
	else
	{
		return false;
	}
}

void CVector::GetMinMax(double* min, double* max)
{
	*min = m_dMin;
	*max = m_dMax;
}

size_t CVector::GetSize()
{
	return m_Measurements.size();
}