/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */

#ifndef OMPI_MPIIO_PUBLIC_H
#define OMPI_MPIIO_PUBLIC_H

#include "ompi_config.h"
#include "mpi.h"

BEGIN_C_DECLS

typedef struct ompi_mpiio_public_api_t {
    int (*comm_dup)(MPI_Comm, MPI_Comm *);
    int (*comm_free)(MPI_Comm *);
    int (*barrier)(MPI_Comm);
    int (*bcast)(void *, int, MPI_Datatype, int, MPI_Comm);
    int (*allreduce)(const void *, void *, int, MPI_Datatype, MPI_Op, MPI_Comm);
    int (*allgather)(const void *, int, MPI_Datatype, void *, int, MPI_Datatype, MPI_Comm);
    int (*allgatherv)(const void *, int, MPI_Datatype, void *, const int *, const int *,
                      MPI_Datatype, MPI_Comm);
    int (*isend)(const void *, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *);
    int (*irecv)(void *, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *);
    int (*waitall)(int, MPI_Request[], MPI_Status[]);
    int (*type_create_hindexed)(int, const int[], const MPI_Aint[], MPI_Datatype,
                                MPI_Datatype *);
    int (*type_commit)(MPI_Datatype *);
} ompi_mpiio_public_api_t;

OMPI_DECLSPEC extern bool ompi_mpiio_internal_use_mpi;
OMPI_DECLSPEC void ompi_mpiio_register_public_api(const ompi_mpiio_public_api_t *api);
OMPI_DECLSPEC const ompi_mpiio_public_api_t *ompi_mpiio_public_api(void);
OMPI_DECLSPEC void ompi_mpiio_install_public_api(void);

END_C_DECLS

#endif /* OMPI_MPIIO_PUBLIC_H */
