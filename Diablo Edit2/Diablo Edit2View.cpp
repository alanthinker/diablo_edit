
// Diablo Edit2View.cpp: CDiabloEdit2View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Diablo Edit2.h"
#endif

#include "Diablo Edit2Doc.h"
#include "Diablo Edit2View.h"

#include "DlgCharBasicInfo.h"
//#include "DlgSkills.h"
#include "DlgSelectChar.h"
#include "DlgCharItems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDiabloEdit2View

IMPLEMENT_DYNCREATE(CDiabloEdit2View, CFormView)

BEGIN_MESSAGE_MAP(CDiabloEdit2View, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDiabloEdit2View::OnTcnSelchangeTab1)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPEN, &CDiabloEdit2View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CDiabloEdit2View::OnFileSave)
	ON_COMMAND(ID_CLOSE_FILE, &CDiabloEdit2View::OnCloseFile)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE1, &CDiabloEdit2View::OnUpdateLanguage1)
	ON_COMMAND(ID_LANGUAGE1, &CDiabloEdit2View::OnLanguage1)
	ON_COMMAND(ID_LANGUAGE2, &CDiabloEdit2View::OnLanguage2)
	ON_COMMAND(ID_LANGUAGE3, &CDiabloEdit2View::OnLanguage3)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE2, &CDiabloEdit2View::OnUpdateLanguage2)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE3, &CDiabloEdit2View::OnUpdateLanguage3)
	ON_COMMAND(ID_FILE_SAVE_AS, &CDiabloEdit2View::OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, &CDiabloEdit2View::OnFileNew)
END_MESSAGE_MAP()

// CDiabloEdit2View 构造/析构

CDiabloEdit2View::CDiabloEdit2View() noexcept
	: CFormView(IDD_DIABLOEDIT2_FORM)
	, m_dlgTabPage(0)
{
	// TODO: 在此处添加构造代码

}

CDiabloEdit2View::~CDiabloEdit2View()
{
	if(m_dlgTabPage){
		for(int i = 0;i < m_nTabPageCount;++i)
			delete m_dlgTabPage[i];
		delete [] m_dlgTabPage;
		m_dlgTabPage = 0;
	}
}

void CDiabloEdit2View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tcTab);
}

BOOL CDiabloEdit2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CDiabloEdit2View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	//初始化界面
	InitUI();
}

// CDiabloEdit2View 诊断

#ifdef _DEBUG
void CDiabloEdit2View::AssertValid() const
{
	CFormView::AssertValid();
}

void CDiabloEdit2View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDiabloEdit2Doc* CDiabloEdit2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiabloEdit2Doc)));
	return (CDiabloEdit2Doc*)m_pDocument;
}
#endif //_DEBUG


// CDiabloEdit2View 消息处理程序

void CDiabloEdit2View::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	m_dlgTabPage[m_nTabCurSel]->ShowWindow(SW_HIDE);
	m_nTabCurSel = m_tcTab.GetCurSel();
	m_dlgTabPage[m_nTabCurSel]->ShowWindow(SW_SHOW);
}

void CDiabloEdit2View::OnPaint()
{
	RefreshUI();
	CFormView::OnPaint();
}

void CDiabloEdit2View::InitUI(void)
{
	if(!m_dlgTabPage){
		m_tcTab.InsertItem(0,_T(""));
		m_tcTab.InsertItem(1,_T(""));
		//在此处添加新的属性页
		m_nTabPageCount = m_tcTab.GetItemCount();

		m_dlgTabPage = new CPropertyDialog*[m_nTabPageCount];
		m_dlgTabPage[0] = new CDlgCharBasicInfo;
		m_dlgTabPage[0]->Create(CDlgCharBasicInfo::IDD,GetDlgItem(IDC_TAB1));
		m_dlgTabPage[1] = new CDlgCharItems;
		m_dlgTabPage[1]->Create(CDlgCharItems::IDD,GetDlgItem(IDC_TAB1));
		m_dlgTabPage[1]->ShowWindow(SW_HIDE);
		//在此处添加新的属性窗体
		m_nTabCurSel = 0;
		m_tcTab.SetCurSel(m_nTabCurSel);
		m_dlgTabPage[m_nTabCurSel]->ShowWindow(SW_SHOW);

		LoadText();
	}
}

void CDiabloEdit2View::RefreshUI(void)
{
	CRect rect;
	GetClientRect(&rect);
	m_tcTab.MoveWindow(rect);
	m_tcTab.GetClientRect(&rect);
	rect.top += 20;
	if(m_dlgTabPage)
		for(int i = 0;i < m_nTabPageCount;++i)
			m_dlgTabPage[i]->MoveWindow(rect);
}

void CDiabloEdit2View::OnFileNew()
{
	const INT INIT_POINTS[7][7] = {
		{20,25,15,20,84 << 8,50 << 8,15 << 8},		//亚马逊
		{10,25,35,10,74 << 8,40 << 8,35 << 8},		//法师
		{15,25,25,15,79 << 8,45 << 8,25 << 8},		//死灵法师
		{25,20,15,25,89 << 8,55 << 8,15 << 8},		//圣骑士
		{30,20,10,25,92 << 8,55 << 8,10 << 8},		//野蛮人
		{15,20,20,25,84 << 8,55 << 8,20 << 8},		//德鲁依
		{20,20,25,20,95 << 8,50 << 8,25 << 8},		//刺客
	};
	if(m_sPathName.GetLength() && m_sTitle.GetLength()){
		int mb = MessageBox(CSFormat(::theApp.MsgBoxInfo(8), m_sTitle), ::theApp.MsgWarning(), MB_YESNOCANCEL | MB_ICONWARNING);
		if(mb == IDCANCEL)
			return;
		else if(mb == IDOK)
			OnFileSave();
	}
	int sel;
	CDlgSelectChar dlg(sel);
	if(dlg.DoModal() == IDOK){
		if(m_Character.ReadFile(CFile(::theApp.AppPath() + _T("new.dat"),CFile::modeRead))){
			for(int i = 0;i < 4;++i)
				m_Character.PlayerStats.m_adwValue[i] = INIT_POINTS[sel][i];
			m_Character.PlayerStats.m_adwValue[0xA] = 
				m_Character.PlayerStats.m_adwValue[0xB] = INIT_POINTS[sel][4];
			m_Character.PlayerStats.m_adwValue[6] = 
				m_Character.PlayerStats.m_adwValue[7] = INIT_POINTS[sel][5];
			m_Character.PlayerStats.m_adwValue[8] = 
				m_Character.PlayerStats.m_adwValue[9] = INIT_POINTS[sel][6];
			m_Character.charClass = sel;
			m_Character.dwTime = DWORD(CTime::GetCurrentTime().GetTime());
			GetDocument()->SetTitle(m_sTitle = _T("new"));
			m_sPathName.Empty();
			for(int i = 0;i < m_nTabPageCount;++i)
				m_dlgTabPage[i]->UpdateUI(m_Character);
		}else
			MessageBox(::theApp.MsgBoxInfo(10), ::theApp.MsgError(), MB_ICONERROR);
	}
}

void CDiabloEdit2View::OnFileOpen()
{
	CFileDialog open(TRUE,_T("d2s"),0,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,_T("Diablo II Character(*.d2s)|*.d2s|All File(*.*)|*.*||"));
	if(open.DoModal() == IDOK){
		if(m_sPathName.GetLength()){
			int mb = MessageBox(CSFormat(::theApp.MsgBoxInfo(8), m_sTitle), ::theApp.MsgWarning(), MB_YESNOCANCEL | MB_ICONWARNING);
			if(mb == IDCANCEL)
				return;
			else if(mb == IDOK)
				OnFileSave();
		}
		m_sPathName = open.GetPathName();
		if(m_Character.ReadFile(CFile(m_sPathName,CFile::modeRead))){
			for(int i = 0;i < m_nTabPageCount;++i)
				m_dlgTabPage[i]->UpdateUI(m_Character);
			m_sTitle = open.GetFileTitle();
			GetDocument()->SetTitle(m_sTitle);
			GetDocument()->SetPathName(m_sPathName);
		}else
			m_sPathName.Empty();
	}
}

BOOL CDiabloEdit2View::GatherAllData(void)
{
	for(int i = 0;i < m_nTabPageCount;++i)
		if(!m_dlgTabPage[i]->GatherData(m_Character))
			return FALSE;
	return TRUE;
}

void CDiabloEdit2View::LoadText()
{
    TCITEM tci;
    tci.mask = TCIF_TEXT;
	GetDocument()->SetTitle(::theApp.MsgNoTitle());
	//基本信息,物品
    tci.pszText = (LPWSTR)::theApp.CharBasicInfoUI(0).GetString();
    m_tcTab.SetItem(0,&tci);
    tci.pszText = (LPWSTR)::theApp.OtherUI(9).GetString();
    m_tcTab.SetItem(1,&tci);
    Invalidate();
}

void CDiabloEdit2View::OnFileSave()
{
	if(m_sTitle.GetLength() > 0 && GatherAllData()){
		if(m_sPathName.GetLength()){
			if(m_sTitle == CString(m_Character.Name) ||
				MessageBox(::theApp.MsgBoxInfo(9), ::theApp.MsgWarning(), MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				m_Character.WriteFile(CFile(m_sPathName,CFile::modeWrite | CFile::modeCreate));
				GetDocument()->SetTitle(m_sTitle = m_Character.Name);
			}
		}else{
			CFileDialog save_as(FALSE,0,CString(m_Character.Name) + _T(".d2s"),OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,_T("Diablo II Character(*.d2s)|*.d2s|All File(*.*)|*.*||"));
			if(save_as.DoModal() == IDOK){
				m_sPathName = save_as.GetPathName();
				m_Character.WriteFile(CFile(m_sPathName,CFile::modeCreate | CFile::modeWrite));
				GetDocument()->SetTitle(m_sTitle = m_Character.Name);
				GetDocument()->SetPathName(m_sPathName);
			}
		}
	}
}

void CDiabloEdit2View::OnFileSaveAs()
{
	if(m_sTitle.GetLength() && GatherAllData()){
		CFileDialog save_as(FALSE,0,CString(m_Character.Name) + _T(".d2s"),OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,_T("Diablo II Character(*.d2s)|*.d2s|All File(*.*)|*.*||"));
		if(save_as.DoModal() == IDOK){
			m_sPathName = save_as.GetPathName();
			m_Character.WriteFile(CFile(m_sPathName,CFile::modeCreate | CFile::modeWrite));
			GetDocument()->SetTitle(m_sTitle = m_Character.Name);
			GetDocument()->SetPathName(m_sPathName);
		}
	}
}

void CDiabloEdit2View::OnCloseFile()
{
	if(m_sTitle.GetLength()){
		m_sTitle = m_sPathName = _T("");
		GetDocument()->SetTitle(::theApp.MsgNoTitle());
		if(m_dlgTabPage)
			for(int i = 0;i < m_nTabPageCount;++i)
				m_dlgTabPage[i]->ResetAll();
	}
}

void CDiabloEdit2View::OnUpdateLanguage1(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(::theApp.m_nLangIndex == 0);
	pCmdUI->SetText(::theApp.LangTitle(0));
}
void CDiabloEdit2View::OnUpdateLanguage2(CCmdUI *pCmdUI)
{
	if(::theApp.LangCount() > 1){ 
		pCmdUI->SetRadio(::theApp.m_nLangIndex == 1);
		pCmdUI->SetText(::theApp.LangTitle(1));
	}else
		pCmdUI->Enable(FALSE);
}
void CDiabloEdit2View::OnUpdateLanguage3(CCmdUI *pCmdUI)
{
	if(::theApp.LangCount() > 2){ 
		pCmdUI->SetRadio(::theApp.m_nLangIndex == 2);
		pCmdUI->SetText(::theApp.LangTitle(2));
	}else
		pCmdUI->Enable(FALSE);
}
void CDiabloEdit2View::OnLanguage1()
{
	if(::theApp.m_nLangIndex != 0 && m_dlgTabPage){
		::theApp.m_nLangIndex = 0;
		for(int i = 0;i < m_nTabPageCount;++i)
			m_dlgTabPage[i]->LoadText();
        LoadText();
	}
}
void CDiabloEdit2View::OnLanguage2()
{
	if(::theApp.m_nLangIndex != 1 && m_dlgTabPage){
		::theApp.m_nLangIndex = 1;
		for(int i = 0;i < m_nTabPageCount;++i)
			m_dlgTabPage[i]->LoadText();
        LoadText();
	}
}
void CDiabloEdit2View::OnLanguage3()
{
	if(::theApp.m_nLangIndex != 2 && m_dlgTabPage){
		::theApp.m_nLangIndex = 2;
		for(int i = 0;i < m_nTabPageCount;++i)
			m_dlgTabPage[i]->LoadText();
        LoadText();
	}
}

