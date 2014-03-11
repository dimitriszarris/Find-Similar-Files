
// Find Similar Files.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFindSimilarFilesApp:
// See Find Similar Files.cpp for the implementation of this class
//

class CFindSimilarFilesApp : public CWinApp
{
public:
	CFindSimilarFilesApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFindSimilarFilesApp theApp;