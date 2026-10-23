#include "stdafx.h"
#include "DLGDefine.h"
#include "ButtonPropertyDlg.h"
#include ".\buttonpropertydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg dialog


CButtonPropertyDlg::CButtonPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CButtonPropertyDlg::IDD, pParent)
{
}


void CButtonPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// leon add
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_KIND_NAME	, 	m_editBTN_Property	[BTN_KIND_NAME]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_ID	, 	m_editBTN_Property	[BTN_RESOURCE_ID]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_X_COORD	, 	m_editBTN_Property	[BTN_X_COORD]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_Y_COORD	, 	m_editBTN_Property	[BTN_Y_COORD]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_WIDTH	, 	m_editBTN_Property	[BTN_WIDTH]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_HEIGHT	, 	m_editBTN_Property	[BTN_HEIGHT]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_IMAGE_FULLPATH	, 	m_editBTN_Property	[BTN_BG_IMAGE_FULLPATH]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_BG_WIDTH	, 	m_editBTN_Property	[BTN_BG_WIDTH]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_BG_HEIGHT	, 	m_editBTN_Property	[BTN_BG_HEIGHT]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_BG_TILE	, 	m_editBTN_Property	[BTN_BG_TILE]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_BG_TILE_IMAGE_NUM	, 	m_editBTN_Property	[BTN_BG_TILE_IMAGE_NUM])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_START_FONT	, 	m_editBTN_Property	[BTN_BG_START_FONT])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_TEXT_COLOR	, 	m_editBTN_Property	[BTN_TEXT_COLOR])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_R	, 	m_editBTN_Property	[BTN_TEXT_COLOR_R])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_G	, 	m_editBTN_Property	[BTN_TEXT_COLOR_G])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_B	, 	m_editBTN_Property	[BTN_TEXT_COLOR_B])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_WEIGHT, 	m_editBTN_Property	[BTN_WEIGHT])	;

	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_TEXT_STYLE	, 	m_editBTN_Property	[BTN_TEXT_STYLE])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_TEXT_SIZE	, 	m_editBTN_Property	[BTN_TEXT_SIZE])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_GROUP_NUM	, 	m_editBTN_Property	[BTN_GROUP_NUM]	)	;

	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_TEXT	, 	m_editBTN_Property	[BTN_TEXT_STR]	)	;

	DDX_Control	(	pDX	, 	IDC_EDIT_BTN_ANIMATION	, 	m_editBTN_Property	[BTN_ANIMATION]	)	;
}


BEGIN_MESSAGE_MAP(CButtonPropertyDlg, CDialog)

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_BTN_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_BTN_HIDE, OnBnClickedHide)
	ON_BN_CLICKED(IDC_BUTTON_BTN_SEARCH, OnBnClickedButtonBtnSearch)
	ON_BN_CLICKED(IDC_BUTTON_BTN_PATH_EMPTY, OnBnClickedButtonBtnPathEmpty)

	ON_BN_CLICKED(IDC_BUTTON_BTN_TEXT_SEARCH, OnBnClickedButtonBtnTextSearch)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg virtuals

// Button Property Dialog 상자를 열 때 초기화 함
// Button 객체의 크기 조정 - SetRect(...)함수 호출시 호출 됨

// 값을 셋팅할 필요가 있을 때 호출됨
// 작업 관계 : 컨트롤의 실제 데이터 저장소 => 컨트롤의 속성창
void CButtonPropertyDlg::SetValues()
{
	CString strTmp;

	int i =0;

	if( m_hWnd )
	{
		if( GetEntity() )
		{
			/**
			leon add
			* button property 일괄 처리를 위해 배열 형태로 변경함
			*/
			for(i=0; i<BTN_PROPERTY_COUNT; i++)
			{
				// 실제 저장된 데이터가 기준
				// 데이터 값 얻음
				GetEntity()->GetPropertyWithIndex(strTmp, i);
			
				if(i == BTN_BG_IMAGE_FULLPATH)
				{
					// 실제 저장된 데이터 값과 Dialog 속성 창의 미니 이미지가 다르다면
					// 미니 이미지 갱신!!
					if(strTmp != m_picFullPath || !GetBufferDC(REAL_BACKGROUND_NUM) ) // todo - 다른 이미지 변경 속성 처리 요!!
					{
						DeleteAllBmp();
						
						ClearAllOldBmp();

						if(m_picFullPath != strTmp)
							m_picFullPath = strTmp;

						m_editBTN_Property[i].SetWindowText(strTmp);

						if(strTmp.Trim(STR_TRIM_LETTER) != STR_EMPTY)
						{
							if(m_spr.LoadSpr( (char *)(LPCTSTR) strTmp.Trim(STR_TRIM_LETTER)))
							{
                                //MakeMemDC(this->GetDC());

								CString strTile, strStartFont, strTileImageNum;

								GetEntity()->GetPropertyWithIndex(strTile, BTN_BG_TILE);
								GetEntity()->GetPropertyWithIndex(strStartFont, BTN_BG_START_FONT);
								GetEntity()->GetPropertyWithIndex(strTileImageNum, BTN_BG_TILE_IMAGE_NUM);

								if(strTile != STR_ZERO)
									MakeMemDC( this->GetDC(), atoi(strStartFont), atoi(strTileImageNum));
								else
									MakeMemDC( this->GetDC(), atoi(strStartFont), WHOLE_IMAGE_STYLE_FONTNUM);
							}
						}

						if(m_oldPicFullPath != m_picFullPath)
							m_oldPicFullPath = m_picFullPath;
					}

					//CRect rect(MINI_PICTURE_LEFT, MINI_PICTURE_TOP, MINI_PICTURE_LEFT + MINI_PICTURE_WIDTH, MINI_PICTURE_TOP + MINI_PICTURE_HEIGHT);
					//CStdGrfx::draw3dFrame(this->GetDC(), rect);
					//CStdGrfx::drawframed3dBox( this->GetDC(), rect );

					//if(strTmp.Trim(STR_TRIM_LETTER) != STR_EMPTY)
					//{
					//	BitBltDS( this->GetDC(), GetBufferDC(MASK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
					//	BitBltDS( this->GetDC(), GetBufferDC(REAL_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
					//}

					CRect rect(0, 0, m_spr.clHeader.siXsize+2*PIXEL_OFFSET_X, m_spr.clHeader.siYsize+2*PIXEL_OFFSET_Y);

					if(strTmp.Trim(STR_TRIM_LETTER) != STR_EMPTY)
					{
						CStdGrfx::drawframed3dBox( GetBufferDC(WORK_BACKGROUND_NUM), rect );

						BitBltDS( GetBufferDC(WORK_BACKGROUND_NUM), GetBufferDC(MASK_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, PIXEL_OFFSET_X, PIXEL_OFFSET_Y);
						BitBltDS( GetBufferDC(WORK_BACKGROUND_NUM), GetBufferDC(REAL_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, PIXEL_OFFSET_X, PIXEL_OFFSET_Y);

						CString tmpCStr;
						int iTmp = -1;
						int iTileNum = -1;
						m_editBTN_Property[BTN_BG_TILE_IMAGE_NUM].GetWindowText( tmpCStr );
						iTmp = _tstoi( tmpCStr );

						DrawMiniPicture( this->GetDC(), iTmp );

						//CString tmpCStr;
						//int iTmp = -1;
						//int iTileNum = -1;
						//m_editBTN_Property[BTN_BG_TILE_IMAGE_NUM].GetWindowText( tmpCStr );
						//iTmp = _tstoi( tmpCStr );

						//switch( iTmp )
						//{
						//case VALID_TILEIMAGE_NUM0:
						//	{
						//		//iTileNum = VALID_TILEIMAGE_NUM0;
						//		iTileNum = 1;

						//		//if( GetEntity() != NULL && iTileNum != -1 && m_spr.clHeader.siXsize < MINI_PICTURE_WIDTH && m_spr.clHeader.siYsize < MINI_PICTURE_HEIGHT )
						//		//	GetEntity()->DrawTileImg( this->GetDC(), SPR_PUTSPRT, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP, MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, iTileNum, true);
						//		//else
						//		//	BitBltDS( this->GetDC(), GetBufferDC(WORK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize + PIXEL_OFFSET_X*2, m_spr.clHeader.siYsize + PIXEL_OFFSET_Y*2, SRCCOPY, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
						//	}
						//	break;
						//case VALID_TILEIMAGE_NUM1:
						//	{
						//		iTileNum = VALID_TILEIMAGE_NUM1;
						//	}
						//	break;
						//case VALID_TILEIMAGE_NUM3:
						//	{
						//		iTileNum = VALID_TILEIMAGE_NUM3;
						//	}
						//	break;
						//case VALID_TILEIMAGE_NUM9:
						//	{
						//		iTileNum = VALID_TILEIMAGE_NUM9;
						//	}
						//	break;
						//}

						//if( GetEntity() != NULL && iTileNum != -1 )
						//	GetEntity()->DrawTileImg( this->GetDC(), SPR_PUTSPRT, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP, MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, iTileNum, true);

						// 미니 그림 크기에 맞게 무조건 Scratch Mode!
						//if( m_spr.clHeader.siXsize > MINI_PICTURE_WIDTH && m_spr.clHeader.siYsize > MINI_PICTURE_HEIGHT )
						//	BitBltDS( this->GetDC(), GetBufferDC(WORK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize + PIXEL_OFFSET_X*2, m_spr.clHeader.siYsize + PIXEL_OFFSET_Y*2, SRCCOPY, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
						//else
						//	BitBltDS( this->GetDC(), GetBufferDC(WORK_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize + PIXEL_OFFSET_X*2, m_spr.clHeader.siYsize + PIXEL_OFFSET_Y*2, SRCCOPY, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);

					}
					else
					{
						CStdGrfx::drawframed3dBox( GetBufferDC(WORK_BACKGROUND_NUM), rect );
						BitBltDS( this->GetDC(), GetBufferDC(WORK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize + PIXEL_OFFSET_X*2, m_spr.clHeader.siYsize + PIXEL_OFFSET_Y*2, SRCCOPY, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
						m_picFullPath.Empty();
						m_oldPicFullPath.Empty();
					}
				}
				else
					m_editBTN_Property[i].SetWindowText(strTmp);

			}

		}
	}
}

// 목표 대상 : 다이얼로그 윈도우의 컨트롤
// 작업 관계 : 컨트롤의 속성창값 => 컨트롤의 실제 데이터 저장소
void CButtonPropertyDlg::OnButtonApply() 
{
	int i=0;
	m_bError = false;

	if( m_hWnd )
	{
		if( GetEntity() )
		{
			CString	strTemp[BTN_PROPERTY_COUNT];
			CString strTemp2;
			CString strTileImageNum;
			CString strTile;
			CString strTimeImageStartFont;
			CString tmpstr;

			int R = 0, G = 0, B = 0;
            m_editBTN_Property[BTN_TEXT_COLOR_R].GetWindowText(tmpstr);
			R = _ttoi(tmpstr);
			m_editBTN_Property[BTN_TEXT_COLOR_G].GetWindowText(tmpstr);
			G = _ttoi(tmpstr);
			m_editBTN_Property[BTN_TEXT_COLOR_B].GetWindowText(tmpstr);
			B = _ttoi(tmpstr);
			tmpstr.Format("%d", RGB(R,G,B) );
            m_editBTN_Property[BTN_TEXT_COLOR].SetWindowText(tmpstr);

			for(i=0; i<BTN_PROPERTY_COUNT; i++)
			{
				// 컨트롤에 입력한 값 얻음
                m_editBTN_Property[i].GetWindowText(strTemp[i]);
				
				if(i == BTN_RESOURCE_ID)
				{
                    if(GetEntity()->IsOverLapping(GetEntity(), strTemp[BTN_RESOURCE_ID]))
					{
						AfxMessageBox("중복된 ID가 이미 존재합니다.\n 다시 입력하세요!!");
						m_bError = true;
						return;
					}
				}

				// 실제 데이터로 저장하고 있는 값
				GetEntity()->GetPropertyWithIndex(strTemp2, i);

				if(strTemp2 != strTemp[i])
				{
					// 실제 데이터 저장소에 컨트롤 속성 값 저장
					GetEntity()->SetPropertyWithIndex(strTemp[i], i);
				
					if( (GetEntity()->GetPicFullPath().Trim(STR_TRIM_LETTER) != STR_EMPTY && GetEntity()->GetPicFullPath() != GetEntity()->GetOldPicFullPath()
						&& i == BTN_BG_IMAGE_FULLPATH )/*|| i == BTN_BG_TILE*/ || i == BTN_BG_START_FONT
						|| i == BTN_BG_WIDTH || i == BTN_BG_HEIGHT || i == BTN_BG_TILE_IMAGE_NUM )
					{
						if(GetEntity()->GetPicFullPath() != strTemp[BTN_BG_IMAGE_FULLPATH])
							GetEntity()->SetPicFullPath(strTemp[BTN_BG_IMAGE_FULLPATH]);

						//if(GetEntity()->GetPicFullPath().Trim(STR_TRIM_LETTER) != STR_EMPTY && GetEntity()->GetPicFullPath() != GetEntity()->GetOldPicFullPath())
						{
							GetEntity()->DeleteAllBmp();
							GetEntity()->DeleteAllBufferDC();
							GetEntity()->ClearAllOldBmp();

							GetEntity()->m_spr.ClearImageData();

							// 타일 이미지 개수에 따른 크기로 메모리 버퍼 생성!!
							if(GetEntity()->m_spr.LoadSpr( (char*)(LPCTSTR) strTemp[BTN_BG_IMAGE_FULLPATH].Trim(STR_TRIM_LETTER)))
							{
								m_editBTN_Property[BTN_BG_TILE_IMAGE_NUM].GetWindowText(strTileImageNum);
								m_editBTN_Property[BTN_BG_TILE].GetWindowText(strTile);
								m_editBTN_Property[BTN_BG_START_FONT].GetWindowText(strTimeImageStartFont);

                                if(strTile != STR_ZERO) // 타일 방식일때 폰트 이미지 개수
									GetEntity()->MakeMemDC(GetParent()->GetDC(), atoi(strTimeImageStartFont), atoi(strTileImageNum) );
								else	// 통 이미지 방식 일때
									GetEntity()->MakeMemDC(GetParent()->GetDC(), atoi(strTimeImageStartFont), WHOLE_IMAGE_STYLE_FONTNUM);
							}

							if(GetEntity()->GetOldPicFullPath() != strTemp[BTN_BG_IMAGE_FULLPATH])
								GetEntity()->SetOldPicFullPath(strTemp[BTN_BG_IMAGE_FULLPATH]);

							//if(m_picFullPath != strTemp[BTN_BG_IMAGE_FULLPATH])
							//	m_picFullPath = strTemp[BTN_BG_IMAGE_FULLPATH];

							//if(m_oldPicFullPath != m_picFullPath)
							//	m_oldPicFullPath = m_picFullPath;
						}
					}
				}

			} // for(...)

			strTemp2.Empty();
			int sub_width = 0, sub_BG_width =0, sub_height = 0, sub_BG_height = 0;

			sub_width = _ttoi(strTemp[BTN_WIDTH]);
			sub_height = _ttoi(strTemp[BTN_HEIGHT]);
			sub_BG_width = _ttoi(strTemp[BTN_BG_WIDTH]);
			sub_BG_height = _ttoi(strTemp[BTN_BG_HEIGHT]);

			// tile size => strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_width, m_spr.clHeader.siXsize));
			strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_width, 1));
			m_editBTN_Property[BTN_WIDTH].SetWindowText(strTemp2);
			GetEntity()->SetPropertyWithIndex(strTemp2, BTN_WIDTH);
	
			// Disable로 해서 상관없음
			//if(sub_width > sub_BG_width)
			//{
	            m_editBTN_Property[BTN_BG_WIDTH].SetWindowText(strTemp2);
				GetEntity()->SetPropertyWithIndex(strTemp2, BTN_BG_WIDTH);
			//}
			//else
			//{
			//	strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_BG_width, m_spr.clHeader.siXsize));
	  //          m_editBTN_Property[BTN_BG_WIDTH].SetWindowText(strTemp2);
			//	GetEntity()->SetPropertyWithIndex(strTemp2, BTN_BG_WIDTH);
			//}

			//tile size => strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_height, m_spr.clHeader.siYsize));
			strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_height, 1));
			m_editBTN_Property[BTN_HEIGHT].SetWindowText(strTemp2);
			GetEntity()->SetPropertyWithIndex(strTemp2, BTN_HEIGHT);
	
			//if(sub_height > sub_BG_height)
			//{
				m_editBTN_Property[BTN_BG_HEIGHT].SetWindowText(strTemp2);
				GetEntity()->SetPropertyWithIndex(strTemp2, BTN_BG_HEIGHT);
			//}
			//else
			//{
			//	strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_BG_height, m_spr.clHeader.siYsize));
			//	m_editBTN_Property[BTN_BG_HEIGHT].SetWindowText(strTemp2);
			//	GetEntity()->SetPropertyWithIndex(strTemp2, BTN_BG_HEIGHT);
			//}

			// id 글자수가 30자를 넘지 못하게 함
			CString tmpStr;           
			m_editBTN_Property[RESOURCE_ID].GetWindowText(tmpStr);
			if(tmpStr.GetLength() > ID_LENGTH)
			{
				AfxMessageBox(tmpStr + " ID 값이 30자를 초과했습니다.\n 다시 입력하세요!");
				tmpStr = tmpStr.Left( 30 );
				m_editBTN_Property[RESOURCE_ID].SetWindowText(tmpStr);
				m_bError = true;
			}

            
			GetParent()->RedrawWindow();
		}
	}	
}

// 버튼 속성창의 이미지 미리 보기 처리
// 목표 대상 : 컨트롤의 속성창
void CButtonPropertyDlg::OnBnClickedButtonBtnSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

			m_editBTN_Property[BTN_BG_IMAGE_FULLPATH].SetWindowText( fileDlg.GetPathName() );

			if(m_spr.LoadSpr( (char *)(LPCTSTR)fileDlg.GetPathName().Trim(STR_TRIM_LETTER)))
				MakeMemDC(this->GetDC());

			if(m_oldPicFullPath != m_picFullPath)
				m_oldPicFullPath = m_picFullPath;

			//CRect rect(MINI_PICTURE_LEFT, MINI_PICTURE_TOP, MINI_PICTURE_LEFT + MINI_PICTURE_WIDTH, MINI_PICTURE_TOP + MINI_PICTURE_HEIGHT);
			//CStdGrfx::draw3dFrame(this->GetDC(), rect);
			//CStdGrfx::drawframed3dBox( this->GetDC(), rect );

			CRect rect(0, 0, m_spr.clHeader.siXsize+2*PIXEL_OFFSET_X, m_spr.clHeader.siYsize+2*PIXEL_OFFSET_Y);

			CStdGrfx::drawframed3dBox( GetBufferDC(WORK_BACKGROUND_NUM), rect );

			BitBltDS( GetBufferDC(WORK_BACKGROUND_NUM), GetBufferDC(MASK_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, PIXEL_OFFSET_X, PIXEL_OFFSET_Y);
			BitBltDS( GetBufferDC(WORK_BACKGROUND_NUM), GetBufferDC(REAL_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, PIXEL_OFFSET_X, PIXEL_OFFSET_Y);
			BitBltDS( this->GetDC(), GetBufferDC(WORK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize + PIXEL_OFFSET_X*2, m_spr.clHeader.siYsize + PIXEL_OFFSET_Y*2, SRCCOPY, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);

			//BitBltDS( this->GetDC(), GetBufferDC(MASK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
			//BitBltDS( this->GetDC(), GetBufferDC(REAL_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);

			CString strTmp;

			// 입력한 이미지 파일 fullpath 파싱
			ParsingFileFullPath(fileDlg.GetPathName());

			CString tileMode = GetTileMode();
			CString tileImageNum = GetTileImageNum();

			if(tileMode.Trim() != STR_EMPTY)
			{
				if(tileMode.Trim() == STR_ZERO && _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM0)	// 통맵 모드
					m_editBTN_Property[BTN_BG_TILE].SetWindowText(STR_ZERO);
				else // "1"이면 가로 모드, "2"이면 세로 모드
					m_editBTN_Property[BTN_BG_TILE].SetWindowText(STR_ONE);
			}
			else // 디폴트는 통맵방식
				m_editBTN_Property[BTN_BG_TILE].SetWindowText(STR_ZERO);

			if(tileImageNum.Trim() != STR_EMPTY)
			{
				if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM0 || _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM1
					|| _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM3 || _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM9)
				{
					strTmp.Format("%d", _ttoi(tileImageNum));
					m_editBTN_Property[BTN_BG_TILE_IMAGE_NUM].SetWindowText(strTmp);
				}
				else
				{
					strTmp = STR_ZERO;
					m_editBTN_Property[BTN_BG_TILE_IMAGE_NUM].SetWindowText(strTmp);
				}
			}

			//if(_ttoi(tileImageNum) < 2)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize);
			//	m_editBTN_Property[BTN_WIDTH].SetWindowText(strTmp);
			//	strTmp.Format("%d", m_spr.clHeader.siYsize);
			//	m_editBTN_Property[BTN_HEIGHT].SetWindowText(strTmp);
			//}
			//else if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM3)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize * _ttoi(tileImageNum));
			//	m_editBTN_Property[BTN_WIDTH].SetWindowText(strTmp);
			//	strTmp.Format("%d", m_spr.clHeader.siYsize);
			//	m_editBTN_Property[BTN_HEIGHT].SetWindowText(strTmp);
			//}
			//else if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM9)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize * 3);
			//	m_editBTN_Property[BTN_WIDTH].SetWindowText(strTmp);
			//	strTmp.Format("%d", m_spr.clHeader.siYsize * 3 );
			//	m_editBTN_Property[BTN_HEIGHT].SetWindowText(strTmp);
			//}

			//// 크기에 맞춰.. surface 생성
			//m_editBTN_Property[BTN_WIDTH].GetWindowText(strTmp);
			//m_editBTN_Property[BTN_BG_WIDTH].SetWindowText(strTmp);
			//m_editBTN_Property[BTN_HEIGHT].GetWindowText(strTmp);
			//m_editBTN_Property[BTN_BG_HEIGHT].SetWindowText(strTmp);

			// 다이얼로그 크기 변화 없이 이미지 적용 시키기!!
			int sub_width = 0, sub_height = 0;
			m_editBTN_Property[BTN_WIDTH].GetWindowText(strTmp);
			sub_width = _ttoi(strTmp);
			m_editBTN_Property[BTN_HEIGHT].GetWindowText(strTmp);
			sub_height = _ttoi(strTmp);
            
			// tile size => strTmp.Format("%d", GetEntity()->SetToMultipleSize(sub_width, m_spr.clHeader.siXsize));
			strTmp.Format("%d", GetEntity()->SetToMultipleSize(sub_width, 1));
			// 크기에 맞춰.. surface 생성
			m_editBTN_Property[BTN_WIDTH].SetWindowText(strTmp);
			m_editBTN_Property[BTN_BG_WIDTH].SetWindowText(strTmp);

			// tile size => strTmp.Format("%d", GetEntity()->SetToMultipleSize(sub_height, m_spr.clHeader.siYsize));
			strTmp.Format("%d", GetEntity()->SetToMultipleSize(sub_height, 1));
			m_editBTN_Property[BTN_HEIGHT].SetWindowText(strTmp);
			m_editBTN_Property[BTN_BG_HEIGHT].SetWindowText(strTmp);

		}
	}
	
	//this->RedrawWindow();
	this->SendMessage( WM_PAINT );
}

void CButtonPropertyDlg::OnPaint()
{
	CDiagramPropertyDlg::OnPaint();
}

BOOL CButtonPropertyDlg::OnInitDialog()
{
	CDiagramPropertyDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CButtonPropertyDlg::OnBnClickedHide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}

void CButtonPropertyDlg::OnBnClickedButtonBtnPathEmpty()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_editBTN_Property[BTN_BG_IMAGE_FULLPATH].SetWindowText(STR_EMPTY);
	m_editBTN_Property[BTN_BG_TILE].SetWindowText(STR_ZERO);
	m_editBTN_Property[BTN_BG_TILE_IMAGE_NUM].SetWindowText(STR_ZERO);
	m_editBTN_Property[BTN_BG_START_FONT].SetWindowText(STR_ZERO);

	
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

// Font 대화상자에서 => 컨트롤 속성 다이얼로그 상자로 셋팅
void CButtonPropertyDlg::OnBnClickedButtonBtnTextSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowFontDialog();
	//CString tmpStr = m_fontName;
	//tmpStr.Format("%s %d %d", tmpStr, m_fontSize, m_fontColor);
	//AfxMessageBox(tmpStr);
	CString tmpStr;

	m_editBTN_Property[BTN_TEXT_STYLE].SetWindowText(GetFontName());
	tmpStr.Format("%d", GetFontSize());
	m_editBTN_Property[BTN_TEXT_SIZE].SetWindowText(tmpStr);
	tmpStr.Format("%d", GetFontColor());
	m_editBTN_Property[BTN_TEXT_COLOR].SetWindowText(tmpStr);

}

void CButtonPropertyDlg::OnOK()
{
	OnButtonApply();
	if(m_bError == false)
		CDialog::OnOK();
}
void CButtonPropertyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	
	CDiagramPropertyDlg::OnShowWindow(bShow, nStatus);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
