#include "amount_set_str.h"
#include "./tests/test_utilities.h"
#include "amount_set_str_tests.h"
// our test for the bouns
int main()
{
    RUN_TEST(testCreate);
    RUN_TEST(testDestroy);
    RUN_TEST(testRegstierProducts);
    RUN_TEST(testCopyDestroy);
    RUN_TEST(testAmount);
    RUN_TEST(testiterator);
    return 0;
}