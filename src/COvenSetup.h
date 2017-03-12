//**********************************************************************************************
//                                        COvenSetup.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef OvenSetup_H	
#define OvenSetup_H

#include <vector>
#include "COvenStep.h"


class COvenSetup
{
public:
	COvenSetup();
	~COvenSetup();

	void Reset();
	void SetNbStep(long);
	void SetRefTime(long);
	long GetNbStep();
	long GetRefTime();
	void SetSteps(COvenStep*); 
	void GetSteps(COvenStep*);
		
private:
	long m_lNbStep;
	long m_lRefTime;  // 0: H,m  1: m,s
	std::vector <COvenStep> m_Programm;
};


#endif