
// KSipServerMonitorDoc.h : CKSipServerMonitorDoc Ŭ������ �������̽�
//


#pragma once


class CKSipServerMonitorDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CKSipServerMonitorDoc();
	DECLARE_DYNCREATE(CKSipServerMonitorDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CKSipServerMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


