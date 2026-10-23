#include "StdAfx.h"
#include ".\comboboxpropertydlg.h"

CComboBoxPropertyDlg::CComboBoxPropertyDlg(CWnd* pParent)
: CDiagramPropertyDlg(CComboBoxPropertyDlg::IDD, pParent)
{
}

CComboBoxPropertyDlg::~CComboBoxPropertyDlg(void)
{
}

// Control과 멤버 변수와 연결
void CComboBoxPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_KIND_NAME	, 	m_editCBX_Property	[CBX_KIND_NAME]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_ID	, 	m_editCBX_Property	[CBX_RESOURCE_ID]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_X_COORD	, 	m_editCBX_Property	[CBX_X_COORD]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_Y_COORD	, 	m_editCBX_Property	[CBX_Y_COORD]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_WIDTH	, 	m_editCBX_Property	[CBX_WIDTH]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_HEIGHT	, 	m_editCBX_Property	[CBX_HEIGHT]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_IMAGE_FULLPATH	, 	m_editCBX_Property	[CBX_BG_IMAGE_FULLPATH]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_BG_WIDTH	, 	m_editCBX_Property	[CBX_BG_WIDTH]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_BG_HEIGHT	, 	m_editCBX_Property	[CBX_BG_HEIGHT]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_BG_TILE	, 	m_editCBX_Property	[CBX_BG_TILE]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_BG_TILE_IMAGE_NUM	, 	m_editCBX_Property	[CBX_BG_TILE_IMAGE_NUM])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_START_FONT	, 	m_editCBX_Property	[CBX_BG_START_FONT])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_TEXT_COLOR	, 	m_editCBX_Property	[CBX_TEXT_COLOR])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_R	, 	m_editCBX_Property	[CBX_TEXT_COLOR_R])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_G	, 	m_editCBX_Property	[CBX_TEXT_COLOR_G])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_B	, 	m_editCBX_Property	[CBX_TEXT_COLOR_B])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_WEIGHT, 	m_editCBX_Property	[CBX_WEIGHT])	;

	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_TEXT_STYLE	, 	m_editCBX_Property	[CBX_TEXT_STYLE])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_TEXT_SIZE	, 	m_editCBX_Property	[CBX_TEXT_SIZE])	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_GROUP_NUM	, 	m_editCBX_Property	[CBX_GROUP_NUM]	)	;

	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_POPUP_HEIGHT	, 	m_editCBX_Property	[CBX_POPUP_HEIGHT]	)	;
	DDX_Control	(	pDX	, 	IDC_EDIT_CBX_LINE_HEIGHT	, 	m_editCBX_Property	[CBX_LINE_HEIGHT]	)	;
}

// Control과 메세지 함수 연결
BEGIN_MESSAGE_MAP(CComboBoxPropertyDlg, CDialog)

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CBX_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_CBX_HIDE, OnBnClickedHide)
	ON_BN_CLICKED(IDC_BUTTON_CBX_SEARCH, OnBnClickedButtonCbxSearch)

	ON_BN_CLICKED(IDC_BUTTON_CBX_PATH_EMPTY, OnBnClickedButtonCbxPathEmpty)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CComboBoxPropertyDlg virtuals

// Button Property Dialog 상자를 열 때 초기화 함

void CComboBoxPropertyDlg::SetValues()
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
			for(i=0; i<CBX_PROPERTY_COUNT; i++)
			{
				GetEntity()->GetPropertyWithIndex(strTmp, i);
			
				if(i == CBX_BG_IMAGE_FULLPATH)
				{
					if(strTmp != m_picFullPath || !GetBufferDC(REAL_BACKGROUND_NUM) ) // todo - 다른 이미지 변경 속성 처리 요!!
					{
						DeleteAllBmp();
						
						ClearAllOldBmp();

						if(m_picFullPath != strTmp)
							m_picFullPath = strTmp;

						m_editCBX_Property[i].SetWindowText(strTmp);

						if(strTmp.Trim(STR_TRIM_LETTER) != STR_EMPTY)
						{
							if(m_spr.LoadSpr( (char *)(LPCTSTR) strTmp.Trim(STR_TRIM_LETTER)))
                                MakeMemDC(this->GetDC());
						}
					}

					//CRect rect(MINI_PICTURE_LEFT, MINI_PICTURE_TOP, MINI_PICTURE_LEFT + MINI_PICTURE_WIDTH, MINI_PICTURE_TOP + MINI_PICTURE_HEIGHT);
					////CStdGrfx::draw3dFrame(this->GetDC(), rect);
					//CStdGrfx::drawframed3dBox( this->GetDC(), rect );

					//if(strTmp.Trim(STR_TRIM_LETTER) != STR_EMPTY)
					//{
					//	BitBltDS( this->GetDC(), GetBufferDC(MASK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
					//	BitBltDS( this->GetDC(), GetBufferDC(REAL_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
					//}

					CRect rect(0, 0, m_spr.clHeader.siXsize+2*PIXEL_OFFSET_X, m_spr.clHeader.siYsize+2*PIXEL_OFFSET_Y);

					if(strTmp.Trim(STR_TRIM_LETTER) != STR_EMPTY)
					{
						//CStdGrfx::drawframed3dBox( this->GetDC(), rect );
						//BitBltDS( this->GetDC(), GetBufferDC(MASK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
						//BitBltDS( this->GetDC(), GetBufferDC(REAL_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
						CStdGrfx::drawframed3dBox( GetBufferDC(WORK_BACKGROUND_NUM), rect );
	
						BitBltDS( GetBufferDC(WORK_BACKGROUND_NUM), GetBufferDC(MASK_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, PIXEL_OFFSET_X, PIXEL_OFFSET_Y);
						BitBltDS( GetBufferDC(WORK_BACKGROUND_NUM), GetBufferDC(REAL_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, PIXEL_OFFSET_X, PIXEL_OFFSET_Y);
						BitBltDS( this->GetDC(), GetBufferDC(WORK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize + PIXEL_OFFSET_X*2, m_spr.clHeader.siYsize + PIXEL_OFFSET_Y*2, SRCCOPY, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
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
					m_editCBX_Property[i].SetWindowText(strTmp);

			}

		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CComboBoxPropertyDlg message handlers

void CComboBoxPropertyDlg::OnButtonApply() 
{
	int i=0;
	m_bError = false;

	if( m_hWnd )
	{
		if( GetEntity() )
		{
			CString	strTemp[CBX_PROPERTY_COUNT];
			CString strTemp2;
			CString strTileImageNum;
			CString strTile;
			CString strTimeImageStartFont;
			CString tmpstr;

			int R = 0, G = 0, B = 0;
            m_editCBX_Property[CBX_TEXT_COLOR_R].GetWindowText(tmpstr);
			R = _ttoi(tmpstr);
			m_editCBX_Property[CBX_TEXT_COLOR_G].GetWindowText(tmpstr);
			G = _ttoi(tmpstr);
			m_editCBX_Property[CBX_TEXT_COLOR_B].GetWindowText(tmpstr);
			B = _ttoi(tmpstr);
			tmpstr.Format("%d", RGB(R,G,B) );
            m_editCBX_Property[CBX_TEXT_COLOR].SetWindowText(tmpstr);

			for(i=0; i<CBX_PROPERTY_COUNT; i++)
			{
				// 컨트롤에 입력한 값 얻음
                m_editCBX_Property[i].GetWindowText(strTemp[i]);
				
				if(i == CBX_RESOURCE_ID)
				{
                    if(GetEntity()->IsOverLapping(GetEntity(), strTemp[CBX_RESOURCE_ID]))
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
						&& i == CBX_BG_IMAGE_FULLPATH )/*|| i == CBX_BG_TILE*/ || i == CBX_BG_START_FONT
						|| i == CBX_BG_WIDTH || i == CBX_BG_HEIGHT || i == BTN_BG_TILE_IMAGE_NUM )
					{
						if(GetEntity()->GetPicFullPath() != strTemp[CBX_BG_IMAGE_FULLPATH])
							GetEntity()->SetPicFullPath(strTemp[CBX_BG_IMAGE_FULLPATH]);

						//if(GetEntity()->GetPicFullPath().Trim(STR_TRIM_LETTER) != STR_EMPTY && GetEntity()->GetPicFullPath() != GetEntity()->GetOldPicFullPath())
						{
							GetEntity()->DeleteAllBmp();
							GetEntity()->DeleteAllBufferDC();
							GetEntity()->ClearAllOldBmp();

							GetEntity()->m_spr.ClearImageData();

							// 타일 이미지 개수에 따른 크기로 메모리 버퍼 생성!!
							if(GetEntity()->m_spr.LoadSpr( (char*)(LPCTSTR) strTemp[CBX_BG_IMAGE_FULLPATH].Trim(STR_TRIM_LETTER)))
							{
								m_editCBX_Property[CBX_BG_TILE_IMAGE_NUM].GetWindowText(strTileImageNum);
								m_editCBX_Property[CBX_BG_TILE].GetWindowText(strTile);
								m_editCBX_Property[CBX_BG_START_FONT].GetWindowText(strTimeImageStartFont);

                                if(strTile != STR_ZERO) // 타일 방식일때 폰트 이미지 개수
									GetEntity()->MakeMemDC(GetParent()->GetDC(), atoi(strTimeImageStartFont), atoi(strTileImageNum) );
								else	// 통 이미지 방식 일때
									GetEntity()->MakeMemDC(GetParent()->GetDC(), atoi(strTimeImageStartFont), WHOLE_IMAGE_STYLE_FONTNUM);
							}

							if(GetEntity()->GetOldPicFullPath() != strTemp[CBX_BG_IMAGE_FULLPATH])
								GetEntity()->SetOldPicFullPath(strTemp[CBX_BG_IMAGE_FULLPATH]);

							//if(m_picFullPath != strTemp[CBX_BG_IMAGE_FULLPATH])
							//	m_picFullPath = strTemp[CBX_BG_IMAGE_FULLPATH];

							//if(m_oldPicFullPath != m_picFullPath)
							//	m_oldPicFullPath = m_picFullPath;
						}
					}
				}

			} // for(...)


			// ComboBox만 surface 높이 = 한줄 높이 + 팝업창 높이로 자동 셋팅 되어야함
			strTemp2.Empty();
			int sub_width = 0, sub_BG_width =0, sub_height = 0, sub_BG_height = 0, sub_Popup_height =0;

			sub_width = _ttoi(strTemp[CBX_WIDTH]);
			sub_height = _ttoi(strTemp[CBX_HEIGHT]);
			sub_BG_width = _ttoi(strTemp[CBX_BG_WIDTH]);
			sub_BG_height = _ttoi(strTemp[CBX_BG_HEIGHT]);

			sub_Popup_height = _ttoi(strTemp[CBX_POPUP_HEIGHT]);
			//sub_Popup_height = GetEntity()->SetToMultipleSize(sub_Popup_height, m_spr.clHeader.siYsize);
			sub_Popup_height = GetEntity()->SetToMultipleSize(sub_Popup_height, 1);
			strTemp2.Format("%d", sub_Popup_height );
			m_editCBX_Property[CBX_POPUP_HEIGHT].SetWindowText(strTemp2);
			GetEntity()->SetPropertyWithIndex(strTemp2, CBX_POPUP_HEIGHT);

			//sub_width = GetEntity()->SetToMultipleSize(sub_width, m_spr.clHeader.siXsize);
			sub_width = GetEntity()->SetToMultipleSize(sub_width, 1);
			strTemp2.Format("%d", sub_width);
			m_editCBX_Property[CBX_WIDTH].SetWindowText(strTemp2);
			GetEntity()->SetPropertyWithIndex(strTemp2, CBX_WIDTH);
	
			//if(sub_width > sub_BG_width)
			//{
	            m_editCBX_Property[CBX_BG_WIDTH].SetWindowText(strTemp2);
				GetEntity()->SetPropertyWithIndex(strTemp2, CBX_BG_WIDTH);
			//}
			//else
			//{
			//	strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_BG_width, m_spr.clHeader.siXsize));
	  //          m_editCBX_Property[CBX_BG_WIDTH].SetWindowText(strTemp2);
			//	GetEntity()->SetPropertyWithIndex(strTemp2, CBX_BG_WIDTH);
			//}

			//strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_height, m_spr.clHeader.siYsize));
			strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_height, 1));
			m_editCBX_Property[CBX_HEIGHT].SetWindowText(strTemp2);
			GetEntity()->SetPropertyWithIndex(strTemp2, CBX_HEIGHT);
	
			//if(sub_height + sub_Popup_height > sub_BG_height)
			//{
				strTemp2.Format("%d", sub_height + sub_Popup_height);
				m_editCBX_Property[CBX_BG_HEIGHT].SetWindowText(strTemp2);
				GetEntity()->SetPropertyWithIndex(strTemp2, CBX_BG_HEIGHT);
			//}
			//else
			//{
			//	sub_BG_height = sub_height + sub_Popup_height;
			//	strTemp2.Format("%d", GetEntity()->SetToMultipleSize(sub_BG_height, m_spr.clHeader.siYsize));
			//	m_editCBX_Property[CBX_BG_HEIGHT].SetWindowText(strTemp2);
			//	GetEntity()->SetPropertyWithIndex(strTemp2, CBX_BG_HEIGHT);
			//}

			// id 글자수가 30자를 넘지 못하게 함
			CString tmpStr;           
			m_editCBX_Property[RESOURCE_ID].GetWindowText(tmpStr);
			if(tmpStr.GetLength() > ID_LENGTH)
			{
				AfxMessageBox(tmpStr + " ID 값이 30자를 초과했습니다.\n 다시 입력하세요!");
				tmpStr = tmpStr.Left( 30 );
				m_editCBX_Property[RESOURCE_ID].SetWindowText(tmpStr);
				m_bError = true;
			}

			GetParent()->RedrawWindow();
		}
	}	
}

void CComboBoxPropertyDlg::OnBnClickedButtonCbxSearch()
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

			m_editCBX_Property[CBX_BG_IMAGE_FULLPATH].SetWindowText( fileDlg.GetPathName() );
			
			if(m_spr.LoadSpr( (char *)(LPCTSTR)fileDlg.GetPathName().Trim(STR_TRIM_LETTER)))
                MakeMemDC(this->GetDC());

			if(m_oldPicFullPath != m_picFullPath)
				m_oldPicFullPath = m_picFullPath;

			//CRect rect(MINI_PICTURE_LEFT, MINI_PICTURE_TOP, MINI_PICTURE_LEFT + MINI_PICTURE_WIDTH, MINI_PICTURE_TOP + MINI_PICTURE_HEIGHT);
			////CStdGrfx::draw3dFrame(this->GetDC(), rect);
			//CStdGrfx::drawframed3dBox( this->GetDC(), rect );

			//BitBltDS( this->GetDC(), GetBufferDC(MASK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);
			//BitBltDS( this->GetDC(), GetBufferDC(REAL_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);

			CRect rect(0, 0, m_spr.clHeader.siXsize+2*PIXEL_OFFSET_X, m_spr.clHeader.siYsize+2*PIXEL_OFFSET_Y);

			CStdGrfx::drawframed3dBox( GetBufferDC(WORK_BACKGROUND_NUM), rect );

			BitBltDS( GetBufferDC(WORK_BACKGROUND_NUM), GetBufferDC(MASK_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCAND, 0, 0, PIXEL_OFFSET_X, PIXEL_OFFSET_Y);
			BitBltDS( GetBufferDC(WORK_BACKGROUND_NUM), GetBufferDC(REAL_BACKGROUND_NUM), m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, m_spr.clHeader.siXsize, m_spr.clHeader.siYsize, SRCPAINT, 0, 0, PIXEL_OFFSET_X, PIXEL_OFFSET_Y);
			BitBltDS( this->GetDC(), GetBufferDC(WORK_BACKGROUND_NUM), MINI_PICTURE_WIDTH, MINI_PICTURE_HEIGHT, m_spr.clHeader.siXsize + PIXEL_OFFSET_X*2, m_spr.clHeader.siYsize + PIXEL_OFFSET_Y*2, SRCCOPY, 0, 0, MINI_PICTURE_LEFT, MINI_PICTURE_TOP);

			CString strTmp;
			
			//strTmp.Format("%d", m_spr.clHeader.siXsize);
			//m_editCBX_Property[CBX_BG_WIDTH].SetWindowText(strTmp);
			//strTmp.Format("%d", m_spr.clHeader.siYsize);
			//m_editCBX_Property[CBX_BG_HEIGHT].SetWindowText(strTmp);

			// 입력한 이미지 파일 fullpath 파싱
			ParsingFileFullPath(fileDlg.GetPathName());

			CString tileMode = GetTileMode();
			CString tileImageNum = GetTileImageNum();

			if(tileMode.Trim() != STR_EMPTY)
			{
				if(tileMode.Trim() == STR_ZERO && _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM0)	// 통맵 모드
					m_editCBX_Property[CBX_BG_TILE].SetWindowText(STR_ZERO);
				else // "1"이면 가로 모드, "2"이면 세로 모드
					m_editCBX_Property[CBX_BG_TILE].SetWindowText(STR_ONE);
			}
			else // 디폴트는 통맵방식
				m_editCBX_Property[CBX_BG_TILE].SetWindowText(STR_ZERO);

			if(tileImageNum.Trim() != STR_EMPTY)
			{
				if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM0 || _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM1
					|| _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM3 || _ttoi(tileImageNum) == VALID_TILEIMAGE_NUM9)
				{
					strTmp.Format("%d", _ttoi(tileImageNum));
					m_editCBX_Property[CBX_BG_TILE_IMAGE_NUM].SetWindowText(strTmp);
				}
				else
				{
					strTmp = STR_ZERO;
					m_editCBX_Property[CBX_BG_TILE_IMAGE_NUM].SetWindowText(strTmp);
				}
			}

			//if(_ttoi(tileImageNum) < 2)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize);
			//	m_editCBX_Property[CBX_WIDTH].SetWindowText(strTmp);
			//	// Combo라 세로는 취소함
			//	//strTmp.Format("%d", m_spr.clHeader.siYsize);
			//	//m_editCBX_Property[CBX_HEIGHT].SetWindowText(strTmp);
			//}
			//else if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM3)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize * _ttoi(tileImageNum));
			//	m_editCBX_Property[CBX_WIDTH].SetWindowText(strTmp);
			//	// Combo라 세로는 취소함
			//	//strTmp.Format("%d", m_spr.clHeader.siYsize);
			//	//m_editCBX_Property[CBX_HEIGHT].SetWindowText(strTmp);
			//}
			//else if(_ttoi(tileImageNum) == VALID_TILEIMAGE_NUM9)
			//{
			//	strTmp.Format("%d", m_spr.clHeader.siXsize * 3);
			//	m_editCBX_Property[CBX_WIDTH].SetWindowText(strTmp);
			//	// Combo라 세로는 취소함
			//	//strTmp.Format("%d", m_spr.clHeader.siYsize * 3 );
			//	//m_editCBX_Property[CBX_HEIGHT].SetWindowText(strTmp);
			//}
			//// 크기에 맞춰.. surface 생성
			//m_editCBX_Property[CBX_WIDTH].GetWindowText(strTmp);
			//m_editCBX_Property[CBX_BG_WIDTH].SetWindowText(strTmp);
			//m_editCBX_Property[CBX_HEIGHT].GetWindowText(strTmp);
			//m_editCBX_Property[CBX_BG_HEIGHT].SetWindowText(strTmp);

			// 다이얼로그 크기 변화 없이 이미지 적용 시키기!!
			int sub_width = 0, sub_height = 0;
			m_editCBX_Property[CBX_WIDTH].GetWindowText(strTmp);
			sub_width = _ttoi(strTmp);
			m_editCBX_Property[CBX_HEIGHT].GetWindowText(strTmp);
			sub_height = _ttoi(strTmp);
            
			//strTmp.Format("%d", GetEntity()->SetToMultipleSize(sub_width, m_spr.clHeader.siXsize));
			strTmp.Format("%d", GetEntity()->SetToMultipleSize(sub_width, 1));
			// 크기에 맞춰.. surface 생성
			m_editCBX_Property[CBX_WIDTH].SetWindowText(strTmp);
			m_editCBX_Property[CBX_BG_WIDTH].SetWindowText(strTmp);

			//strTmp.Format("%d", GetEntity()->SetToMultipleSize(sub_height, m_spr.clHeader.siYsize));
			strTmp.Format("%d", GetEntity()->SetToMultipleSize(sub_height, 1));
			m_editCBX_Property[CBX_HEIGHT].SetWindowText(strTmp);
			m_editCBX_Property[CBX_BG_HEIGHT].SetWindowText(strTmp);

		}
	}
	
	//this->RedrawWindow();
	this->SendMessage( WM_PAINT );
}

void CComboBoxPropertyDlg::OnPaint()
{
	CDiagramPropertyDlg::OnPaint();
}

BOOL CComboBoxPropertyDlg::OnInitDialog()
{
	CDiagramPropertyDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CComboBoxPropertyDlg::OnBnClickedHide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}



void CComboBoxPropertyDlg::OnBnClickedButtonCbxPathEmpty()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_editCBX_Property[CBX_BG_IMAGE_FULLPATH].SetWindowText(STR_EMPTY);
	m_editCBX_Property[CBX_BG_TILE].SetWindowText(STR_ZERO);
	m_editCBX_Property[CBX_BG_TILE_IMAGE_NUM].SetWindowText(STR_ZERO);
	m_editCBX_Property[CBX_BG_START_FONT].SetWindowText(STR_ZERO);

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

void CComboBoxPropertyDlg::OnOK()
{
	OnButtonApply();
	if(m_bError == false)
		CDialog::OnOK();
}
void CComboBoxPropertyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDiagramPropertyDlg::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
