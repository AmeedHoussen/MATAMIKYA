#include "amount_set_str.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    char *  element;
    double amount;
    struct list* next;
} *List;


struct  AmountSet_t{
    List  list;
    List iterator;
};

static char* stringDuplicate(const char* str) {
    char* copy = malloc(strlen(str) + 1);
    return copy ? strcpy(copy, str) : NULL;
}



static List createElement(const char* element)
{
    List ptr = malloc(sizeof(*ptr));
    if(ptr==NULL) {
        return NULL;
    }
    ptr->element=stringDuplicate(element);
    if (!ptr->element) {
        free(ptr);
        return NULL;
    }
    ptr->amount=0;
    ptr->next=NULL;
    return  ptr;
}


AmountSet asCreate(){

    AmountSet ptr = malloc(sizeof(*ptr));
    if(ptr==NULL) {
        return NULL;
    }
    ptr->list=NULL;
    ptr->iterator=NULL;

    return ptr;
}

void asDestroy (AmountSet ptr){
     asClear(ptr);
       free(ptr);
}

AmountSet asCopy(AmountSet set) {
    if (set == NULL) {
        return NULL;
    }
    AmountSet new_set=asCreate();
    if (new_set == NULL)
    {
        return NULL;
    }
    List list=set->list;
    List helper=new_set->list;
    AmountSetResult result=AS_SUCCESS;
    while(list && result==AS_SUCCESS){
        result=asRegister(new_set, list->element);
        if (result==AS_OUT_OF_MEMORY)
        {
            asDestroy(new_set);
            return  NULL;
        }
        helper=new_set->list;
        while(strcmp(helper->element,list->element))
        {
            helper=helper->next;
        }
        helper->amount=list->amount;
        list=list->next;
    }
    new_set->iterator=set->iterator;
    return new_set;
}

int asGetSize(AmountSet set) {
    if (set==NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    int count=0;
    for (List list = set->list;list != NULL;list =list->next) {
        count++;
    }
    return count;
}

bool asContains(AmountSet set, const char* element){

    if (set==NULL || element==NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    AmountSet ptr = set;
    List list=ptr->list;
    for (; list != NULL; list = list->next)
    {
         if (!strcmp(element,list->element)) {
             return true;
         }
     }
    return false;
    }


AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount){

    if (set==NULL || element==NULL || outAmount==NULL)
{
return AS_NULL_ARGUMENT;
}

    for (List list = set->list; list != NULL; list = list->next){
    if (strcmp(element,(list->element))==0)
{
   *outAmount = list->amount;
return AS_SUCCESS;
}
   }

   return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asRegister(AmountSet set, const char* element)
{
    if (set==NULL || element==NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    if (asContains(set,element))
    {
     return   AS_ITEM_ALREADY_EXISTS;
    }

    List head=set->list;
    List new_element = createElement (element);
    if (new_element==NULL)
    {
        return AS_OUT_OF_MEMORY;
    }
    if (head==NULL)
    {
        set->list=new_element;
        new_element->next=NULL;
        return AS_SUCCESS;
    }

    while (strcmp(element,head->element)>=0)
    {
        head=head->next;
        if(head==NULL)
            break;
    }
    if (head==set->list)
    {
        set->list=new_element;
        new_element->next=head;
        return AS_SUCCESS;
    }
    List runner =set->list;
    while (runner->next!=head)
    {
        runner=runner->next;
    }
    runner->next=new_element;
    new_element->next=head;
    return AS_SUCCESS;
}

AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount){

    if (set==NULL || element==NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    List list=set->list;
    for (;list != NULL;list =list->next){
        if (strcmp(element,(list->element))==0)
        {
            if (((list->amount)+amount)<=0)
            {
                return AS_INSUFFICIENT_AMOUNT;
            }
            else
            {
                list->amount=(list->amount)+amount;
                return AS_SUCCESS;
            }
        }
    }
    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asDelete(AmountSet set, const char* element) {
    if (set == NULL || element == NULL) {
        return AS_NULL_ARGUMENT;
    }
    if(!asContains(set,element)){
        return AS_ITEM_DOES_NOT_EXIST;
    }

    List ptr = set->list;

    while (strcmp(ptr->element,element)!=0){
        ptr=ptr->next;
    }

    if (ptr == set->list)
    {
        set->list=ptr->next;
        free(ptr->element);
        free(ptr);
        return AS_SUCCESS;
    }
    List head=set->list;

    while((head->next)!=ptr) {
        head=head->next;
    }

    head->next=ptr->next;
    free(ptr->element);
    free(ptr);
        return AS_SUCCESS;
    }


AmountSetResult asClear(AmountSet set){
    if(set==NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    List head = set->list;
    List helper = set->list;
    while (head)
    {
        head=head->next;
        free(helper->element);
        free(helper);
        helper=head;
    }
    set->list=NULL;
    set->iterator=NULL;
        return AS_SUCCESS;
}

char* asGetFirst(AmountSet set)
{
     if( (!set) || (asGetSize(set)==0)) return NULL;
     AmountSet ptr = set;
     set->iterator = ptr->list;
    return (set->iterator->element);
}

char* asGetNext(AmountSet set)
{
    if (set==NULL)
    {
        return NULL;
    }

    if((set->iterator->next)==NULL ||  (asGetSize(set)==0) )
    {
        return NULL;

    }
    set->iterator= set->iterator->next;

    return (set->iterator->element);
}
