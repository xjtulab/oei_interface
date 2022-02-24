/*
 * FILE: osapi_heap.h - Heap interface definition
 *
 */
/*ce
 * \file 
 * \brief Heap interface definition
 */
#ifndef osapi_heap_h
#define osapi_heap_h

#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define TYPE OEI_UINT32

/*e \defgroup OSAPI_HeapClass OSAPI Heap
 *  \ingroup OSAPIModule
 */

/*ce \ingroup OSAPI_HeapClass
  @brief The OSAPIAlignment is the alignment in bytes; an address
  is aligned when it is a positive integer multiple of the alignment
*/
typedef OEI_INT32 OSAPI_Alignment_T;

/*e \ingroup OSAPIAlignmentClass
  @brief Certain methods allow a default alignment: this should
  be an alignment that follows the "malloc" alignment of
  the architecture (aligned sufficiently to store any C-structure
  efficiently).
*/
#define OSAPI_ALIGNMENT_DEFAULT (-1)

#ifndef OEI_CERT
extern OSAPIDllVariable OEI_BOOL OSAPI_Heap_gv_disabled_alloc;
#endif /* !OEI_CERT */

/*e \ingroup OSAPI_HeapClass
 * \brief Allocates zero-initialized memory from the heap.
 *
 * \details
 *
 * The function allocates `count` elements of size `size`
 * and sets the memory region to 0.
 *
 * NOTE: This operation assumes the specified size to be > 0; this operation
 * is never used directly but it is only accessed by using one of the other
 * allocate operations of OSAPI_Heap (e.g. allocate_struct, allocate_array,
 * allocate_string...); these operations are implemented as macros and accept
 * a "type" argument which is always converted to a size > 0 using the sizeof
 * operator.
 *
 * \param[in] count The number of elements to allocate.
 *
 * \param[in] size  The size of the element; size is assumed to be always
 *                  greater than 0.
 *
 * \return. A pointer to a contiguous memory area guaranteed to be large
 * enough store `count` elements of size `size`.
 *
 * \sa \ref OSAPI_Heap_free
 */
OSAPIDllExport void*
OSAPI_Heap_allocate(OEI_INT32 count,OEI_INT32 size);

/*e \ingroup OSAPI_HeapClass
 * \brief Reallocate  memory from the heap.
 *
 * \param[in] ptr Currently allocated buffer
 *
 * \param[in] size The new desired size
 *
 * \return. A pointer to a contiguous memory area guaranteed to be large
 * enough store size bytes, NULL if the reallocation failed.
 *
 * \sa \ref OSAPI_Heap_free
 */
OSAPIDllExport void*
OSAPI_Heap_realloc(void *ptr,OEI_INT32 size);

/*e \ingroup OSAPI_HeapClass
 *
 * \brief Frees memory allocated from the heap.
 *
 * \details
 *
 * The function frees memory previously allocated with \ref OSAPI_Heap_allocate
 * back to the heap.
 *
 * \param[in] ptr Pointer to region previous allocated
 *                 with \ref OSAPI_Heap_allocate
 *
 *
 * \sa \ref OSAPI_Heap_allocate
 */
OSAPIDllExport void
OSAPI_Heap_free(void *ptr);

/*e \ingroup OSAPI_HeapClass
    \brief Allocates space on the heap for a C structure and
    initializes the structure with 0's.

    Example:
    \code
    struct MyStruct *myStruct;
    OSAPI_Heap_allocate_struct(&myStruct, struct MyStruct);
    if (myStruct==NULL) {
        return;
    }
    \endcode

    The returned space must be freed with \ref OSAPI_Heap_free_struct.

    NOTE: This operation is implemented as a macro in order to support the
    specification of a "type" argument (converted to a size value using the
    sizeof operator).
*/
OSAPIDllExport void
OSAPI_Heap_allocate_struct(TYPE **pointer, TYPE);


/*e \ingroup OSAPI_HeapClass
    \brief Returns previously allocated
    (with \ref OSAPI_Heap_allocate_struct) space to the heap.

    @param[in] pointer If NULL, no op.

    Example:
    \code
    char *c;
    OSAPI_Heap_allocate_struct(&c, char);
    OSAPI_Heap_free_struct(c);
    \endcode
 */
OSAPIDllExport void
OSAPI_Heap_free_struct(TYPE *pointer);


/*e \ingroup OSAPI_HeapClass
    \brief A static method to allocate space on the heap for an array of C
    structures; the array is initialized with 0's.

    Example:
    \code
    struct MyElement *array;
    OSAPI_Heap_allocate_struct(&array, 7, struct MyElement);
    if (array==NULL) {
        return;
    }
    // elements of the array can be accessed as array[i]
    \endcode

    The returned space must be freed with \ref OSAPI_Heap_free_array.

    NOTE: This operation is implemented as a macro in order to support the
    specification of a "type" argument (converted to a size value using the
    sizeof operator).
*/
OSAPIDllExport void
OSAPI_Heap_allocate_array(TYPE ** pointer, int count,TYPE);


/*e \ingroup OSAPI_HeapClass
    \brief A static method to return space (that was previously
    allocated with \ref OSAPI_Heap_allocate_array) to the heap.

    Example:
    \code
    char *c;
    OSAPI_Heap_allocate_array(&c, 1, char);
    OSAPI_Heap_free_array(c);
    \endcode

    @param[in] storage If NULL, no op.
 */
OSAPIDllExport void
OSAPI_Heap_free_array(TYPE *storage);


/*e \ingroup OSAPI_HeapClass
    \brief A static method to allocate space on the heap for a string of up to
    a given size. The string is initialized with 0's.

    Example:
    \code
    char* myString;
    OSAPI_Heap_allocate_string(&myString, 128);
    \endcode
 
    @param[out] pointer  String buffer.
    @param[in]  size     Size in bytes to allocate; this length must not include
    the string terminator, which is automatically added to the specified value.

    The returned space must be freed with \ref OSAPI_Heap_free_string.
 */
OSAPIDllExport void
OSAPI_Heap_allocate_string(char **pointer,OEI_UINT32 size);

/*e \ingroup OSAPI_HeapClass
    \brief A static method to return space string space to the heap.

    @param[in] pointer If NULL, no op.
 */
OSAPIDllExport void
OSAPI_Heap_free_string(char *pointer);

/*e \ingroup OSAPI_HeapClass
  \brief A static method to allocate a block of memory of the provided 
  size from the heap.

  @post The returned address will be a multiple of 2^alignment; its
  memory will be initialized with 0's.

  @param[out] buffer
  @param[in] size  > 0
  @param[in] alignment Power of 2 and >0 or OSAPI_ALIGNMENT_DEFAULT.
  If the alignment is OSAPI_ALIGNMENT_DEFAULT,
  the returned pointer will have "default alignment" (meaning that any 
  C-structure can start at the beginning of the buffer).

  The block must be returned to the heap with \ref OSAPI_Heap_free_buffer.
*/
OSAPIDllExport void
OSAPI_Heap_allocate_buffer(char **buffer,
                         OEI_UINT32 size,
                         OSAPI_Alignment_T alignment);

#ifndef OEI_CERT
/*e \ingroup OSAPI_HeapClass
  \brief A static method to return a block (that was previously
  allocated with \ref OSAPI_Heap_allocate_buffer) to the heap.

  @param[in] buffer If NULL, no op.
*/
OSAPIDllExport void
OSAPI_Heap_free_buffer(void *buffer);
#endif /* !OEI_CERT */

#ifndef OEI_CERT
/*e \ingroup OSAPI_HeapClass
  \brief Current allocated bytes
*/
OSAPIDllVariable extern OEI_SIZE_T OSAPI_gv_AllocatedByteCount;
#endif /* !OEI_CERT */

#ifndef OEI_CERT
/*e \ingroup OSAPI_HeapClass
  \brief Return current allocated memory

  \mtsafety UNSAFE
*/
OSAPIDllExport OEI_SIZE_T
OSAPI_Heap_get_allocated_byte_count(void);
#endif /* !OEI_CERT */

#ifndef OEI_CERT
/*e \ingroup OSAPI_HeapClass
  \brief Disable memory allocation

*/
OSAPIDllExport void
OSAPI_Heap_disable_alloc(void);
#endif /* !OEI_CERT */

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#include "osapi/osapi_heap_impl.h"

#endif /* osapi_heap_h */
