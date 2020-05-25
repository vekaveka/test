#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _slist_tag 
{
    struct _slist_tag *next;
    int data;
} slist_t;

slist_t *slist_head=NULL;

void slist_destroy(slist_t **h)
{
    slist_t *local_h = NULL;
    if(!h || !*h)
    {
        return;
    }
    local_h=*h;
    slist_t *p=local_h->next;
    while(local_h)
    {
        free(local_h);
        
        local_h=p;
        if(p)
        {
           p=p->next;
        }
    }

    *h=NULL;
    return;
}

void slist_dump(const char *prefix, slist_t *h)
{
    slist_t *p=h;
    printf("%s slist head:\n", prefix?prefix:"");
    while(p)
    {
        printf("%d ", p->data);
        p=p->next;
    }
    printf("\n");

    return;
}


int slist_generate(slist_t **h, int count)
{
    int rc = -1;
    if (count <=0 || !h)
    {
        rc = -1;
        goto out;
    }

    int i=0;
    for(i=0; i<count;i++)
    {
        slist_t *s = malloc(sizeof(slist_t));
        if(!s)
        {
            rc=-1;
            goto out;
        }
        s->data=i;
        if(*h)
        {
            s->next=(*h);
        }

        *h=s;
        
    }

    //slist_dump("slist generate: ", *h);
    return 0;
out:
    
    slist_destroy(h);
    return rc;
}


int  slist_revert_newlist(slist_t **h)
{
    slist_t *new_h =NULL;
    slist_t *p=*h;
    while(p)
    {
        //slist_dump("new list: ", new_h);
        //slist_dump("old list: ", *h);
        *h=p->next;
        p->next=new_h;
        new_h=p;
        p=*h;
    }

    *h=new_h;
    return 0;
}

int  slist_revert_local(slist_t **h)
{
    slist_t *p=NULL;
    slist_t *new_h=NULL;
    p=*h;
    if(!p)
    {
        return -1;
    }

    new_h = p;
    *h=p->next;
    p->next=NULL;
    while(*h)
    {
        //slist_dump("new list: ", new_h);
        //slist_dump("old list: ", *h);
        p = *h;
        *h=p->next;
        p->next=new_h;
        new_h=p;
    }
    
    *h=new_h;
    return 0;
}

void slist_test()
{
    slist_generate(&slist_head, 8);
    slist_dump("origin list:", slist_head);
    slist_revert_newlist(&slist_head);
    slist_dump("newlist reverted list:", slist_head);
    slist_revert_local(&slist_head);
    slist_dump("local reverted list:", slist_head);
    slist_destroy(&slist_head);
    slist_dump("destroyed list:", slist_head);
    return;
}

int str_revert(char *s, size_t len)
{
    int i = 0;
    char c = 0;
    if(!s)
    {
       return -1;
    }

    for(i=0;i<len/2;i++)
    {
        c=s[i];
        s[i]=s[len-i-1];
        s[len-i-1]=c;
    }

    return 0;
}

void str_test()
{
    char buf[]={'1', '2', '3', '4', '5', '6', '7'};
    printf("income str: %s\n", buf);
    str_revert(buf, strlen(buf));
    printf("reverted str: %s\n", buf);
    return;
}

int main(int argc, char **argv)
{
    slist_test();
    str_test();
}


