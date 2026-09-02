/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */

#include "ompi_config.h"

#include "mpi.h"
#include "ompi/runtime/ompi_mpiio_public.h"

void
ompi_mpiio_install_public_api(void)
{
    static const ompi_mpiio_public_api_t api = {
        .comm_dup = MPI_Comm_dup,
        .comm_free = MPI_Comm_free,
        .barrier = MPI_Barrier,
        .bcast = MPI_Bcast,
        .allreduce = MPI_Allreduce,
        .allgather = MPI_Allgather,
        .allgatherv = MPI_Allgatherv,
        .isend = MPI_Isend,
        .irecv = MPI_Irecv,
        .waitall = MPI_Waitall,
        .type_create_hindexed = MPI_Type_create_hindexed,
        .type_commit = MPI_Type_commit
    };

    ompi_mpiio_register_public_api(&api);
}
