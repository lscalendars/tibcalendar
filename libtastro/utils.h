// first a useful list:
extern long int zerolst[5];

void add_lst_6 ( long int a1[6], long int a2[6], long int a3[6], long int n0,
              long int n4, long int n5 );
void mul_lst ( long int res[5], long int lst[5], long int x, long int n0,
               long int n4 );
void add_lst ( long int a1[5], long int a2[5], long int a3[5], long int n0,
               long int n4);
void sub_lst ( long int a1[5], long int a2[5], long int a3[5], long int n0,
               long int n4 );
void div_lst_6 ( long int a1[6], long int a2[6], long int x, long int n4, long int n5 );
extern inline void clear_lst ( long int l[5]);
extern inline void copy_lst ( long int dest[5], long int src[5]);
void mul_lst_6 ( long int res[6], long int lst[6], long int x, long int n4, long int n5 );
void sub_lst_6 ( long int a1[6], long int a2[6], long int a3[6], long int n0, long int n4, long int n5 );
extern inline void clear_lst_6 ( long int l[6] );
extern inline void copy_lst_6 ( long int dest[6], long int src[6]);
