#include <stdio.h>
#include <string.h>
#include "matamikya.h"
#include <malloc.h>
#include <math.h>
#include "matamikya_print.h"
#define errorRange 0.001
#define halfIntgerCheckRange 0.5





typedef struct List
{
    char *  element;
    unsigned int id;
    double amount;
    MatamikyaAmountType type;
    MtmProductData product_data;
    double prod_price;
    double profit;
    MtmGetProductPrice price_func;
    MtmFreeData free_func;
    struct List* next_product;
}*List;

typedef struct Simple_List
{
    char *  element;
    unsigned int id_product;
    double amount;
    double prod_price;
    MatamikyaAmountType amount_tybe;
    struct Simple_List* next;
}*Simple_list;


typedef struct Set
{
    Simple_list list;
    unsigned int id_order;
    unsigned int size;
    struct Set* next_order;
}*Set ;


struct Matamikya_t
{
    List product;
    unsigned int product_num ;
    Set Orders;
    unsigned int orders_num ;
};

static char* stringDuplicate(const char* str) {
    char* copy = malloc(strlen(str) + 1);
    return copy ? strcpy(copy, str) : NULL;
}

static List findMaxProfit ( List ptr)

{
    List first=ptr;
    List runner=ptr;
    for(;runner!=NULL;runner=runner->next_product)
    {
        if (runner->profit > first->profit)
        {
            first=runner;
        }
        if ( runner->profit == first->profit && runner->id < first->id)
        {
            first=runner;
        }
    }

    return first;

}

static List createProduct(const char* name)
{
    List ptr = malloc(sizeof(*ptr));
    if(ptr==NULL) {
        return NULL;
    }
    ptr->element= stringDuplicate(name);
    if( ptr->element==NULL) {
        return NULL;
    }
    return ptr;

}

static Set createOrder()
{

    Set ptr = malloc(sizeof(*ptr));
    if(ptr==NULL) {
        return NULL;
    }
    ptr->list=NULL;
    ptr->id_order=0;
    ptr->size=0;
    return  ptr;
}

bool asContains(List list,const unsigned int id )
{

    if (list==NULL)
    {
        return false;
    }
    List ptr=list;
    for (; ptr != NULL; ptr =ptr->next_product)
    {
        if (id==(ptr->id)) {
            return true;
        }
    }
    return false;
}


static bool asContainsOrder( Set order,const unsigned int id ) {
    Simple_list head = order->list;
    if(head==NULL)
    {
        return false;

    }
    for (; head != NULL; head = head->next) {
        if (id == (head->id_product)) {
            return true;
        }
    }
    return false;
}


static void removeFromOrder(Set Order ,const unsigned int id  )
{
    Simple_list helper=Order->list;
    Simple_list runner=Order->list;
    while (helper->id_product!=id)
    {
        helper=helper->next;
    }
    if (runner==helper)
    {
        Order->list=runner->next;
        free(runner->element);
        free(runner);
        Order->size=(Order->size)-1;
    }
    else {
        while (runner->next != helper) {
            runner = runner->next;
        }


        runner->next = helper->next;
        free(helper->element);
        free(helper);
        Order->size = (Order->size) - 1;
    }
}

static List findProduct(Matamikya matamikya , const double id )
{
    List ptr=matamikya->product;
    while (ptr)
    {
        if(ptr->id==id)
        {
            break;
        }
        ptr=ptr->next_product;
    }
    return ptr;
}


static bool checkSells (Matamikya matamikya)
{
    List head=matamikya->product;
    for(;head!=NULL;head=head->next_product)
    {
        if (head->profit>0)
        {
            return true;
        }

    }
    return false;
}

static Simple_list addToOrder (const unsigned int id ,Matamikya matamikya, const double amount)
{
    List helper= findProduct(matamikya,id);
    MtmProductData helper_data=helper->product_data;
    MtmGetProductPrice earn=helper->price_func;
    Simple_list ptr = malloc(sizeof(*ptr));
    if(ptr==NULL) {
        free(ptr);
        return NULL;
    }
    List ptr_helper=matamikya->product;
    while (ptr_helper->id!=id)
    {
        ptr_helper=ptr_helper->next_product;
    }
    ptr->element = stringDuplicate(ptr_helper->element);
    if( ptr->element==NULL) {
        return NULL;
    }
    ptr->amount=amount;
    ptr->id_product=id;
    ptr->prod_price=earn(helper_data,amount);
    ptr->amount_tybe=ptr_helper->type;
    ptr->next=NULL;
    return ptr;


}

static bool chackName (const char* name)
{
    char* check=stringDuplicate(name);
    char first_letter=check[0];
    if ( (first_letter) >= 'A' && (first_letter) <= 'Z')
    {
        free(check);
        return true;
    }
    if ( (first_letter) >= 'a' && (first_letter) <= 'z') {
        free(check);
        return true;
    }
    if ( (first_letter) >= '1' && (first_letter) <= '9'){
        free(check);
        return true;
    }
    free(check);
    return false;
}

static bool checkVaildAmount(const double amount, MatamikyaAmountType tybe )
{
    if (tybe==MATAMIKYA_ANY_AMOUNT)
    {
        return true;
    }
    else if(tybe==MATAMIKYA_INTEGER_AMOUNT )
    {
        double amount_check = fabs(amount - (int)amount);
        if (amount_check <=  || amount_check >=  (1-errorRange) )
        {
            return true;
        }
        return false;
    }
    else if(tybe==MATAMIKYA_HALF_INTEGER_AMOUNT) {
        double amount_check = fabs(amount - (int) amount);
        if (amount_check >= ( halfIntgerCheckRange-errorRange )&& amount_check <=  (1-errorRange) )
        {
            if (fabs(amount_check - halfIntgerCheckRange) <= errorRange)
            {
                return true;
            }
        }
        else if (amount_check <= errorRange)
        {

            return true;
        }
        else if (amount_check >= (1-errorRange) )
        {
            if (fabs(amount_check - 1) <= errorRange)
            {
                return true;
            }
        }
        return false;
    }
    return false;
}

static void removeFromOrders( Matamikya matamikya ,const unsigned int id  )
{

    Set helper=matamikya->Orders;

    for(;helper!=NULL ;helper=helper->next_order)
    {
        if (asContainsOrder(helper,id))
        {
            Simple_list head=helper->list;
            Simple_list runner=helper->list;
            while(head->id_product!=id)
            {
                head=head->next;
            }
            if (head==runner)
            {
                helper->list=head->next;
                free(head->element);
                free(head);
                helper->size=(helper->size)-1;
                return;
            }

            {
                while (runner->next!=head)
                {

                    runner=runner->next;
                }
                runner->next=head->next;
                free(head->element);
                free(head);
                helper->size=(helper->size)-1;
            }
        }
        else continue;

    }

}
static void calucateEarns(Matamikya matamikya , Set ptr )
{
    for (Simple_list helper= ptr->list; helper != NULL; helper = helper->next) {
        for (List runner = matamikya->product; runner != NULL; runner = runner->next_product)  {
            if (runner->id == helper->id_product) {
                if (mtmChangeProductAmount(matamikya, runner->id, -(helper->amount)) == MATAMIKYA_SUCCESS) {
                    List profit=findProduct(matamikya,runner->id);
                    MtmProductData data=profit->product_data;
                    MtmGetProductPrice earn=profit->price_func;
                    runner->profit += earn(data,helper->amount) ;
                }
            }
        }
    }
}

Matamikya matamikyaCreate()
{
    Matamikya ptr = malloc(sizeof(*ptr));
    if(ptr==NULL) {
        return NULL;
    }
    ptr->product=NULL;
    ptr->product_num=0;
    ptr->orders_num=0;
    ptr->Orders=NULL;
    return ptr;
}


void matamikyaDestroy(Matamikya matamikya)
{
    if (matamikya==NULL)
    {
        return;
    }
    if((matamikya->product_num)>0) {
        List ptr=matamikya->product;
        List helper=matamikya->product;
        while (ptr) {
            MtmFreeData freeDate=ptr->free_func;
            ptr = ptr->next_product;
            free(helper->element);
            if (helper->product_data==NULL)
            {
                free(helper->product_data);
            }
            else {
                freeDate(helper->product_data);
            }
            free(helper);
            helper = ptr;
        }
    }

    if((matamikya->orders_num)>0) {
        Set toDelete=matamikya->Orders;
        Set toDeleteHelper=matamikya->Orders;
        while (toDelete) {
            toDelete = toDelete->next_order;
            mtmCancelOrder(matamikya, toDeleteHelper->id_order);
            toDeleteHelper = toDelete;
        }
    }
    free(matamikya);
}


MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    if (matamikya==NULL || name== NULL || customData ==NULL || copyData==NULL || freeData==NULL || prodPrice==NULL )
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(!chackName(name))
    {
        return MATAMIKYA_INVALID_NAME;
    }
    if (!checkVaildAmount(amount,amountType) || amount<0)
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }

    if (asContains(matamikya->product,id))
    {
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    if ( id<0)
    {
        return MATAMIKYA_SUCCESS;
    }
    List new_product=createProduct(name);
    if (new_product->element==NULL)
    {
        free(new_product->element);
        free(new_product);
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    new_product->id=id;
    new_product->amount=amount;
    new_product->type=amountType;
    new_product->product_data=copyData(customData);
    if (new_product->product_data==NULL)
    {
        free(new_product->element);
        freeData(new_product->product_data);
        free(new_product);
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    new_product->prod_price=prodPrice(customData,1);
    new_product->price_func=prodPrice;
    new_product->free_func=freeData;
    new_product->profit=0;

    List helper=matamikya->product;
    if (helper==NULL)
    {
        matamikya->product=new_product;
        new_product->next_product=NULL;
        matamikya->product_num+=1;
        return MATAMIKYA_SUCCESS;
    }

    while (helper)
    {
        if ((helper->id)>(new_product->id))
        {
            break;
        }
        helper=helper->next_product;
    }
    List helper_runner=matamikya->product;
    while (helper_runner->next_product!=helper)
    {
        helper_runner=helper_runner->next_product;
    }


    helper_runner->next_product=new_product;
    new_product->next_product = helper;
    matamikya->product_num+=1;
    return MATAMIKYA_SUCCESS;
}


MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{

    if (matamikya==NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    if (!asContains(matamikya->product,id))
    {
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }

    List list=matamikya->product;
    for (;list != NULL;list =list->next_product){
        if (id==(list->id))
        {
            if (!checkVaildAmount(amount,list->type))
            {
                return MATAMIKYA_INVALID_AMOUNT;
            }
            if (!checkVaildAmount(((list->amount)+amount),list->type))
            {
                return MATAMIKYA_INVALID_AMOUNT;
            }
            if (((list->amount)+amount)<0)
            {
                return MATAMIKYA_INSUFFICIENT_AMOUNT;
            }
            else
            {
                list->amount=(list->amount)+amount;
                return MATAMIKYA_SUCCESS;
            }
        }
    }
    return MATAMIKYA_PRODUCT_NOT_EXIST;
}


MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
    if (matamikya == NULL|| id<0 ) { //what about id
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if (!asContains(matamikya->product,id))
    {
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }

    List head = matamikya->product;
    List helper= matamikya->product;

    while ( head->id != id )
    {
        head=head->next_product;
    }
    if (head==helper)
    {
        matamikya->product=matamikya->product->next_product;
        free(head->element);
        free(head->product_data);
        free(head);
        removeFromOrders(matamikya,id);
        matamikya->product_num-=1;
        return MATAMIKYA_SUCCESS;
    }
    while ((helper->next_product)!=(head))
    {
        helper=helper->next_product;
    }
    helper->next_product=head->next_product;
    free(head->element);
    MtmFreeData freeData=head->free_func;
    freeData(head->product_data);
    free(head);

    removeFromOrders(matamikya,id);
    matamikya->product_num-=1;
    return MATAMIKYA_SUCCESS;

}

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{

    Set new_order=createOrder(); // from zero or 1?
    if (new_order==NULL || matamikya==NULL)
    {
        free(new_order);
        return 0;
    }

    Set head= matamikya->Orders;
    if(head==NULL)
    {
        matamikya->Orders=new_order;
        new_order->id_order = 1;
        new_order->next_order=NULL;
        matamikya->orders_num+=1;
        return 1;
    }
    while(head->next_order!=NULL)
    {
        head=head->next_order;
    }
    head->next_order=new_order;
    new_order->id_order = (head->id_order)+1;
    new_order->next_order=NULL;
    matamikya->orders_num+=1;
    return (head->id_order)+1;
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya , const unsigned int
orderId, const unsigned int productId, const double amount) {
    if (matamikya == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Set head = matamikya->Orders;
    while (head != NULL) {
        if (head->id_order == orderId) {
            break;
        }
        head = head->next_order;
    }

    if (head == NULL) {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }

    Simple_list helper = head->list;
    if (asContainsOrder(head, productId)) {
        while (helper->id_product != productId) {
            helper = helper->next;
        }

        if (!checkVaildAmount(amount, helper->amount_tybe)) {
            return MATAMIKYA_INVALID_AMOUNT;
        }

        if (((helper->amount) + amount) <= 0) {
            removeFromOrder(head, helper->id_product);
            head->size = (head->size) - 1;
            return MATAMIKYA_SUCCESS;
        }
        if (!checkVaildAmount((amount+helper->amount), helper->amount_tybe)) {
            return MATAMIKYA_INVALID_AMOUNT;
        }
        Simple_list ptr = addToOrder(productId, matamikya, ((helper->amount) + amount));
        if (ptr==NULL)
        {
            return MATAMIKYA_OUT_OF_MEMORY;
        }
        helper->amount = ptr->amount;
        helper->prod_price = (ptr->prod_price);
        free(ptr->element);
        free(ptr);
        return MATAMIKYA_SUCCESS;
    }
    else
    {
        if (!asContains(matamikya->product, productId)) {
            return MATAMIKYA_PRODUCT_NOT_EXIST;
        }
        Simple_list ptr = addToOrder(productId, matamikya, amount);
        if (!checkVaildAmount(amount, ptr->amount_tybe)) {
            free(ptr->element);
            free(ptr);
            return MATAMIKYA_INVALID_AMOUNT;
        }
        if (amount <= 0) {
            return MATAMIKYA_SUCCESS;
        }

        if (helper == NULL) {
            head->list = ptr;
            ptr->next = NULL;
            (head->size)++;
            return MATAMIKYA_SUCCESS;
        }
        while (helper) {
            if ((helper->id_product) > ptr->id_product) {
                break;
            }
            helper = helper->next;
        }
        if (helper == head->list) {
            head->list = ptr;
            ptr->next = helper;
            (head->size)++;
            return MATAMIKYA_SUCCESS;

        }
        Simple_list runner = head->list;
        while (runner->next != helper) {
            runner = runner->next;
        }
        runner->next = ptr;
        ptr->next = helper;
        (head->size)++;
        return MATAMIKYA_SUCCESS;

    }
}



MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int
orderId) {
    if (matamikya==NULL)
    {
        return  MATAMIKYA_NULL_ARGUMENT;
    }
    Set ptr = matamikya->Orders;
    while (ptr) {
        if (ptr->id_order == orderId) {
            break;
        }
        ptr = ptr->next_order;
    }
    if (ptr == NULL) {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    if (ptr->size==0)
    {

        if (mtmCancelOrder(matamikya, orderId) == MATAMIKYA_SUCCESS) {
            return MATAMIKYA_SUCCESS;
        }
    }

    Simple_list helper = ptr->list;


    for (; helper != NULL; helper = helper->next) {
        for (List runner= matamikya->product; runner != NULL; runner = runner->next_product) {
            if (runner->id == helper->id_product) {
                if (((runner->amount) - (helper->amount)) < 0) {
                    return MATAMIKYA_INSUFFICIENT_AMOUNT;
                }

            } else continue;
        }
    }


    calucateEarns(matamikya,ptr);

    if (mtmCancelOrder(matamikya, orderId) == MATAMIKYA_SUCCESS)

        return MATAMIKYA_SUCCESS;
    return MATAMIKYA_OUT_OF_MEMORY;
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int
orderId)
{
    if (matamikya==NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Set head=matamikya->Orders;
    while (head)
    {
        if (head->id_order==orderId)
        {
            break;
        }
        head=head->next_order;
    }
    if (head==NULL) {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }


    Set helper = matamikya->Orders;
    Simple_list runner=head->list;
    Simple_list toDelete = runner;
    if(helper==head)
    {
        matamikya->Orders=head->next_order;
        if (head->size==0)
        {
            free(head);
            return MATAMIKYA_SUCCESS;

        }

        while(runner)
        {
            runner = runner->next;
            free(toDelete->element);
            free((toDelete));
            toDelete=runner;
        }
        free(helper);
        matamikya->orders_num-=1;
        return MATAMIKYA_SUCCESS;
    }
    while (helper->next_order!=head)
    {
        helper=helper->next_order;
    }

    while (runner)
    {
        runner = runner->next;
        free(toDelete->element);
        free(toDelete);
        toDelete=runner;
    }
    helper->next_order=head->next_order;
    free(head);
    matamikya->orders_num-=1;
    return MATAMIKYA_SUCCESS;

}


MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output)
{
    if (matamikya==NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if (!output) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    List helper=matamikya->product;
    fprintf(output,"Inventory Status:\n");
    for (;helper!=NULL; helper=helper->next_product) {
        mtmPrintProductDetails(helper->element,helper->id,helper->amount,helper->prod_price,output);
    }
    return MATAMIKYA_SUCCESS;
}


MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int
orderId, FILE *output)
{
    if (matamikya==NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if (!output) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Set head=matamikya->Orders;
    if (head==NULL)
    {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    while (head->next_order!=NULL )
    {
        if (head->id_order==orderId)
        {
            break;
        }
        head=head->next_order;
    }
    if (head->id_order!=orderId) {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }

    double total_price=0;
    mtmPrintOrderHeading(orderId,output);
    Simple_list list = head->list;
    for (;list!=NULL; list=list->next) {
        mtmPrintProductDetails(list->element,list->id_product,list->amount,list->prod_price,output);
        total_price+=list->prod_price;
    }
    mtmPrintOrderSummary(total_price,output);
    return MATAMIKYA_SUCCESS;

}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{

    if (matamikya==NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if (!output) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    fprintf(output,"Best Selling Product:\n");
    if (!checkSells(matamikya))
    {
        fprintf(output,"none\n");
        return MATAMIKYA_SUCCESS;
    }

    List   best_sell=findMaxProfit(matamikya->product);
    mtmPrintIncomeLine(best_sell->element, best_sell->id , best_sell->profit ,output);
    return MATAMIKYA_SUCCESS;

}
