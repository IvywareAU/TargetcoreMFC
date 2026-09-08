#pragma once

class CInPlaceEdit : public CEdit 
{
    // Constructors and destructor
    public:	
        CInPlaceEdit ( int iItem, int iSubItem, CString strInitText
                     , DWORD dwWM_APP_LISTCTRL_IPEDIT ); 
		
	// ClassWizard generated virtual function overrides	
	//{{AFX_VIRTUAL(CInPlaceEdit)	
	public:	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	//}}AFX_VIRTUAL
		
public:	virtual ~CInPlaceEdit();
		
	// Generated message map functions
protected:	
	//{{AFX_MSG(CInPlaceEdit)	
	afx_msg void OnKillFocus(CWnd* pNewWnd);	
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	//}}AFX_MSG 	
	
	DECLARE_MESSAGE_MAP()

    // Attributes
    public:	
      DWORD   m_dwWM_APP_LISTCTRL_IPEDIT{0};
      int     m_iItem;	
      int     m_iSubItem;	
      CString m_sInitText;
      CString m_strIPEditext;	
      BOOL    m_bESC;
};