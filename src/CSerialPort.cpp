//**********************************************************************************************
//                                        CSerialPort.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "string.h"
#include "CSerialPort.h"


CSerialPort::CSerialPort()
{
	m_hPort = INVALID_HANDLE_VALUE;
}

CSerialPort::~CSerialPort()
{}

bool CSerialPort::Open(int num)
{
	wxString port_name;
	port_name.Printf("//./com%d", num);
	
	wchar_t* buf = new wchar_t[port_name.Len() + 1];
	_tcsncpy(buf, port_name.wc_str(), port_name.Len()+1);

	m_hPort = CreateFile(buf, 
						 GENERIC_READ|GENERIC_WRITE,
						 0,
						 0,
						 OPEN_EXISTING,
						 0,
						 NULL);

	delete[] buf;

	if(m_hPort == INVALID_HANDLE_VALUE)
		return false;
	else
		return true;
}

bool CSerialPort::Configure(int baudrate, int bytesize, int stopbits, bool parity, int parityscheme)
{
	if(!GetCommState(m_hPort, &m_dcb))
	{
		this->Close();
		return false;
	}
		
	m_dcb.BaudRate        = baudrate;
	m_dcb.ByteSize        = bytesize;
	m_dcb.Parity          = parityscheme;
	m_dcb.StopBits        = stopbits;
	m_dcb.fParity         = parity;

	m_dcb.fBinary         = true;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fOutX           = false;
	m_dcb.fInX            = false;
	m_dcb.fNull           = false;
	m_dcb.fAbortOnError   = true;
	m_dcb.fOutxCtsFlow    = false;
	m_dcb.fOutxDsrFlow    = false;
	m_dcb.fDtrControl     = DTR_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fRtsControl     = RTS_CONTROL_DISABLE;
	m_dcb.fOutxCtsFlow    = false;
	m_dcb.fOutxCtsFlow    = false;

	if(!SetCommState(m_hPort, &m_dcb))
	{
		this->Close();
		return false;
	}
	else
		return true;
}

bool CSerialPort::WriteString(wxString str)
{
	char* buf = new char[str.Len()+1];
	strcpy(buf, str.c_str());

	DWORD NumberOfytesWritten = 0;

	int res = WriteFile(m_hPort, buf, (DWORD)str.Len(), &NumberOfytesWritten, NULL);

	delete[] buf;

	if(res == 0)
		return false;
	else
	{
		FlushFileBuffers(m_hPort);
		return true;
	}
}

bool CSerialPort::ReadString(wxString* str, int len)
{
	char* buf = new char[len+1];
	buf[0] = '\0';
	buf[len] = '\0';

	DWORD NumberOfBytesRead = 0;

	int res = ReadFile(m_hPort, buf, (DWORD)len, &NumberOfBytesRead, 0);

	str->Printf("%s", buf);
	delete[] buf;

	if(res ==0)
		return false;
	else
		return true;
}

bool CSerialPort::Timeout(int millisecondes)
{
	if(!GetCommTimeouts(m_hPort, &m_timeout))
		return false;

	m_timeout.ReadIntervalTimeout         = MAXDWORD;
	m_timeout.ReadTotalTimeoutConstant    = 0;
	m_timeout.ReadTotalTimeoutMultiplier  = 0;
	m_timeout.WriteTotalTimeoutConstant   = 0;
	m_timeout.WriteTotalTimeoutMultiplier = 0;

	return SetCommTimeouts(m_hPort, &m_timeout);
}

bool CSerialPort::Close()
{
	if(m_hPort != INVALID_HANDLE_VALUE)
		CloseHandle(m_hPort);

    return true;
}