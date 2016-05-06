// comp_str.cpp --- composition string of mzimeja
//////////////////////////////////////////////////////////////////////////////

#include "mzimeja.h"

//////////////////////////////////////////////////////////////////////////////

DWORD LogCompStrExtra::GetTotalSize() const {
  DWORD total = sizeof(COMPSTREXTRA);
  for (size_t i = 0; i < hiragana_phonemes.size(); ++i) {
    total += (hiragana_phonemes[i].size() + 1) * sizeof(WCHAR);
  }
  for (size_t i = 0; i < typing_phonemes.size(); ++i) {
    total += (typing_phonemes[i].size() + 1) * sizeof(WCHAR);
  }
  total += phoneme_to_flags.size() * sizeof(BYTE);
  return total;
}

void LogCompStrExtra::clear() {
  FOOTMARK();
  iClause = 0;
  iPhoneme = 0;
  hiragana_phonemes.clear();
  typing_phonemes.clear();
  phoneme_to_flags.clear();
}

DWORD LogCompStrExtra::GetPhonemeCount() const {
  FOOTMARK();
  return (DWORD)hiragana_phonemes.size();
}

DWORD LogCompStrExtra::GetPhonemeCount2() const {
  FOOTMARK();
  return (DWORD)phoneme_to_flags.size();
}

void LogCompStrExtra::InsertThere(
  DWORD dwPhonemeIndex, std::vector<std::wstring>& strs, std::wstring& str)
{
  FOOTMARK();
  strs.insert(strs.begin() + dwPhonemeIndex, str.c_str());
}

void LogCompStrExtra::InsertPhoneme(
  DWORD dwPhonemeIndex, std::wstring& typed, std::wstring& translated)
{
  FOOTMARK();
  if (phoneme_to_flags.empty()) phoneme_to_flags.push_back(1);
  assert(dwPhonemeIndex < GetPhonemeCount2());
  InsertThere(dwPhonemeIndex, typing_phonemes, typed);
  InsertThere(dwPhonemeIndex, hiragana_phonemes, translated);
  if (phoneme_to_flags[dwPhonemeIndex] & 1) {
    phoneme_to_flags.insert(phoneme_to_flags.begin() + dwPhonemeIndex + 1, 0x00);
  } else {
    phoneme_to_flags.insert(phoneme_to_flags.begin() + dwPhonemeIndex, 0x00);
  }
}

WCHAR LogCompStrExtra::GetPrevChar() const {
  FOOTMARK();
  if (0 < iPhoneme && iPhoneme < GetPhonemeCount()) {
    std::wstring str = hiragana_phonemes[iPhoneme - 1];
    if (str.size()) {
      return str[str.size() - 1];
    }
  }
  return 0;
}

std::wstring
LogCompStrExtra::Join(const std::vector<std::wstring>& strs) const {
  FOOTMARK();
  std::wstring str;
  for (size_t i = 0; i < strs.size(); ++i) {
    str += strs[i].c_str();
  }
  return str;
}

//////////////////////////////////////////////////////////////////////////////

LPWSTR COMPSTREXTRA::GetHiraganaPhonemes(DWORD& dwPhonemeCount) {
  dwPhonemeCount = dwHiraganaPhonemeCount;
  if (dwPhonemeCount) {
    return (LPWSTR)(GetBytes() + dwHiraganaPhonemeOffset);
  }
  return NULL;
}

LPWSTR COMPSTREXTRA::GetTypingPhonemes(DWORD& dwPhonemeCount) {
  dwPhonemeCount = dwTypingPhonemeCount;
  if (dwPhonemeCount) {
    return (LPWSTR)(GetBytes() + dwTypingPhonemeOffset);
  }
  return NULL;
}

LPBYTE COMPSTREXTRA::GetPhonemeToFlags(DWORD& dwCount) {
  dwCount = dwPhonemeToFlagsCount;
  if (dwCount) {
    return GetBytes() + dwPhonemeToFlagsOffset;
  }
  return NULL;
}

void COMPSTREXTRA::GetLog(LogCompStrExtra& log) {
  FOOTMARK();
  log.clear();
  log.iClause = iClause;
  log.iPhoneme = iPhoneme;

  DWORD dwCount;
  LPWSTR pch = GetHiraganaPhonemes(dwCount);
  if (pch && dwCount) {
    while (dwCount--) {
      log.hiragana_phonemes.push_back(pch);
      pch += lstrlenW(pch) + 1;
    }
  }

  pch = GetTypingPhonemes(dwCount);
  if (pch && dwCount) {
    while (dwCount--) {
      log.typing_phonemes.push_back(pch);
      pch += lstrlenW(pch) + 1;
    }
  }

  LPBYTE pb = GetPhonemeToFlags(dwCount);
  if (pb && dwCount) {
    log.phoneme_to_flags.resize(dwCount);
    log.assign(pb, pb + dwCount);
  }
} // COMPSTREXTRA::GetLog

DWORD COMPSTREXTRA::Store(const LogCompStrExtra *log) {
  FOOTMARK();
  assert(this);
  assert(log);

  LPBYTE pb = GetBytes();
  dwSignature = 0xDEADFACE;
  iClause = log->iClause;
  iPhoneme = log->iPhoneme;
  pb += sizeof(COMPSTREXTRA);

  DWORD size;

  dwHiraganaPhonemeOffset = (DWORD)(pb - GetBytes());
  dwHiraganaPhonemeCount = 0;
  for (size_t i = 0; i < log->hiragana_phonemes.size(); ++i) {
    size = (log->hiragana_phonemes[i].size() + 1) * sizeof(WCHAR);
    memcpy(pb, &log->hiragana_phonemes[i][0], size);
    ++dwHiraganaPhonemeCount;
    pb += size;
  }

  dwTypingPhonemeOffset = (DWORD)(pb - GetBytes());
  dwTypingPhonemeCount = 0;
  for (size_t i = 0; i < log->typing_phonemes.size(); ++i) {
    size = (log->typing_phonemes[i].size() + 1) * sizeof(WCHAR);
    memcpy(pb, &log->typing_phonemes[i][0], size);
    ++dwTypingPhonemeCount;
    pb += size;
  }

  dwPhonemeToFlagsOffset = (DWORD)(pb - GetBytes());
  dwPhonemeToFlagsCount = (DWORD)log->phoneme_to_flags.size();
  memcpy(pb, &log->phoneme_to_flags[0], dwPhonemeToFlagsCount);
  pb += dwPhonemeToFlagsCount;

  assert(log->GetTotalSize() == (DWORD)(pb - GetBytes()));
  return (DWORD)(pb - GetBytes());
} // COMPSTREXTRA::Store

//////////////////////////////////////////////////////////////////////////////

DWORD LogCompStr::GetClauseCount() const {
  FOOTMARK();
  if (comp_clause.size() < 2) return 0;
  return (DWORD)(comp_clause.size() - 1);
}

BOOL LogCompStr::CompCharInClause(DWORD iCompChar, DWORD iClause) const {
  FOOTMARK();
  return iClause < GetClauseCount() && comp_clause[iClause] <= iCompChar &&
         iCompChar <= comp_clause[iClause + 1];
}

BYTE LogCompStr::PhonemeToFlags(DWORD dwPhonemeIndex) const {
  const DWORD cPhoneme = GetPhonemeCount();
  assert(dwPhonemeIndex <= cPhoneme);
  if (dwPhonemeIndex < cPhoneme)
    return extra.phoneme_to_flags[dwPhonemeIndex];
  if (dwPhonemeIndex == cPhoneme)
    return 1;
  return 0;
}

BYTE LogCompStr::GetClauseAttr(DWORD dwClauseIndex) const {
  FOOTMARK();
  BYTE ret = ATTR_INPUT;
  if (dwClauseIndex < GetClauseCount()) {
    DWORD ich = ClauseToCompChar(dwClauseIndex);
    ret = GetCompCharAttr(ich);
  }
  return ret;
}

void LogCompStr::SetClauseAttr(DWORD dwClauseIndex, BYTE attr) {
  FOOTMARK();
  if (dwClauseIndex + 1 <= GetClauseCount()) {
    DWORD ich = comp_clause[dwClauseIndex];
    DWORD ichEnd = comp_clause[dwClauseIndex + 1];
    for (DWORD i = ich; i < ichEnd; ++i) {
      comp_attr[i] = attr;
    }
  }
}

BOOL LogCompStr::IsClauseConverted(DWORD dwClauseIndex) const {
  return GetClauseAttr(dwClauseIndex) != ATTR_INPUT;
}

BOOL LogCompStr::HasClauseSelected() const {
  FOOTMARK();
  return IsClauseConverted(extra.iClause);
}

BYTE LogCompStr::GetCompCharAttr(DWORD ich) const {
  FOOTMARK();
  BYTE ret = ATTR_INPUT;
  if (ich < (DWORD)comp_attr.size()) {
    ret = comp_attr[ich];
  }
  return ret;
}

DWORD LogCompStr::ClauseToPhoneme(DWORD dwClauseIndex) const {
  FOOTMARK();
  if (dwClauseIndex >= GetClauseCount()) return GetPhonemeCount();
  DWORD iClause = 0;
  const DWORD cPhoneme = GetPhonemeCount();
  for (DWORD iPhoneme = 0; iPhoneme < cPhoneme; ++iPhoneme) {
    if (PhonemeToFlags(iPhoneme) & 1) {
      if (iClause == dwClauseIndex) return iPhoneme;
      ++iClause;
    }
  }
  return cPhoneme;
}

DWORD LogCompStr::PhonemeToClause(DWORD dwPhonemeIndex) const {
  FOOTMARK();
  if (dwPhonemeIndex >= GetPhonemeCount()) return GetClauseCount();
  DWORD iClause = 0;
  const DWORD cPhoneme = GetPhonemeCount();
  for (DWORD iPhoneme = 0; iPhoneme < cPhoneme; ++iPhoneme) {
    if (iPhoneme == dwPhonemeIndex) break;
    if (PhonemeToFlags(iPhoneme) & 1) ++iClause;
  }
  return iClause;
}

DWORD LogCompStr::ClauseToCompChar(DWORD dwClauseIndex) const {
  FOOTMARK();
  if (dwClauseIndex >= GetClauseCount()) return GetCompCharCount();
  return comp_clause[dwClauseIndex];
}

DWORD LogCompStr::CompCharToClause(DWORD iCompChar) const {
  FOOTMARK();
  DWORD dwClauseIndex = 0;
  const DWORD cClause = GetClauseCount();
  for (DWORD iClause = 0; iClause <= cClause; ++iClause) {
    if (comp_clause[iClause] <= iCompChar) {
      dwClauseIndex = iClause;
    } else {
      break;
    }
  }
  return dwClauseIndex;
}

void LogCompStr::clear_read() {
  FOOTMARK();
  comp_read_attr.clear();
  comp_read_clause.clear();
  comp_read_str.clear();
}

void LogCompStr::clear_comp() {
  FOOTMARK();
  dwCursorPos = 0;
  comp_attr.clear();
  comp_clause.clear();
  comp_str.clear();
}

void LogCompStr::clear() {
  FOOTMARK();
  clear_read();
  clear_comp();
  clear_result();
  clear_extra();
}

BOOL LogCompStr::IsBeingConverted() {
  FOOTMARK();
  for (size_t i = 0; i < comp_attr.size(); ++i) {
    if (comp_attr[i] != ATTR_INPUT) return TRUE;
  }
  return FALSE;
}

DWORD LogCompStr::GetCompCharCount() const {
  FOOTMARK();
  return (DWORD)comp_str.size();
}

void LogCompStr::clear_result() {
  FOOTMARK();
  result_read_clause.clear();
  result_read_str.clear();
  result_clause.clear();
  result_str.clear();
}

DWORD LogCompStr::GetTotalSize() const {
  FOOTMARK();
  size_t total = sizeof(COMPOSITIONSTRING);
  total += comp_read_attr.size() * sizeof(BYTE);
  total += comp_read_clause.size() * sizeof(DWORD);
  total += (comp_read_str.size() + 1) * sizeof(WCHAR);
  total += comp_attr.size() * sizeof(BYTE);
  total += comp_clause.size() * sizeof(DWORD);
  total += (comp_str.size() + 1) * sizeof(WCHAR);
  total += result_read_clause.size() * sizeof(DWORD);
  total += (result_read_str.size() + 1) * sizeof(WCHAR);
  total += result_clause.size() * sizeof(DWORD);
  total += (result_str.size() + 1) * sizeof(WCHAR);
  total += extra.GetTotalSize();
  return total;
}

DWORD LogCompStr::GetPhonemeCount() const {
  FOOTMARK();
  return extra.GetPhonemeCount();
}

std::wstring LogCompStr::GetClauseCompString(DWORD dwClauseIndex) const {
  FOOTMARK();
  std::wstring ret;
  if (dwClauseIndex + 1 <= GetClauseCount()) {
    DWORD ich0 = comp_clause[dwClauseIndex];
    DWORD ich1 = comp_clause[dwClauseIndex + 1];
    ret = comp_str.substr(ich0, ich1 - ich0);
  }
  return ret;
}

std::wstring LogCompStr::GetClauseHiraganaString(DWORD dwClauseIndex) const {
  FOOTMARK();
  std::wstring ret;
  if (dwClauseIndex + 1 <= GetClauseCount()) {
    DWORD iph0 = extra.clause_to_phoneme[dwClauseIndex];
    DWORD iph1 = extra.clause_to_phoneme[dwClauseIndex + 1];
    for (DWORD i = iph0; i < iph1; ++i) {
      ret += extra.hiragana_phonemes[i];
    }
  }
  return ret;
}

std::wstring LogCompStr::GetClauseTypingString(DWORD dwClauseIndex) const {
  FOOTMARK();
  std::wstring ret;
  if (dwClauseIndex + 1 <= GetClauseCount()) {
    DWORD iph0 = extra.clause_to_phoneme[dwClauseIndex];
    DWORD iph1 = extra.clause_to_phoneme[dwClauseIndex + 1];
    for (DWORD i = iph0; i < iph1; ++i) {
      ret += extra.typing_phonemes[i];
    }
  }
  return ret;
}

---

void LogCompStr::SetClauseCompString(
  DWORD dwClauseIndex, std::wstring& strNew, BOOL bConverted)
{
  FOOTMARK();
  if (dwClauseIndex >= GetClauseCount()) return;

  // replace old with new
  std::wstring strOld = GetClauseCompString(dwClauseIndex);
  std::wstring strLeft = GetLeft(dwClauseIndex);
  comp_str.replace(strLeft.size(), strOld.size(), strNew);

  // untarget
  for (size_t i = 0; i < comp_attr.size(); ++i) {
    if (comp_attr[i] == ATTR_TARGET_CONVERTED) comp_attr[i] = ATTR_CONVERTED;
  }

  // replace attr
  comp_attr.erase(
    comp_attr.begin() + strLeft.size(),
    comp_attr.begin() + strLeft.size() + strOld.size());
  comp_attr.insert(
    comp_attr.begin() + strLeft.size(), strNew.size(),
    (bConverted ? ATTR_TARGET_CONVERTED : ATTR_INPUT));

  // update clause
  if (strNew.size() > strOld.size()) {
    for (size_t i = dwClauseIndex + 1; i < comp_clause.size(); ++i) {
      comp_clause[i] += (DWORD)(strNew.size() - strOld.size());
    }
  } else {
    if (strNew.size() < strOld.size()) {
      for (size_t i = dwClauseIndex + 1; i < comp_clause.size(); ++i) {
        comp_clause[i] -= (DWORD)(strOld.size() - strNew.size());
      }
    }
  }

  if (bConverted) {
    dwCursorPos = (DWORD)comp_str.size();
    extra.iClause = dwClauseIndex;
  } else {
    dwCursorPos = (DWORD)(strLeft.size() + strNew.size());
    extra.iClause = GetClauseCount();
  }
} // LogCompStr::SetClauseCompString

std::wstring LogCompStr::GetLeft(DWORD dwClauseIndex) const {
  FOOTMARK();
  assert(dwClauseIndex < (DWORD)comp_clause.size());
  DWORD ich = comp_clause[dwClauseIndex];
  return comp_str.substr(0, ich);
}

std::wstring LogCompStr::GetRight(DWORD dwClauseIndex) const {
  FOOTMARK();
  assert(dwClauseIndex < (DWORD)comp_clause.size());
  DWORD ich = comp_clause[dwClauseIndex];
  return comp_str.substr(ich);
}

void LogCompStr::MakeHiragana() {
  FOOTMARK();
  std::wstring str = GetClauseHiraganaString(extra.iClause);
  SetClauseCompString(extra.iClause, str, TRUE);
}

void LogCompStr::MakeKatakana() {
  FOOTMARK();
  std::wstring str = GetClauseHiraganaString(extra.iClause);
  str = lcmap(str, LCMAP_KATAKANA);
  SetClauseCompString(extra.iClause, str, TRUE);
}

void LogCompStr::MakeHankaku() {
  FOOTMARK();
  std::wstring str = lcmap(GetClauseTypingString(extra.iClause),
                           LCMAP_HALFWIDTH | LCMAP_KATAKANA);
  SetClauseCompString(extra.iClause, str, TRUE);
}

void LogCompStr::MakeZenEisuu() {
  FOOTMARK();
  std::wstring str = GetClauseTypingString(extra.iClause);
  str = lcmap(str, LCMAP_FULLWIDTH);
  SetClauseCompString(extra.iClause, str, TRUE);
}

void LogCompStr::MakeHanEisuu() {
  FOOTMARK();
  std::wstring str = GetClauseTypingString(extra.iClause);
  str = lcmap(str, LCMAP_HALFWIDTH);
  SetClauseCompString(extra.iClause, str, TRUE);
}

void LogCompStr::MakeResult() {
  FOOTMARK();
  result_read_clause = comp_read_clause;  // TODO: generate reading
  result_read_str = comp_read_str;
  result_clause = comp_clause;
  result_str = comp_str;
  clear_read();
  clear_comp();
  clear_extra();
  dwCursorPos = 0;
}

void LogCompStr::MakeCompString(DWORD dwConversion) {
  // if no clause, create one
  if (GetClauseCount() == 0) {
    comp_clause.resize(2);
    comp_clause[0] = 0;
    comp_clause[1] = GetCompCharCount();
  }
  // extract and update composition clause strings
  std::vector<std::wstring> strs;
  DWORD i, count = GetClauseCount();
  for (i = 0; i < count; ++i) {
    if (GetClauseAttr(i) == ATTR_INPUT) {
      DWORD ich = ClauseToCompChar(i);
      DWORD ichNext = ClauseToCompChar(i + 1);
      std::wstring hiragana, typing;
      DWORD iph = CompCharToPhoneme(ich);
      DWORD iphNext = CompCharToPhoneme(ichNext);
      for (DWORD k = iph; k < iphNext; ++k) {
        hiragana += extra.hiragana_phonemes[k];
        typing += extra.typing_phonemes[k];
      }
      std::wstring str = Translate(hiragana, typing, dwConversion);
      SetClauseCompString(i, str, FALSE);
      strs.push_back(str);
    } else {
      strs.push_back(GetClauseCompString(i));
    }
  }
  // create composition string from extracted clause strings
  DWORD ich = 0;
  std::wstring str;
  for (i = 0; i < count; ++i) {
    comp_clause[i] = ich;
    ich += strs[i].size();
    str += strs[i];
  }
  comp_str = str;
  comp_clause[i] = GetCompCharCount();
  dwCursorPos = PhonemeToCompChar(extra.iPhoneme);
  // create the reading string
  str = extra.Join(extra.hiragana_phonemes);
  comp_read_str = lcmap(str, LCMAP_HALFWIDTH | LCMAP_KATAKANA);
} // LogCompStr::MakeCompString

void LogCompStr::AddKanaChar(
  std::wstring& typed, std::wstring& translated, DWORD dwConversion)
{
  FOOTMARK();
  // if there was dakuon combination
  WCHAR chDakuon = dakuon_shori(extra.GetPrevChar(), translated[0]);
  if (chDakuon) {
    // store a dakuon combination
    std::wstring str;
    str += chDakuon;
    extra.hiragana_phonemes[extra.iPhoneme - 1] = str;
    extra.typing_phonemes[extra.iPhoneme - 1] += L'\uFF9E'; // dakuon
    extra.phoneme_to_flags.insert(extra.phoneme_to_flags.begin() + extra.iPhoneme, 0x00);
  } else {  // if not dakuon
    // insert the typed and translated strings
    extra.InsertPhoneme(typed, translated);
    // move cursor
    ++dwCursorPos;
    ++extra.iPhoneme;
  }
  // create the composition string
  MakeCompString(dwConversion);
} // LogCompStr::AddKanaChar

void LogCompStr::AddRomanChar(
  std::wstring& typed, std::wstring& translated, DWORD dwConversion)
{
  FOOTMARK();
  const WCHAR chTyped = typed[0];
  // create the typed string and translated string
  if (is_hiragana(chTyped)) {
    // a hiragana character was directly typed. do reverse roman conversion
    translated = typed;
    typed = hiragana_to_roman(typed);
    // insert phonemes
    extra.InsertPhoneme(typed, translated);
    ++extra.iPhoneme;
  } else if (is_zenkaku_katakana(chTyped)) {
    // a katakana character was directly typed. make it hiragana
    translated = lcmap(typed, LCMAP_HIRAGANA);
    // do reverse roman conversion
    typed = hiragana_to_roman(translated);
    // insert phonemes
    extra.InsertPhoneme(typed, translated);
    ++extra.iPhoneme;
  } else if (is_kanji(chTyped)) {
    // a kanji (Chinese-like) character was directly typed
    translated = typed;
    // insert phonemes without conversion
    extra.InsertPhoneme(typed, translated);
    ++extra.iPhoneme;
  } else {
    // if a roman char or period was inputted
    if (std::isalnum(chTyped) || chTyped == L'\'') {
      // an alphabet or apostorophe was typed. get previous char
      WCHAR chPrev = extra.GetPrevChar();
      if (GetPhonemeCount() == 0) {  // there is no phoneme
        if (dwConversion & IME_CMODE_ROMAN) {
          // insert phonemes with roman conversion
          translated = roman_to_hiragana(typed);
          extra.InsertPhoneme(typed, translated);
          ++extra.iPhoneme;
        } else {
          translated = typed;
          extra.InsertPhoneme(typed, translated);
          ++extra.iPhoneme;
        }
      } else if (is_hiragana(chPrev) || chPrev == 0) {
        // if prev is hiragana or NUL, then
        translated = typed;
        extra.InsertPhoneme(typed, translated);
        ++extra.iPhoneme;
      } else {
        // get previous hiragana phoneme plus typed
        std::wstring str;
        str = extra.hiragana_phonemes[extra.iPhoneme - 1];
        str += typed;
        // do roman conversion
        std::wstring strConverted = roman_to_hiragana(str);
        // do pattern matching between str and strConverted
        std::wstring part;
        for (size_t i = 0; i < strConverted.size(); ++i) {
          if (str[i] == strConverted[i]) {
            part += str[i];
          } else {
            break;
          }
        }
        if (part.empty()) {
          // if no match, insert without conversion
          translated = typed;
          extra.InsertPhoneme(typed, translated);
          ++extra.iPhoneme;
        } else {
          // there was match
          if (part.size() > 5) {
            // too long matching. shrink part to split phoneme
            part = part.substr(0, 1);
          }
          // set part to phonemes
          extra.hiragana_phonemes[extra.iPhoneme - 1] = part;
          extra.typing_phonemes[extra.iPhoneme - 1] = part;
          // get remainder and insert it
          str = str.substr(part.size());
          strConverted = strConverted.substr(part.size());
          extra.InsertPhoneme(str, strConverted);
          ++extra.iPhoneme;
        }
      }
    } else {
      translated = typed;
      // insert phonemes without conversion
      extra.InsertPhoneme(typed, translated);
      ++extra.iPhoneme;
    }
  }
  // create the composition string
  MakeCompString(dwConversion);
} // LogCompStr::AddRomanChar

void LogCompStr::AddChar(WCHAR chTyped, WCHAR chTranslated, DWORD dwConversion) {
  FOOTMARK();
  if (extra.phoneme_to_flags.empty()) {
    extra.phoneme_to_flags.push_back(1);
  }
  std::wstring strTyped, strTranslated;
  if (chTranslated && !(dwConversion & IME_CMODE_ROMAN)) {   // kana input
    assert(is_hiragana(chTranslated));
    strTyped += chTyped;
    strTranslated += chTranslated;
    AddKanaChar(strTyped, strTranslated, dwConversion);
  } else {  // roman input
    strTyped += chTyped;
    strTranslated = strTyped;
    AddRomanChar(strTyped, strTranslated, dwConversion);
  }
} // LogCompStr::AddChar

void LogCompStr::RevertText() {
  FOOTMARK();
  // reset composition
  if (extra.iClause < GetClauseCount()) {
    // get a hiragana string of the current clause
    std::wstring str = GetClauseHiraganaString(extra.iClause);
    // set the hiragana string to the current clause
    SetClauseCompString(extra.iClause, str, FALSE);
    // set delta start
    dwDeltaStart = ClauseToCompChar(extra.iClause);
  }
}

void LogCompStr::ErasePhoneme(DWORD dwPhonemeIndex) {
  assert(dwPhonemeIndex < GetPhonemeCount());
  hiragana_phonemes.erase(hiragana_phonemes.begin() + dwPhonemeIndex);
  typing_phonemes.erase(typing_phonemes.begin() + dwPhonemeIndex);
  if (PhonemeToFlags(dwPhonemeIndex) & 1) {
    phoneme_to_flags.erase(phoneme_to_flags.begin() + dwPhonemeIndex);
  } else {
    phoneme_to_flags.erase(phoneme_to_flags.begin() + dwPhonemeIndex);
  }
}

void LogCompStr::DeleteChar(BOOL bRoman, BOOL bBackSpace/* = FALSE*/) {
  FOOTMARK();
  // is the current clause being converted?
  if (IsClauseConverted(extra.iClause) != ATTR_INPUT) { // being converted
    // set the hiragana string to the clause
    std::wstring str = GetClauseHiraganaString(extra.iClause);
    SetClauseCompString(extra.iClause, str, FALSE);
  } else {  // not being converted
    BOOL flag = FALSE;
    // is it back space?
    if (bBackSpace) { // back space
      if (0 < dwCursorPos && dwCursorPos <= GetCompCharCount()) {
        --dwCursorPos;  // move left
        extra.iPhoneme = CompCharToPhoneme(dwCursorPos);
        flag = TRUE;
      }
    } else {  // not back space
      if (0 <= dwCursorPos && dwCursorPos < GetCompCharCount()) {
        assert(extra.iPhoneme < GetPhonemeCount());
        flag = TRUE;
      }
    }
    if (flag) {
      // get the current phoneme string as str
      std::wstring& str = extra.hiragana_phonemes[extra.iPhoneme];
      if (str.size() <= 1) {
        extra.ErasePhoneme(extra.iPhoneme);  // erase phoneme
        comp_str.erase(dwCursorPos, 1);   // delete one char
      } else {
        comp_str.erase(dwCursorPos, 1);   // delete one char
        // scan and compare comp_str and str
        for (size_t i = 0; i < str.size(); ++i) {
          if (str[i] != comp_str[dwCursorPos + i]) {
            // erase one char of phoneme
            str.erase(i, 1);
            // generate typing phonemes
            if (bRoman) {
              extra.typing_phonemes[extra.iPhoneme] =
                hiragana_to_roman(str);
            } else {
              extra.typing_phonemes[extra.iPhoneme] =
                lcmap(str, LCMAP_HALFWIDTH | LCMAP_KATAKANA);
            }
            break;
          }
        }
      }
    }
  }
} // LogCompStr::DeleteChar

void LogCompStr::MoveLeft(BOOL bShift) {
  FOOTMARK();
  // TODO: bShift
  // is the current clause being converted?
  if (IsClauseConverted(extra.iClause)) { // being converted
    // untarget
    SetClauseAttr(extra.iClause, ATTR_CONVERTED);
    // set the current clause
    if (extra.iClause > 0) {
      --extra.iClause;
    } else {
      assert(comp_clause.size() >= 2);
      extra.iClause = GetClauseCount() - 1;
    }
    // retarget
    SetClauseAttr(extra.iClause, ATTR_TARGET_CONVERTED);
    // set the current phoneme
    extra.iPhoneme = ClauseToPhoneme(extra.iClause);
  } else {
    // move cursor
    if (dwCursorPos > 0) --dwCursorPos;
    // went out of clause?
    if (!CompCharInClause(dwCursorPos, extra.iClause)) {
      // untarget
      SetClauseAttr(extra.iClause, ATTR_CONVERTED);
      // set the current clause
      extra.iClause = CompCharToClause(dwCursorPos);
      // retarget
      SetClauseAttr(extra.iClause, ATTR_TARGET_CONVERTED);
    }
    // set the current phoneme
    extra.iPhoneme = CompCharToPhoneme(dwCursorPos);
  }
} // LogCompStr::MoveLeft

void LogCompStr::MoveRight(BOOL bShift) {
  FOOTMARK();
  // TODO: bShift
  // is the current clause being converted?
  if (IsClauseConverted(extra.iClause)) { // being converted
    // untarget
    SetClauseAttr(extra.iClause, ATTR_CONVERTED);
    // set current clause
    ++extra.iClause;
    if (extra.iClause >= GetClauseCount()) { // exceeded
      extra.iClause = 0;
    }
    // retarget
    SetClauseAttr(extra.iClause, ATTR_TARGET_CONVERTED);
    // set current phoneme
    extra.iPhoneme = ClauseToPhoneme(extra.iClause);
  } else {  // not being converted
    // move cursor
    if (dwCursorPos + 1 < GetCompCharCount()) {
      ++dwCursorPos;
    }
    // went out of clause?
    if (!CompCharInClause(dwCursorPos, extra.iClause)) {
      // untarget
      SetClauseAttr(extra.iClause, ATTR_CONVERTED);
      // set current clause
      extra.iClause = CompCharToClause(dwCursorPos);
      // retarget
      SetClauseAttr(extra.iClause, ATTR_TARGET_CONVERTED);
    }
    // set current phoneme
    extra.iPhoneme = CompCharToPhoneme(dwCursorPos);
  }
} // LogCompStr::MoveRight

std::wstring
LogCompStr::Translate(
  const std::wstring& hiragana, const std::wstring& typing,
  DWORD dwConversion)
{
  INPUT_MODE imode = InputModeFromConversionMode(TRUE, dwConversion);
  switch (imode) {
  case IMODE_ZEN_HIRAGANA:
    return lcmap(hiragana, LCMAP_HIRAGANA);
  case IMODE_ZEN_KATAKANA:
    return lcmap(hiragana, LCMAP_KATAKANA);
  case IMODE_ZEN_EISUU:
    return lcmap(typing, LCMAP_FULLWIDTH);
  case IMODE_HAN_KANA: case IMODE_HAN_EISUU:
    return lcmap(typing, LCMAP_HALFWIDTH | LCMAP_KATAKANA);
  default:
    break;
  }
  return typing;
} // LogCompStr::Transfer

//////////////////////////////////////////////////////////////////////////////

void CompStr::GetLog(LogCompStr& log) {
  FOOTMARK();
  log.dwCursorPos = dwCursorPos;
  log.dwDeltaStart = dwDeltaStart;
  log.comp_read_attr.assign(GetCompReadAttr(), GetCompReadAttr() + dwCompReadAttrLen);
  log.comp_read_clause.assign(GetCompReadClause(), GetCompReadClause() + dwCompReadClauseLen / sizeof(DWORD));
  log.comp_read_str.assign(GetCompReadStr(), dwCompReadStrLen);
  log.comp_attr.assign(GetCompAttr(), GetCompAttr() + dwCompAttrLen);
  log.comp_clause.assign(GetCompClause(), GetCompClause() + dwCompClauseLen / sizeof(DWORD));
  log.comp_str.assign(GetCompStr(), dwCompStrLen);
  log.result_read_clause.assign(GetResultReadClause(), GetResultReadClause() + dwResultReadClauseLen / sizeof(DWORD));
  log.result_read_str.assign(GetResultReadStr(), dwResultReadStrLen);
  log.result_clause.assign(GetResultClause(), GetResultClause() + dwResultClauseLen / sizeof(DWORD));
  log.result_str.assign(GetResultStr(), dwResultStrLen);
  COMPSTREXTRA *extra = GetExtra();
  if (extra && extra->dwSignature == 0xDEADFACE) {
    extra->GetLog(log.extra);
  } else {
    // TODO: do something
  }
}

/*static*/ HIMCC CompStr::ReCreate(HIMCC hCompStr, const LogCompStr *log) {
  FOOTMARK();
  LogCompStr log_comp_str;
  if (log == NULL) {
    log = &log_comp_str;
  }
  const DWORD total = log->GetTotalSize();
  HIMCC hNewCompStr = ::ImmReSizeIMCC(hCompStr, total);
  if (hNewCompStr) {
    CompStr *lpCompStr = (CompStr *)::ImmLockIMCC(hNewCompStr);
    if (lpCompStr) {
      LPBYTE pb = lpCompStr->GetBytes();
      lpCompStr->dwSize = total;
      lpCompStr->dwCursorPos = log->dwCursorPos;
      lpCompStr->dwDeltaStart = log->dwDeltaStart;
      pb += sizeof(COMPOSITIONSTRING);

#define ADD_BYTES(member) \
  if (log->member.size()) { \
    memcpy(pb, &log->member[0], log->member.size() * sizeof(BYTE)); \
    pb += log->member.size() * sizeof(BYTE); \
  }

#define ADD_DWORDS(member) \
  if (log->member.size()) { \
    memcpy(pb, &log->member[0], log->member.size() * sizeof(DWORD)); \
    pb += log->member.size() * sizeof(DWORD); \
  }

#define ADD_STRING(member) \
  if (log->member.size()) { \
    memcpy(pb, &log->member[0], (log->member.size() + 1) * sizeof(WCHAR)); \
    pb += (log->member.size() + 1) * sizeof(WCHAR); \
  } else { \
    pb[0] = pb[1] = 0; \
    pb += 1 * sizeof(WCHAR); \
  }

      lpCompStr->dwCompReadAttrOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwCompReadAttrLen = log->comp_read_attr.size() * sizeof(BYTE);
      ADD_BYTES(comp_read_attr);
      lpCompStr->dwCompReadClauseOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwCompReadClauseLen = log->comp_read_clause.size() * sizeof(DWORD);
      ADD_DWORDS(comp_read_clause);
      lpCompStr->dwCompReadStrOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwCompReadStrLen = log->comp_read_str.size();
      ADD_STRING(comp_read_str);
      lpCompStr->dwCompAttrOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwCompAttrLen = log->comp_attr.size() * sizeof(BYTE);
      ADD_BYTES(comp_attr);
      lpCompStr->dwCompClauseOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwCompClauseLen = log->comp_clause.size() * sizeof(DWORD);
      ADD_DWORDS(comp_clause);
      lpCompStr->dwCompStrOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwCompStrLen = log->comp_str.size();
      ADD_STRING(comp_str);
      lpCompStr->dwResultReadClauseOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwResultReadClauseLen = log->result_read_clause.size() * sizeof(DWORD);
      ADD_DWORDS(result_read_clause);
      lpCompStr->dwResultReadStrOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwResultReadStrLen = log->result_read_str.size();
      ADD_STRING(result_read_str);
      lpCompStr->dwResultClauseOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwResultClauseLen = log->result_clause.size() * sizeof(DWORD);
      ADD_DWORDS(result_clause);
      lpCompStr->dwResultStrOffset = (DWORD)(pb - lpCompStr->GetBytes());
      lpCompStr->dwResultStrLen = log->result_str.size();
      ADD_STRING(result_str);

      COMPSTREXTRA *extra = (COMPSTREXTRA *)pb;
      lpCompStr->dwPrivateSize = log->extra.GetTotalSize();
      lpCompStr->dwPrivateOffset = (DWORD)(pb - lpCompStr->GetBytes());
      pb += extra->Store(&log->extra);

#undef ADD_BYTES
#undef ADD_DWORDS
#undef ADD_STRING

      assert((DWORD)(pb - lpCompStr->GetBytes()) == total);

      ::ImmUnlockIMCC(hNewCompStr);
      hCompStr = hNewCompStr;
    } else {
      DebugPrint(TEXT("CompStr::ReCreate: failed #2"));
    }
  } else {
    DebugPrint(TEXT("CompStr::ReCreate: failed"));
  }
  return hCompStr;
} // CompStr::ReCreate

//////////////////////////////////////////////////////////////////////////////
// for debugging

bool LogCompStrExtra::IsValid() {
  bool ret = true;
  if (hiragana_phonemes.size() != typing_phonemes.size()) {
    FOOTMARK_PRINT_CALL_STACK();
    ret = false;
  }
  if (iPhoneme > GetPhonemeCount()) {
    FOOTMARK_PRINT_CALL_STACK();
    ret = false;
  }
  for (size_t i = 0; i < hiragana_phonemes.size(); ++i) {
    if (hiragana_phonemes[i].empty()) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    if (typing_phonemes[i].empty()) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    if (hiragana_phonemes[i].size() > 5) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    if (typing_phonemes[i].size() > 5) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
  }
  if (GetPhonemeCount() != GetPhonemeCount2()) {
    FOOTMARK_PRINT_CALL_STACK();
    ret = false;
  }
  return ret;
} // LogCompStrExtra::IsValid

bool LogCompStr::IsValid() {
  bool ret = true;
  if (dwCursorPos > GetCompCharCount()) {
    FOOTMARK_PRINT_CALL_STACK();
    ret = false;
  }
  if (comp_attr.size()) {
    if (comp_attr.size() != comp_str.size()) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
  }
  if (comp_clause.size()) {
    if (comp_clause[0] != 0) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    if (comp_clause[comp_clause.size() - 1] != GetCompCharCount()) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    if (extra.iClause > (DWORD)comp_clause.size()) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    for (size_t i = 1; i < comp_clause.size(); ++i) {
      if (comp_clause[i] > GetCompCharCount()) {
        FOOTMARK_PRINT_CALL_STACK();
        ret = false;
      }
      if (comp_clause[i - 1] > comp_clause[i]) {
        FOOTMARK_PRINT_CALL_STACK();
        ret = false;
      }
    }
  }
  if (result_read_clause.size()) {
    if (result_read_clause[0] != 0) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    if (result_read_clause[result_read_clause.size() - 1] != 0) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    for (size_t i = 1; i < result_read_clause.size(); ++i) {
      if (result_read_clause[i] > (DWORD)result_read_str.size()) {
        FOOTMARK_PRINT_CALL_STACK();
        ret = false;
      }
      if (result_read_clause[i - 1] > result_read_clause[i]) {
        FOOTMARK_PRINT_CALL_STACK();
        ret = false;
      }
    }
  }
  if (result_clause.size()) {
    if (result_clause[0] != 0) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    if (result_clause[result_clause.size() - 1] != 0) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
    for (size_t i = 1; i < result_clause.size(); ++i) {
      if (result_clause[i] > (DWORD)result_str.size()) {
        FOOTMARK_PRINT_CALL_STACK();
        ret = false;
      }
      if (result_clause[i - 1] > result_clause[i]) {
        FOOTMARK_PRINT_CALL_STACK();
        ret = false;
      }
    }
  }
  if (!extra.phoneme_to_flags.empty()) {
    if ((DWORD)extra.phoneme_to_flags.size() != GetPhonemeCount()) {
      FOOTMARK_PRINT_CALL_STACK();
      ret = false;
    }
  }
  if (ret) {
    ret = extra.IsValid();
  }
  if (!ret) {
    Dump();
  }
  return ret;
} // LogCompStr::IsValid

void CompStr::Dump() {
  FOOTMARK();
#ifndef NDEBUG
  DebugPrint(TEXT("dwSize: %08X\n"), dwSize);
  DebugPrint(TEXT("dwCursorPos: %08X\n"), dwCursorPos);
  DebugPrint(TEXT("dwDeltaStart: %08X\n"), dwDeltaStart);
  DebugPrint(TEXT("CompReadAttr: "));
  if (dwCompReadAttrLen) {
    LPBYTE attrs = GetCompReadAttr();
    for (DWORD i = 0; i < dwCompReadAttrLen; ++i) {
      DebugPrint(TEXT("%02X "), attrs[i]);
    }
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("CompReadClause: "));
  if (dwCompReadClauseLen) {
    LPDWORD clauses = GetCompReadClause();
    for (DWORD i = 0; i < dwCompReadClauseLen / 4; ++i) {
      DebugPrint(TEXT("%08X "), clauses[i]);
    }
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("CompReadStr: "));
  if (dwCompReadStrLen) {
    LPTSTR str = GetCompReadStr();
    DebugPrint(TEXT("%s"), str);
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("CompAttr: "));
  if (dwCompAttrLen) {
    LPBYTE attrs = GetCompAttr();
    for (DWORD i = 0; i < dwCompAttrLen; ++i) {
      DebugPrint(TEXT("%02X "), attrs[i]);
    }
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("CompClause: "));
  if (dwCompClauseLen) {
    LPDWORD clauses = GetCompClause();
    for (DWORD i = 0; i < dwCompClauseLen / 4; ++i) {
      DebugPrint(TEXT("%08X "), clauses[i]);
    }
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("CompStr: "));
  if (dwCompStrLen) {
    LPTSTR str = GetCompStr();
    DebugPrint(TEXT("%s"), str);
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("ResultReadClause: "));
  if (dwResultReadClauseLen) {
    LPDWORD clauses = GetResultReadClause();
    for (DWORD i = 0; i < dwResultReadClauseLen / 4; ++i) {
      DebugPrint(TEXT("%08X "), clauses[i]);
    }
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("ResultReadStr: "));
  if (dwResultReadStrLen) {
    LPTSTR str = GetResultReadStr();
    DebugPrint(TEXT("%s"), str);
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("ResultClause: "));
  if (dwResultClauseLen) {
    LPDWORD clauses = GetResultClause();
    for (DWORD i = 0; i < dwResultClauseLen / 4; ++i) {
      DebugPrint(TEXT("%08X "), clauses[i]);
    }
  }
  DebugPrint(TEXT("\n"));
  DebugPrint(TEXT("ResultStr: "));
  if (dwResultStrLen) {
    LPTSTR str = GetResultStr();
    DebugPrint(TEXT("%s"), str);
  }
  DebugPrint(TEXT("\n"));
#endif  // ndef NDEBUG
} // CompStr::Dump

void LogCompStr::Dump() {
#ifndef NDEBUG
  DebugPrint(TEXT("### LogCompStr ###\n"));
  DebugPrint(TEXT("+ dwCursorPos: %08X\n"), dwCursorPos);
  DebugPrint(TEXT("+ dwDeltaStart: %08X\n"), dwDeltaStart);

  DebugPrint(TEXT("+ comp_read_attr: "));
  for (size_t i = 0; i < comp_read_attr.size(); ++i) {
    DebugPrint(TEXT("%02X "), comp_read_attr[i]);
  }
  DebugPrint(TEXT("\n"));

  DebugPrint(TEXT("+ comp_read_clause: "));
  for (size_t i = 0; i < comp_read_clause.size(); ++i) {
    DebugPrint(TEXT("%08X "), comp_read_clause[i]);
  }
  DebugPrint(TEXT("\n"));

  DebugPrint(TEXT("+ comp_read_str: %ls\n"), comp_read_str.c_str());
  DebugPrint(TEXT("+ comp_attr: "));
  for (size_t i = 0; i < comp_attr.size(); ++i) {
    DebugPrint(TEXT("%02X "), comp_attr[i]);
  }
  DebugPrint(TEXT("\n"));

  DebugPrint(TEXT("+ comp_clause: "));
  for (size_t i = 0; i < comp_clause.size(); ++i) {
    DebugPrint(TEXT("%08X "), comp_clause[i]);
  }
  DebugPrint(TEXT("\n"));

  DebugPrint(TEXT("+ comp_str: %ls\n"), comp_str.c_str());

  DebugPrint(TEXT("+ result_read_clause: "));
  for (size_t i = 0; i < result_read_clause.size(); ++i) {
    DebugPrint(TEXT("%08X "), result_read_clause[i]);
  }
  DebugPrint(TEXT("\n"));

  DebugPrint(TEXT("+ result_read_str: %ls\n"), result_read_str.c_str());

  DebugPrint(TEXT("+ result_clause: "));
  for (size_t i = 0; i < result_clause.size(); ++i) {
    DebugPrint(TEXT("%08X "), result_clause[i]);
  }
  DebugPrint(TEXT("\n"));

  DebugPrint(TEXT("+ extra.iClause: %08X\n"), extra.iClause);
  DebugPrint(TEXT("+ extra.iPhoneme: %08X\n"), extra.iPhoneme);

  DebugPrint(TEXT("+ extra.hiragana_phonemes: "));
  for (size_t i = 0; i < extra.hiragana_phonemes.size(); ++i) {
    DebugPrint(TEXT("%ls "), extra.hiragana_phonemes[i].c_str());
  }
  DebugPrint(TEXT("\n"));

  DebugPrint(TEXT("+ extra.typing_phonemes: "));
  for (size_t i = 0; i < extra.typing_phonemes.size(); ++i) {
    DebugPrint(TEXT("%ls "), extra.typing_phonemes[i].c_str());
  }
  DebugPrint(TEXT("\n"));

  DebugPrint(TEXT("+ extra.phoneme_to_flags: "));
  for (size_t i = 0; i < extra.phoneme_to_flags.size(); ++i) {
    DebugPrint(TEXT("%02X "), extra.phoneme_to_flags[i]);
  }
  DebugPrint(TEXT("\n"));
#endif  // ndef NDEBUG
}

//////////////////////////////////////////////////////////////////////////////
