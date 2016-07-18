#include "heap.h"
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#define MEM_ALLOC_C(var, n) (var = calloc(n, sizeof(*var)))

static void heap_up(heap_t *heap, int index);
static void heap_down(heap_t *heap, int index, int len);
static void heap_swap(heap_t *heap, int i, int j);

heap_t *heap_init(int cap, heap_less less)
{
	if (cap <= 0 || less == NULL) {
		return NULL;
	}

	heap_t *heap;

	if (MEM_ALLOC_C(heap, 1) == NULL || MEM_ALLOC_C(heap->data, cap) == NULL) {
		goto failed;
	}

    heap->cap = cap;
    heap->len = 0;
    heap->less = less;

    return heap;

failed:
	if (heap->data) free(heap->data);
	if (heap) free(heap);

	return NULL;
}

int heap_push(heap_t *heap, void *data)
{
	if (heap->len + 1 > heap->cap) {
		return -1;
	}

	heap->data[heap->len++] = data;
	heap_up(heap, heap->len - 1);

	return 0;
}

void *heap_pop(heap_t *heap)
{
	if (heap->len == 0) {
		return NULL;
	}

	int n = heap->len - 1;

	heap_swap(heap, 0, n);
	heap_down(heap, 0, n);

	return heap->data[--heap->len];
}

void *heap_remove(heap_t *heap, int index)
{
	int n = heap->len - 1;

	if (n != index) {
		heap_swap(heap, index, n);
		heap_down(heap, index, n);
		heap_up(heap, index);
	}

	return heap_pop(heap);
}

void heap_fix(heap_t *heap, int index)
{
	heap_down(heap, index, heap->len);
	heap_up(heap, index);
}

void heap_destroy(heap_t *heap)
{
	if (heap->data) free(heap->data);
	if (heap) free(heap);
}

static void heap_up(heap_t *heap, int index)
{
	int i;

	for (;;) {
		i = (index - 1) / 2;
		
		if (i == index || !heap->less(heap->data[index], heap->data[i])) {
			break;
		}

		heap_swap(heap, i, index);
		index = i;
	}
}

static void heap_down(heap_t *heap, int index, int len)
{
	int i;

	for (;;) {
		i = 2 * index + 1;
		
		if (i >= len || i < 0) {
			break;
		}

		if (i + 1 < len && !heap->less(heap->data[i], heap->data[i + 1])) {
			i = i + 1;
		}

		if (!heap->less(heap->data[i], heap->data[index])) {
			break;
		}

		heap_swap(heap, index, i);
		index = i;
	}
}

static void heap_swap(heap_t *heap, int i, int j)
{
	void *tmp;

	tmp = heap->data[i];
	heap->data[i] = heap->data[j];
	heap->data[j] = tmp;
}


