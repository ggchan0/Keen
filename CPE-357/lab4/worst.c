#include <stdio.h>
#include <unistd.h>

#include "mem.h"

typedef long Align;

union header
{
   struct
   {
      union header *ptr;
      size_t size;
   } s;
   Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;

static Header *morecore(size_t);

void *lab_malloc(size_t nbytes)
{
   Header *p, *prevp;
   size_t nunits;
   Header *biggest_block = NULL;
   Header *biggest_block_prev = NULL;

   nunits = (nbytes + sizeof(Header) - 1)/ sizeof(Header) + 1;
   if ((prevp = freep) == NULL)
   {
      base.s.ptr = freep = prevp = &base;
      base.s.size = 0;
   }
   
   for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr)
   {
      if (p->s.size >= nunits)
      {
         if (biggest_block == NULL || p->s.size > biggest_block->s.size) {
            biggest_block = p;
            biggest_block_prev = prevp;
         }
      }
      if (p == freep)
      {
         if (biggest_block != NULL) {
            biggest_block->s.size -= nunits;
            biggest_block += biggest_block->s.size;
            biggest_block->s.size = nunits;
            freep = biggest_block_prev;
            return (void *)(biggest_block + 1);
         }         
         if ((p = morecore(nunits)) == NULL)
         {
            return NULL;
         }
      }
   }
}

#define NALLOC 1024
static Header *morecore(size_t nu)
{
   char *cp;
   Header *up;

   if (nu < NALLOC)
   {
      nu = NALLOC;
   }
   cp = sbrk(nu * sizeof(Header));
   if (cp == (char *) -1)
   {
      return NULL;
   }
   up = (Header *) cp;
   up->s.size = nu;
   lab_free((void *)(up + 1));
   return freep;
}

void lab_free(void *ap)
{
   Header *bp, *p;

   bp = (Header *)ap - 1;
   for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
   {
      if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      {
         break;
      }
   }
   if (bp + bp->s.size == p->s.ptr)
   {
      bp->s.size += p->s.ptr->s.size;
      bp->s.ptr = p->s.ptr->s.ptr;
   }
   else
   {
      bp->s.ptr = p->s.ptr;
   }
   if (p + p->s.size == bp)
   {
      p->s.size += bp->s.size;
      p->s.ptr = bp->s.ptr;
   }
   else
   {
      p->s.ptr = bp;
   }
   freep = p;
}

