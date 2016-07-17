// mzimeja.h --- MZ-IME Japanese Input (mzimeja)
//////////////////////////////////////////////////////////////////////////////
// (Japanese, Shift_JIS)

#ifndef MZIMEJA_H_
#define MZIMEJA_H_

#define _CRT_SECURE_NO_WARNINGS   // use fopen

#include "targetver.h"      // target Windows version

#ifndef _INC_WINDOWS
  #include <windows.h>      // Windows
#endif
#include <commctrl.h>       // for Windows Common Controls
#include <tchar.h>          // for Windows generic text

#include <string>           // for std::string, std::wstring, ...
#include <vector>           // for std::vector
#include <set>              // for std::set
#include <map>              // for std::map
#include <algorithm>        // for std::sort

#include <cstdlib>          // for C standard library
#include <cstdio>           // for C standard I/O
#include <cctype>           // for C character types
#include <cassert>          // for assert
#include <cstring>          // for C string

#include <process.h>        // for _beginthreadex

#include "indicml.h"        // for system indicator
#include "immdev.h"         // for IME/IMM development
#include "input.h"          // for INPUT_MODE and InputContext

#define UNBOOST_USE_STRING_ALGORITHM
#define UNBOOST_USE_UNORDERED_MAP
#include "unboost.hpp"      // Unboost

//////////////////////////////////////////////////////////////////////////////
// _countof macro --- get the number of elements in an array

#ifndef _countof
  #define _countof(array)   (sizeof(array) / sizeof(array[0]))
#endif

//////////////////////////////////////////////////////////////////////////////
// for debugging

#ifdef NDEBUG
  #define DebugPrintA   (void)
  #define DebugPrintW   (void)
  #define DebugPrint    (void)
#else
  extern "C" {
    int DebugPrintA(const char *lpszFormat, ...);
    int DebugPrintW(const WCHAR *lpszFormat, ...);
  } // extern "C"
  #define DebugPrintA DebugPrintA
  #define DebugPrintW DebugPrintW
  #ifdef UNICODE
    #define DebugPrint DebugPrintW
  #else
    #define DebugPrint DebugPrintA
  #endif
#endif

#include "footmark.hpp"   // for FOOTMARK and FOOTMARK_PRINT_CALL_STACK

//////////////////////////////////////////////////////////////////////////////

// for limit of MZ-IME
#define MAXCOMPWND  10  // maximum number of composition windows
#define MAXGLCHAR   32  // maximum number of guideline characters

// special messages
#define WM_UI_UPDATE      (WM_USER + 500)
#define WM_UI_HIDE        (WM_USER + 501)

#define WM_UI_STATEMOVE   (WM_USER + 601)
#define WM_UI_DEFCOMPMOVE (WM_USER + 602)
#define WM_UI_CANDMOVE    (WM_USER + 603)
#define WM_UI_GUIDEMOVE   (WM_USER + 604)

// Escape Functions
#define IME_ESC_PRI_GETDWORDTEST (IME_ESC_PRIVATE_FIRST + 0)

// special style
#define WS_COMPDEFAULT    (WS_DISABLED | WS_POPUP)
#define WS_COMPNODEFAULT  (WS_DISABLED | WS_POPUP)

// ID of guideline table
#define MYGL_NODICTIONARY   0
#define MYGL_TYPINGERROR    1
#define MYGL_TESTGUIDELINE  2

// window extra of child UI windows
#define FIGWL_MOUSE 0
#define FIGWLP_SERVERWND        (FIGWL_MOUSE + sizeof(LONG))
#define FIGWLP_FONT             (FIGWLP_SERVERWND + sizeof(LONG_PTR))
#define FIGWL_COMPSTARTSTR      (FIGWLP_FONT + sizeof(LONG_PTR))
#define FIGWL_COMPSTARTNUM      (FIGWL_COMPSTARTSTR + sizeof(LONG))
#define FIGWLP_STATUSBMP        (FIGWL_COMPSTARTNUM + sizeof(LONG))
#define FIGWLP_CLOSEBMP         (FIGWLP_STATUSBMP + sizeof(LONG_PTR))
#define FIGWL_PUSHSTATUS        (FIGWLP_CLOSEBMP + sizeof(LONG_PTR))
#define FIGWLP_CHILDWND         (FIGWL_PUSHSTATUS + sizeof(LONG))
#define UIEXTRASIZE             (FIGWLP_CHILDWND + sizeof(LONG_PTR))

// The flags of FIGWL_MOUSE
#define FIM_CAPUTURED 0x01
#define FIM_MOVED 0x02

// The flags of the close button
#define PUSHED_STATUS_CLOSE 0x08

// Statue Close Button
#define STCLBT_X  (BTX * 2 + 3)
#define STCLBT_Y  1
#define STCLBT_DX 12
#define STCLBT_DY 12

//////////////////////////////////////////////////////////////////////////////
// additional GCS bits

#define GCS_COMPALL                                                  \
  (GCS_COMPSTR | GCS_COMPATTR | GCS_COMPREADSTR | GCS_COMPREADATTR | \
   GCS_COMPCLAUSE | GCS_COMPREADCLAUSE)

#define GCS_RESULTALL \
  (GCS_RESULTSTR | GCS_RESULTREADSTR | GCS_RESULTCLAUSE | GCS_RESULTREADCLAUSE)

//////////////////////////////////////////////////////////////////////////////
// Structures

// UICHILD
struct UICHILD {
  HWND hWnd;
  POINT pt;
};

// UICHILD2
struct UICHILD2 {
  HWND hWnd;
  RECT rc;
};

// UIEXTRA
struct UIEXTRA {
  HIMC      hIMC;
  HWND      hwndStatus;
  UICHILD   uiCand;
  DWORD     dwCompStyle;
  HFONT     hFont;
  BOOL      bVertical;
  HWND      hwndDefComp;
  UICHILD2  uiComp[MAXCOMPWND];
  HWND      hwndGuide;
};

// MZGUIDELINE
struct MZGUIDELINE {
  DWORD dwLevel;
  DWORD dwIndex;
  DWORD dwStrID;
  DWORD dwPrivateID;
};

//////////////////////////////////////////////////////////////////////////////
// externs

extern const WCHAR szUIServerClassName[];
extern const WCHAR szCompStrClassName[];
extern const WCHAR szCandClassName[];
extern const WCHAR szStatusClassName[];
extern const WCHAR szGuideClassName[];
extern const MZGUIDELINE glTable[];

//////////////////////////////////////////////////////////////////////////////

extern "C" {

// notify.c
BOOL PASCAL NotifyUCSetOpen(HIMC hIMC);
BOOL PASCAL NotifyUCConversionMode(HIMC hIMC);
BOOL PASCAL NotifyUCSetCompositionWindow(HIMC hIMC);

// ui.c
LRESULT CALLBACK MZIMEWndProc(HWND, UINT, WPARAM, LPARAM);
LONG NotifyCommand(HIMC hIMC, HWND hWnd, WPARAM wParam, LPARAM lParam);
LONG ControlCommand(HIMC hIMC, HWND hWnd, WPARAM wParam, LPARAM lParam);
void DrawUIBorder(LPRECT lprc);
void DragUI(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int GetCompFontHeight(UIEXTRA *lpUIExtra);
BOOL IsImeMessage(UINT message);
BOOL IsImeMessage2(UINT message);

// uistate.c
HWND StatusWnd_Create(HWND hWnd, UIEXTRA *lpUIExtra);
LRESULT CALLBACK StatusWnd_WindowProc(HWND, UINT, WPARAM, LPARAM);
void StatusWnd_Update(UIEXTRA *lpUIExtra);

// uicand.c
LRESULT CALLBACK CandWnd_WindowProc(HWND, UINT, WPARAM, LPARAM);
void CandWnd_Paint(HWND hCandWnd);
void CandWnd_Create(HWND hUIWnd, UIEXTRA *lpUIExtra, InputContext *lpIMC);
void CandWnd_Resize(UIEXTRA *lpUIExtra, InputContext *lpIMC);
void CandWnd_Hide(UIEXTRA *lpUIExtra);
void CandWnd_Move(HWND hUIWnd, InputContext *lpIMC, UIEXTRA *lpUIExtra,
                  BOOL fForceComp);

// uicomp.c
LRESULT CALLBACK CompWnd_WindowProc(HWND, UINT, WPARAM, LPARAM);
void CompWnd_Paint(HWND hCompWnd);
void CompWnd_Create(HWND hUIWnd, UIEXTRA *lpUIExtra, InputContext *lpIMC);
void CompWnd_Move(UIEXTRA *lpUIExtra, InputContext *lpIMC);
void CompWnd_Hide(UIEXTRA *lpUIExtra);
void CompWnd_SetFont(UIEXTRA *lpUIExtra);
HWND GetCandPosHintFromComp(UIEXTRA *lpUIExtra, InputContext *lpIMC,
                            DWORD iClause, LPPOINT ppt);

// uiguide.c
LRESULT CALLBACK GuideWnd_WindowProc(HWND, UINT, WPARAM, LPARAM);
void GuideWnd_Paint(HWND hGuideWnd, HDC hDC, LPPOINT lppt, DWORD dwPushedGuide);
void GuideWnd_Button(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void GuideWnd_Update(UIEXTRA *lpUIExtra);
LRESULT CALLBACK LineWndProc(HWND, UINT, WPARAM, LPARAM);

// config.c
INT_PTR CALLBACK RegWordDlgProc(HWND hDlg, UINT message, WPARAM wParam,
                                LPARAM lParam);
INT_PTR CALLBACK SelectDictionaryDlgProc(HWND hDlg, UINT message, WPARAM wParam,
                                         LPARAM lParam);
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam,
                              LPARAM lParam);
INT_PTR CALLBACK GeneralDlgProc(HWND hDlg, UINT message, WPARAM wParam,
                                LPARAM lParam);
INT_PTR CALLBACK DebugOptionDlgProc(HWND hDlg, UINT message, WPARAM wParam,
                                    LPARAM lParam);

// immsec.cpp
SECURITY_ATTRIBUTES *CreateSecurityAttributes(void);
void FreeSecurityAttributes(SECURITY_ATTRIBUTES *psa);
BOOL IsNT(void);

// mzimeja.cpp
void      RepositionWindow(HWND hWnd);
HFONT     CheckNativeCharset(HDC hDC);
HGLOBAL   GetUIExtraFromServerWnd(HWND hwndServer);
void      SetUIExtraToServerWnd(HWND hwndServer, HGLOBAL hUIExtra);
UIEXTRA * LockUIExtra(HWND hwndServer);
void      UnlockUIExtra(HWND hwndServer);
void      FreeUIExtra(HWND hwndServer);

//////////////////////////////////////////////////////////////////////////////

}  // extern "C"

//////////////////////////////////////////////////////////////////////////////
// keychar.cpp

// conversion between roman and hiragana
std::wstring hiragana_to_roman(std::wstring hiragana);
std::wstring roman_to_hiragana(std::wstring roman);
std::wstring roman_to_hiragana(std::wstring roman, size_t ichTarget);
// conversion between roman and katakana
std::wstring roman_to_katakana(std::wstring roman);
std::wstring roman_to_katakana(std::wstring roman, size_t ichTarget);
// conversion between roman and halfwidth katakana
std::wstring roman_to_halfwidth_katakana(std::wstring roman);
std::wstring roman_to_halfwidth_katakana(std::wstring roman, size_t ichTarget);

// character map for kana input
WCHAR vkey_to_hiragana(BYTE vk, BOOL bShift);
// character map for typing keys
WCHAR typing_key_to_char(BYTE vk, BOOL bShift, BOOL bCapsLock);
// dakuon (voiced consonant) processor
WCHAR dakuon_shori(WCHAR ch0, WCHAR ch1);
// locale-dependent string conversion
std::wstring lcmap(const std::wstring& str, DWORD dwFlags);
// convert hiragana to typing characters
std::wstring hiragana_to_typing(std::wstring hiragana);
// convert fullwidth ascii to halfwidth
std::wstring fullwidth_ascii_to_halfwidth(const std::wstring& str);

// is the character hiragana?
BOOL is_hiragana(WCHAR ch);
// is the character fullwidth katakana?
BOOL is_fullwidth_katakana(WCHAR ch);
// is the character halfwidth katakana?
BOOL is_halfwidth_katakana(WCHAR ch);
// is the character kanji?
BOOL is_kanji(WCHAR ch);
// is the character the education kanji?
BOOL is_education_kanji(WCHAR ch);
// is the character the common use kanji?
BOOL is_common_use_kanji(WCHAR ch);
// is the character fullwidth ASCII?
BOOL is_fullwidth_ascii(WCHAR ch);
// is the character a period?
BOOL is_period(WCHAR ch);
// is the character a comma?
BOOL is_comma(WCHAR ch);

//////////////////////////////////////////////////////////////////////////////

struct MzConversionCandidate {
  std::wstring hiragana;
  std::wstring converted;
  void clear() {
    hiragana.clear();
    converted.clear();
  }
};

struct MzConversionClause {
  std::vector<MzConversionCandidate> candidates;
  void clear() { candidates.clear(); }
};

struct MzConversionResult {
  std::vector<MzConversionClause> clauses;
  void clear() { clauses.clear(); }
};

//////////////////////////////////////////////////////////////////////////////

struct IMAGE_BASE {
  DWORD   dwSignature;
  DWORD   dwSharedDictDataSize;
};

enum GYOU {
  GYOU_A,
  GYOU_KA,
  GYOU_GA,
  GYOU_SA,
  GYOU_ZA,
  GYOU_TA,
  GYOU_DA,
  GYOU_NA,
  GYOU_HA,
  GYOU_BA,
  GYOU_PA,
  GYOU_MA,
  GYOU_YA,
  GYOU_RA,
  GYOU_WA,
  GYOU_NN
};

enum DAN {
  DAN_A,
  DAN_I,
  DAN_U,
  DAN_E,
  DAN_O
};

enum HINSHI_BUNRUI {
  HB_START_NODE,        // �J�n�m�[�h
  HB_MEISHI,            // ����
  HB_IKEIYOUSHI,        // ���`�e��
  HB_NAKEIYOUSHI,       // �Ȍ`�e��
  HB_RENTAISHI,         // �A�̎�
  HB_FUKUSHI,           // ����
  HB_SETSUZOKUSHI,      // �ڑ���
  HB_KANDOUSHI,         // ������
  HB_JOSHI,             // ����
  HB_MIZEN_JODOUSHI,    // ���R������
  HB_RENYOU_JODOUSHI,   // �A�p������
  HB_SHUUSHI_JODOUSHI,  // �I�~������
  HB_RENTAI_JODOUSHI,   // �A�̏�����
  HB_KATEI_JODOUSHI,    // ���菕����
  HB_MEIREI_JODOUSHI,   // ���ߏ�����
  HB_GODAN_DOUSHI,      // �ܒi����
  HB_ICHIDAN_DOUSHI,    // ��i����
  HB_KAHEN_DOUSHI,      // �J�ϓ���
  HB_SAHEN_DOUSHI,      // �T�ϓ���
  HB_KANGO,             // ����
  HB_SETTOUGO,          // �ړ���
  HB_SETSUBIGO,         // �ڔ���
  HB_END_NODE           // �I���m�[�h
}; // enum HINSHI_BUNRUI

enum KATSUYOU_KEI {
  MIZEN_KEI,            // ���R�`
  RENYOU_KEI,           // �A�p�`
  SHUUSHI_KEI,          // �I�~�`
  RENTAI_KEI,           // �A�̌`
  KATEI_KEI,            // ����`
  MEIREI_KEI,           // ���ߌ`
  MEISHI_KEI            // �����`
};

struct DICT_ENTRY {
  std::wstring  pre;
  HINSHI_BUNRUI bunrui;
  std::wstring  post;
  std::wstring  tags;
  GYOU          gyou;
};

//////////////////////////////////////////////////////////////////////////////
// The IME

class MZIMEJA {
public:
  HINSTANCE       m_hInst;
  HKL             m_hMyKL;
  BOOL            m_bWinLogOn;
  LPTRANSMSGLIST  m_lpCurTransKey;
  UINT            m_uNumTransKey;
  BOOL            m_fOverflowKey;

public:
  MZIMEJA();

  // initialize the IME
  BOOL Init(HINSTANCE hInstance);

  // register classes
  BOOL RegisterClasses(HINSTANCE hInstance);
  void UnregisterClasses();

  // uninitialize
  VOID Uninit(VOID);

  // load a bitmap from resource
  HBITMAP LoadBMP(LPCTSTR pszName);
  HBITMAP LoadBMP(UINT nID) {
    return LoadBMP(MAKEINTRESOURCE(nID));
  }

  // load a string from resource
  WCHAR *LoadSTR(INT nID);

  // update the indicator icon
  void UpdateIndicIcon(HIMC hIMC);

  // get the keyboard layout handle
  HKL GetHKL(VOID);

  // lock the input context
  InputContext *LockIMC(HIMC hIMC);
  // unlock the input context
  VOID UnlockIMC(HIMC hIMC);

  // generate a message
  BOOL GenerateMessage(LPTRANSMSG lpGeneMsg);
  BOOL GenerateMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
  BOOL GenerateMessageToTransKey(LPTRANSMSG lpGeneMsg);

  // do command
  BOOL DoCommand(HIMC hIMC, DWORD dwCommand);

  // basic dictionary
  BOOL LoadBasicDict();
  BOOL IsBasicDictLoaded() const;
  WCHAR *LockBasicDict();
  void UnlockBasicDict(WCHAR *data);

  // convert
  void PluralClauseConversion(LogCompStr& comp, LogCandInfo& cand, BOOL bRoman);
  void PluralClauseConversion(const std::wstring& strHiragana,
                              MzConversionResult& result);
  void SingleClauseConversion(LogCompStr& comp, LogCandInfo& cand, BOOL bRoman);
  void SingleClauseConversion(const std::wstring& strHiragana,
                              MzConversionClause& result);
  BOOL StretchClauseLeft(LogCompStr& comp, LogCandInfo& cand, BOOL bRoman);
  BOOL StretchClauseRight(LogCompStr& comp, LogCandInfo& cand, BOOL bRoman);

  // computer settings
  std::wstring GetComputerString(LPCWSTR pszSettingName);
  BOOL SetComputerString(LPCWSTR pszSettingName, LPCWSTR pszValue);
  BOOL GetComputerDword(LPCWSTR pszSettingName, DWORD *ptr);
  BOOL SetComputerDword(LPCWSTR pszSettingName, DWORD data);
  BOOL GetComputerData(LPCWSTR pszSettingName, void *ptr, DWORD size);
  BOOL SetComputerData(LPCWSTR pszSettingName, const void *ptr, DWORD size);

  // user settings
  std::wstring GetUserString(LPCWSTR pszSettingName);
  BOOL SetUserString(LPCWSTR pszSettingName, LPCWSTR pszValue);
  BOOL GetUserDword(LPCWSTR pszSettingName, DWORD *ptr);
  BOOL SetUserDword(LPCWSTR pszSettingName, DWORD data);
  BOOL GetUserData(LPCWSTR pszSettingName, void *ptr, DWORD size);
  BOOL SetUserData(LPCWSTR pszSettingName, const void *ptr, DWORD size);

protected:
  HANDLE          m_hMutex;         // mutex
  HANDLE          m_hBaseData;      // file mapping
  BOOL LoadBasicDictFile(std::vector<DICT_ENTRY>& entries);
  BOOL DeployDictData(IMAGE_BASE *data, SECURITY_ATTRIBUTES *psa,
                      const std::vector<DICT_ENTRY>& entries);

  // input context
  HIMC            m_hIMC;
  InputContext *  m_lpIMC;

  HANDLE          m_hBasicDictData; // file mapping
  IMAGE_BASE *LockImeBaseData();
  void UnlockImeBaseData(IMAGE_BASE *data);

  // literal map
  unboost::unordered_map<wchar_t,wchar_t>   m_vowel_map;
  unboost::unordered_map<wchar_t,wchar_t>   m_consonant_map;
  void MakeLiteralMaps();

  // registry
  LONG OpenRegKey(HKEY hKey, LPCWSTR pszSubKey, BOOL bWrite, HKEY *phSubKey) const;
  LONG CreateRegKey(HKEY hKey, LPCWSTR pszSubKey, HKEY *phSubKey);
  LONG OpenComputerSettingKey(BOOL bWrite, HKEY *phKey);
  LONG OpenUserSettingKey(BOOL bWrite, HKEY *phKey);
}; // class MZIMEJA

extern MZIMEJA TheIME;

//////////////////////////////////////////////////////////////////////////////

#endif  // ndef MZIMEJA_H_

//////////////////////////////////////////////////////////////////////////////
