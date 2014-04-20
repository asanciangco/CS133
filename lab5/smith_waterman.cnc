
[ float sa ];
[ float sb ];
[ float * score ];

< int [2] scoreTag >;

< scoreTag > :: ( computeScore );

[ sa: i ] -> ( computeScore: i, j );
[ sb: j ] -> ( computeScore: i, j );
[ score: i-1, j-1 ] -> ( computeScore: i, j );
[ score: i  , j-1 ] -> ( computeScore: i, j );
[ score: i-1, j   ] -> ( computeScore: i, j );

( computeScore: i, j ) -> [ score: i, j ];

env -> [ sa: {0 .. N} ], [ sb: {0 .. M} ];
env -> < scoreTag: {0 .. N},{0 .. M} >;
[ score: {0 .. N}, {0 .. M} ] -> env;
