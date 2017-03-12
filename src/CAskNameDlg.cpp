//**********************************************************************************************
//                                        CAskNameDlg.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "wx/wx.h"
#include "CAskNameDlg.h"


CAskNameDlg::CAskNameDlg(wxWindow* parent, const wxString& message, const wxString& caption)
	:wxTextEntryDialog(parent, message, caption)
{
	wxArrayString exclude_char_list;
	exclude_char_list.Add("\\");
	exclude_char_list.Add(".");
	exclude_char_list.Add("/");
	exclude_char_list.Add(":");
	exclude_char_list.Add("*");
	exclude_char_list.Add("?");
	exclude_char_list.Add("\"");
	exclude_char_list.Add("<");
	exclude_char_list.Add(">");
	exclude_char_list.Add("|");

	wxTextValidator validator(wxFILTER_EXCLUDE_CHAR_LIST, &m_value );
	validator.SetExcludes(exclude_char_list);

    m_textctrl->SetValidator( validator );
}

wxString CAskNameDlg::GetValue() const
{
	wxString strVal = wxTextEntryDialog::GetValue();
	strVal.Trim();

	return strVal;
}