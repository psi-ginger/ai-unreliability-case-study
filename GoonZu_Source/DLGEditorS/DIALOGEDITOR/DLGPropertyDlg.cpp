// E:\MySRC\DLGEditorS\DLGEditorS\DialogEditor\DLGPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../DLGEditorS.h"
#include "DLGPropertyDlg.h"
#include ".\dlgpropertydlg.h"

#include "./DiagramEditor/DiagramEditor.h"

#include "DiagramEditor/Tokenizer.h"
// CDLGPropertyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDLGPropertyDlg, CDialog)
CDLGPropertyDlg::CDLGPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CDLGPropertyDlg::IDD, pParent)
{
	CDiagramEntity* tmp = new CDiagramEntity(DLG_PROPERTY_COUNT);

	m_pParent = NULL;
	m_pDLGEditor = NULL;

	SetEntity(tmp);
}

CDLGPropertyDlg::~CDLGPropertyDlg()
{
	if(GetEntity())
		delete [] GetEntity();
}

void CDLGPropertyDlg::OnPaint()
{
	CDiagramPropertyDlg::OnPaint();
}

void CDLGPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGPropertyDlg)

	// DIALOG PROPERTY
	DDX_Control(pDX, 	 IDC_EDIT_DLG_KIND_NAME	,	m_editDLG_Property[DLG_KIND_NAME] 	);	// 다이얼로그 혹은 컨트롤 종류
	DDX_Control(pDX, 	 IDC_EDIT_DLG_ID                 	,	m_editDLG_Property[DLG_RESOURCE_ID] 	);	// ID : 리소스 ID
	DDX_Control(pDX, 	 IDC_EDIT_DLG_X_COORD            	,	m_editDLG_Property[DLG_X_COORD] 	);	// X좌표
	DDX_Control(pDX, 	 IDC_EDIT_DLG_Y_COORD            	,	m_editDLG_Property[DLG_Y_COORD] 	);	// Y좌표
	DDX_Control(pDX, 	 IDC_EDIT_DLG_WIDTH	,	m_editDLG_Property[DLG_WIDTH] 	);	// 가로폭
	DDX_Control(pDX, 	 IDC_EDIT_DLG_HEIGHT	,	m_editDLG_Property[DLG_HEIGHT] 	);	// 세로폭
	DDX_Control(pDX, 	 IDC_EDIT_DLG_IMAGE_FULLPATH     	,	m_editDLG_Property[DLG_BG_IMAGE_FULLPATH] 	);	// 백그라운드 이미지 
	DDX_Control(pDX, 	 IDC_EDIT_DLG_BG_WIDTH           	,	m_editDLG_Property[DLG_BG_WIDTH] 	);	// 백그라운드 이미지 가로폭
	DDX_Control(pDX, 	 IDC_EDIT_DLG_BG_HEIGHT          	,	m_editDLG_Property[DLG_BG_HEIGHT] 	);	// 백그라운드 세로폭
	DDX_Control(pDX, 	 IDC_EDIT_DLG_BG_TILE           	,	m_editDLG_Property[DLG_BG_TILE] 	);	// 백그라운드 이미지가 타일 인지 통맵인지 여부
	DDX_Control(pDX, 	 IDC_EDIT_DLG_BG_TILE_IMAGE_NUM    	,	m_editDLG_Property[DLG_BG_TILE_IMAGE_NUM] 	);	// 백그라운드 이미지가 타일 인지 통맵인지 여부
	DDX_Control(pDX, 	 IDC_EDIT_DLG_START_FONT         	,	m_editDLG_Property[DLG_BG_START_FONT] 	);	// 파일에서 백그라운드 이미지 시작 위치
	DDX_Control	(	pDX	, 	IDC_EDIT_DLG_TEXT_COLOR	, 	m_editDLG_Property	[DLG_TEXT_COLOR])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_DLG_TEXT_STYLE	, 	m_editDLG_Property	[DLG_TEXT_STYLE])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_DLG_TEXT_SIZE	, 	m_editDLG_Property	[DLG_TEXT_SIZE])	;
						
	//DDX_Control(pDX, 	 IDC_EDIT_DLG_CALLBACK_POINTER	,	m_editDLG_Property[DLG_CALLBACK_POINTER] 	);	// 다이얼로그 콜백 함수 포인터명
	DDX_Control(pDX, 	 IDC_EDIT_DLG_TITLE_STR	,	m_editDLG_Property[DLG_TITLE_STR] 	);	// 다이얼로그 타이틀 명
	DDX_Control(pDX, 	 IDC_EDIT_DLG_TITLE_VISIBLE	,	m_editDLG_Property[DLG_TITLE_VISIBLE] 	);	// 다이얼로그 타이틀이 보이는지 안보이는지 여부
	DDX_Control(pDX, 	 IDC_EDIT_DLG_MOVABLE	,	m_editDLG_Property[DLG_MOVABLE] 	);	// 다이얼로그가 이동 가능한지 여부
	DDX_Control(pDX, 	 IDC_EDIT_DLG_CLOSABLE	,	m_editDLG_Property[DLG_CLOSABLE] 	);	// 다이얼로그 닫기 버튼 존재 여부

	//DDX_Control(pDX, 	 IDC_EDIT_DLG_CHILD_ID	,	m_editDLG_Property[DLG_CHILD_FILENAMES] 	);	// 다이얼로그 닫기 버튼 존재 여부
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CDLGPropertyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DLG_APPLY, OnBnClickedButtonDlgApply)
	ON_BN_CLICKED(IDC_BUTTON_DLG_CLOSE, OnBnClickedButtonDlgClose)
	ON_BN_CLICKED(IDC_BUTTON_DLG_SEARCH, OnBnClickedButtonDlgSearch)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_DLG_PATH_EMPTY2, OnBnClickedButtonDlgPathEmpty2)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDLGPropertyDlg 메시지 처리기입니다.

BOOL CDLGPropertyDlg::OnInitDialog()
{
	CDiagramPropertyDlg::OnInitDialog();


	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 목표 대상 : 다이얼로그 윈도우의 속성
// 작업 관계 : 다이얼로그의 속성창값 => 다이얼로그 속성의 실제 데이터 저장소
void CDLGPropertyDlg::OnBnClickedButtonDlgApply()
{
	int i=0;

	m_bError = false;

	if( m_hWnd )
	{
		if( GetEntity() )
		{
			CString	strTemp[DLG_PROPERTY_COUNT];
			CString strTemp2;
			CString strTileImageNum;
			CString strTile;
			CString strTimeImageStartFont;

			for(i=0; i<DLG_PROPERTY_COUNT; i++)
			{
                m_editDLG_Property[i].GetWindowText(strTemp[i]);
				
				strTemp2 = m_pDLGEditor->m_objs->m_DLG_Property[i];
				
				// 사용자 정의 다이얼로그 아이디 적용
				if(i == DLG_RESOURCE_ID)
					strDLGUniqueID = strTemp[DLG_RESOURCE_ID];

				if(strTemp2 != strTemp[i])
				{
					// 자식창 10개로 만드는 루틴 추가!! ######################################################
					if(i == DLG_CHILD_FILENAMES)
					{
                        CTokenizer tok(strTemp[DLG_CHILD_FILENAMES], DLG_CHILD_FILENAME_SPLITTER_STR);
						int difference=0, p=0;

						difference = DLG_CHILD_FILENUM - tok.GetSize();

						if(difference < 0)
						{
                            AfxMessageBox("자식창 파일명 개수가 최대치 (10개)를 초과했습니다!!\n다시 입력하세요!!");
							
							m_editDLG_Property[DLG_CHILD_FILENAMES].SetWindowText(m_pDLGEditor->m_objs->m_DLG_Property[DLG_CHILD_FILENAMES]);

							m_bError = true;

							return;
						}

                        for(p=0; p<difference; p++)
						{
							strTemp[DLG_CHILD_FILENAMES] += DLG_CHILD_FILENAME_SPLITTER_CHAR;
						}
					}
					//########################################################################################

					m_pDLGEditor->m_objs->m_DLG_Property[i] = strTemp[i];
					
					// 이미지 FullPath 관리
					if( ((m_pDLGEditor->m_objs->m_DLG_Property[DLG_BG_IMAGE_FULLPATH] != strTemp[DLG_BG_IMAGE_FULLPATH]
						&& i == DLG_BG_IMAGE_FULLPATH) /*i == DLG_BG_TILE*/ || i == DLG_BG_START_FONT
						|| i == DLG_BG_WIDTH || i == DLG_BG_HEIGHT || i == DLG_BG_TILE_IMAGE_NUM ) )
					{

						//if(m_pDLGEditor->m_objs->m_DLG_Property[DLG_BG_IMAGE_FULLPATH] != strTemp[DLG_BG_IMAGE_FULLPATH])
							m_pDLGEditor->m_objs->m_DLG_Property[DLG_BG_IMAGE_FULLPATH] = strTemp[DLG_BG_IMAGE_FULLPATH];

						//if(m_pDLGEditor->m_objs->m_DLG_Property[DLG_BG_IMAGE_FULLPATH].Trim(STR_TRIM_LETTER) != STR_EMPTY && GetEntity()->GetPicFullPath() != m_pDLGEditor->m_OldBGFullPath)
						{
							m_pDLGEditor->DeleteAllBmp();
							m_pDLGEditor->DeleteAllBufferDC();
							m_pDLGEditor->ClearAllOldBmp();

							m_pDLGEditor->m_objs->m_spr.ClearImageData();

							if(m_pDLGEditor->m_objs->m_spr.LoadSpr( (char*)(LPCTSTR) strTemp[DLG_BG_IMAGE_FULLPATH].Trim(STR_TRIM_LETTER)))
							{
								//m_pDLGEditor->MakeMemDC(m_pParent->GetDC());
								m_editDLG_Property[DLG_BG_TILE_IMAGE_NUM].GetWindowText(strTileImageNum);
								m_editDLG_Property[DLG_BG_TILE].GetWindowText(strTile);
								m_editDLG_Property[DLG_BG_START_FONT].GetWindowText(strTimeImageStartFont);

                                if(strTile != STR_ZERO) // 타일 방식일때 폰트 이미지 개수
									m_pDLGEditor->MakeMemDC(m_pParent->GetDC(), atoi(strTimeImageStartFont), atoi(strTileImageNum) );
								else	// 통 이미지 방식 일때
									m_pDLGEditor->MakeMemDC(m_pParent->GetDC(), atoi(strTimeImageStartFont), WHOLE_IMAGE_STYLE_FONTNUM);

							}

							if(m_pDLGEditor->m_OldBGFullPath != strTemp[DLG_BG_IMAGE_FULLPATH])
								m_pDLGEditor->m_OldBGFullPath = strTemp[DLG_BG_IMAGE_FULLPATH];
						}
					}

				}

			} // for(...)

			strTemp2.Empty();
			int sub_width = 0, sub_BG_width =0, sub_height = 0, sub_BG_height = 0;

			sub_width = _ttoi(strTemp[DLG_WIDTH]);
			sub_height = _ttoi(strTemp[DLG_HEIGHT]);
			sub_BG_width = _ttoi(strTemp[DLG_BG_WIDTH]);
			sub_BG_height = _ttoi(strTemp[DLG_BG_HEIGHT]);

			strTemp2.Format("%d", m_pDLGEditor->m_objs->SetToMultipleSize(sub_width, 1));
			//tile 크기로 맞춤 => strTemp2.Format("%d", m_pDLGEditor->m_objs->SetToMultipleSize(sub_width, m_spr.clHeader.siXsize));
			m_editDLG_Property[DLG_WIDTH].SetWindowText(strTemp2);
			m_pDLGEditor->m_objs->m_DLG_Property[DLG_WIDTH] = strTemp2;
	
	        m_editDLG_Property[DLG_BG_WIDTH].SetWindowText(strTemp2);
			m_pDLGEditor->m_objs->m_DLG_Property[DLG_BG_WIDTH] = strTemp2;

			strTemp2.Format("%d", m_pDLGEditor->m_objs->SetToMultipleSize(sub_height, 1));
			//tile 크리로 맞춤 => strTemp2.Format("%d", m_pDLGEditor->m_objs->SetToMultipleSize(sub_height, m_spr.clHeader.siYsize));
			m_editDLG_Property[DLG_HEIGHT].SetWindowText(strTemp2);
			m_pDLGEditor->m_objs->m_DLG_Property[DLG_HEIGHT] = strTemp2;
	
			m_editDLG_Property[DLG_BG_HEIGHT].SetWindowText(strTemp2);
			m_pDLGEditor->m_objs->m_DLG_Property[DLG_HEIGHT] = strTemp2;

			// id 글자수가 30자를 넘지 못하게 함
			CString tmpStr;           
			m_editDLG_Property[RESOURCE_ID].GetWindowText(tmpStr);
			if(tmpStr.GetLength() > ID_LENGTH)
			{
				AfxMessageBox(tmpStr + " ID 값이 30자를 초과했습니다.\n 다시 입력하세요!");
				tmpStr = tmpStr.Left( 30 );
				m_editDLG_Property[RESOURCE_ID].SetWindowText(tmpStr);

				m_bError = true;
			}

			m_pDLGEditor->m_objs->SetModified( TRUE );
			m_pDLGEditor->RedrawWindow();
		}
	}	
}

void CDLGPropertyDlg::OnBnClickedButtonDlgClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

// 값을 셋팅할 필요가 있을 때 호출됨
// 작업 관계 : 실제 데이터 저장소 => 속성창
void CDLGPropertyDlg::SetValues()
{
	//CString real_picFullPath;
	CString strTmp;

	int i =0;

	if( m_hWnd )
	{
		if( m_pDLGEditor )
		{
			/**
			leon add
			* Dialog property 일괄 처리를 위해 배열 형태로 변경함
			*/
			for(i=0; i<DLG_PROPERTY_COUNT; i++)
			{
				strTmp = m_pDLGEditor->m_objs->m_DLG_Property[i];
			
				if(i == DLG_BG_IMAGE_FULLPATH)
				{
					if(strTmp != m_picFullPath || !GetBufferDC(REAL_BACKGROUND_NUM) ) // todo - 다른 이미지 변경 속성 처리 요!!
					{
						DeleteAllBmp();
						
						ClearAllOldBmp();

						if(m_picFullPath != strTmp)
							m_picFullPath = strTmp;

						m_editDLG_Property[i].SetWindowText(strTmp);

						if(strTmp.Trim(STR_TRIM_LETTER) != STR_EMPTY)
						{
							if(m_spr.LoadSpr( (char *)(LPCTSTR) strTmp.Trim(STR_TRIM_LETTER)))
                                MakeMemDC(this->GetDC());
						}

						if(m_oldPicFullPath != m_picFullPath)
							m_oldPicFullPath = m_picFullPath;
					}

					CRect rect(MINI_PICTURE_LEFT, MINI_PICTURE_TOP, MINI_PICTURE_LEFT + MINI_PICTURE_WIDTH, MINI_PICTURE_TOP + MINI_PICTURE_HEIGHT);
					//CStdGrfx::draw3dFrame(this->GetDC(), rect);
					CStdGrfx::drawframed3dBox( this->GetDC(), rect );

					if(strTmp.Trim(STR_TRIM_LETTER) != STR_EMPTY)
					{
						BitBltDS( this->GetDC(), GetBufferDC(MASK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
						BitBltDS( this->GetDC(), GetBufferDC(REAL_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
					}
				}
				else
					m_editDLG_Property[i].SetWindowText(strTmp);
			}

		}
	}
}

void CDLGPropertyDlg::OnBnClickedButtonDlgSearch()
{
	char szFilters[] = "Spr Files(*.spr)|*.spr|Mpr Files(*.mpr)|*.mpr|All Files(*.*)|*.*||";

	CFileDialog fileDlg( TRUE, NULL, "*.spr", OFN_FILEMUSTEXIST, szFilters, this);

	if( IDOK == fileDlg.DoModal() )
	{
		m_picFullPath = fileDlg.GetPathName();

		if(m_picFullPath.Trim(STR_TRIM_LETTER) != STR_EMPTY && m_picFullPath != m_oldPicFullPath)
		{
			DeleteAllBmp();
			DeleteAllBufferDC();
			ClearAllOldBmp();
			m_spr.ClearImageData();

			m_editDLG_Property[DLG_BG_IMAGE_FULLPATH].SetWindowText( fileDlg.GetPathName() );
			
			if(m_spr.LoadSpr( (char *)(LPCTSTR)fileDlg.GetPathName().Trim(STR_TRIM_LETTER)))
				MakeMemDC(this->GetDC());

			if(m_oldPicFullPath != m_picFullPath)
				m_oldPicFullPath = m_picFullPath;

			CRect rect(MINI_PICTURE_LEFT, MINI_PICTURE_TOP, MINI_PICTURE_LEFT + MINI_PICTURE_WIDTH, MINI_PICTURE_TOP + MINI_PICTURE_HEIGHT);
			//CStdGrfx::draw3dFrame(this->GetDC(), rect);
			CStdGrfx::drawframed3dBox( this->GetDC(), rect );

			BitBltDS( this->GetDC(), GetBufferDC(MASK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
			BitBltDS( this->GetDC(), GetBufferDC(REAL_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);

			CString strTmp;
			
			//strTmp.Format("%d", m_spr.clHeader.siXsize);
			//m_editDLG_Property[DLG_BG_WIDTH].SetWindowText(strTmp);
			//strTmp.Format("%d", m_spr.clHeader.siYsize);
			//m_editDLG_Property[DLG_BG_HEIGHT].SetWindowText(strTmp);

			// 입력한 이미지 파일 fullpath 파싱
			ParsingFileFullPath(fileDlg.GetPathName());

			CString tileMode = GetTileMode();
			CString tileImageNum = GetTileImageNum();

			if(tileMode.Trim() != STR_EMPTY)
			{
				if(tileMode.Trim() == STR_ZERO && _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM0)	// 통맵 모드
					m_editDLG_Property[DLG_BG_TILE].SetWindowText(STR_ZERO);
				else // "1"이면 가로 모드, "2"이면 세로 모드
					m_editDLG_Property[DLG_BG_TILE].SetWindowText(STR_ONE);
			}
			else // 디폴트는 통맵방식
				m_editDLG_Property[DLG_BG_TILE].SetWindowText(STR_ZERO);

			if(tileImageNum.Trim() != STR_EMPTY)
			{
				if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM0 || _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM1
					|| _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM3 || _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM9)
				{
					strTmp.Format("%d", _ttoi(tileImageNum));
					m_editDLG_Property[DLG_BG_TILE_IMAGE_NUM].SetWindowText(strTmp);
				}
				else
				{
					strTmp = STR_ZERO;
					m_editDLG_Property[DLG_BG_TILE_IMAGE_NUM].SetWindowText(strTmp);
				}
			}

			//if(_ttoi(tileImageNum) < 2)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize);
			//	m_editDLG_Property[DLG_WIDTH].SetWindowText(strTmp);
			//	strTmp.Format("%d", m_spr.clHeader.siYsize);
			//	m_editDLG_Property[DLG_HEIGHT].SetWindowText(strTmp);
			//}
			//else if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM3)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize * _ttoi(tileImageNum));
			//	m_editDLG_Property[DLG_WIDTH].SetWindowText(strTmp);
			//	strTmp.Format("%d", m_spr.clHeader.siYsize);
			//	m_editDLG_Property[DLG_HEIGHT].SetWindowText(strTmp);
			//}
			//else if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM9)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize * 3);
			//	m_editDLG_Property[DLG_WIDTH].SetWindowText(strTmp);
			//	strTmp.Format("%d", m_spr.clHeader.siYsize * 3 );
			//	m_editDLG_Property[DLG_HEIGHT].SetWindowText(strTmp);
			//}

			//// 크기에 맞춰.. surface 생성
			//m_editDLG_Property[DLG_WIDTH].GetWindowText(strTmp);
			//m_editDLG_Property[DLG_BG_WIDTH].SetWindowText(strTmp);
			//m_editDLG_Property[DLG_HEIGHT].GetWindowText(strTmp);
			//m_editDLG_Property[DLG_BG_HEIGHT].SetWindowText(strTmp);

			// 다이얼로그 크기 변화 없이 이미지 적용 시키기!!
			int sub_width = 0, sub_height = 0;
			m_editDLG_Property[DLG_WIDTH].GetWindowText(strTmp);
			sub_width = _ttoi(strTmp);
			m_editDLG_Property[DLG_HEIGHT].GetWindowText(strTmp);
			sub_height = _ttoi(strTmp);
            
			strTmp.Format("%d", m_pDLGEditor->m_objs->SetToMultipleSize(sub_width, 1));
			// tile size => strTmp.Format("%d", m_pDLGEditor->m_objs->SetToMultipleSize(sub_width, m_spr.clHeader.siXsize));
			// 크기에 맞춰.. surface 생성
			m_editDLG_Property[DLG_WIDTH].SetWindowText(strTmp);
			m_editDLG_Property[DLG_BG_WIDTH].SetWindowText(strTmp);

			strTmp.Format("%d", m_pDLGEditor->m_objs->SetToMultipleSize(sub_height, 1));
			// tile size => strTmp.Format("%d", m_pDLGEditor->m_objs->SetToMultipleSize(sub_height, m_spr.clHeader.siYsize));

			m_editDLG_Property[DLG_HEIGHT].SetWindowText(strTmp);
			m_editDLG_Property[DLG_BG_HEIGHT].SetWindowText(strTmp);
           
		}
	}
	
	//this->RedrawWindow();
	this->SendMessage( WM_PAINT );
}

void CDLGPropertyDlg::ShowProperties( CWnd* parent, BOOL show )
/* ============================================================
	Function :		CDiagramEntity::ShowProperties
	Description :	Shows the property dialog for the object.
	Access :		Public

	Return :		void
	Parameters :	CWnd* parent	-	Parent of the dialog
					BOOL show		-	"TRUE" to show, "FALSE" 
										to hide.

	Usage :			Call to show the property dialog for this 
					object.

   ============================================================*/
{
	m_pParent = parent;
	//GetEntity()->SetParent(parent);

	if( show )
	{
		if( !m_hWnd )
			Create( ( UINT ) CDLGPropertyDlg::IDD, parent );

		ShowWindow( SW_SHOW );
		//MoveWindow(700,200, 300, 800);
		SetValues();
		SetFocus();

	}
	else
		if( m_hWnd )
			ShowWindow( SW_HIDE );

}

void CDLGPropertyDlg::SetDLGEditor(CDiagramEditor *pDLGEditor)
{
    m_pDLGEditor = pDLGEditor;
}

void CDLGPropertyDlg::OnBnClickedButtonDlgPathEmpty2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_editDLG_Property[DLG_BG_IMAGE_FULLPATH].SetWindowText(STR_EMPTY);
	m_editDLG_Property[DLG_BG_TILE].SetWindowText(STR_ZERO);
	m_editDLG_Property[DLG_BG_TILE_IMAGE_NUM].SetWindowText(STR_ZERO);
	m_editDLG_Property[DLG_BG_START_FONT].SetWindowText(STR_ZERO);
	
	if(m_spr.Image != NULL)
	{
		CRect rect(0, 0, m_spr.clHeader.siXsize+2*PIXEL_OFFSET_X, m_spr.clHeader.siYsize+2*PIXEL_OFFSET_Y);

		CStdGrfx::drawframed3dBox( GetBufferDC(WORK_BACKGROUND_NUM), rect );
		BitBltDS( this->GetDC(), GetBufferDC(WORK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize + PIXEL_OFFSET_X*2, m_spr.clHeader.siYsize + PIXEL_OFFSET_Y*2, SRCCOPY, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
		m_picFullPath.Empty();
		m_oldPicFullPath.Empty();
	}

	m_spr.ClearImageData();
}

void CDLGPropertyDlg::OnOK()
{
	OnBnClickedButtonDlgApply();
	if(m_bError == false)
		CDialog::OnOK();
}
void CDLGPropertyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDiagramPropertyDlg::OnShowWindow(bShow, nStatus);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}
