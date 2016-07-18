#ifndef __heap_h_
#define __heap_h_

typedef struct heap_s heap_t;

typedef int (*heap_less)(void *l, void *r);

struct heap_s {
	int         cap;
	int         len;
	void      **data;
	heap_less   less;
};

heap_t *heap_init(int cap, heap_less less);
int     heap_push(heap_t *heap, void *data);
void   *heap_pop(heap_t *heap);
void   *heap_remove(heap_t *heap, int index);
void    heap_fix(heap_t *heap, int index);
void    heap_destroy(heap_t *heap);



#endif
