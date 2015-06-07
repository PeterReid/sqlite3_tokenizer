var searchIndex = {};
searchIndex['sqlite3_tokenizer'] = {"items":[[0,"","sqlite3_tokenizer","Tokenizes SQL strings as SQLite would.",null,null],[3,"Token","","A single parsed token, describing its type and the string it came from",null,null],[12,"kind","","",0,null],[12,"text","","",0,null],[3,"Tokenizer","","Iterates over tokens in a string",null,null],[4,"TokenKind","","Describes the kind of token recognized",null,null],[13,"Semicolon","","",1,null],[13,"Explain","","",1,null],[13,"Query","","",1,null],[13,"Plan","","",1,null],[13,"Begin","","",1,null],[13,"Transaction","","",1,null],[13,"Deferred","","",1,null],[13,"Immediate","","",1,null],[13,"Exclusive","","",1,null],[13,"Commit","","",1,null],[13,"End","","",1,null],[13,"Rollback","","",1,null],[13,"Savepoint","","",1,null],[13,"Release","","",1,null],[13,"To","","",1,null],[13,"Table","","",1,null],[13,"Create","","",1,null],[13,"If","","",1,null],[13,"Not","","",1,null],[13,"Exists","","",1,null],[13,"Temp","","",1,null],[13,"LeftParen","","",1,null],[13,"RightParen","","",1,null],[13,"As","","",1,null],[13,"Without","","",1,null],[13,"Comma","","",1,null],[13,"Id","","",1,null],[13,"Indexed","","",1,null],[13,"Abort","","",1,null],[13,"Action","","",1,null],[13,"After","","",1,null],[13,"Analyze","","",1,null],[13,"Asc","","",1,null],[13,"Attach","","",1,null],[13,"Before","","",1,null],[13,"By","","",1,null],[13,"Cascade","","",1,null],[13,"Cast","","",1,null],[13,"ColumnKw","","",1,null],[13,"Conflict","","",1,null],[13,"Database","","",1,null],[13,"Desc","","",1,null],[13,"Detach","","",1,null],[13,"Each","","",1,null],[13,"Fail","","",1,null],[13,"For","","",1,null],[13,"Ignore","","",1,null],[13,"Initially","","",1,null],[13,"Instead","","",1,null],[13,"Like","","",1,null],[13,"Match","","",1,null],[13,"No","","",1,null],[13,"Key","","",1,null],[13,"Of","","",1,null],[13,"Offset","","",1,null],[13,"Pragma","","",1,null],[13,"Raise","","",1,null],[13,"Recursive","","",1,null],[13,"Replace","","",1,null],[13,"Restrict","","",1,null],[13,"Row","","",1,null],[13,"Trigger","","",1,null],[13,"Vacuum","","",1,null],[13,"View","","",1,null],[13,"Virtual","","",1,null],[13,"With","","",1,null],[13,"Reindex","","",1,null],[13,"Rename","","",1,null],[13,"CurrentTime","","",1,null],[13,"Any","","",1,null],[13,"Or","","",1,null],[13,"And","","",1,null],[13,"Is","","",1,null],[13,"Between","","",1,null],[13,"In","","",1,null],[13,"IsNull","","",1,null],[13,"NotNull","","",1,null],[13,"NotEqual","","",1,null],[13,"Equal","","",1,null],[13,"Greater","","",1,null],[13,"LessOrEqual","","",1,null],[13,"Less","","",1,null],[13,"GreaterOrEqual","","",1,null],[13,"Escape","","",1,null],[13,"BitAnd","","",1,null],[13,"BitOr","","",1,null],[13,"LeftShift","","",1,null],[13,"RightShift","","",1,null],[13,"Plus","","",1,null],[13,"Minus","","",1,null],[13,"Star","","",1,null],[13,"Slash","","",1,null],[13,"Remainder","","",1,null],[13,"Concat","","",1,null],[13,"Collate","","",1,null],[13,"BitNot","","",1,null],[13,"String","","",1,null],[13,"JoinKw","","",1,null],[13,"Constraint","","",1,null],[13,"Default","","",1,null],[13,"Null","","",1,null],[13,"Primary","","",1,null],[13,"Unique","","",1,null],[13,"Check","","",1,null],[13,"References","","",1,null],[13,"AutoIncrement","","",1,null],[13,"On","","",1,null],[13,"Insert","","",1,null],[13,"Delete","","",1,null],[13,"Update","","",1,null],[13,"Set","","",1,null],[13,"Deferrable","","",1,null],[13,"Foreign","","",1,null],[13,"Drop","","",1,null],[13,"Union","","",1,null],[13,"All","","",1,null],[13,"Except","","",1,null],[13,"Intersect","","",1,null],[13,"Select","","",1,null],[13,"Values","","",1,null],[13,"Distinct","","",1,null],[13,"Dot","","",1,null],[13,"From","","",1,null],[13,"Join","","",1,null],[13,"Using","","",1,null],[13,"Order","","",1,null],[13,"Group","","",1,null],[13,"Having","","",1,null],[13,"Limit","","",1,null],[13,"Where","","",1,null],[13,"Into","","",1,null],[13,"Integer","","",1,null],[13,"Float","","",1,null],[13,"Blob","","",1,null],[13,"Variable","","",1,null],[13,"Case","","",1,null],[13,"When","","",1,null],[13,"Then","","",1,null],[13,"Else","","",1,null],[13,"Index","","",1,null],[13,"Alter","","",1,null],[13,"Add","","",1,null],[13,"ToText","","",1,null],[13,"ToBlob","","",1,null],[13,"ToNumeric","","",1,null],[13,"ToInt","","",1,null],[13,"ToReal","","",1,null],[13,"IsNot","","",1,null],[13,"EndOfFile","","",1,null],[13,"Illegal","","",1,null],[13,"Space","","",1,null],[13,"UnclosedString","","",1,null],[13,"Function","","",1,null],[13,"Column","","",1,null],[13,"AggFunction","","",1,null],[13,"AggColumn","","",1,null],[13,"UnaryMinus","","",1,null],[13,"UnaryPlus","","",1,null],[13,"Register","","",1,null],[11,"eq","","",1,{"inputs":[{"name":"tokenkind"},{"name":"tokenkind"}],"output":{"name":"bool"}}],[11,"ne","","",1,{"inputs":[{"name":"tokenkind"},{"name":"tokenkind"}],"output":{"name":"bool"}}],[11,"clone","","",1,{"inputs":[{"name":"tokenkind"}],"output":{"name":"tokenkind"}}],[11,"fmt","","",1,{"inputs":[{"name":"tokenkind"},{"name":"formatter"}],"output":{"name":"result"}}],[11,"clone","","",0,{"inputs":[{"name":"token"}],"output":{"name":"token"}}],[11,"fmt","","",0,{"inputs":[{"name":"token"},{"name":"formatter"}],"output":{"name":"result"}}],[11,"clone","","",2,{"inputs":[{"name":"tokenizer"}],"output":{"name":"tokenizer"}}],[11,"fmt","","",2,{"inputs":[{"name":"tokenizer"},{"name":"formatter"}],"output":{"name":"result"}}],[11,"new","","",2,{"inputs":[{"name":"tokenizer"},{"name":"str"}],"output":{"name":"tokenizer"}}],[6,"Item","","",null,null],[11,"next","","",2,{"inputs":[{"name":"tokenizer"}],"output":{"name":"option"}}]],"paths":[[3,"Token"],[4,"TokenKind"],[3,"Tokenizer"]]};
initSearch(searchIndex);
