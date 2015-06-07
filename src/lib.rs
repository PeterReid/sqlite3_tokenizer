//! Tokenizes SQL strings as SQLite would.
//!
//! This is intended for implementors of SQLite libraries.
//!
//! # Example
//!
//! ```{.ignore .rust}
//! for token in Tokenizer::new("SELECT * FROM t") {
//!     println!("Token of kind {:?} is written {:?}", token.kind, token.text);
//! }
//! ```
//! 
//! outputs
//!
//! ```{.ignore}
//! Token of kind Select is written "SELECT"
//! Token of kind Space is written " "
//! Token of kind Star is written "*"
//! Token of kind Space is written " "
//! Token of kind From is written "FROM"
//! Token of kind Space is written " "
//! Token of kind Id is written "t"
//! ```
//!
extern crate libc;

use std::ffi::CString;
use std::slice;
use std::str;

#[link(name = "sqlite3tokenize")]
extern "C" {
    fn sqlite3_get_token(z: *const libc::c_char, tokenType: *mut libc::c_int) -> libc::c_int;
}

/// Describes the kind of token recognized
/// 
/// Many of these tokens are simply fixed words (e.g. SELECT) or punctuation.
///
/// Some of the more varied ones are:
///
///  - ``Variable``: A placeholder for a value in a prepared statement. ``?`` or ``?7`` or ``@a`` or ``:b`` or ``#c``
///  - ``Space``: Whitespace, but also comments.
///  - ``Id``: A user-defined identifier. ``my_column`` or ``[some table]`` or ``"that function"`` or `` `a trigger` ``.
///  - ``String``: A string literal. ``'abcd'`` or ``'planet''s moon'``
///  - ``Blob``: A hexadecimal-encoded byte sequence literal. ``x'FEFF6564'``
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum TokenKind {
    Semicolon,
    Explain,
    Query,
    Plan,
    Begin,
    Transaction,
    Deferred,
    Immediate,
    Exclusive,
    Commit,
    End,
    Rollback,
    Savepoint,
    Release,
    To,
    Table,
    Create,
    If,
    Not,
    Exists,
    Temp,
    LeftParen,
    RightParen,
    As,
    Without,
    Comma,
    Id,
    Indexed,
    Abort,
    Action,
    After,
    Analyze,
    Asc,
    Attach,
    Before,
    By,
    Cascade,
    Cast,
    ColumnKw,
    Conflict,
    Database,
    Desc,
    Detach,
    Each,
    Fail,
    For,
    Ignore,
    Initially,
    Instead,
    Like,
    Match,
    No,
    Key,
    Of,
    Offset,
    Pragma,
    Raise,
    Recursive,
    Replace,
    Restrict,
    Row,
    Trigger,
    Vacuum,
    View,
    Virtual,
    With,
    Reindex,
    Rename,
    CurrentTime,
    Any,
    Or,
    And,
    Is,
    Between,
    In,
    IsNull,
    NotNull,
    NotEqual,
    Equal,
    Greater,
    LessOrEqual,
    Less,
    GreaterOrEqual,
    Escape,
    BitAnd,
    BitOr,
    LeftShift,
    RightShift,
    Plus,
    Minus,
    Star,
    Slash,
    Remainder,
    Concat,
    Collate,
    BitNot,
    String,
    JoinKw,
    Constraint,
    Default,
    Null,
    Primary,
    Unique,
    Check,
    References,
    AutoIncrement,
    On,
    Insert,
    Delete,
    Update,
    Set,
    Deferrable,
    Foreign,
    Drop,
    Union,
    All,
    Except,
    Intersect,
    Select,
    Values,
    Distinct,
    Dot,
    From,
    Join,
    Using,
    Order,
    Group,
    Having,
    Limit,
    Where,
    Into,
    Integer,
    Float,
    Blob,
    Variable,
    Case,
    When,
    Then,
    Else,
    Index,
    Alter,
    Add,
    ToText,
    ToBlob,
    ToNumeric,
    ToInt,
    ToReal,
    IsNot,
    EndOfFile,
    Illegal,
    Space,
    UnclosedString,
    Function,
    Column,
    AggFunction,
    AggColumn,
    UnaryMinus,
    UnaryPlus,
    Register,
}

impl TokenKind {
    fn from_int(x: libc::c_int) -> Option<TokenKind> {
        Some(match x {
            1 => TokenKind::Semicolon,
            2 => TokenKind::Explain,
            3 => TokenKind::Query,
            4 => TokenKind::Plan,
            5 => TokenKind::Begin,
            6 => TokenKind::Transaction,
            7 => TokenKind::Deferred,
            8 => TokenKind::Immediate,
            9 => TokenKind::Exclusive,
            10 => TokenKind::Commit,
            11 => TokenKind::End,
            12 => TokenKind::Rollback,
            13 => TokenKind::Savepoint,
            14 => TokenKind::Release,
            15 => TokenKind::To,
            16 => TokenKind::Table,
            17 => TokenKind::Create,
            18 => TokenKind::If,
            19 => TokenKind::Not,
            20 => TokenKind::Exists,
            21 => TokenKind::Temp,
            22 => TokenKind::LeftParen,
            23 => TokenKind::RightParen,
            24 => TokenKind::As,
            25 => TokenKind::Without,
            26 => TokenKind::Comma,
            27 => TokenKind::Id,
            28 => TokenKind::Indexed,
            29 => TokenKind::Abort,
            30 => TokenKind::Action,
            31 => TokenKind::After,
            32 => TokenKind::Analyze,
            33 => TokenKind::Asc,
            34 => TokenKind::Attach,
            35 => TokenKind::Before,
            36 => TokenKind::By,
            37 => TokenKind::Cascade,
            38 => TokenKind::Cast,
            39 => TokenKind::ColumnKw,
            40 => TokenKind::Conflict,
            41 => TokenKind::Database,
            42 => TokenKind::Desc,
            43 => TokenKind::Detach,
            44 => TokenKind::Each,
            45 => TokenKind::Fail,
            46 => TokenKind::For,
            47 => TokenKind::Ignore,
            48 => TokenKind::Initially,
            49 => TokenKind::Instead,
            50 => TokenKind::Like,
            51 => TokenKind::Match,
            52 => TokenKind::No,
            53 => TokenKind::Key,
            54 => TokenKind::Of,
            55 => TokenKind::Offset,
            56 => TokenKind::Pragma,
            57 => TokenKind::Raise,
            58 => TokenKind::Recursive,
            59 => TokenKind::Replace,
            60 => TokenKind::Restrict,
            61 => TokenKind::Row,
            62 => TokenKind::Trigger,
            63 => TokenKind::Vacuum,
            64 => TokenKind::View,
            65 => TokenKind::Virtual,
            66 => TokenKind::With,
            67 => TokenKind::Reindex,
            68 => TokenKind::Rename,
            69 => TokenKind::CurrentTime,
            70 => TokenKind::Any,
            71 => TokenKind::Or,
            72 => TokenKind::And,
            73 => TokenKind::Is,
            74 => TokenKind::Between,
            75 => TokenKind::In,
            76 => TokenKind::IsNull,
            77 => TokenKind::NotNull,
            78 => TokenKind::NotEqual,
            79 => TokenKind::Equal,
            80 => TokenKind::Greater,
            81 => TokenKind::LessOrEqual,
            82 => TokenKind::Less,
            83 => TokenKind::GreaterOrEqual,
            84 => TokenKind::Escape,
            85 => TokenKind::BitAnd,
            86 => TokenKind::BitOr,
            87 => TokenKind::LeftShift,
            88 => TokenKind::RightShift,
            89 => TokenKind::Plus,
            90 => TokenKind::Minus,
            91 => TokenKind::Star,
            92 => TokenKind::Slash,
            93 => TokenKind::Remainder,
            94 => TokenKind::Concat,
            95 => TokenKind::Collate,
            96 => TokenKind::BitNot,
            97 => TokenKind::String,
            98 => TokenKind::JoinKw,
            99 => TokenKind::Constraint,
            100 => TokenKind::Default,
            101 => TokenKind::Null,
            102 => TokenKind::Primary,
            103 => TokenKind::Unique,
            104 => TokenKind::Check,
            105 => TokenKind::References,
            106 => TokenKind::AutoIncrement,
            107 => TokenKind::On,
            108 => TokenKind::Insert,
            109 => TokenKind::Delete,
            110 => TokenKind::Update,
            111 => TokenKind::Set,
            112 => TokenKind::Deferrable,
            113 => TokenKind::Foreign,
            114 => TokenKind::Drop,
            115 => TokenKind::Union,
            116 => TokenKind::All,
            117 => TokenKind::Except,
            118 => TokenKind::Intersect,
            119 => TokenKind::Select,
            120 => TokenKind::Values,
            121 => TokenKind::Distinct,
            122 => TokenKind::Dot,
            123 => TokenKind::From,
            124 => TokenKind::Join,
            125 => TokenKind::Using,
            126 => TokenKind::Order,
            127 => TokenKind::Group,
            128 => TokenKind::Having,
            129 => TokenKind::Limit,
            130 => TokenKind::Where,
            131 => TokenKind::Into,
            132 => TokenKind::Integer,
            133 => TokenKind::Float,
            134 => TokenKind::Blob,
            135 => TokenKind::Variable,
            136 => TokenKind::Case,
            137 => TokenKind::When,
            138 => TokenKind::Then,
            139 => TokenKind::Else,
            140 => TokenKind::Index,
            141 => TokenKind::Alter,
            142 => TokenKind::Add,
            143 => TokenKind::ToText,
            144 => TokenKind::ToBlob,
            145 => TokenKind::ToNumeric,
            146 => TokenKind::ToInt,
            147 => TokenKind::ToReal,
            148 => TokenKind::IsNot,
            149 => TokenKind::EndOfFile,
            150 => TokenKind::Illegal,
            151 => TokenKind::Space,
            152 => TokenKind::UnclosedString,
            153 => TokenKind::Function,
            154 => TokenKind::Column,
            155 => TokenKind::AggFunction,
            156 => TokenKind::AggColumn,
            157 => TokenKind::UnaryMinus,
            158 => TokenKind::UnaryPlus,
            159 => TokenKind::Register,
            _ => { return None; }
        })
    }

}

/// A single parsed token, describing its type and the string it came from
#[derive(Debug, Clone, Copy)]
pub struct Token<'a> {
    pub kind: TokenKind,
    pub text: &'a str,
}

/// Iterates over tokens in a string
#[derive(Debug, Clone)]
pub struct Tokenizer<'a> {
    sql: CString, // sqlite3_get_token depends on the 0 terminator
    text_source: &'a str,
    offset: isize,
}

// CStrings cannot contain internal 0s, and SQLite's tokenizer will stop at a 0 anyway, so we
// truncate the string to be free of internal 0s.
fn until_internal_0(s: &str) -> CString {
    let before_internal_0 = s.split('\0').next().unwrap();
    CString::new(before_internal_0).unwrap()
}

impl<'a> Tokenizer<'a> {
    pub fn new(sql: &str) -> Tokenizer {
        Tokenizer {
            sql: until_internal_0(sql),
            text_source: sql,
            offset: 0,
        }
    }
}

impl<'a> Iterator for Tokenizer<'a> {
    type Item = Token<'a>;
    
    fn next(&mut self) -> Option<Self::Item> {
        let mut token_type : libc::c_int = 0;
        
        let token_begin = unsafe { self.sql.as_ptr().offset(self.offset) };
        
        if unsafe{ *token_begin } == 0 {
            return None;
        }
        
        let token_len = unsafe {
            sqlite3_get_token(token_begin, &mut token_type)
        };
        
        if token_len <= 0 {
            return None;
        }
        
        let bytes = unsafe { slice::from_raw_parts(
            self.text_source.as_ptr().offset(self.offset) as *const u8,
            token_len as usize
        )};
        // We got a valid UTF8 sequence as input and this tokenizer won't divide up the byte
        // sequence, across a single codepoint's encoding, or even separate two codepoints that are
        // not, encoded in one byte. This guarentees that we get valid utf8 out.
        let text = unsafe { str::from_utf8_unchecked(bytes) };
        let kind = TokenKind::from_int(token_type).unwrap_or(TokenKind::Illegal);
        
        self.offset += token_len as isize;
        
        Some(Token{
            text: text,
            kind: kind,
        })
    }
}

#[cfg(test)]
mod test {
    use super::{TokenKind, Tokenizer};
    
    fn collect_types(sql: &str) -> Vec<TokenKind> {
        Tokenizer::new(sql).map(|t| t.kind).collect()
    }
    
    #[test]
    fn it_works() {
        let mut t = Tokenizer::new("SELECT * FROM t");
        
        assert_eq!(t.next().unwrap().text, "SELECT");
        assert_eq!(t.next().unwrap().text, " ");
        assert_eq!(t.next().unwrap().text, "*");
        assert_eq!(t.next().unwrap().text, " ");
        assert_eq!(t.next().unwrap().text, "FROM");
        assert_eq!(t.next().unwrap().text, " ");
        assert_eq!(t.next().unwrap().text, "t");
        assert!(t.next().is_none());
    }
    
    #[test]
    fn insert_types() {
        assert_eq!(collect_types("INSERT INTO t(x) VALUES (?)"),
            [TokenKind::Insert, TokenKind::Space, TokenKind::Into, TokenKind::Space, // "INSERT INTO "
             TokenKind::Id, TokenKind::LeftParen, TokenKind::Id, TokenKind::RightParen, // "t(x)"
             TokenKind::Space, TokenKind::Values, TokenKind::Space, // " VALUES "
             TokenKind::LeftParen, TokenKind::Variable, TokenKind::RightParen // "(?)"
            ].to_vec());
    }
    
    #[test]
    fn empty() {
        assert_eq!(collect_types(""), Vec::new());
    }
    
    #[test]
    fn internal_0() {
        assert_eq!(collect_types("SELECT\0 * FROM t"), [TokenKind::Select].to_vec());
    }
    
    #[test]
    fn subslice() {
        // Ensure the lack of a trailing 0 doesn't cause us to run off the end
        let whole_slice = "CREATE TABLE t(x)";
        let first_word: &str = whole_slice.split(' ').next().unwrap();
        assert_eq!(collect_types(first_word), [TokenKind::Create].to_vec());
    }
}
