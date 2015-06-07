extern crate gcc;

fn main() {
    gcc::compile_library("libsqlite3tokenize.a", &["src/tokenize.c"]);
}