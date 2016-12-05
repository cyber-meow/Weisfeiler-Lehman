/* weisfeiler-lehman.h */
#ifndef _WL_H_
#define _WL_H_

#include "graph.h"
#include "partition.h"

bool verify_isomorphism(const graph *G1, const graph *G2, const array *pho);
array *isomorphism_naif(const graph *G1, graph *G2);
array *isomorphism_degree(const graph *G1, const graph *G2);
partition* stable_partition(const graph *G, const partition *P);
array *isomorphism(const graph *G1, const graph *G2);

#endif
