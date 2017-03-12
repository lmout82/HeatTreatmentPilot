//**********************************************************************************************
//                                        Cst.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef Cst_H
#define Cst_H

#include "wx/wx.h"

const wxString MAIN_WND_NAME = "Heat treatment & data acquisition";

const wxString FOLDER_NAME_BASE        = "Heat treatment projects";
const wxString FOLDER_NAME_WITHOUT_TPL = "without template";

const int COM1 = 1;
const int COM2 = 2;

enum
{
	TIMER = 6000,
	NOTEBOOK,
	NOTEBOOK_PLOT,
	NOTEBOOK_GRID,
	PROG_QUIT ,
	MEAS_EXPORT_CSV,
	LOG_ADD,
	PJT_OPEN,
	PJT_NEW,
	PJT_NEW_TPL,
	PJT_STOP,
	TPL_NEW,
	LIST_GRAPH,
	BUTTON_UPDATE,
	TOOLS_SUB_REG,
	REGU_REBOOT,
	REGU_EXE,
};

DECLARE_EVENT_TYPE(wxEVT_COMMAND_GRAPH_SELECTION_CHANGED, 800)

#endif