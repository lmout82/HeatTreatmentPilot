//**********************************************************************************************
//                                        COvenStep.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef COvenStep_H
#define COvenStep_H


class COvenStep
{
public:
	COvenStep();

	void SetType(long);   
	void SetTemp(double);
	void SetDuration(double);
	void SetGradient(double);

	long   GetType();
	double GetTemp();
	double GetDuration();
	double GetGradient();

private:
	long   m_lType;
	double m_dDuration;
	double m_dTemp;
	double m_dGradient;
};

#endif