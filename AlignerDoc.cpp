
// AlignerDoc.cpp : implementation of the CAlignerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AlignerApp.h"
#endif

#include "Utility.h"

#include "AlignerFrm.h"
#include "AlignerDoc.h"
#include "AlignerRecipe.h"

#include "View\InitView.h"
#include "View\ManualView.h"
#include "View\AutoView.h"
#include "View\ModelView.h"
#include "View\MaskView.h"
#include "View\LogView.h"

#include "View\SubAutoStateDlg.h"
#include "View\SubAutoDataDlg.h"
#include "View\SubModelMainDlg.h"
#include "View\SubModelShotDlg.h"
#include "View\SubModelSCSDlg.h"
#include "View\SubModelPositionDlg.h"
#include "View\SubLogIncidentDlg.h"
#include "View\SubMachineGlassStageDlg.h"
#include "View\SubMachineMaskStageDlg.h"
#include "View\SubMachineLightCtrlerDlg.h"
#include "View\SubMachineRobotDlg.h"
//NS_NAM Ãß°¡..
#include "View\SubLogAddMotorDistanceDlg.h"

#include "View\MeasureView.h"
#include "View\SubMeasureMaskStageDlg.h"
#include "View\SubMeasureGlassStageDlg.h"
#include "View\SubMeasureLightCtrlerDlg.h"

#include "View\MachineView.h"
#include "View\LightCtrlerView.h"
#include "View\SubMachineMainDlg.h"
#include "View\SubLightCtrlerScsDlg.h"
#include "View\SubLightCtrlerLampDlg.h"
#include "View\CheckView.h"

#include "Machine\Aligner.h"

using namespace LetsClass;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAlignerDoc

IMPLEMENT_DYNCREATE(CAlignerDoc, CLetsBaseDoc)

BEGIN_MESSAGE_MAP(CAlignerDoc, CLetsBaseDoc)
END_MESSAGE_MAP()


// CAlignerDoc construction/destruction

CAlignerDoc::CAlignerDoc()
{
	SetAppBaseInfo();

	SetBaseMachine(CMachine::CreateMachineNRecipe<CAligner, CAlignerRecipe>(IDR_MAINFRAME, false == CUtility::IsValidMacId(NOTEBOOK_MAC_ADDRESS_VECTOR) ? CMachine::NORMAL : CMachine::NO_MACHINE));

	if (nullptr == GetMachine() || SUCCESS != const_cast<CAligner*>(GetMachine())->Open((CDevice*)this, CDevice::CDeviceInfo(_T("Aligner"), 0, CDevice::MACHINE)))
	{
		SHOW_DEV_DLG(_T("Machine(=0x%X) Create & Open Device Error!! Exit Program."), GetMachine());
	}
}


CAlignerDoc::~CAlignerDoc()
{
	if (nullptr != GetMachine())	GetMachine()->Close();
}


BOOL CAlignerDoc::OnNewDocument()
{
	if (!CLetsBaseDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CAlignerDoc serialization

void CAlignerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAlignerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAlignerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAlignerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAlignerDoc diagnostics

#ifdef _DEBUG
void CAlignerDoc::AssertValid() const
{
	CLetsBaseDoc::AssertValid();
}

void CAlignerDoc::Dump(CDumpContext& dc) const
{
	CLetsBaseDoc::Dump(dc);
}
#endif //_DEBUG


// CAlignerDoc commands
void CAlignerDoc::SetAppBaseInfo()
{
	constexpr static Int32 SCREEN_X_SIZE	= 1920;
	constexpr static Int32 SCREEN_Y_SIZE	= 1080;

	constexpr static Int32 CHILD_WND_LEFT	= 1;
	constexpr static Int32 CHILD_WND_TOP	= 44;
	constexpr static Int32 CHILD_WND_RIGHT	= 1783;
	constexpr static Int32 CHILD_WND_BOTTOM	= 1200;

	m_AppBaseInfo.m_WndSize.cx			= SCREEN_X_SIZE;
	m_AppBaseInfo.m_WndSize.cy			= SCREEN_Y_SIZE;

	m_AppBaseInfo.m_ChildWndRect.left	= CHILD_WND_LEFT;
	m_AppBaseInfo.m_ChildWndRect.top	= CHILD_WND_TOP;
	m_AppBaseInfo.m_ChildWndRect.right	= CHILD_WND_RIGHT;
	m_AppBaseInfo.m_ChildWndRect.bottom	= CHILD_WND_BOTTOM;

	m_AppBaseInfo.m_ModeChangeButtonID	= IDC_PB_BASE_USER_MODE;
	m_AppBaseInfo.m_ExitButtonID		= IDC_PB_BASE_EXIT;
	m_AppBaseInfo.m_IncidentListIDMap	= { { 0, { IDC_LT_AUTO_INCIDENT, _T("") } }};

	constexpr Int32 TAB_WIDTH					= 180;

	CAutoView* pAutoView						= new CAutoView;
	constexpr RECT AUTO_SUB_VIEW_RECT			= {3, 42, 1767, 650};

	CModelView* pModelView						= new CModelView;
	constexpr RECT MODEL_SUB_VIEW_RECT			= {3, 42, 1331, 1023};

	CLightCtrlerView* pLightCtrlerView			= new CLightCtrlerView;
	constexpr RECT LIGHT_CTRLER_SUB_VIEW_RECT	= {3, 42, 1767, 1023};

	CLogView* pLogView							= new CLogView;
	constexpr RECT LOG_SUB_VIEW_RECT			= {3, 42, 1767, 1023};

	CMeasureView* pMeasureView					= new CMeasureView;
	constexpr RECT MEASURE_SUB_VIEW_RECT		= {3, 42, 1767, 1023};

	CMachineView* pMachineView					= new CMachineView;
	constexpr RECT MACHINE_SUB_VIEW_RECT		= {3, 42, 1767, 1023};

	m_AppBaseInfo.m_ModePageVector.push_back
	({
		CAppBaseInfo::CPageInfo(pMeasureView, IDC_PB_BASE_MEASURE_PAGE,
		{
			IDC_TB_MEASURE_VIEW,
			TAB_WIDTH,
			MEASURE_SUB_VIEW_RECT,
			{
				{ _T(" [ Mask Stage Flatness ] "),		IDI_PROC_QUESTION,		IDD_SUB_MEASURE_MASK_STAGE_DLG,		new CSubMeasureMaskStageDlg(pMeasureView)	},
				{ _T(" [ Glass Stage Flatness ] "),		IDI_PROC_QUESTION,		IDD_SUB_MEASURE_GLASS_STAGE_DLG,	new CSubMeasureGlassStageDlg(pMeasureView)	},
				{ _T(" [ Light Ctrler Uniformity ] "),	IDI_LIGHT_CTRLER_PAGE,	IDD_SUB_MEASURE_LIGHT_CTRLER_DLG,	new CSubMeasureLightCtrlerDlg(pMeasureView)	},
			}
		}),

		CAppBaseInfo::CPageInfo(pMachineView, IDC_PB_BASE_MACHINE_PAGE,
		{
			IDC_TB_MACHINE_VIEW,
			TAB_WIDTH,
			MACHINE_SUB_VIEW_RECT,
			{
				{ _T(" [ Main ] "),			IDI_PROC_QUESTION,		IDD_SUB_MACHINE_MAIN_DLG,			new CSubMachineMainDlg(pMachineView)		},
				{ _T(" [ Mask Stage ] "),	IDI_PROC_QUESTION,		IDD_SUB_MACHINE_MASK_STAGE_DLG,		new CSubMachineMaskStageDlg(pMachineView)	},
				{ _T(" [ Glass Stage ] "),	IDI_PROC_QUESTION,		IDD_SUB_MACHINE_GLASS_STAGE_DLG,	new CSubMachineGlassStageDlg(pMachineView)	},
				{ _T(" [ Light System ] "),	IDI_LIGHT_CTRLER_PAGE,	IDD_SUB_MACHINE_LIGHT_CTRLER_DLG,	new CSubMachineLightCtrlerDlg(pMachineView)	},
				{ _T(" [ Robot ] "),		IDI_PROC_QUESTION,		IDD_SUB_MACHINE_ROBOT_DLG,			new CSubMachineRobotDlg(pMachineView)		}
			}
		}),

		CAppBaseInfo::CPageInfo(pLightCtrlerView, IDC_PB_BASE_LIGHT_CTRLER_PAGE,
		{
			IDC_TB_LIGHT_CTRLER_VIEW,
			TAB_WIDTH,
			LIGHT_CTRLER_SUB_VIEW_RECT,
			{
				{ _T(" [ Lamp ] "),	IDI_LIGHT_CTRLER_PAGE,	IDD_SUB_LIGHT_CTRLER_LAMP_DLG,	new CSubLightCtrlerLampDlg(pLightCtrlerView)},
				{ _T(" [ SCS ] "),	IDI_PROC_QUESTION,	IDD_SUB_LIGHT_CTRLER_SCS_DLG,	new CSubLightCtrlerScsDlg(pLightCtrlerView)	},
			}
		}),

		CAppBaseInfo::CPageInfo(pLogView, IDC_PB_BASE_LOG_PAGE,
		{
			IDC_TB_LOG_VIEW,
			TAB_WIDTH,
			LOG_SUB_VIEW_RECT,
			{
				{ _T(" [ Incident ] "),	IDI_PROC_QUESTION,	IDD_SUB_LOG_INCIDENT_DLG,	new CSubLogIncidentDlg(pLogView)},
				{ _T(" [ AddMotorDistance ] "),	IDI_PROC_QUESTION,	IDD_SUB_LOG_ADDMOTORDISTANCE_DLG,	new CSubLogAddMotorDistanceDlg(pLogView)},
	}
		}),

		CAppBaseInfo::CPageInfo(new CCheckView,		IDC_PB_BASE_CHECK_PAGE)
	});

	m_AppBaseInfo.m_ModePageVector.push_back
	({
		CAppBaseInfo::CPageInfo(pAutoView, IDC_PB_BASE_AUTO_PAGE,
		{
			IDC_TB_AUTO_VIEW,
			TAB_WIDTH,
			AUTO_SUB_VIEW_RECT,
			{
				{ _T(" [ State ] "),	IDI_PROC_QUESTION,	IDD_SUB_AUTO_STATE_DLG,	new CSubAutoStateDlg(pAutoView)	},
				{ _T(" [ Data ] "),		IDI_PROC_QUESTION,	IDD_SUB_AUTO_DATA_DLG,	new CSubAutoDataDlg(pAutoView)	}
			}
		}),
		CAppBaseInfo::CPageInfo(new CManualView,	IDC_PB_BASE_MANUAL_PAGE),
		CAppBaseInfo::CPageInfo(new CMaskView,		IDC_PB_BASE_MASK_PAGE),
		CAppBaseInfo::CPageInfo(pModelView,			IDC_PB_BASE_MODEL_PAGE,
		{
			IDC_TB_MODEL_VIEW,
			TAB_WIDTH,
			MODEL_SUB_VIEW_RECT,
			{
				{ _T(" [ Main ] "),		IDI_PROC_QUESTION,	IDD_SUB_MODEL_MAIN_DLG,		new CSubModelMainDlg(pModelView)	},
				{ _T(" [ Shot ] "),		IDI_PROC_QUESTION,	IDD_SUB_MODEL_SHOT_DLG,		new CSubModelShotDlg(pModelView)	},
				{ _T(" [ SCS ] "),		IDI_PROC_QUESTION,	IDD_SUB_MODEL_SCS_DLG,		new CSubModelSCSDlg(pModelView)		},
				{ _T(" [ Position ] "),	IDI_PROC_QUESTION,	IDD_SUB_MODEL_POS_DLG,		new CSubModelPositionDlg(pModelView)}
			}
		})
	});

	m_AppBaseInfo.m_ModePageVector.push_back
	({
		CAppBaseInfo::CPageInfo(new CInitView, IDC_PB_BASE_INIT_PAGE)
	});
}
