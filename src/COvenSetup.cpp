//**********************************************************************************************
//                                        COvenSetup.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include <vector>
#include "COvenSetup.h"


COvenSetup::COvenSetup():m_Programm(16)
{
	m_lNbStep  = 0;
	m_lRefTime = 0;
}

void COvenSetup::Reset()
{
	m_lNbStep  = 0;
	m_lRefTime = 0;
	m_Programm.clear();
	m_Programm.resize(16);
}

COvenSetup::~COvenSetup()
{
	m_Programm.clear();
}

void COvenSetup::SetNbStep(long nb_step)
{
	m_lNbStep = nb_step;
}

void COvenSetup::SetRefTime(long ref_time)
{
	m_lRefTime = ref_time;
}

void COvenSetup::SetSteps(COvenStep* step)
{
	m_Programm.clear();
	m_Programm.resize(16);

	for(long i=0; i<m_lNbStep; i++)
	{
		m_Programm[i].SetType(step[i].GetType());
		m_Programm[i].SetTemp(step[i].GetTemp());
		m_Programm[i].SetDuration(step[i].GetDuration());
		m_Programm[i].SetGradient(step[i].GetGradient());
	}
}

void COvenSetup::GetSteps(COvenStep* step)
{
	for(long i=0; i<m_lNbStep; i++)
	{
		step[i].SetType(m_Programm[i].GetType());
		step[i].SetTemp(m_Programm[i].GetTemp());
		step[i].SetDuration(m_Programm[i].GetDuration());
		step[i].SetGradient(m_Programm[i].GetGradient());
	}
}

long COvenSetup::GetNbStep()
{
	return m_lNbStep;
}

long COvenSetup::GetRefTime()
{
	return m_lRefTime;
}