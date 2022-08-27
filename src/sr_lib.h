
#include "sr.h"
#include "sr_math.h"

/**
 * sr_lib.h
 * --------
 * an easy interface with the sr pipeline for
 * your average use case
 * 
 * provides a uniform with model view projection matrix,
 * and matrix stack-like operations to build it
 * 
 */

#ifndef SR_LIB_H
#define SR_LIB_H

/*********************************************************************
 *                                                                   *
 *                          uniform struct                           *
 *                                                                   *
 *********************************************************************/

struct sr_uniform {
    struct mat4* mvp;
};

#endif  /* SR_LIB_H */

