/*  SPDX-License-Identifier: MIT */
/*
 *  Copyright (c) 2008 Bo Berry
 *  Copyright (c) 2008-2011 Cisco Systems
 */

#include "safeclib_private.h"
#include "safe_mem_constraint.h"
#include "safe_mem_lib.h"

/**
 * NAME
 *    memcmp32_s
 *
 * SYNOPSIS
 *    #include "safe_mem_lib.h"
 *    errno_t
 *    memcmp32_s(const uint32_t *dest, rsize_t dmax,
 *               const uint32_t *src,  rsize_t smax, int *diff)
 *
 * DESCRIPTION
 *    Compares memory until they differ, and their difference is
 *    returned in diff.  If the block of memory is the same, diff=0.
 *
 * EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 *  INPUT PARAMETERS
 *    dest      pointer to memory to compare against
 *
 *    dmax      maximum length of dest, in uint32_t
 *
 *    src       pointer to the source memory to compare with dest
 *
 *    smax      maximum length of src, in uint32_t
 *
 *    *diff     pointer to the diff which is an integer greater
 *              than, equal to or less than zero according to
 *              whether the object pointed to by dest is
 *              greater than, equal to or less than the object
 *              pointed to by src.
 *
 *  OUTPUT PARAMETERS
 *    none
 *
 * RUNTIME CONSTRAINTS
 *    Neither dest nor src shall be a null pointer.
 *    Neither dmax nor smax shall be zero.
 *    dmax shall not be greater than RSIZE_MAX_MEM.
 *    smax shall not be greater than dmax.
 *
 * RETURN VALUE
 *    EOK        successful operation
 *    ESNULLP    NULL pointer
 *    ESZEROL    zero length
 *    ESLEMAX    length exceeds max limit
 *
 * ALSO SEE
 *    memcmp_s(), memcmp16_s()
 *
 */
errno_t
memcmp32_s (const uint32_t *dest, rsize_t dmax,
            const uint32_t *src,  rsize_t smax, int *diff)
{
    /*
     * must be able to return the diff
     */
    if (diff == NULL) {
        invoke_safe_mem_constraint_handler("memcmp32_s: diff is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }
    *diff = -1;  /* default diff */


    if (dest == NULL) {
        invoke_safe_mem_constraint_handler("memcmp32_s: dest is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (src == NULL) {
        invoke_safe_mem_constraint_handler("memcmp32_s: src is null",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (dmax == 0) {
        invoke_safe_mem_constraint_handler("memcmp32_s: dmax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (dmax > RSIZE_MAX_MEM32) {
        invoke_safe_mem_constraint_handler("memcmp32_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (smax == 0) {
        invoke_safe_mem_constraint_handler("memcmp32_s: smax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (smax > dmax) {
       invoke_safe_mem_constraint_handler("memcmp32_s: smax exceeds dmax",
                  NULL, ESLEMAX);
       return (RCNEGATE(ESLEMAX));
    }

    /*
     * no need to compare the same memory
     */
    if (dest == src) {
        *diff = 0;
        return (RCNEGATE(EOK));
    }

    /*
     * now compare src to dest
     */
    *diff = 0;
    while (dmax != 0 && smax != 0) {
        if (*dest != *src) {
            *diff = *dest - *src;
            break;
        }

        dmax--;
        smax--;

        dest++;
        src++;
    }

    return (RCNEGATE(EOK));
}
EXPORT_SYMBOL(memcmp32_s)
