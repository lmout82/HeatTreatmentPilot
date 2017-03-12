//**********************************************************************************************
//                                        COvenStep.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "COvenStep.h"


COvenStep::COvenStep()
{
	m_lType     = 0;
	m_dDuration = 0;
	m_dTemp     = 0;
	m_dGradient = 0;
}

// Step: 0, Rampe : 1
void COvenStep::SetType(long type)
{
	m_lType = type;
}


void COvenStep::SetDuration(double duration)
{
	m_dDuration = duration;
}

void COvenStep::SetTemp(double temp)
{
	m_dTemp = temp;
}

void COvenStep::SetGradient(double gradient)
{
	m_dGradient = gradient;
}

long   COvenStep::GetType()
{
	return m_lType;
}


double COvenStep::GetDuration()
{
	return m_dDuration;
}

double COvenStep::GetTemp()
{
	return m_dTemp;
}

double COvenStep::GetGradient()
{
	return m_dGradient;
}