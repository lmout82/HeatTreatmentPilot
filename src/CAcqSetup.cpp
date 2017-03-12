//**********************************************************************************************
//                                        CAcqSetup.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "CAcqSetup.h"


CAcqSetup::CAcqSetup()
{
	m_dDuration = 0;
	m_dPeriod   = 1;
}

void CAcqSetup::Reset()
{
	m_dDuration = 0;
	m_dPeriod   = 1;
}

CAcqSetup::~CAcqSetup()
{
}

void CAcqSetup::SetDuration(double duration)
{
	m_dDuration = duration;
}

void CAcqSetup::SetPeriod(double period)
{
	m_dPeriod = period;
}

double CAcqSetup::GetDuration()
{
	return m_dDuration;
}

double CAcqSetup::GetPeriod()
{
	return m_dPeriod;
}