#ifndef _MYIO_H
#define _MYIO_H

void PopMapFileInitialize(HWND hwnd);
BOOL PopMapFileOpenDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName);
BOOL PopMapFileSaveDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName);

#endif