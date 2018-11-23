#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/////////////////////////////////////////////
struct DNODE {
    int val;
    struct DNODE *prev, *next;
};

/////////////////////////////////////////////
void freelist(struct DNODE *head)
{
    struct DNODE *next;
    while ( head ) {
        next = head->next;
        free(head);
        head = next;
    }
}

/////////////////////////////////////////////
void printlist(struct DNODE *head)
{
    int i = 0;
    while ( head ) {
        fprintf(stdout, "[%6d] : %d\n", i, head->val);
        head = head->next;
        i++;
    }
}

/////////////////////////////////////////////
// List QUICK SORT
// You may define other functions if necessary
struct DNODE *partition(struct DNODE *head)
{
    struct DNODE *pivot, *L, *R, *tempL, *tempR;

    R = tempR = NULL;
    pivot = L = tempL = head;
    head = head->next;

    while(head) {
        if((pivot->val) > (head->val)) {
            tempL = head;
            head = head->next;
        }
        else {
            if(head->next != NULL) {
                tempL->next = head->next;
                head->next->prev = tempL;

                if(R == NULL) {
                    R = tempR = head;
                    head->prev = NULL;
                    head = head->next;
                    tempR->next = NULL;
                }
                else {
                    tempR->next = head;
                    head->prev = tempR;
                    tempR = tempR->next;
                    head = head->next;
                    tempR->next = NULL;
                }
            }
            else {
                tempL->next = NULL;
                head->prev = tempR;
                if(tempR == NULL) {
                    R = tempR = head;
                }
                else {
                    tempR->next = head;
                }
                head = head->next;
            }
        }
    }

    if(R == NULL) {
        L = L->next;
        R = pivot;
        pivot->next = NULL;
    }

    L->prev = R;

    return L;
}

struct DNODE *concatenate(struct DNODE *L, struct DNODE *R) {
    struct DNODE *temp;

    if(L->next == NULL) {
        L->prev = L;
        L->next = L;
    }
    if(R->next == NULL) {
        R->prev = R;
        R->next = R;
    }

    L->prev->next = R;
    R->prev->next = L;
    temp = L->prev;
    L->prev = R->prev;
    R->prev = temp;

    return L;
}

struct DNODE *quicksort_list(struct DNODE *head)
{
    struct DNODE *sorted, *L, *R;

    if((head != NULL) && ((head->next) != NULL)) {
        L = partition(head);

        R = L->prev;
        L->prev = NULL;

        L = quicksort_list(L);
        R = quicksort_list(R);

        sorted = concatenate(L, R);

        return sorted;
    }
    else
        return head;
}

int main()
{
    FILE *fp;
    fp = fopen("input100000.txt", "r");
    int i, N;
    int val;
    struct DNODE *head, *tail, *cur;

    fprintf(stderr, "How many numbers? ");
    fscanf(fp, "%d", &N);
    printf("%d\n", N);

    head = tail = NULL;
    for (i=0; i<N; i++) {
        if ( fscanf(fp, "%d", &val) != 1 ) {
            fprintf(stderr, "cannot read integer from standard input.\n");
            freelist(head);
            exit(0);
        }
        else {
            cur = (struct DNODE*) malloc(sizeof(struct DNODE));
            cur->val = val;
            cur->next = NULL;

            if ( !head ) {
                cur->prev = NULL;
                head = tail = cur;
            }
            else {
                tail->next = cur;
                cur->prev = tail;
                tail = cur;
            }
        }
    }
    head = quicksort_list(head);

    head->prev->next = NULL;
    head->prev = NULL;

////////////////////////////////////////////////////////////////////
//print median value
    struct DNODE *temp;
    temp = head;
////////////////////////////////////////////////////////////////////

    printlist(head);
////////////////////////////////////////////////////////////////////
//print median value
    head = temp;
    i = 0;
    if(N%2 == 0) {
        while ( head ) {
            if(i == N/2-1) {
                fprintf(stdout, "[%6d] : %d\n", i, head->val);
                break;
            }
            head = head->next;
            i++;
        }
    }
    else {
        while ( head ) {
            if(i == N/2) {
                fprintf(stdout, "[%6d] : %d\n", i, head->val);
                break;
            }
            head = head->next;
            i++;
        }
    }
////////////////////////////////////////////////////////////////////
    fclose(fp);

    // free memory
    freelist(head);

    return 0;
}
