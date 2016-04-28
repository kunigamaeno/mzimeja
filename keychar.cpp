// keychar.cpp
//////////////////////////////////////////////////////////////////////////////

#include "mzimeja.h"
#include "vksub.h"

#define UNBOOST_USE_STRING_ALGORITHM
#include "unboost.hpp"

//////////////////////////////////////////////////////////////////////////////

struct TABLEENTRY {
  LPCWSTR key;
  LPCWSTR value;
  LPCWSTR extra;
};

static TABLEENTRY halfkana_table[] = {
// �y�~���Ƀ\�[�g�z��������
  {L"�[", L"-"},
  {L"�E", L"�"},
  {L"�K", L"�"},
  {L"�J", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"��"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"���J", L"��"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"��", L"�"},
  {L"�v", L"�"},
  {L"�u", L"�"},
  {L"�B", L"�"},
  {L"�A", L"�"},
// �y�~���Ƀ\�[�g�z�����܂�
};

static TABLEENTRY kana_table[] = {
// �y�~���Ƀ\�[�g�z��������
  {L"�", L"�K"},
  {L"�", L"�J"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"�["},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"��"},
  {L"�", L"�E"},
  {L"�", L"�A"},
  {L"�", L"�v"},
  {L"�", L"�u"},
  {L"�", L"�B"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
  {L"���", L"��"},
// �y�~���Ƀ\�[�g�z�����܂�
};

static TABLEENTRY kigou_table[] = {
// �y�~���Ƀ\�[�g�z��������
  {L"~", L"�`"},
  {L"}", L"�p"},
  {L"|", L"�b"},
  {L"{", L"�o"},
  {L"`", L"�M"},
  {L"_", L"�Q"},
  {L"^", L"�O"},
  {L"]", L"�v"},
  {L"\\", L"��"},
  {L"[", L"�u"},
  {L"@", L"��"},
  {L"?", L"�H"},
  {L">", L"��"},
  {L"=", L"��"},
  {L"<", L"��"},
  {L";", L"�G"},
  {L":", L"�F"},
  {L"/", L"�E"},
  {L".", L"�B"},
  {L"-", L"�["},
  {L",", L"�A"},
  {L"+", L"�{"},
  {L"*", L"��"},
  {L")", L"�j"},
  {L"(", L"�i"},
  {L"'", L"�f"},
  {L"&", L"��"},
  {L"%", L"��"},
  {L"$", L"��"},
  {L"#", L"��"},
  {L"\"", L"��"},
  {L"!", L"�I"},
// �y�~���Ƀ\�[�g�z�����܂�
};

static TABLEENTRY reverse_table[] = {
// �y�~���Ƀ\�[�g�z��������
  {L"�`", L"~"},
  {L"�[", L"-"},
  {L"��", L"n'o"},
  {L"��", L"n'e"},
  {L"��", L"n'u"},
  {L"��", L"n'i"},
  {L"��", L"n'a"},
  {L"��", L"n"},
  {L"��", L"wo"},
  {L"��", L"we"},
  {L"��", L"wi"},
  {L"��", L"wa"},
  {L"��", L"xwa"},
  {L"��", L"ro"},
  {L"��", L"re"},
  {L"��", L"ru"},
  {L"���", L"ryo"},
  {L"���", L"ryu"},
  {L"���", L"rya"},
  {L"�肥", L"rye"},
  {L"�股", L"ryi"},
  {L"��", L"ri"},
  {L"��", L"ra"},
  {L"��", L"yo"},
  {L"��", L"xyo"},
  {L"��", L"yu"},
  {L"��", L"xyu"},
  {L"��", L"ya"},
  {L"��", L"xya"},
  {L"��", L"mo"},
  {L"��", L"me"},
  {L"��", L"mu"},
  {L"�݂�", L"myo"},
  {L"�݂�", L"myu"},
  {L"�݂�", L"mya"},
  {L"�݂�", L"mye"},
  {L"�݂�", L"myi"},
  {L"��", L"mi"},
  {L"��", L"ma"},
  {L"��", L"po"},
  {L"��", L"bo"},
  {L"��", L"ho"},
  {L"��", L"pe"},
  {L"��", L"be"},
  {L"��", L"he"},
  {L"��", L"pu"},
  {L"��", L"bu"},
  {L"��", L"hu"},
  {L"�҂�", L"pyo"},
  {L"�҂�", L"pyu"},
  {L"�҂�", L"pya"},
  {L"�҂�", L"pye"},
  {L"�҂�", L"pyi"},
  {L"��", L"pi"},
  {L"�т�", L"byo"},
  {L"�т�", L"byu"},
  {L"�т�", L"bya"},
  {L"�т�", L"bye"},
  {L"�т�", L"byi"},
  {L"��", L"bi"},
  {L"�Ђ�", L"hyo"},
  {L"�Ђ�", L"hyu"},
  {L"�Ђ�", L"hya"},
  {L"�Ђ�", L"hye"},
  {L"�Ђ�", L"hyi"},
  {L"��", L"hi"},
  {L"��", L"pa"},
  {L"��", L"ba"},
  {L"��", L"ha"},
  {L"��", L"no"},
  {L"��", L"ne"},
  {L"��", L"nu"},
  {L"�ɂ�", L"nyo"},
  {L"�ɂ�", L"nyu"},
  {L"�ɂ�", L"nya"},
  {L"�ɂ�", L"nye"},
  {L"�ɂ�", L"nyi"},
  {L"��", L"ni"},
  {L"��", L"na"},
  {L"��", L"do"},
  {L"��", L"to"},
  {L"�ł�", L"dhi"},
  {L"��", L"de"},
  {L"��", L"te"},
  {L"��", L"du"},
  {L"��", L"tu"},
  {L"����", L"wwo"},
  {L"����", L"wwe"},
  {L"����", L"wwi"},
  {L"����", L"wwa"},
  {L"����", L"xxwa"},
  {L"����", L"rro"},
  {L"����", L"rre"},
  {L"����", L"rru"},
  {L"����", L"r", L"��"},
  {L"����", L"rra"},
  {L"����", L"yyo"},
  {L"����", L"xxyo"},
  {L"����", L"yyu"},
  {L"����", L"xxyu"},
  {L"����", L"yya"},
  {L"����", L"xxya"},
  {L"����", L"mmo"},
  {L"����", L"mme"},
  {L"����", L"mmu"},
  {L"����", L"m", L"��"},
  {L"����", L"mma"},
  {L"����", L"ppo"},
  {L"����", L"bbo"},
  {L"����", L"hho"},
  {L"����", L"ppe"},
  {L"����", L"bbe"},
  {L"����", L"hhe"},
  {L"����", L"ppu"},
  {L"����", L"bbu"},
  {L"����", L"hhu"},
  {L"����", L"p", L"��"},
  {L"����", L"b", L"��"},
  {L"����", L"h", L"��"},
  {L"����", L"ppa"},
  {L"����", L"bba"},
  {L"����", L"hha"},
  {L"����", L"ddo"},
  {L"����", L"tto"},
  {L"����", L"dde"},
  {L"����", L"tte"},
  {L"����", L"ddu"},
  {L"����", L"ttu"},
  {L"����", L"d", L"��"},
  {L"����", L"t", L"��"},
  {L"����", L"dda"},
  {L"����", L"tta"},
  {L"����", L"zzo"},
  {L"����", L"sso"},
  {L"����", L"zze"},
  {L"����", L"sse"},
  {L"����", L"zzu"},
  {L"����", L"ssu"},
  {L"����", L"z", L"��"},
  {L"����", L"s", L"��"},
  {L"����", L"zza"},
  {L"����", L"ssa"},
  {L"����", L"ggo"},
  {L"����", L"kko"},
  {L"����", L"gge"},
  {L"����", L"kke"},
  {L"����", L"ggu"},
  {L"����", L"kku"},
  {L"����", L"g", L"��"},
  {L"����", L"k", L"��"},
  {L"����", L"gga"},
  {L"����", L"kka"},
  {L"����", L"xxo"},
  {L"����", L"xxe"},
  {L"�����J", L"v", L"���J"},
  {L"����", L"xxu"},
  {L"����", L"xxi"},
  {L"����", L"xxa"},
  {L"��", L"xtu"},
  {L"����", L"dyo"},
  {L"����", L"dyu"},
  {L"����", L"dya"},
  {L"����", L"dye"},
  {L"����", L"dyi"},
  {L"��", L"di"},
  {L"����", L"tyo"},
  {L"����", L"tyu"},
  {L"����", L"tya"},
  {L"����", L"tye"},
  {L"����", L"tyi"},
  {L"��", L"ti"},
  {L"��", L"da"},
  {L"��", L"ta"},
  {L"��", L"zo"},
  {L"��", L"so"},
  {L"��", L"ze"},
  {L"��", L"se"},
  {L"��", L"zu"},
  {L"��", L"su"},
  {L"����", L"zyo"},
  {L"����", L"zyu"},
  {L"����", L"zya"},
  {L"����", L"zye"},
  {L"����", L"zyi"},
  {L"��", L"zi"},
  {L"����", L"syo"},
  {L"����", L"syu"},
  {L"����", L"sya"},
  {L"����", L"sye"},
  {L"����", L"syi"},
  {L"��", L"si"},
  {L"��", L"za"},
  {L"��", L"sa"},
  {L"��", L"go"},
  {L"��", L"ko"},
  {L"��", L"ge"},
  {L"��", L"ke"},
  {L"��", L"gu"},
  {L"��", L"ku"},
  {L"����", L"gyo"},
  {L"����", L"gyu"},
  {L"����", L"gya"},
  {L"����", L"gye"},
  {L"����", L"gyi"},
  {L"��", L"gi"},
  {L"����", L"kyo"},
  {L"����", L"kyu"},
  {L"����", L"kya"},
  {L"����", L"kye"},
  {L"����", L"kyi"},
  {L"��", L"ki"},
  {L"��", L"ga"},
  {L"��", L"ka"},
  {L"��", L"o"},
  {L"��", L"xo"},
  {L"��", L"e"},
  {L"��", L"xe"},
  {L"���J��", L"vo"},
  {L"���J��", L"ve"},
  {L"���J��", L"vi"},
  {L"���J��", L"va"},
  {L"���J", L"vu"},
  {L"��", L"u"},
  {L"��", L"xu"},
  {L"��", L"i"},
  {L"��", L"xi"},
  {L"��", L"a"},
  {L"��", L"xa"},
  {L"�v", L"]"},
  {L"�u", L"["},
// �y�~���Ƀ\�[�g�z�����܂�
};

static TABLEENTRY romaji_table[] = {
// �y�~���Ƀ\�[�g�z��������
  {L"zyu", L"����"},
  {L"zyo", L"����"},
  {L"zyi", L"����"},
  {L"zye", L"����"},
  {L"zya", L"����"},
  {L"zu", L"��"},
  {L"zo", L"��"},
  {L"zi", L"��"},
  {L"ze", L"��"},
  {L"za", L"��"},
  {L"yu", L"��"},
  {L"yo", L"��"},
  {L"yi", L"��"},
  {L"ye", L"����"},
  {L"ya", L"��"},
  {L"xyu", L"��"},
  {L"xyo", L"��"},
  {L"xyi", L"��"},
  {L"xye", L"��"},
  {L"xya", L"��"},
  {L"xwa", L"��"},
  {L"xu", L"��"},
  {L"xtu", L"��"},
  {L"xo", L"��"},
  {L"xn", L"��"},
  {L"xke", L"��"},
  {L"xka", L"��"},
  {L"xi", L"��"},
  {L"xe", L"��"},
  {L"xa", L"��"},
  {L"wu", L"��"},
  {L"wo", L"��"},
  {L"wi", L"����"},
  {L"whu", L"��"},
  {L"who", L"����"},
  {L"whi", L"����"},
  {L"whe", L"����"},
  {L"wha", L"����"},
  {L"we", L"����"},
  {L"wa", L"��"},
  {L"vyu", L"����"},
  {L"vyo", L"����"},
  {L"vyi", L"����"},
  {L"vye", L"����"},
  {L"vya", L"����"},
  {L"vu", L"��"},
  {L"vo", L"����"},
  {L"vi", L"����"},
  {L"ve", L"����"},
  {L"va", L"����"},
  {L"u", L"��"},
  {L"tyu", L"����"},
  {L"tyo", L"����"},
  {L"tyi", L"����"},
  {L"tye", L"����"},
  {L"tya", L"����"},
  {L"twu", L"�Ƃ�"},
  {L"two", L"�Ƃ�"},
  {L"twi", L"�Ƃ�"},
  {L"twe", L"�Ƃ�"},
  {L"twa", L"�Ƃ�"},
  {L"tu", L"��"},
  {L"tsu", L"��"},
  {L"tso", L"��"},
  {L"tsi", L"��"},
  {L"tse", L"��"},
  {L"tsa", L"��"},
  {L"to", L"��"},
  {L"ti", L"��"},
  {L"thu", L"�Ă�"},
  {L"tho", L"�Ă�"},
  {L"thi", L"�Ă�"},
  {L"the", L"�Ă�"},
  {L"tha", L"�Ă�"},
  {L"te", L"��"},
  {L"ta", L"��"},
  {L"syu", L"����"},
  {L"syo", L"����"},
  {L"syi", L"����"},
  {L"sye", L"����"},
  {L"sya", L"����"},
  {L"swu", L"����"},
  {L"swo", L"����"},
  {L"swi", L"����"},
  {L"swe", L"����"},
  {L"swa", L"����"},
  {L"su", L"��"},
  {L"so", L"��"},
  {L"si", L"��"},
  {L"shu", L"����"},
  {L"sho", L"����"},
  {L"shi", L"��"},
  {L"she", L"����"},
  {L"sha", L"����"},
  {L"se", L"��"},
  {L"sa", L"��"},
  {L"ryu", L"���"},
  {L"ryo", L"���"},
  {L"ryi", L"�股"},
  {L"rye", L"�肥"},
  {L"rya", L"���"},
  {L"ru", L"��"},
  {L"ro", L"��"},
  {L"ri", L"��"},
  {L"re", L"��"},
  {L"ra", L"��"},
  {L"qyu", L"����"},
  {L"qyo", L"����"},
  {L"qyi", L"����"},
  {L"qye", L"����"},
  {L"qya", L"����"},
  {L"qwu", L"����"},
  {L"qwo", L"����"},
  {L"qwi", L"����"},
  {L"qwe", L"����"},
  {L"qwa", L"����"},
  {L"qu", L"��"},
  {L"qo", L"����"},
  {L"qi", L"����"},
  {L"qe", L"����"},
  {L"qa", L"����"},
  {L"pyu", L"�҂�"},
  {L"pyo", L"�҂�"},
  {L"pyi", L"�҂�"},
  {L"pye", L"�҂�"},
  {L"pya", L"�҂�"},
  {L"pu", L"��"},
  {L"po", L"��"},
  {L"pi", L"��"},
  {L"pe", L"��"},
  {L"pa", L"��"},
  {L"o", L"��"},
  {L"n�f", L"��"},
  {L"nz", L"��", L"z"},
  {L"nyu", L"�ɂ�"},
  {L"nyo", L"�ɂ�"},
  {L"nyi", L"�ɂ�"},
  {L"nye", L"�ɂ�"},
  {L"nya", L"�ɂ�"},
  {L"nx", L"��", L"x"},
  {L"nw", L"��", L"w"},
  {L"nv", L"��", L"v"},
  {L"nu", L"��"},
  {L"nt", L"��", L"t"},
  {L"ns", L"��", L"s"},
  {L"nr", L"��", L"r"},
  {L"nq", L"��", L"q"},
  {L"np", L"��", L"p"},
  {L"no", L"��"},
  {L"nn", L"��"},
  {L"nm", L"��", L"m"},
  {L"nl", L"��", L"l"},
  {L"nk", L"��", L"k"},
  {L"nj", L"��", L"j"},
  {L"ni", L"��"},
  {L"nh", L"��", L"h"},
  {L"ng", L"��", L"g"},
  {L"nf", L"��", L"f"},
  {L"ne", L"��"},
  {L"nd", L"��", L"d"},
  {L"nc", L"��", L"c"},
  {L"nb", L"��", L"b"},
  {L"na", L"��"},
  {L"n@", L"��", L"@"},
  {L"n-", L"��", L"-"},
  {L"myu", L"�݂�"},
  {L"myo", L"�݂�"},
  {L"myi", L"�݂�"},
  {L"mye", L"�݂�"},
  {L"mya", L"�݂�"},
  {L"mu", L"��"},
  {L"mo", L"��"},
  {L"mi", L"��"},
  {L"me", L"��"},
  {L"ma", L"��"},
  {L"lyu", L"��"},
  {L"lyo", L"��"},
  {L"lyi", L"��"},
  {L"lye", L"��"},
  {L"lya", L"��"},
  {L"lwa", L"��"},
  {L"lu", L"��"},
  {L"ltu", L"��"},
  {L"ltsu", L"��"},
  {L"lo", L"��"},
  {L"lke", L"��"},
  {L"lka", L"��"},
  {L"li", L"��"},
  {L"le", L"��"},
  {L"la", L"��"},
  {L"kyu", L"����"},
  {L"kyo", L"����"},
  {L"kyi", L"����"},
  {L"kye", L"����"},
  {L"kya", L"����"},
  {L"kwu", L"����"},
  {L"kwo", L"����"},
  {L"kwi", L"����"},
  {L"kwe", L"����"},
  {L"kwa", L"����"},
  {L"ku", L"��"},
  {L"ko", L"��"},
  {L"ki", L"��"},
  {L"ke", L"��"},
  {L"ka", L"��"},
  {L"jyu", L"����"},
  {L"jyo", L"����"},
  {L"jyi", L"����"},
  {L"jye", L"����"},
  {L"jya", L"����"},
  {L"ju", L"����"},
  {L"jo", L"����"},
  {L"ji", L"��"},
  {L"je", L"����"},
  {L"ja", L"����"},
  {L"i", L"��"},
  {L"hyu", L"�Ђ�"},
  {L"hyo", L"�Ђ�"},
  {L"hyi", L"�Ђ�"},
  {L"hye", L"�Ђ�"},
  {L"hya", L"�Ђ�"},
  {L"hu", L"��"},
  {L"ho", L"��"},
  {L"hi", L"��"},
  {L"he", L"��"},
  {L"ha", L"��"},
  {L"gyu", L"����"},
  {L"gyo", L"����"},
  {L"gyi", L"����"},
  {L"gye", L"����"},
  {L"gya", L"����"},
  {L"gwu", L"����"},
  {L"gwo", L"����"},
  {L"gwi", L"����"},
  {L"gwe", L"����"},
  {L"gwa", L"����"},
  {L"gu", L"��"},
  {L"go", L"��"},
  {L"gi", L"��"},
  {L"ge", L"��"},
  {L"ga", L"��"},
  {L"fyu", L"�ӂ�"},
  {L"fyo", L"�ӂ�"},
  {L"fyi", L"�ӂ�"},
  {L"fye", L"�ӂ�"},
  {L"fya", L"�ӂ�"},
  {L"fwu", L"�ӂ�"},
  {L"fwo", L"�ӂ�"},
  {L"fwi", L"�ӂ�"},
  {L"fwe", L"�ӂ�"},
  {L"fwa", L"�ӂ�"},
  {L"fu", L"��"},
  {L"fo", L"�ӂ�"},
  {L"fi", L"�ӂ�"},
  {L"fe", L"�ӂ�"},
  {L"fa", L"�ӂ�"},
  {L"e", L"��"},
  {L"dyu", L"����"},
  {L"dyo", L"����"},
  {L"dyi", L"����"},
  {L"dye", L"����"},
  {L"dya", L"����"},
  {L"dwu", L"�ǂ�"},
  {L"dwo", L"�ǂ�"},
  {L"dwi", L"�ǂ�"},
  {L"dwe", L"�ǂ�"},
  {L"dwa", L"�ǂ�"},
  {L"du", L"��"},
  {L"do", L"��"},
  {L"di", L"��"},
  {L"dhu", L"�ł�"},
  {L"dho", L"�ł�"},
  {L"dhi", L"�ł�"},
  {L"dhe", L"�ł�"},
  {L"dha", L"�ł�"},
  {L"de", L"��"},
  {L"da", L"��"},
  {L"cyu", L"����"},
  {L"cyo", L"����"},
  {L"cyi", L"����"},
  {L"cye", L"����"},
  {L"cya", L"����"},
  {L"cu", L"��"},
  {L"co", L"��"},
  {L"ci", L"��"},
  {L"chu", L"����"},
  {L"cho", L"����"},
  {L"chi", L"��"},
  {L"che", L"����"},
  {L"cha", L"����"},
  {L"ce", L"��"},
  {L"ca", L"��"},
  {L"byu", L"�т�"},
  {L"byo", L"�т�"},
  {L"byi", L"�т�"},
  {L"bye", L"�т�"},
  {L"bya", L"�т�"},
  {L"bu", L"��"},
  {L"bo", L"��"},
  {L"bi", L"��"},
  {L"be", L"��"},
  {L"ba", L"��"},
  {L"a", L"��"},
// �y�~���Ƀ\�[�g�z�����܂�
};

static TABLEENTRY sokuon_table[] = {
// �y�~���Ƀ\�[�g�z��������
  {L"zzyu", L"������"},
  {L"zzyo", L"������"},
  {L"zzyi", L"������"},
  {L"zzye", L"������"},
  {L"zzya", L"������"},
  {L"zzu", L"����"},
  {L"zzo", L"����"},
  {L"zzi", L"����"},
  {L"zze", L"����"},
  {L"zza", L"����"},
  {L"yyu", L"����"},
  {L"yyo", L"����"},
  {L"yyi", L"����"},
  {L"yye", L"������"},
  {L"yya", L"����"},
  {L"xxyu", L"����"},
  {L"xxyo", L"����"},
  {L"xxyi", L"����"},
  {L"xxye", L"����"},
  {L"xxya", L"����"},
  {L"xxwa", L"����"},
  {L"xxu", L"����"},
  {L"xxtu", L"����"},
  {L"xxo", L"����"},
  {L"xxn", L"����"},
  {L"xxke", L"����"},
  {L"xxka", L"����"},
  {L"xxi", L"����"},
  {L"xxe", L"����"},
  {L"xxa", L"����"},
  {L"wwu", L"����"},
  {L"wwo", L"����"},
  {L"wwi", L"������"},
  {L"wwhu", L"����"},
  {L"wwho", L"������"},
  {L"wwhi", L"������"},
  {L"wwhe", L"������"},
  {L"wwha", L"������"},
  {L"wwe", L"������"},
  {L"wwa", L"����"},
  {L"vvyu", L"������"},
  {L"vvyo", L"������"},
  {L"vvyi", L"������"},
  {L"vvye", L"������"},
  {L"vvya", L"������"},
  {L"vvu", L"����"},
  {L"vvo", L"������"},
  {L"vvi", L"������"},
  {L"vve", L"������"},
  {L"vva", L"������"},
  {L"ttyu", L"������"},
  {L"ttyo", L"������"},
  {L"ttyi", L"������"},
  {L"ttye", L"������"},
  {L"ttya", L"������"},
  {L"ttwu", L"���Ƃ�"},
  {L"ttwo", L"���Ƃ�"},
  {L"ttwi", L"���Ƃ�"},
  {L"ttwe", L"���Ƃ�"},
  {L"ttwa", L"���Ƃ�"},
  {L"ttu", L"����"},
  {L"ttsu", L"����"},
  {L"ttso", L"����"},
  {L"ttsi", L"����"},
  {L"ttse", L"����"},
  {L"ttsa", L"����"},
  {L"tto", L"����"},
  {L"tti", L"����"},
  {L"tthu", L"���Ă�"},
  {L"ttho", L"���Ă�"},
  {L"tthi", L"���Ă�"},
  {L"tthe", L"���Ă�"},
  {L"ttha", L"���Ă�"},
  {L"tte", L"����"},
  {L"tta", L"����"},
  {L"ssyu", L"������"},
  {L"ssyo", L"������"},
  {L"ssyi", L"������"},
  {L"ssye", L"������"},
  {L"ssya", L"������"},
  {L"sswu", L"������"},
  {L"sswo", L"������"},
  {L"sswi", L"������"},
  {L"sswe", L"������"},
  {L"sswa", L"������"},
  {L"ssu", L"����"},
  {L"sso", L"����"},
  {L"ssi", L"����"},
  {L"sshu", L"������"},
  {L"ssho", L"������"},
  {L"sshi", L"����"},
  {L"sshe", L"������"},
  {L"ssha", L"������"},
  {L"sse", L"����"},
  {L"ssa", L"����"},
  {L"rryu", L"�����"},
  {L"rryo", L"�����"},
  {L"rryi", L"���股"},
  {L"rrye", L"���肥"},
  {L"rrya", L"�����"},
  {L"rru", L"����"},
  {L"rro", L"����"},
  {L"rri", L"����"},
  {L"rre", L"����"},
  {L"rra", L"����"},
  {L"qqyu", L"������"},
  {L"qqyo", L"������"},
  {L"qqyi", L"������"},
  {L"qqye", L"������"},
  {L"qqya", L"������"},
  {L"qqwu", L"������"},
  {L"qqwo", L"������"},
  {L"qqwi", L"������"},
  {L"qqwe", L"������"},
  {L"qqwa", L"������"},
  {L"qqu", L"����"},
  {L"qqo", L"������"},
  {L"qqi", L"������"},
  {L"qqe", L"������"},
  {L"qqa", L"������"},
  {L"ppyu", L"���҂�"},
  {L"ppyo", L"���҂�"},
  {L"ppyi", L"���҂�"},
  {L"ppye", L"���҂�"},
  {L"ppya", L"���҂�"},
  {L"ppu", L"����"},
  {L"ppo", L"����"},
  {L"ppi", L"����"},
  {L"ppe", L"����"},
  {L"ppa", L"����"},
  {L"mmyu", L"���݂�"},
  {L"mmyo", L"���݂�"},
  {L"mmyi", L"���݂�"},
  {L"mmye", L"���݂�"},
  {L"mmya", L"���݂�"},
  {L"mmu", L"����"},
  {L"mmo", L"����"},
  {L"mmi", L"����"},
  {L"mme", L"����"},
  {L"mma", L"����"},
  {L"llyu", L"����"},
  {L"llyo", L"����"},
  {L"llyi", L"����"},
  {L"llye", L"����"},
  {L"llya", L"����"},
  {L"llwa", L"����"},
  {L"llu", L"����"},
  {L"lltu", L"����"},
  {L"lltsu", L"����"},
  {L"llo", L"����"},
  {L"llke", L"����"},
  {L"llka", L"����"},
  {L"lli", L"����"},
  {L"lle", L"����"},
  {L"lla", L"����"},
  {L"kkyu", L"������"},
  {L"kkyo", L"������"},
  {L"kkyi", L"������"},
  {L"kkye", L"������"},
  {L"kkya", L"������"},
  {L"kkwu", L"������"},
  {L"kkwo", L"������"},
  {L"kkwi", L"������"},
  {L"kkwe", L"������"},
  {L"kkwa", L"������"},
  {L"kku", L"����"},
  {L"kko", L"����"},
  {L"kki", L"����"},
  {L"kke", L"����"},
  {L"kka", L"����"},
  {L"jjyu", L"������"},
  {L"jjyo", L"������"},
  {L"jjyi", L"������"},
  {L"jjye", L"������"},
  {L"jjya", L"������"},
  {L"jju", L"������"},
  {L"jjo", L"������"},
  {L"jji", L"����"},
  {L"jje", L"������"},
  {L"jja", L"������"},
  {L"hhyu", L"���Ђ�"},
  {L"hhyo", L"���Ђ�"},
  {L"hhyi", L"���Ђ�"},
  {L"hhye", L"���Ђ�"},
  {L"hhya", L"���Ђ�"},
  {L"hhu", L"����"},
  {L"hho", L"����"},
  {L"hhi", L"����"},
  {L"hhe", L"����"},
  {L"hha", L"����"},
  {L"ggyu", L"������"},
  {L"ggyo", L"������"},
  {L"ggyi", L"������"},
  {L"ggye", L"������"},
  {L"ggya", L"������"},
  {L"ggwu", L"������"},
  {L"ggwo", L"������"},
  {L"ggwi", L"������"},
  {L"ggwe", L"������"},
  {L"ggwa", L"������"},
  {L"ggu", L"����"},
  {L"ggo", L"����"},
  {L"ggi", L"����"},
  {L"gge", L"����"},
  {L"gga", L"����"},
  {L"ffyu", L"���ӂ�"},
  {L"ffyo", L"���ӂ�"},
  {L"ffyi", L"���ӂ�"},
  {L"ffye", L"���ӂ�"},
  {L"ffya", L"���ӂ�"},
  {L"ffwu", L"���ӂ�"},
  {L"ffwo", L"���ӂ�"},
  {L"ffwi", L"���ӂ�"},
  {L"ffwe", L"���ӂ�"},
  {L"ffwa", L"���ӂ�"},
  {L"ffu", L"����"},
  {L"ffo", L"���ӂ�"},
  {L"ffi", L"���ӂ�"},
  {L"ffe", L"���ӂ�"},
  {L"ffa", L"���ӂ�"},
  {L"ddyu", L"������"},
  {L"ddyo", L"������"},
  {L"ddyi", L"������"},
  {L"ddye", L"������"},
  {L"ddya", L"������"},
  {L"ddwu", L"���ǂ�"},
  {L"ddwo", L"���ǂ�"},
  {L"ddwi", L"���ǂ�"},
  {L"ddwe", L"���ǂ�"},
  {L"ddwa", L"���ǂ�"},
  {L"ddu", L"����"},
  {L"ddo", L"����"},
  {L"ddi", L"����"},
  {L"ddhu", L"���ł�"},
  {L"ddho", L"���ł�"},
  {L"ddhi", L"���ł�"},
  {L"ddhe", L"���ł�"},
  {L"ddha", L"���ł�"},
  {L"dde", L"����"},
  {L"dda", L"����"},
  {L"ccyu", L"������"},
  {L"ccyo", L"������"},
  {L"ccyi", L"������"},
  {L"ccye", L"������"},
  {L"ccya", L"������"},
  {L"ccu", L"����"},
  {L"cco", L"����"},
  {L"cci", L"����"},
  {L"cchu", L"������"},
  {L"ccho", L"������"},
  {L"cchi", L"����"},
  {L"cche", L"������"},
  {L"ccha", L"������"},
  {L"cce", L"����"},
  {L"cca", L"����"},
  {L"bbyu", L"���т�"},
  {L"bbyo", L"���т�"},
  {L"bbyi", L"���т�"},
  {L"bbye", L"���т�"},
  {L"bbya", L"���т�"},
  {L"bbu", L"����"},
  {L"bbo", L"����"},
  {L"bbi", L"����"},
  {L"bbe", L"����"},
  {L"bba", L"����"},
// �y�~���Ƀ\�[�g�z�����܂�
};

//////////////////////////////////////////////////////////////////////////////

std::wstring zenkaku_to_hankaku(const std::wstring& zenkaku) {
  std::wstring hankaku;
  bool flag;
  wchar_t ch;
  for (size_t k = 0; k < zenkaku.size(); ++k) {
    flag = true;
    ch = zenkaku[k];
    for (size_t i = 0; i < _countof(halfkana_table); ++i) {
      if (ch == halfkana_table[i].key[0]) {
        hankaku += halfkana_table[i].value;
        flag = false;
        break;
      }
    }
    if (flag) {
      hankaku += ch;
    }
  }
  for (size_t i = 0; i < _countof(kigou_table); ++i) {
    for (size_t k = 0; k < zenkaku.size(); ++k) {
      if (hankaku[k] == kigou_table[i].value[0]) {
        hankaku[k] = kigou_table[i].key[0];
        break;
      }
    }
  }
  WCHAR szBuf[1024];
  const LCID langid = MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT);
  szBuf[0] = 0;
  DWORD dwFlags = LCMAP_HALFWIDTH;
  ::LCMapStringW(MAKELCID(langid, SORT_DEFAULT), dwFlags,
                 hankaku.c_str(), -1, szBuf, 1024);
  return szBuf;
} // zenkaku_to_hankaku

std::wstring hankaku_to_zenkaku(const std::wstring& hankaku) {
  std::wstring zenkaku = hankaku;
  for (size_t i = 0; i < _countof(kana_table); ++i) {
    unboost::replace_all(zenkaku, kana_table[i].key, kana_table[i].value);
  }
  for (size_t i = 0; i < _countof(kigou_table); ++i) {
    for (size_t k = 0; k < zenkaku.size(); ++k) {
      if (zenkaku[k] == kigou_table[i].key[0]) {
        zenkaku[k] = kigou_table[i].value[0];
        break;
      }
    }
  }
  WCHAR szBuf[1024];
  const LCID langid = MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT);
  szBuf[0] = 0;
  DWORD dwFlags = LCMAP_FULLWIDTH;
  ::LCMapStringW(MAKELCID(langid, SORT_DEFAULT), dwFlags,
                 zenkaku.c_str(), -1, szBuf, 1024);
  return szBuf;
} // hankaku_to_zenkaku

std::wstring zenkaku_hiragana_to_katakana(const std::wstring& hiragana) {
  WCHAR szBuf[1024];
  const LCID langid = MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT);
  szBuf[0] = 0;
  DWORD dwFlags = LCMAP_FULLWIDTH | LCMAP_KATAKANA;
  ::LCMapStringW(MAKELCID(langid, SORT_DEFAULT), dwFlags,
                 hiragana.c_str(), -1, szBuf, 1024);
  return szBuf;
} // zenkaku_hiragana_to_katakana

std::wstring zenkaku_katakana_to_hiragana(const std::wstring& katakana) {
  WCHAR szBuf[1024];
  const LCID langid = MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT);
  szBuf[0] = 0;
  DWORD dwFlags = LCMAP_FULLWIDTH | LCMAP_HIRAGANA;
  ::LCMapStringW(MAKELCID(langid, SORT_DEFAULT), dwFlags,
                 katakana.c_str(), -1, szBuf, 1024);
  return szBuf;
} // zenkaku_katakana_to_hiragana

std::wstring hiragana_to_romaji(const std::wstring& hiragana) {
  std::wstring romaji = hiragana;
  for (size_t i = 0; i < _countof(sokuon_table); ++i) {
    unboost::replace_all(romaji, sokuon_table[i].value, sokuon_table[i].key);
  }
  for (size_t i = 0; i < _countof(reverse_table); ++i) {
    unboost::replace_all(romaji, reverse_table[i].key, reverse_table[i].value);
  }
  for (size_t i = 0; i < _countof(romaji_table); ++i) {
    unboost::replace_all(romaji, romaji_table[i].value, romaji_table[i].key);
  }
  return romaji;
} // hiragana_to_romaji

std::wstring romaji_to_hiragana(const std::wstring& romaji) {
  std::wstring hiragana = romaji;
  for (size_t i = 0; i < _countof(sokuon_table); ++i) {
    unboost::replace_all(hiragana, sokuon_table[i].key, sokuon_table[i].value);
  }
  for (size_t i = 0; i < _countof(romaji_table); ++i) {
    unboost::replace_all(hiragana, romaji_table[i].key, romaji_table[i].value);
  }
  return hiragana;
} // romaji_to_hiragana

WCHAR alpha_to_hiragana(BYTE vk) {
  static const WCHAR s_table[26] = {
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
    L'��',
  };
  if (VK_A <= vk && vk <= VK_Z) {
    return s_table[vk - VK_A];
  }
  return 0;
} // alpha_to_hiragana

BOOL is_hiragana(WCHAR ch) {
  if (0x3040 <= ch && ch <= 0x309F) return TRUE;
  switch (ch) {
  case 0x3095: case 0x3096: case 0x3099: case 0x309A:
  case 0x309B: case 0x309C: case 0x309D: case 0x309E:
  case 0x30FC:
    return TRUE;
  default:
    return FALSE;
  }
}

BOOL is_zenkaku_katakana(WCHAR ch) {
  if (0x30A0 <= ch && ch <= 0x30FF) return TRUE;
  switch (ch) {
  case 0x30FD: case 0x30FE: case 0x3099: case 0x309A:
  case 0x309B: case 0x309C: case 0x30FC:
    return TRUE;
  default:
    return FALSE;
  }
}

BOOL is_hankaku_katakana(WCHAR ch) {
  if (0xFF65 <= ch && ch <= 0xFF9F) return TRUE;
  switch (ch) {
  case 0xFF61: case 0xFF62: case 0xFF63: case 0xFF64:
    return TRUE;
  default:
    return FALSE;
  }
}

BOOL is_kanji(WCHAR ch) {
  // CJK��������
  if (0x4E00 <= ch && ch <= 0x9FFF) return TRUE;
  // CJK�݊�����
  if (0xF900 <= ch && ch <= 0xFAFF) return TRUE;
  return FALSE;
}

BOOL is_fullwidth_ascii(WCHAR ch) {
  return (0xFF00 <= ch && ch <= 0xFFEF);
}

void add_romaji_char(std::wstring& strComp, WCHAR ch, DWORD& dwCursorPos) {
  strComp.insert(dwCursorPos, 1, ch);
  ++dwCursorPos;

  for (size_t i = 0; i < _countof(sokuon_table); ++i) {
    const std::wstring& key = sokuon_table[i].key;
    if ((DWORD)key.size() <= dwCursorPos) {
      DWORD i = dwCursorPos - (DWORD)key.size();
      if (strComp.substr(i, key.size()) == key) {
        strComp.replace(i, key.size(), sokuon_table[i].value);
        return;
      }
    }
  }

  for (size_t i = 0; i < _countof(romaji_table); ++i) {
    const std::wstring& key = romaji_table[i].key;
    if ((DWORD)key.size() <= dwCursorPos) {
      DWORD i = dwCursorPos - (DWORD)key.size();
      if (strComp.substr(i, key.size()) == key) {
        strComp.replace(i, key.size(), romaji_table[i].value);
        return;
      }
    }
  }
} // add_romaji_char

void add_hiragana_char(std::wstring& strComp, WCHAR ch, DWORD& dwCursorPos) {
  if (is_zenkaku_katakana(ch)) {
    WCHAR sz[2] = {ch, 0};
    const LCID langid = MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT);
    sz[0] = 0;
    DWORD dwFlags = LCMAP_FULLWIDTH | LCMAP_HIRAGANA;
    ::LCMapStringW(MAKELCID(langid, SORT_DEFAULT), dwFlags,
                   sz, 1, &ch, 1);
  }

  strComp.insert(dwCursorPos, 1, ch);
  ++dwCursorPos;
} // add_hiragana_char

void add_katakana_char(std::wstring& strComp, WCHAR ch, DWORD& dwCursorPos) {
  if (is_hiragana(ch)) {
    WCHAR sz[2] = {ch, 0};
    const LCID langid = MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT);
    sz[0] = 0;
    DWORD dwFlags = LCMAP_FULLWIDTH | LCMAP_KATAKANA;
    ::LCMapStringW(MAKELCID(langid, SORT_DEFAULT), dwFlags,
                   sz, 1, &ch, 1);
  }

  strComp.insert(dwCursorPos, 1, ch);
  ++dwCursorPos;
} // add_katakana_char

void add_ascii_char(std::wstring& strComp, WCHAR ch, DWORD& dwCursorPos) {
  strComp.insert(dwCursorPos, 1, ch);
  ++dwCursorPos;
}

//////////////////////////////////////////////////////////////////////////////