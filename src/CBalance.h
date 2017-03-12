//**********************************************************************************************
//                                        CBalance.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CBalance_H
#define CBalance_H

#include "wx/wx.h"

class CSerialPort;


class CBalance
{
public:
	CBalance();
	~CBalance();

	bool GetMass(double*);

private:
	CSerialPort* m_pPortCom;
	bool         m_bPortOK;
};

#endif