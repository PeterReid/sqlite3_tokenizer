/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** An tokenizer for SQL
**
** This file contains C code that splits an SQL input string up into
** individual tokens and sends those tokens one-by-one over to the
** parser for analysis.
*/
#include <stdlib.h>

#define SQLITE_ASCII
#define SQLITE_PRIVATE static

#define sqlite3StrNICmp sqlite3_strnicmp
#define testcase(x)

#define TK_SEMI                             1
#define TK_EXPLAIN                          2
#define TK_QUERY                            3
#define TK_PLAN                             4
#define TK_BEGIN                            5
#define TK_TRANSACTION                      6
#define TK_DEFERRED                         7
#define TK_IMMEDIATE                        8
#define TK_EXCLUSIVE                        9
#define TK_COMMIT                          10
#define TK_END                             11
#define TK_ROLLBACK                        12
#define TK_SAVEPOINT                       13
#define TK_RELEASE                         14
#define TK_TO                              15
#define TK_TABLE                           16
#define TK_CREATE                          17
#define TK_IF                              18
#define TK_NOT                             19
#define TK_EXISTS                          20
#define TK_TEMP                            21
#define TK_LP                              22
#define TK_RP                              23
#define TK_AS                              24
#define TK_WITHOUT                         25
#define TK_COMMA                           26
#define TK_ID                              27
#define TK_INDEXED                         28
#define TK_ABORT                           29
#define TK_ACTION                          30
#define TK_AFTER                           31
#define TK_ANALYZE                         32
#define TK_ASC                             33
#define TK_ATTACH                          34
#define TK_BEFORE                          35
#define TK_BY                              36
#define TK_CASCADE                         37
#define TK_CAST                            38
#define TK_COLUMNKW                        39
#define TK_CONFLICT                        40
#define TK_DATABASE                        41
#define TK_DESC                            42
#define TK_DETACH                          43
#define TK_EACH                            44
#define TK_FAIL                            45
#define TK_FOR                             46
#define TK_IGNORE                          47
#define TK_INITIALLY                       48
#define TK_INSTEAD                         49
#define TK_LIKE_KW                         50
#define TK_MATCH                           51
#define TK_NO                              52
#define TK_KEY                             53
#define TK_OF                              54
#define TK_OFFSET                          55
#define TK_PRAGMA                          56
#define TK_RAISE                           57
#define TK_RECURSIVE                       58
#define TK_REPLACE                         59
#define TK_RESTRICT                        60
#define TK_ROW                             61
#define TK_TRIGGER                         62
#define TK_VACUUM                          63
#define TK_VIEW                            64
#define TK_VIRTUAL                         65
#define TK_WITH                            66
#define TK_REINDEX                         67
#define TK_RENAME                          68
#define TK_CTIME_KW                        69
#define TK_ANY                             70
#define TK_OR                              71
#define TK_AND                             72
#define TK_IS                              73
#define TK_BETWEEN                         74
#define TK_IN                              75
#define TK_ISNULL                          76
#define TK_NOTNULL                         77
#define TK_NE                              78
#define TK_EQ                              79
#define TK_GT                              80
#define TK_LE                              81
#define TK_LT                              82
#define TK_GE                              83
#define TK_ESCAPE                          84
#define TK_BITAND                          85
#define TK_BITOR                           86
#define TK_LSHIFT                          87
#define TK_RSHIFT                          88
#define TK_PLUS                            89
#define TK_MINUS                           90
#define TK_STAR                            91
#define TK_SLASH                           92
#define TK_REM                             93
#define TK_CONCAT                          94
#define TK_COLLATE                         95
#define TK_BITNOT                          96
#define TK_STRING                          97
#define TK_JOIN_KW                         98
#define TK_CONSTRAINT                      99
#define TK_DEFAULT                        100
#define TK_NULL                           101
#define TK_PRIMARY                        102
#define TK_UNIQUE                         103
#define TK_CHECK                          104
#define TK_REFERENCES                     105
#define TK_AUTOINCR                       106
#define TK_ON                             107
#define TK_INSERT                         108
#define TK_DELETE                         109
#define TK_UPDATE                         110
#define TK_SET                            111
#define TK_DEFERRABLE                     112
#define TK_FOREIGN                        113
#define TK_DROP                           114
#define TK_UNION                          115
#define TK_ALL                            116
#define TK_EXCEPT                         117
#define TK_INTERSECT                      118
#define TK_SELECT                         119
#define TK_VALUES                         120
#define TK_DISTINCT                       121
#define TK_DOT                            122
#define TK_FROM                           123
#define TK_JOIN                           124
#define TK_USING                          125
#define TK_ORDER                          126
#define TK_GROUP                          127
#define TK_HAVING                         128
#define TK_LIMIT                          129
#define TK_WHERE                          130
#define TK_INTO                           131
#define TK_INTEGER                        132
#define TK_FLOAT                          133
#define TK_BLOB                           134
#define TK_VARIABLE                       135
#define TK_CASE                           136
#define TK_WHEN                           137
#define TK_THEN                           138
#define TK_ELSE                           139
#define TK_INDEX                          140
#define TK_ALTER                          141
#define TK_ADD                            142
#define TK_TO_TEXT                        143
#define TK_TO_BLOB                        144
#define TK_TO_NUMERIC                     145
#define TK_TO_INT                         146
#define TK_TO_REAL                        147
#define TK_ISNOT                          148
#define TK_END_OF_FILE                    149
#define TK_ILLEGAL                        150
#define TK_SPACE                          151
#define TK_UNCLOSED_STRING                152
#define TK_FUNCTION                       153
#define TK_COLUMN                         154
#define TK_AGG_FUNCTION                   155
#define TK_AGG_COLUMN                     156
#define TK_UMINUS                         157
#define TK_UPLUS                          158
#define TK_REGISTER                       159


SQLITE_PRIVATE const unsigned char sqlite3UpperToLower[] = {
#ifdef SQLITE_ASCII
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
     18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
     36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
     54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99,100,101,102,103,
    104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,
    122, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,
    108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,
    126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,
    198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
    216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,
    234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,
    252,253,254,255
#endif
#ifdef SQLITE_EBCDIC
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, /* 0x */
     16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, /* 1x */
     32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, /* 2x */
     48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, /* 3x */
     64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, /* 4x */
     80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, /* 5x */
     96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111, /* 6x */
    112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127, /* 7x */
    128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143, /* 8x */
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159, /* 9x */
    160,161,162,163,164,165,166,167,168,169,170,171,140,141,142,175, /* Ax */
    176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191, /* Bx */
    192,129,130,131,132,133,134,135,136,137,202,203,204,205,206,207, /* Cx */
    208,145,146,147,148,149,150,151,152,153,218,219,220,221,222,223, /* Dx */
    224,225,162,163,164,165,166,167,168,169,234,235,236,237,238,239, /* Ex */
    240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255, /* Fx */
#endif
};

SQLITE_PRIVATE const unsigned char sqlite3CtypeMap[256] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 00..07    ........ */
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,  /* 08..0f    ........ */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 10..17    ........ */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 18..1f    ........ */
  0x01, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,  /* 20..27     !"#$%&' */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 28..2f    ()*+,-./ */
  0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,  /* 30..37    01234567 */
  0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 38..3f    89:;<=>? */

  0x00, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x02,  /* 40..47    @ABCDEFG */
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  /* 48..4f    HIJKLMNO */
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  /* 50..57    PQRSTUVW */
  0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x40,  /* 58..5f    XYZ[\]^_ */
  0x00, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x22,  /* 60..67    `abcdefg */
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  /* 68..6f    hijklmno */
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  /* 70..77    pqrstuvw */
  0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 78..7f    xyz{|}~. */

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 80..87    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 88..8f    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 90..97    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 98..9f    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* a0..a7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* a8..af    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* b0..b7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* b8..bf    ........ */

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* c0..c7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* c8..cf    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* d0..d7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* d8..df    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* e0..e7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* e8..ef    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* f0..f7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40   /* f8..ff    ........ */
};

# define sqlite3Isspace(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x01)
# define sqlite3Isdigit(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x04)
# define sqlite3Isxdigit(x)  (sqlite3CtypeMap[(unsigned char)(x)]&0x08)

/*
** The charMap() macro maps alphabetic characters into their
** lower-case ASCII equivalent.  On ASCII machines, this is just
** an upper-to-lower case map.  Only alphabetic characters and 
** underscores need to be translated.
*/
#define charMap(X) sqlite3UpperToLower[(unsigned char)X]
#define UpperToLower sqlite3UpperToLower

static int sqlite3_strnicmp(const char *zLeft, const char *zRight, int N){
  register unsigned char *a, *b;
  if( zLeft==0 ){
    return zRight ? -1 : 0;
  }else if( zRight==0 ){
    return 1;
  }
  a = (unsigned char *)zLeft;
  b = (unsigned char *)zRight;
  while( N-- > 0 && *a!=0 && UpperToLower[*a]==UpperToLower[*b]){ a++; b++; }
  return N<0 ? 0 : UpperToLower[*a] - UpperToLower[*b];
}


/*
** The sqlite3KeywordCode function looks up an identifier to determine if
** it is a keyword.  If it is a keyword, the token code of that keyword is 
** returned.  If the input is not a keyword, TK_ID is returned.
**
** The implementation of this routine was generated by a program,
** mkkeywordhash.h, located in the tool subdirectory of the distribution.
** The output of the mkkeywordhash.c program is written into a file
** named keywordhash.h and then included into this source file by
** the #include below.
*/
/************** Include keywordhash.h in the middle of tokenize.c ************/
/************** Begin file keywordhash.h *************************************/
/***** This file contains automatically generated code ******
**
** The code in this file has been automatically generated by
**
**   sqlite/tool/mkkeywordhash.c
**
** The code in this file implements a function that determines whether
** or not a given identifier is really an SQL keyword.  The same thing
** might be implemented more directly using a hand-written hash table.
** But by using this automatically generated code, the size of the code
** is substantially reduced.  This is important for embedded applications
** on platforms with limited memory.
*/
/* Hash score: 182 */
static int keywordCode(const char *z, int n){
  /* zText[] encodes 834 bytes of keywords in 554 bytes */
  /*   REINDEXEDESCAPEACHECKEYBEFOREIGNOREGEXPLAINSTEADDATABASELECT       */
  /*   ABLEFTHENDEFERRABLELSEXCEPTRANSACTIONATURALTERAISEXCLUSIVE         */
  /*   XISTSAVEPOINTERSECTRIGGEREFERENCESCONSTRAINTOFFSETEMPORARY         */
  /*   UNIQUERYWITHOUTERELEASEATTACHAVINGROUPDATEBEGINNERECURSIVE         */
  /*   BETWEENOTNULLIKECASCADELETECASECOLLATECREATECURRENT_DATEDETACH     */
  /*   IMMEDIATEJOINSERTMATCHPLANALYZEPRAGMABORTVALUESVIRTUALIMITWHEN     */
  /*   WHERENAMEAFTEREPLACEANDEFAULTAUTOINCREMENTCASTCOLUMNCOMMIT         */
  /*   CONFLICTCROSSCURRENT_TIMESTAMPRIMARYDEFERREDISTINCTDROPFAIL        */
  /*   FROMFULLGLOBYIFISNULLORDERESTRICTRIGHTROLLBACKROWUNIONUSING        */
  /*   VACUUMVIEWINITIALLY                                                */
  static const char zText[553] = {
    'R','E','I','N','D','E','X','E','D','E','S','C','A','P','E','A','C','H',
    'E','C','K','E','Y','B','E','F','O','R','E','I','G','N','O','R','E','G',
    'E','X','P','L','A','I','N','S','T','E','A','D','D','A','T','A','B','A',
    'S','E','L','E','C','T','A','B','L','E','F','T','H','E','N','D','E','F',
    'E','R','R','A','B','L','E','L','S','E','X','C','E','P','T','R','A','N',
    'S','A','C','T','I','O','N','A','T','U','R','A','L','T','E','R','A','I',
    'S','E','X','C','L','U','S','I','V','E','X','I','S','T','S','A','V','E',
    'P','O','I','N','T','E','R','S','E','C','T','R','I','G','G','E','R','E',
    'F','E','R','E','N','C','E','S','C','O','N','S','T','R','A','I','N','T',
    'O','F','F','S','E','T','E','M','P','O','R','A','R','Y','U','N','I','Q',
    'U','E','R','Y','W','I','T','H','O','U','T','E','R','E','L','E','A','S',
    'E','A','T','T','A','C','H','A','V','I','N','G','R','O','U','P','D','A',
    'T','E','B','E','G','I','N','N','E','R','E','C','U','R','S','I','V','E',
    'B','E','T','W','E','E','N','O','T','N','U','L','L','I','K','E','C','A',
    'S','C','A','D','E','L','E','T','E','C','A','S','E','C','O','L','L','A',
    'T','E','C','R','E','A','T','E','C','U','R','R','E','N','T','_','D','A',
    'T','E','D','E','T','A','C','H','I','M','M','E','D','I','A','T','E','J',
    'O','I','N','S','E','R','T','M','A','T','C','H','P','L','A','N','A','L',
    'Y','Z','E','P','R','A','G','M','A','B','O','R','T','V','A','L','U','E',
    'S','V','I','R','T','U','A','L','I','M','I','T','W','H','E','N','W','H',
    'E','R','E','N','A','M','E','A','F','T','E','R','E','P','L','A','C','E',
    'A','N','D','E','F','A','U','L','T','A','U','T','O','I','N','C','R','E',
    'M','E','N','T','C','A','S','T','C','O','L','U','M','N','C','O','M','M',
    'I','T','C','O','N','F','L','I','C','T','C','R','O','S','S','C','U','R',
    'R','E','N','T','_','T','I','M','E','S','T','A','M','P','R','I','M','A',
    'R','Y','D','E','F','E','R','R','E','D','I','S','T','I','N','C','T','D',
    'R','O','P','F','A','I','L','F','R','O','M','F','U','L','L','G','L','O',
    'B','Y','I','F','I','S','N','U','L','L','O','R','D','E','R','E','S','T',
    'R','I','C','T','R','I','G','H','T','R','O','L','L','B','A','C','K','R',
    'O','W','U','N','I','O','N','U','S','I','N','G','V','A','C','U','U','M',
    'V','I','E','W','I','N','I','T','I','A','L','L','Y',
  };
  static const unsigned char aHash[127] = {
      76, 105, 117,  74,   0,  45,   0,   0,  82,   0,  77,   0,   0,
      42,  12,  78,  15,   0, 116,  85,  54, 112,   0,  19,   0,   0,
     121,   0, 119, 115,   0,  22,  93,   0,   9,   0,   0,  70,  71,
       0,  69,   6,   0,  48,  90, 102,   0, 118, 101,   0,   0,  44,
       0, 103,  24,   0,  17,   0, 122,  53,  23,   0,   5, 110,  25,
      96,   0,   0, 124, 106,  60, 123,  57,  28,  55,   0,  91,   0,
     100,  26,   0,  99,   0,   0,   0,  95,  92,  97,  88, 109,  14,
      39, 108,   0,  81,   0,  18,  89, 111,  32,   0, 120,  80, 113,
      62,  46,  84,   0,   0,  94,  40,  59, 114,   0,  36,   0,   0,
      29,   0,  86,  63,  64,   0,  20,  61,   0,  56,
  };
  static const unsigned char aNext[124] = {
       0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   2,   0,   0,   0,   0,   0,   0,  13,   0,   0,   0,   0,
       0,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,  33,   0,  21,   0,   0,   0,   0,   0,  50,
       0,  43,   3,  47,   0,   0,   0,   0,  30,   0,  58,   0,  38,
       0,   0,   0,   1,  66,   0,   0,  67,   0,  41,   0,   0,   0,
       0,   0,   0,  49,  65,   0,   0,   0,   0,  31,  52,  16,  34,
      10,   0,   0,   0,   0,   0,   0,   0,  11,  72,  79,   0,   8,
       0, 104,  98,   0, 107,   0,  87,   0,  75,  51,   0,  27,  37,
      73,  83,   0,  35,  68,   0,   0,
  };
  static const unsigned char aLen[124] = {
       7,   7,   5,   4,   6,   4,   5,   3,   6,   7,   3,   6,   6,
       7,   7,   3,   8,   2,   6,   5,   4,   4,   3,  10,   4,   6,
      11,   6,   2,   7,   5,   5,   9,   6,   9,   9,   7,  10,  10,
       4,   6,   2,   3,   9,   4,   2,   6,   5,   7,   4,   5,   7,
       6,   6,   5,   6,   5,   5,   9,   7,   7,   3,   2,   4,   4,
       7,   3,   6,   4,   7,   6,  12,   6,   9,   4,   6,   5,   4,
       7,   6,   5,   6,   7,   5,   4,   5,   6,   5,   7,   3,   7,
      13,   2,   2,   4,   6,   6,   8,   5,  17,  12,   7,   8,   8,
       2,   4,   4,   4,   4,   4,   2,   2,   6,   5,   8,   5,   8,
       3,   5,   5,   6,   4,   9,   3,
  };
  static const unsigned short int aOffset[124] = {
       0,   2,   2,   8,   9,  14,  16,  20,  23,  25,  25,  29,  33,
      36,  41,  46,  48,  53,  54,  59,  62,  65,  67,  69,  78,  81,
      86,  91,  95,  96, 101, 105, 109, 117, 122, 128, 136, 142, 152,
     159, 162, 162, 165, 167, 167, 171, 176, 179, 184, 184, 188, 192,
     199, 204, 209, 212, 218, 221, 225, 234, 240, 240, 240, 243, 246,
     250, 251, 255, 261, 265, 272, 278, 290, 296, 305, 307, 313, 318,
     320, 327, 332, 337, 343, 349, 354, 358, 361, 367, 371, 378, 380,
     387, 389, 391, 400, 404, 410, 416, 424, 429, 429, 445, 452, 459,
     460, 467, 471, 475, 479, 483, 486, 488, 490, 496, 500, 508, 513,
     521, 524, 529, 534, 540, 544, 549,
  };
  static const unsigned char aCode[124] = {
    TK_REINDEX,    TK_INDEXED,    TK_INDEX,      TK_DESC,       TK_ESCAPE,     
    TK_EACH,       TK_CHECK,      TK_KEY,        TK_BEFORE,     TK_FOREIGN,    
    TK_FOR,        TK_IGNORE,     TK_LIKE_KW,    TK_EXPLAIN,    TK_INSTEAD,    
    TK_ADD,        TK_DATABASE,   TK_AS,         TK_SELECT,     TK_TABLE,      
    TK_JOIN_KW,    TK_THEN,       TK_END,        TK_DEFERRABLE, TK_ELSE,       
    TK_EXCEPT,     TK_TRANSACTION,TK_ACTION,     TK_ON,         TK_JOIN_KW,    
    TK_ALTER,      TK_RAISE,      TK_EXCLUSIVE,  TK_EXISTS,     TK_SAVEPOINT,  
    TK_INTERSECT,  TK_TRIGGER,    TK_REFERENCES, TK_CONSTRAINT, TK_INTO,       
    TK_OFFSET,     TK_OF,         TK_SET,        TK_TEMP,       TK_TEMP,       
    TK_OR,         TK_UNIQUE,     TK_QUERY,      TK_WITHOUT,    TK_WITH,       
    TK_JOIN_KW,    TK_RELEASE,    TK_ATTACH,     TK_HAVING,     TK_GROUP,      
    TK_UPDATE,     TK_BEGIN,      TK_JOIN_KW,    TK_RECURSIVE,  TK_BETWEEN,    
    TK_NOTNULL,    TK_NOT,        TK_NO,         TK_NULL,       TK_LIKE_KW,    
    TK_CASCADE,    TK_ASC,        TK_DELETE,     TK_CASE,       TK_COLLATE,    
    TK_CREATE,     TK_CTIME_KW,   TK_DETACH,     TK_IMMEDIATE,  TK_JOIN,       
    TK_INSERT,     TK_MATCH,      TK_PLAN,       TK_ANALYZE,    TK_PRAGMA,     
    TK_ABORT,      TK_VALUES,     TK_VIRTUAL,    TK_LIMIT,      TK_WHEN,       
    TK_WHERE,      TK_RENAME,     TK_AFTER,      TK_REPLACE,    TK_AND,        
    TK_DEFAULT,    TK_AUTOINCR,   TK_TO,         TK_IN,         TK_CAST,       
    TK_COLUMNKW,   TK_COMMIT,     TK_CONFLICT,   TK_JOIN_KW,    TK_CTIME_KW,   
    TK_CTIME_KW,   TK_PRIMARY,    TK_DEFERRED,   TK_DISTINCT,   TK_IS,         
    TK_DROP,       TK_FAIL,       TK_FROM,       TK_JOIN_KW,    TK_LIKE_KW,    
    TK_BY,         TK_IF,         TK_ISNULL,     TK_ORDER,      TK_RESTRICT,   
    TK_JOIN_KW,    TK_ROLLBACK,   TK_ROW,        TK_UNION,      TK_USING,      
    TK_VACUUM,     TK_VIEW,       TK_INITIALLY,  TK_ALL,        
  };
  int h, i;
  if( n<2 ) return TK_ID;
  h = ((charMap(z[0])*4) ^
      (charMap(z[n-1])*3) ^
      n) % 127;
  for(i=((int)aHash[h])-1; i>=0; i=((int)aNext[i])-1){
    if( aLen[i]==n && sqlite3StrNICmp(&zText[aOffset[i]],z,n)==0 ){
      testcase( i==0 ); /* REINDEX */
      testcase( i==1 ); /* INDEXED */
      testcase( i==2 ); /* INDEX */
      testcase( i==3 ); /* DESC */
      testcase( i==4 ); /* ESCAPE */
      testcase( i==5 ); /* EACH */
      testcase( i==6 ); /* CHECK */
      testcase( i==7 ); /* KEY */
      testcase( i==8 ); /* BEFORE */
      testcase( i==9 ); /* FOREIGN */
      testcase( i==10 ); /* FOR */
      testcase( i==11 ); /* IGNORE */
      testcase( i==12 ); /* REGEXP */
      testcase( i==13 ); /* EXPLAIN */
      testcase( i==14 ); /* INSTEAD */
      testcase( i==15 ); /* ADD */
      testcase( i==16 ); /* DATABASE */
      testcase( i==17 ); /* AS */
      testcase( i==18 ); /* SELECT */
      testcase( i==19 ); /* TABLE */
      testcase( i==20 ); /* LEFT */
      testcase( i==21 ); /* THEN */
      testcase( i==22 ); /* END */
      testcase( i==23 ); /* DEFERRABLE */
      testcase( i==24 ); /* ELSE */
      testcase( i==25 ); /* EXCEPT */
      testcase( i==26 ); /* TRANSACTION */
      testcase( i==27 ); /* ACTION */
      testcase( i==28 ); /* ON */
      testcase( i==29 ); /* NATURAL */
      testcase( i==30 ); /* ALTER */
      testcase( i==31 ); /* RAISE */
      testcase( i==32 ); /* EXCLUSIVE */
      testcase( i==33 ); /* EXISTS */
      testcase( i==34 ); /* SAVEPOINT */
      testcase( i==35 ); /* INTERSECT */
      testcase( i==36 ); /* TRIGGER */
      testcase( i==37 ); /* REFERENCES */
      testcase( i==38 ); /* CONSTRAINT */
      testcase( i==39 ); /* INTO */
      testcase( i==40 ); /* OFFSET */
      testcase( i==41 ); /* OF */
      testcase( i==42 ); /* SET */
      testcase( i==43 ); /* TEMPORARY */
      testcase( i==44 ); /* TEMP */
      testcase( i==45 ); /* OR */
      testcase( i==46 ); /* UNIQUE */
      testcase( i==47 ); /* QUERY */
      testcase( i==48 ); /* WITHOUT */
      testcase( i==49 ); /* WITH */
      testcase( i==50 ); /* OUTER */
      testcase( i==51 ); /* RELEASE */
      testcase( i==52 ); /* ATTACH */
      testcase( i==53 ); /* HAVING */
      testcase( i==54 ); /* GROUP */
      testcase( i==55 ); /* UPDATE */
      testcase( i==56 ); /* BEGIN */
      testcase( i==57 ); /* INNER */
      testcase( i==58 ); /* RECURSIVE */
      testcase( i==59 ); /* BETWEEN */
      testcase( i==60 ); /* NOTNULL */
      testcase( i==61 ); /* NOT */
      testcase( i==62 ); /* NO */
      testcase( i==63 ); /* NULL */
      testcase( i==64 ); /* LIKE */
      testcase( i==65 ); /* CASCADE */
      testcase( i==66 ); /* ASC */
      testcase( i==67 ); /* DELETE */
      testcase( i==68 ); /* CASE */
      testcase( i==69 ); /* COLLATE */
      testcase( i==70 ); /* CREATE */
      testcase( i==71 ); /* CURRENT_DATE */
      testcase( i==72 ); /* DETACH */
      testcase( i==73 ); /* IMMEDIATE */
      testcase( i==74 ); /* JOIN */
      testcase( i==75 ); /* INSERT */
      testcase( i==76 ); /* MATCH */
      testcase( i==77 ); /* PLAN */
      testcase( i==78 ); /* ANALYZE */
      testcase( i==79 ); /* PRAGMA */
      testcase( i==80 ); /* ABORT */
      testcase( i==81 ); /* VALUES */
      testcase( i==82 ); /* VIRTUAL */
      testcase( i==83 ); /* LIMIT */
      testcase( i==84 ); /* WHEN */
      testcase( i==85 ); /* WHERE */
      testcase( i==86 ); /* RENAME */
      testcase( i==87 ); /* AFTER */
      testcase( i==88 ); /* REPLACE */
      testcase( i==89 ); /* AND */
      testcase( i==90 ); /* DEFAULT */
      testcase( i==91 ); /* AUTOINCREMENT */
      testcase( i==92 ); /* TO */
      testcase( i==93 ); /* IN */
      testcase( i==94 ); /* CAST */
      testcase( i==95 ); /* COLUMN */
      testcase( i==96 ); /* COMMIT */
      testcase( i==97 ); /* CONFLICT */
      testcase( i==98 ); /* CROSS */
      testcase( i==99 ); /* CURRENT_TIMESTAMP */
      testcase( i==100 ); /* CURRENT_TIME */
      testcase( i==101 ); /* PRIMARY */
      testcase( i==102 ); /* DEFERRED */
      testcase( i==103 ); /* DISTINCT */
      testcase( i==104 ); /* IS */
      testcase( i==105 ); /* DROP */
      testcase( i==106 ); /* FAIL */
      testcase( i==107 ); /* FROM */
      testcase( i==108 ); /* FULL */
      testcase( i==109 ); /* GLOB */
      testcase( i==110 ); /* BY */
      testcase( i==111 ); /* IF */
      testcase( i==112 ); /* ISNULL */
      testcase( i==113 ); /* ORDER */
      testcase( i==114 ); /* RESTRICT */
      testcase( i==115 ); /* RIGHT */
      testcase( i==116 ); /* ROLLBACK */
      testcase( i==117 ); /* ROW */
      testcase( i==118 ); /* UNION */
      testcase( i==119 ); /* USING */
      testcase( i==120 ); /* VACUUM */
      testcase( i==121 ); /* VIEW */
      testcase( i==122 ); /* INITIALLY */
      testcase( i==123 ); /* ALL */
      return aCode[i];
    }
  }
  return TK_ID;
}
SQLITE_PRIVATE int sqlite3KeywordCode(const unsigned char *z, int n){
  return keywordCode((char*)z, n);
}
#define SQLITE_N_KEYWORD 124

/************** End of keywordhash.h *****************************************/
/************** Continuing where we left off in tokenize.c *******************/


/*
** If X is a character that can be used in an identifier then
** IdChar(X) will be true.  Otherwise it is false.
**
** For ASCII, any character with the high-order bit set is
** allowed in an identifier.  For 7-bit characters, 
** sqlite3IsIdChar[X] must be 1.
**
** Ticket #1066.  the SQL standard does not allow '$' in the
** middle of identifiers.  But many SQL implementations do. 
** SQLite will allow '$' in identifiers for compatibility.
** But the feature is undocumented.
*/
#define IdChar(C)  ((sqlite3CtypeMap[(unsigned char)C]&0x46)!=0)

SQLITE_PRIVATE int sqlite3IsIdChar(unsigned char c){ return IdChar(c); }


/*
** Return the length of the token that begins at z[0]. 
** Store the token type in *tokenType before returning.
*/
int sqlite3_get_token(const unsigned char *z, int *tokenType){
  int i, c;
  switch( *z ){
    case ' ': case '\t': case '\n': case '\f': case '\r': {
      testcase( z[0]==' ' );
      testcase( z[0]=='\t' );
      testcase( z[0]=='\n' );
      testcase( z[0]=='\f' );
      testcase( z[0]=='\r' );
      for(i=1; sqlite3Isspace(z[i]); i++){}
      *tokenType = TK_SPACE;
      return i;
    }
    case '-': {
      if( z[1]=='-' ){
        for(i=2; (c=z[i])!=0 && c!='\n'; i++){}
        *tokenType = TK_SPACE;   /* IMP: R-22934-25134 */
        return i;
      }
      *tokenType = TK_MINUS;
      return 1;
    }
    case '(': {
      *tokenType = TK_LP;
      return 1;
    }
    case ')': {
      *tokenType = TK_RP;
      return 1;
    }
    case ';': {
      *tokenType = TK_SEMI;
      return 1;
    }
    case '+': {
      *tokenType = TK_PLUS;
      return 1;
    }
    case '*': {
      *tokenType = TK_STAR;
      return 1;
    }
    case '/': {
      if( z[1]!='*' || z[2]==0 ){
        *tokenType = TK_SLASH;
        return 1;
      }
      for(i=3, c=z[2]; (c!='*' || z[i]!='/') && (c=z[i])!=0; i++){}
      if( c ) i++;
      *tokenType = TK_SPACE;   /* IMP: R-22934-25134 */
      return i;
    }
    case '%': {
      *tokenType = TK_REM;
      return 1;
    }
    case '=': {
      *tokenType = TK_EQ;
      return 1 + (z[1]=='=');
    }
    case '<': {
      if( (c=z[1])=='=' ){
        *tokenType = TK_LE;
        return 2;
      }else if( c=='>' ){
        *tokenType = TK_NE;
        return 2;
      }else if( c=='<' ){
        *tokenType = TK_LSHIFT;
        return 2;
      }else{
        *tokenType = TK_LT;
        return 1;
      }
    }
    case '>': {
      if( (c=z[1])=='=' ){
        *tokenType = TK_GE;
        return 2;
      }else if( c=='>' ){
        *tokenType = TK_RSHIFT;
        return 2;
      }else{
        *tokenType = TK_GT;
        return 1;
      }
    }
    case '!': {
      if( z[1]!='=' ){
        *tokenType = TK_ILLEGAL;
        return 2;
      }else{
        *tokenType = TK_NE;
        return 2;
      }
    }
    case '|': {
      if( z[1]!='|' ){
        *tokenType = TK_BITOR;
        return 1;
      }else{
        *tokenType = TK_CONCAT;
        return 2;
      }
    }
    case ',': {
      *tokenType = TK_COMMA;
      return 1;
    }
    case '&': {
      *tokenType = TK_BITAND;
      return 1;
    }
    case '~': {
      *tokenType = TK_BITNOT;
      return 1;
    }
    case '`':
    case '\'':
    case '"': {
      int delim = z[0];
      testcase( delim=='`' );
      testcase( delim=='\'' );
      testcase( delim=='"' );
      for(i=1; (c=z[i])!=0; i++){
        if( c==delim ){
          if( z[i+1]==delim ){
            i++;
          }else{
            break;
          }
        }
      }
      if( c=='\'' ){
        *tokenType = TK_STRING;
        return i+1;
      }else if( c!=0 ){
        *tokenType = TK_ID;
        return i+1;
      }else{
        *tokenType = TK_ILLEGAL;
        return i;
      }
    }
    case '.': {
#ifndef SQLITE_OMIT_FLOATING_POINT
      if( !sqlite3Isdigit(z[1]) )
#endif
      {
        *tokenType = TK_DOT;
        return 1;
      }
      /* If the next character is a digit, this is a floating point
      ** number that begins with ".".  Fall thru into the next case */
    }
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9': {
      testcase( z[0]=='0' );  testcase( z[0]=='1' );  testcase( z[0]=='2' );
      testcase( z[0]=='3' );  testcase( z[0]=='4' );  testcase( z[0]=='5' );
      testcase( z[0]=='6' );  testcase( z[0]=='7' );  testcase( z[0]=='8' );
      testcase( z[0]=='9' );
      *tokenType = TK_INTEGER;
#ifndef SQLITE_OMIT_HEX_INTEGER
      if( z[0]=='0' && (z[1]=='x' || z[1]=='X') && sqlite3Isxdigit(z[2]) ){
        for(i=3; sqlite3Isxdigit(z[i]); i++){}
        return i;
      }
#endif
      for(i=0; sqlite3Isdigit(z[i]); i++){}
#ifndef SQLITE_OMIT_FLOATING_POINT
      if( z[i]=='.' ){
        i++;
        while( sqlite3Isdigit(z[i]) ){ i++; }
        *tokenType = TK_FLOAT;
      }
      if( (z[i]=='e' || z[i]=='E') &&
           ( sqlite3Isdigit(z[i+1]) 
            || ((z[i+1]=='+' || z[i+1]=='-') && sqlite3Isdigit(z[i+2]))
           )
      ){
        i += 2;
        while( sqlite3Isdigit(z[i]) ){ i++; }
        *tokenType = TK_FLOAT;
      }
#endif
      while( IdChar(z[i]) ){
        *tokenType = TK_ILLEGAL;
        i++;
      }
      return i;
    }
    case '[': {
      for(i=1, c=z[0]; c!=']' && (c=z[i])!=0; i++){}
      *tokenType = c==']' ? TK_ID : TK_ILLEGAL;
      return i;
    }
    case '?': {
      *tokenType = TK_VARIABLE;
      for(i=1; sqlite3Isdigit(z[i]); i++){}
      return i;
    }
#ifndef SQLITE_OMIT_TCL_VARIABLE
    case '$':
#endif
    case '@':  /* For compatibility with MS SQL Server */
    case '#':
    case ':': {
      int n = 0;
      testcase( z[0]=='$' );  testcase( z[0]=='@' );
      testcase( z[0]==':' );  testcase( z[0]=='#' );
      *tokenType = TK_VARIABLE;
      for(i=1; (c=z[i])!=0; i++){
        if( IdChar(c) ){
          n++;
#ifndef SQLITE_OMIT_TCL_VARIABLE
        }else if( c=='(' && n>0 ){
          do{
            i++;
          }while( (c=z[i])!=0 && !sqlite3Isspace(c) && c!=')' );
          if( c==')' ){
            i++;
          }else{
            *tokenType = TK_ILLEGAL;
          }
          break;
        }else if( c==':' && z[i+1]==':' ){
          i++;
#endif
        }else{
          break;
        }
      }
      if( n==0 ) *tokenType = TK_ILLEGAL;
      return i;
    }
#ifndef SQLITE_OMIT_BLOB_LITERAL
    case 'x': case 'X': {
      testcase( z[0]=='x' ); testcase( z[0]=='X' );
      if( z[1]=='\'' ){
        *tokenType = TK_BLOB;
        for(i=2; sqlite3Isxdigit(z[i]); i++){}
        if( z[i]!='\'' || i%2 ){
          *tokenType = TK_ILLEGAL;
          while( z[i] && z[i]!='\'' ){ i++; }
        }
        if( z[i] ) i++;
        return i;
      }
      /* Otherwise fall through to the next case */
    }
#endif
    default: {
      if( !IdChar(*z) ){
        break;
      }
      for(i=1; IdChar(z[i]); i++){}
      *tokenType = keywordCode((char*)z, i);
      return i;
    }
  }
  *tokenType = TK_ILLEGAL;
  return 1;
}