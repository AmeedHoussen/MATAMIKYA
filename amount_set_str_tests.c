#include <stdio.h>
#include "amount_set_str.h"
#include "./tests/test_utilities.h"
#include "string.h"
//this is our test for the bouns
    bool testCreate() {
        AmountSet Set1 = asCreate();
        ASSERT_TEST(Set1 != NULL);
        int size = asGetSize(Set1);
        ASSERT_TEST(size==0);
        asDestroy(Set1);
        return true;
    }
bool testDestroy() {
    AmountSet Set1 = asCreate();
    asDestroy(Set1);
    /* try to elicit undefined behavior in case matamikyaDestroy has bugs */
    AmountSet Set2 = asCreate();
    ASSERT_TEST(Set2 != NULL);
    asDestroy(Set2);
    return true;
}
bool testRegstierProducts() {
    AmountSet Set1 = asCreate();
    ASSERT_TEST(asRegister(Set1, "amed") == AS_SUCCESS);
    ASSERT_TEST(asRegister(Set1, "Omar") == AS_SUCCESS);
    ASSERT_TEST(asGetSize(Set1) == 2);
    ASSERT_TEST(strcmp(asGetFirst(Set1), "Omar") == 0);
    ASSERT_TEST(asRegister(Set1," ")== AS_SUCCESS);
    asDestroy(Set1);
            return true;

}

bool testCopyDestroy() {
    AmountSet Set1 = asCreate();
    ASSERT_TEST(asRegister(Set1, "amed") == AS_SUCCESS);
    ASSERT_TEST(asRegister(Set1, "Omar") == AS_SUCCESS);
    ASSERT_TEST(asGetSize(Set1) == 2);
    ASSERT_TEST(strcmp(asGetFirst(Set1), "Omar") == 0);
    AmountSet CopySet= asCopy(Set1);
    ASSERT_TEST(CopySet!=NULL);
    ASSERT_TEST(asRegister(CopySet, "amed") ==AS_ITEM_ALREADY_EXISTS);
    asDestroy(Set1);
    ASSERT_TEST(asContains(CopySet,"Omar")!=false);
    ASSERT_TEST(asDelete(CopySet,"amed")==AS_SUCCESS);
    ASSERT_TEST(asGetSize(CopySet)==1);
    asDestroy(CopySet);
            return true;


}
bool testAmount()
{
    AmountSet Set1 = asCreate();
    ASSERT_TEST(asRegister(Set1, "amed") == AS_SUCCESS);
    ASSERT_TEST(asRegister(Set1, "Omar") == AS_SUCCESS);
    ASSERT_TEST(asChangeAmount(Set1,"amed",18.57)==AS_SUCCESS);
    AmountSet CopySet= asCopy(Set1);
    asDestroy(Set1);
    double x=0;
    ASSERT_TEST(asGetAmount(CopySet,"amed",&x)==AS_SUCCESS);
    ASSERT_TEST(x==18.57);
    ASSERT_TEST(asChangeAmount(CopySet,"amed",-20)==AS_INSUFFICIENT_AMOUNT);
    asDestroy(CopySet);
            return true;

}

bool testiterator()
{
    AmountSet Set1 = asCreate();
    ASSERT_TEST(asRegister(Set1, "amed") == AS_SUCCESS);
    ASSERT_TEST(asRegister(Set1, "Omar") == AS_SUCCESS);
    ASSERT_TEST(asRegister(Set1, "ARAM") == AS_SUCCESS);
    ASSERT_TEST(asRegister(Set1, "madrid") == AS_SUCCESS);
    ASSERT_TEST(strcmp(asGetFirst(Set1), "ARAM") == 0);
    int count =1;
    while (asGetNext(Set1)!=NULL)
    {
        count++;
    }
    ASSERT_TEST(count==4);
    ASSERT_TEST(strcmp(asGetFirst(Set1), "ARAM") == 0);
    asDestroy(Set1);
            return true;


}
