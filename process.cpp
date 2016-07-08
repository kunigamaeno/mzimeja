// process.cpp --- mzimeja input process
//////////////////////////////////////////////////////////////////////////////

#include "mzimeja.h"
#include "vksub.h"

#define IsCtrlPressed(x)  ((x)[VK_CONTROL] & 0x80)
#define IsShiftPressed(x) ((x)[VK_SHIFT] & 0x80)
#define IsAltPressed(x)   ((x)[VK_ALT] & 0x80)

extern "C" {

//////////////////////////////////////////////////////////////////////////////

// A function which handles WM_IME_KEYDOWN
BOOL IMEKeyDownHandler(HIMC hIMC, WPARAM wParam, BYTE *lpbKeyState,
                       INPUT_MODE imode) {
  FOOTMARK();
  InputContext *lpIMC;
  BYTE vk = (BYTE)wParam;

  // check open
  BOOL bOpen = FALSE;
  if (hIMC) {
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      bOpen = lpIMC->IsOpen();
      TheIME.UnlockIMC(hIMC);
    }
  }

  // check modifiers
  BOOL bAlt = lpbKeyState[VK_MENU] & 0x80;
  BOOL bShift = lpbKeyState[VK_SHIFT] & 0x80;
  BOOL bCtrl = lpbKeyState[VK_CONTROL] & 0x80;
  BOOL bCapsLock = lpbKeyState[VK_CAPITAL] & 0x80;
  BOOL bRoman = IsRomanMode(hIMC);

  // Is Ctrl down?
  if (bCtrl) {
    if (bOpen) {
      if (vk == VK_SPACE) {
        lpIMC = TheIME.LockIMC(hIMC);
        if (lpIMC) {
          if (lpIMC->HasCompStr()) {
            lpIMC->AddChar(L' ', L'\0');
          }
        }
        return TRUE;
      }
    }
    return FALSE;
  }

  // get translated char
  WCHAR chTranslated = 0;
  if (!bRoman) {
    chTranslated = vkey_to_hiragana(vk, bShift);
  }

  // get typed character
  WCHAR chTyped;
  if (vk == VK_PACKET) {
    chTyped = chTranslated = HIWORD(wParam);
  } else {
    chTyped = typing_key_to_char(vk, bShift, bCapsLock);
  }

  if (chTranslated || chTyped) {
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->AddChar(chTyped, chTranslated);
      TheIME.UnlockIMC(hIMC);
    }
    return TRUE;
  }

  switch (vk) {
  case VK_KANJI:
  case VK_OEM_AUTO:
  case VK_OEM_ENLW:
    if (bOpen) {
      SetInputMode(hIMC, IMODE_HALF_ASCII);
    } else {
      SetInputMode(hIMC, IMODE_FULL_KATAKANA);
    }
    break;

  case VK_KANA:
    if (bAlt) {
      SetRomanMode(hIMC, !IsRomanMode(hIMC));
      break;
    }
    switch (imode) {
    case IMODE_FULL_HIRAGANA:
      if (bShift) SetInputMode(hIMC, IMODE_FULL_KATAKANA);
      break;
    case IMODE_FULL_KATAKANA:
      if (!bShift) SetInputMode(hIMC, IMODE_FULL_HIRAGANA);
      break;
    case IMODE_FULL_ASCII:
    case IMODE_HALF_ASCII:
      if (bShift) {
        SetInputMode(hIMC, IMODE_FULL_KATAKANA);
      } else {
        SetInputMode(hIMC, IMODE_FULL_HIRAGANA);
      }
      break;
    case IMODE_HALF_KANA:
      if (!bShift) {
        SetInputMode(hIMC, IMODE_FULL_HIRAGANA);
      }
      break;
    default:
      break;
    }
    break;

  case VK_OEM_COPY:
    if (!bOpen) {
      ImmSetOpenStatus(hIMC, TRUE);
    }
    if (bAlt) {
      SetRomanMode(hIMC, !IsRomanMode(hIMC));
    } else if (bShift) {
      SetInputMode(hIMC, IMODE_FULL_KATAKANA);
    } else {
      SetInputMode(hIMC, IMODE_FULL_HIRAGANA);
    }
    break;

  case VK_ESCAPE:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->Escape();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_DELETE:
  case VK_BACK:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      if (lpIMC->HasCompStr()) {
        lpIMC->DeleteChar(vk == VK_BACK);
      } else {
        if (vk == VK_BACK) {
          TheIME.GenerateMessage(WM_IME_KEYDOWN, VK_BACK);
          TheIME.GenerateMessage(WM_IME_KEYUP, VK_BACK, 0x80000000);
        } else {
          TheIME.GenerateMessage(WM_IME_KEYDOWN, VK_DELETE);
          TheIME.GenerateMessage(WM_IME_KEYUP, VK_DELETE, 0x80000000);
        }
      }
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_SPACE:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      if (lpIMC->HasCompStr()) {
        lpIMC->Convert(bShift);
      } else {
        // add ideographic space
        TheIME.GenerateMessage(WM_IME_CHAR, L'�@', 1);
      }
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_CONVERT:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->Convert(bShift);
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_F6:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MakeHiragana();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_F7:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MakeKatakana();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_F8:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MakeHankaku();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_F9:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MakeZenEisuu();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_F10:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MakeHanEisuu();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_RETURN:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      if (lpIMC->Conversion() & IME_CMODE_CHARCODE) {
        // code input
        lpIMC->CancelText();
      } else {
        lpIMC->MakeResult();
      }
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_LEFT:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MoveLeft(bShift);
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_RIGHT:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MoveRight(bShift);
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_UP:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MoveUp();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_DOWN:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MoveDown();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_PRIOR: // Page Up
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->PageUp();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_NEXT: // Page Down
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->PageDown();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_HOME:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MoveHome();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  case VK_END:
    lpIMC = TheIME.LockIMC(hIMC);
    if (lpIMC) {
      lpIMC->MoveEnd();
      TheIME.UnlockIMC(hIMC);
    }
    break;

  default:
    return FALSE;
  }
  return TRUE;
} // IMEKeyDownHandler

BOOL WINAPI ImeProcessKey(HIMC hIMC, UINT vKey, LPARAM lKeyData,
                          CONST LPBYTE lpbKeyState) {
  BOOL ret = FALSE;
  FOOTMARK();

  BOOL bKeyUp = (lKeyData & 0x80000000);
  if (bKeyUp) {
    return FALSE;
  }

  InputContext *lpIMC = TheIME.LockIMC(hIMC);
  BOOL fOpen = lpIMC->IsOpen();
  BOOL fAlt = (lpbKeyState[VK_MENU] & 0x80);
  BOOL fCtrl = (lpbKeyState[VK_CONTROL] & 0x80);
  BOOL fShift = (lpbKeyState[VK_SHIFT] & 0x80);

  switch (vKey) {
  case VK_KANJI:
  case VK_OEM_AUTO:
  case VK_OEM_ENLW:
    if (!fShift && !fCtrl) ret = TRUE;
    break;
  case VK_KANA:
    ret = TRUE;
    break;
  default:
    if (lpIMC == NULL) return FALSE;
  }

  if (!ret && fOpen) {
    BOOL fCompStr = lpIMC->HasCompStr();
    BOOL fCandInfo = lpIMC->HasCandInfo();
    if (fAlt) {
      // Alt key is down
    } else if (fCtrl) {
      // Ctrl key is down
      if (fCompStr) {
        switch (vKey) {
        case VK_UP: case VK_DOWN:
        case VK_LEFT: case VK_RIGHT:
          // widely move
          ret = TRUE;
          break;
        }
      }
    } else if (fShift) {
      // Shift key is down
      switch (vKey) {
      case VK_LEFT: case VK_RIGHT:
      case VK_SPACE:
        ret = TRUE;
        break;
      }
    } else {
      // Neither Ctrl nor Shift key is down
      ret = FALSE;
      if (fCompStr) {
        switch (vKey) {
        case VK_F6:     // make composition fullwidth Hiragana
        case VK_F7:     // make composition fullwidth Katakana
        case VK_F8:     // make composition halfwidth Katakana
        case VK_F9:     // make composition fullwidth alphanumeric
        case VK_F10:    // make composition halfwidth alphanumeric
        case VK_ESCAPE: // close composition
          ret = TRUE;
          break;
        }
      }
      switch (vKey) {
      case VK_OEM_PLUS: case VK_OEM_MINUS: case VK_OEM_PERIOD:
      case VK_OEM_COMMA:
      case VK_OEM_1: case VK_OEM_2: case VK_OEM_3: case VK_OEM_4:
      case VK_OEM_5: case VK_OEM_6: case VK_OEM_7: case VK_OEM_8:
      case VK_OEM_9: case VK_OEM_102: case VK_OEM_COPY:
        // OEM keys
        ret = TRUE;
        break;
      case VK_ADD: case VK_SUBTRACT:
      case VK_MULTIPLY: case VK_DIVIDE:
      case VK_SEPARATOR: case VK_DECIMAL:
        // numpad keys
        ret = TRUE;
        break;
      case VK_HOME: case VK_END:
      case VK_UP: case VK_DOWN: case VK_LEFT: case VK_RIGHT:
        // arrow and moving key
        ret = TRUE;
        break;
      case VK_SPACE:
        if (!fCtrl || fCompStr) ret = TRUE;
        break;
      case VK_BACK: case VK_DELETE: case VK_RETURN:
      case VK_CAPITAL: case VK_CONVERT: case VK_NONCONVERT:
        // special keys
        ret = TRUE;
        break;
      default:
        if ('0' <= vKey && vKey <= '9') {
          // numbers
          ret = TRUE;
        } else if ('A' <= vKey && vKey <= 'Z') {
          // alphabets
          ret = TRUE;
        } else if (VK_NUMPAD0 <= vKey && vKey <= VK_NUMPAD9) {
          // numpad numbers
          ret = TRUE;
        } else {
          if (fCandInfo) {
            switch (vKey) {
            case VK_PRIOR: case VK_NEXT:
              // next or previous page of candidates
              ret = TRUE;
              break;
            }
          }
        }
      }
    }
  }

  TheIME.UnlockIMC(hIMC);
  return ret;
} // ImeProcessKey

UINT WINAPI ImeToAsciiEx(UINT uVKey, UINT uScanCode, CONST LPBYTE lpbKeyState,
                         LPTRANSMSGLIST lpTransBuf, UINT fuState, HIMC hIMC) {
  UINT ret = 0;
  FOOTMARK();

  TheIME.m_lpCurTransKey = lpTransBuf;
  TheIME.m_uNumTransKey = 0;
  BOOL bKeyUp = (uScanCode & 0x10000000);

  if (hIMC) {
    if (!bKeyUp) {
      INPUT_MODE imode = GetInputMode(hIMC);
      switch ((BYTE)uVKey) {
      case VK_KANJI: case VK_OEM_AUTO: case VK_OEM_ENLW:
        if (::ImmGetOpenStatus(hIMC)) {
          SetInputMode(hIMC, IMODE_HALF_ASCII);
        } else {
          SetInputMode(hIMC, IMODE_FULL_HIRAGANA);
        }
        break;
      case VK_KANA:
        ::ImmSetOpenStatus(hIMC, TRUE);
        if (lpbKeyState[VK_MENU] & 0x80) {
          SetRomanMode(hIMC, !IsRomanMode(hIMC));
        } else if (lpbKeyState[VK_SHIFT] & 0x80) {
          SetInputMode(hIMC, IMODE_FULL_KATAKANA);
        } else {
          SetInputMode(hIMC, IMODE_FULL_HIRAGANA);
        }
        break;
      default:
        IMEKeyDownHandler(hIMC, uVKey, lpbKeyState, imode);
        break;
      }
    }

    if (TheIME.m_fOverflowKey) {
      DebugPrint(TEXT("***************************************\n"));
      DebugPrint(TEXT("*   TransKey OVER FLOW Messages!!!    *\n"));
      DebugPrint(TEXT("*                by MZIMEJA.IME       *\n"));
      DebugPrint(TEXT("***************************************\n"));
    }
    ret = TheIME.m_uNumTransKey;
  }

  TheIME.m_lpCurTransKey = NULL;
  return ret;
} // ImeToAsciiEx

//////////////////////////////////////////////////////////////////////////////

}  // extern "C"

//////////////////////////////////////////////////////////////////////////////
