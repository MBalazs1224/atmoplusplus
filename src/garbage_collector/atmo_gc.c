#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HEAP_CHUNK_SIZE 1024 // 1KB

typedef struct Header {
    size_t size;            // Total block size (header + data)
    unsigned int marked;    // Mark bit for GC
    const char* descriptor; // Descriptor string for pointers
    struct Header* next;    // Next block in free list
    int is_free;            // Whether the block is free
} Header;


static Header* free_list = NULL;
static void* heap_start = NULL;
static void* heap_end = NULL;

void initialize_heap() {
    heap_start = malloc(0);
    void* mem = malloc(HEAP_CHUNK_SIZE);
    if (mem == (void*)-1) {
        perror("malloc");
        exit(1);
    }
    heap_end = malloc(0);
    Header* header = (Header*)mem;
    header->size = HEAP_CHUNK_SIZE;
    header->marked = 0;
    header->descriptor = NULL;
    header->next = NULL;
    header->is_free = 1;
    free_list = header;
}

size_t calculateDataSize(const char* descriptor) {
    size_t size = 0;
    for (; *descriptor; ++descriptor) {
        switch (*descriptor) {
            case 'p': size += sizeof(void*); break; // The field is a pointer, 8 bytes
            case 'n': size += sizeof(int); break; // The fiels is non-pointer, 4 bytes
            default: 
                fprintf(stderr, "Unknown descriptor '%c'\n", *descriptor);
                exit(1);
        }
    }
    return size;
}

void gcCollect();

void* initArray(int data_size)
{
    size_t total_size = sizeof(Header) + data_size;

    // Find a good free block
    Header **prev = &free_list, *curr = free_list;
    while (curr) {
        if (curr->size >= total_size) {
            // Split the block if possible
            if (curr->size > total_size + sizeof(Header)) {
                Header* new_block = (Header*)((char*)curr + total_size);
                new_block->size = curr->size - total_size;
                new_block->is_free = 1;
                new_block->next = curr->next;
                new_block->marked = 0;
                new_block->descriptor = NULL;
                *prev = new_block;
                curr->size = total_size;
            } else {
                *prev = curr->next;
            }
            curr->is_free = 0;
            curr->marked = 0;
            curr->descriptor = "array"; // FIXME: Arrays should have their own descriptors
            curr->next = NULL;
            return (void*)(curr + 1);
        }
        prev = &curr->next;
        curr = curr->next;
    }

    // No block found, trigger GC then retry
    gcCollect();
    curr = free_list;
    while (curr) {
        if (curr->size >= total_size) {
             // Split the block if possible
             if (curr->size > total_size + sizeof(Header)) {
                Header* new_block = (Header*)((char*)curr + total_size);
                new_block->size = curr->size - total_size;
                new_block->is_free = 1;
                new_block->next = curr->next;
                new_block->marked = 0;
                new_block->descriptor = NULL;
                *prev = new_block;
                curr->size = total_size;
            } else {
                *prev = curr->next;
            }
            curr->is_free = 0;
            curr->marked = 0;
            curr->descriptor = "array"; // FIXME: Arrays should have their own descriptors
            curr->next = NULL;
            return (void*)(curr + 1);
        }
        curr = curr->next;
    }

    // Still no memory, request more from OS
    void* mem = malloc(HEAP_CHUNK_SIZE);
    if (mem == (void*)-1)
    {
        return NULL; // Out of memory
    }
    heap_end = malloc(0);
    Header* new_block = (Header*)mem;
    new_block->size = HEAP_CHUNK_SIZE;
    new_block->is_free = 1;
    new_block->next = free_list;
    new_block->marked = 0;
    new_block->descriptor = NULL;
    free_list = new_block;

    return initArray(data_size); // Try to allocate again
}

void* heapAlloc(const char* descriptor) {
    if (heap_start == NULL) initialize_heap(); // If the heap is not allocated yet

    size_t data_size = calculateDataSize(descriptor);
    size_t total_size = sizeof(Header) + data_size;

    // Find a good free block
    Header **prev = &free_list, *curr = free_list;
    while (curr) {
        if (curr->size >= total_size) {
            // Split the block if possible
            if (curr->size > total_size + sizeof(Header)) {
                Header* new_block = (Header*)((char*)curr + total_size);
                new_block->size = curr->size - total_size;
                new_block->is_free = 1;
                new_block->next = curr->next;
                new_block->marked = 0;
                new_block->descriptor = NULL;
                *prev = new_block;
                curr->size = total_size;
            } else {
                *prev = curr->next;
            }
            curr->is_free = 0;
            curr->marked = 0;
            curr->descriptor = descriptor;
            curr->next = NULL;
            return (void*)(curr + 1);
        }
        prev = &curr->next;
        curr = curr->next;
    }

    // No block found, trigger GC then retry
    gcCollect();
    curr = free_list;
    while (curr) {
        if (curr->size >= total_size) {
             // Split the block if possible
             if (curr->size > total_size + sizeof(Header)) {
                Header* new_block = (Header*)((char*)curr + total_size);
                new_block->size = curr->size - total_size;
                new_block->is_free = 1;
                new_block->next = curr->next;
                new_block->marked = 0;
                new_block->descriptor = NULL;
                *prev = new_block;
                curr->size = total_size;
            } else {
                *prev = curr->next;
            }
            curr->is_free = 0;
            curr->marked = 0;
            curr->descriptor = descriptor;
            curr->next = NULL;
            return (void*)(curr + 1);
        }
        curr = curr->next;
    }

    // Still no memory, request more from OS
    void* mem = malloc(HEAP_CHUNK_SIZE);
    if (mem == (void*)-1)
    {
        return NULL; // Out of memory
    }
    heap_end = malloc(0);
    Header* new_block = (Header*)mem;
    new_block->size = HEAP_CHUNK_SIZE;
    new_block->is_free = 1;
    new_block->next = free_list;
    new_block->marked = 0;
    new_block->descriptor = NULL;
    free_list = new_block;

    return heapAlloc(descriptor); // Try to allocate again
}

// ------------------------------- GARBAGE COLLECTOR -------------------------------

typedef struct Root {
    void** root;
    struct Root* next;
} Root;

static Root* roots = NULL;

void GCRegisterRoot(void** root) {
    Root* r = malloc(sizeof(Root));
    if (!r) exit(1);
    r->root = root;
    r->next = roots;
    roots = r;
}

void mark(Header* header) {
    if (header->marked || header->is_free) return;
    header->marked = 1;
    char* data = (char*)(header + 1);
    const char* desc = header->descriptor;
    for (; *desc; ++desc) {
        if (*desc == 'p') {
            void* ptr = *(void**)data;
            if (ptr) {
                Header* child = (Header*)ptr - 1;
                mark(child);
            }
            data += sizeof(void*);
        } else {
            // Advance past non-pointer fields
            switch (*desc) {
                case 'p' : data += sizeof(void*); break;
                case 'n': data += sizeof(int); break;
            }
        }
    }
}

void markAll() {
    Root* curr = roots;
    while (curr) {
        if (*curr->root) {
            Header* hdr = (Header*)(*curr->root) - 1;
            mark(hdr);
        }
        curr = curr->next;
    }
}

void sweep() {
    Header* curr = (Header*)heap_start;
    while ((char*)curr < (char*)heap_end) {
        if (!curr->is_free) {
            if (!curr->marked) {
                curr->is_free = 1;
                curr->descriptor = NULL;
                curr->next = free_list;
                free_list = curr;
            } else {
                curr->marked = 0;
            }
        }
        curr = (Header*)((char*)curr + curr->size);
    }
}

void gcCollect() {
    markAll();
    sweep();
}