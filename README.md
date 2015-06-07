This crate provides Tokenizer, which iterates over tokens in a SQL string as SQLite would.

# Example

    extern crate sqlite3_tokenizer;

    use sqlite3_tokenizer::Tokenizer;

    fn main() {
        for token in Tokenizer::new("SELECT * FROM t") {
            println!("Token of kind {:?} is written {:?}", token.kind, token.text);
        }
    }

outputs

    Token of kind Select is written "SELECT"
    Token of kind Space is written " "
    Token of kind Star is written "*"
    Token of kind Space is written " "
    Token of kind From is written "FROM"
    Token of kind Space is written " "
    Token of kind Id is written "t"

