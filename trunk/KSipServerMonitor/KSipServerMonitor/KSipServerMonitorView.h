
// KSipServerMonitorView.h : CKSipServerMonitorView Ŭ������ �������̽�
//


#pragma once


class CKSipServerMonitorView : public CListView
{
protected: // serialization������ ��������ϴ�.
	CKSipServerMonitorView();
	DECLARE_DYNCREATE(CKSipServerMonitorView)

// Ư���Դϴ�.
public:
	CKSipServerMonitorDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CKSipServerMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // KSipServerMonitorView.cpp�� ����� ����
inline CKSipServerMonitorDoc* CKSipServerMonitorView::GetDocument() const
   { return reinterpret_cast<CKSipServerMonitorDoc*>(m_pDocument); }
#endif

