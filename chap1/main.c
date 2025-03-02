#include "prog1.h"
#include "slp.h"
#include <stdio.h>

int max(int a, int b) {
    if(a > b) return a;
    return b;
}

int count_stm(A_stm);
int count_exp(A_exp);
int count_expList(A_expList);
int count_in_expList(A_expList);

int maxargs(A_stm mystm) {
    return count_stm(mystm);
}

int main() {
    A_stm myprog = prog();    
    printf("The maxargs: %d\n", maxargs(myprog));
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