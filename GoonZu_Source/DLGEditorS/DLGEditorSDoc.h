// DLGEditorSDoc.h : CDLGEditorSDoc 클래스의 인터페이스
//


#pragma once

// --- DiagramEditor ---
#include "./DialogEditor/DiagramEditor/DiagramEntityContainer.h"

class CDLGEditorSDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CDLGEditorSDoc();
	DECLARE_DYNCREATE(CDLGEditorSDoc)

// 특성
public:
	// --- DiagramEditor ---
	CDiagramEntityContainer*	GetData();

// 작업
public:
	// --- DiagramEditor ---
	void	Export();

// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CDLGEditorSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL SaveModified();

// --- DiagramEditor ---
private:
	// 모든 컨트롤 데이터(All Entity)와 다이얼로그 에디터 배경 이미지 데이터를 읽어옴
	CDiagramEntityContainer	m_objs; // The object data

public:
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
};


