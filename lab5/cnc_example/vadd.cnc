
[ float va ];
[ float vb ];
[ float vc ];

< int [1] addTag >;

< addTag > :: ( addStep );

[ va: i ] -> ( addStep: i );
[ vb: i ] -> ( addStep: i );
( addStep: i ) -> [ vc: i ];

env -> [ va: {0 .. N} ], [ vb: {0 .. N} ];
env -> < addTag: {0 .. N} >;
[ vc: {0 .. N} ] -> env;
