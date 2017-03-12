//**********************************************************************************************
//                                        CAcqSetup.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CAcqSetup_H
#define CAcqSetup_H


class CAcqSetup
{
public:
	CAcqSetup();
	~CAcqSetup();

	void Reset();
	void SetDuration(double);
	void SetPeriod(double);
	double GetDuration();
	double GetPeriod();

private:
	double m_dDuration;
	double m_dPeriod;
};

#endif