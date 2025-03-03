#include "prog1.h"
#include "slp.h"
#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    if(a > b) return a;
    return b;
}

/*Max args part*/
int count_stm(A_stm);
int count_exp(A_exp);
int count_expList(A_expList);
int count_in_expList(A_expList);

int maxargs(A_stm mystm) {
    return count_stm(mystm);
}
/*End of max args part*/

/*Interpret statment part*/

//Table data and functions
typedef struct table *Table_;

struct table{
    string id;
    int value;
    Table_ tail;
};

Table_ Table(string id, int value, struct table *tail) {
    Table_ t = malloc(sizeof(*t));
    t->id = id;
    t->value = value;
    t->tail = tail;
    return t;
}

Table_ update(Table_ t1, string id, int val) {
    return Table(id, val, t1);
}

int lookup(Table_ t, string key) {
    if(t == NULL) {
        printf("Error in lookup table 1!\n");
        return 0;
    }

    if(t->id == key) return t->value;
    return lookup(t->tail, key);
}

struct IntAndTable {
    int i;
    Table_ t;
};

Table_ interpStm(A_stm s, Table_ t);
struct IntAndTable interpExp(A_exp s, Table_ t);

/*End of Interpret statment part*/

int main() {
    A_stm myprog = prog();    
    Table_ t = NULL;
    interpStm(myprog, t);
}

int count_stm(A_stm mystm) {
    int res = 0;
    if(mystm->kind == A_compoundStm) {
        res = max(res, count_stm(mystm->u.compound.stm1));
        res = max(res, count_stm(mystm->u.compound.stm2));
    } else if(mystm->kind == A_printStm) {
        res = max(res, count_expList(mystm->u.print.exps));
        res = max(res, count_in_expList(mystm->u.print.exps));
    } else {
        res = max(res, count_exp(mystm->u.assign.exp));
    }
    return res;
}

int count_exp(A_exp myexp) {
    int res = 0;
    if(myexp->kind == A_idExp) return 0;
    else if(myexp->kind == A_numExp) return 0;
    else if(myexp->kind == A_opExp) {
        res = max(res, count_exp(myexp->u.op.left));     
        res = max(res, count_exp(myexp->u.op.right));     
    } else if(myexp->kind == A_eseqExp) {
        res = max(res, count_stm(myexp->u.eseq.stm));
        res = max(res, count_exp(myexp->u.eseq.exp));
    } else {
        printf("ERROR!\n");
    }
}

int count_expList(A_expList myexpList) {
    if(myexpList->kind == A_pairExpList) {
        return 1 + count_expList(myexpList->u.pair.tail);
    }
    return 1;
}

int count_in_expList(A_expList myexpList) {
    int res = 0;
    if(myexpList->kind == A_pairExpList) {
        res = max(res, count_exp(myexpList->u.pair.head));
        res = max(res, count_in_expList(myexpList->u.pair.tail));
    } else {
        res = max(res, count_exp(myexpList->u.last));
    }
    return res;
}

Table_ interpStm(A_stm s, Table_ t) {
    if(s == NULL) return t;
    if(s->kind == A_compoundStm) {
        Table_ t1 = interpStm(s->u.compound.stm1, t);
        Table_ t2 = interpStm(s->u.compound.stm2, t1);
        return t2;
    } else if(s->kind == A_assignStm) {
        struct IntAndTable t1 = interpExp(s->u.assign.exp, t);
        return update(t1.t, s->u.assign.id, t1.i);
    } else {
        A_expList expList = s->u.print.exps;
        while(expList->kind == A_pairExpList) {
            struct IntAndTable res = interpExp(expList->u.pair.head, t);
            t = res.t;
            printf("%d ", res.i);
            expList = expList->u.pair.tail;
        }

        struct IntAndTable res = interpExp(expList->u.last, t);
        printf("%d ", res.i);
        return res.t;
    }
}

struct IntAndTable interpExp(A_exp s, Table_ t) {
    if(s == NULL) {
        struct IntAndTable res;
        res.i = 0;
        res.t = t;
        return res;
    }

    if(s->kind == A_idExp) {
        struct IntAndTable res;
        res.i = lookup(t, s->u.id);
        res.t = t;
        return res;
    } else if(s->kind == A_numExp) {
        struct IntAndTable res;
        res.i = s->u.num;
        res.t = t;
        return res;
    } else if(s->kind == A_opExp) {
        struct IntAndTable res;

        struct IntAndTable tmp = interpExp(s->u.op.left, t);
        struct IntAndTable tmp1 = interpExp(s->u.op.right, tmp.t);
        if(s->u.op.oper == A_plus) {
            res.i = tmp.i + tmp1.i;
        } else if(s->u.op.oper == A_times) {
            res.i = tmp.i * tmp1.i;
        } else if(s->u.op.oper == A_minus) {
            res.i = tmp.i - tmp1.i;
        } else {
            res.i = tmp.i / tmp1.i;
        }
        res.t = tmp1.t;
        return res;
    } else {
        t = interpStm(s->u.eseq.stm, t);
        struct IntAndTable res = interpExp(s->u.eseq.exp, t);
        return res;
    }
}