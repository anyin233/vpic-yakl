#define IN_sf_interface
#include "sf_interface_private.h"

void
clear_accumulators_pipeline( accumulators_pipeline_args_t * args,
                             int pipeline_rank,
                             int n_pipeline ) {
  accumulator_t * ALIGNED(16) a = args->a;
  int n = args->n, n_array = args->n_array, s_array = args->s_array, i;
  DISTRIBUTE(n, accumulators_n_block, pipeline_rank, n_pipeline, i, n); a += i;
  for( ; n_array; n_array--, a+=s_array ) CLEAR( a, n );
}

#if defined(V4_ACCELERATION) && defined(HAS_V4_PIPELINE)

#error "V4 version not hooked up yet!"

#endif

#define VOX(x,y,z) VOXEL(x,y,z, aa->g->nx,aa->g->ny,aa->g->nz)

void
clear_accumulator_array( accumulator_array_t * RESTRICT aa ) {
  DECLARE_ALIGNED_ARRAY( accumulators_pipeline_args_t, 128, args, 1 );
  int i0;

  if( !aa ) ERROR(( "Bad args" ));

  i0 = (VOX(1,1,1)/2)*2; // Round i0 down to even for 128B align on Cell */

  args->a       = aa->a + i0;
  args->n       = ((( VOX(aa->g->nx,aa->g->ny,aa->g->nz) - i0 + 1 )+1)/2)*2;
  args->n_array = aa->n_pipeline + 1;
  args->s_array = aa->stride;
  EXEC_PIPELINES( clear_accumulators, args, 0 );
  WAIT_PIPELINES();
}

void
clear_accumulator_array_kokkos(accumulator_array_t* RESTRICT aa) {
    if(!aa) ERROR(("Bad args"));
    const grid_t* g = aa->g;
    const k_accumulators_t& k_accum = aa->k_a_d;

    Kokkos::deep_copy(k_accum, 0.0f);
}
