/* The copyright in this software is being made available under the BSD
   License, included below. This software may be subject to contributor and
   other third party rights, including patent rights, and no such rights are
   granted under this license.

   Copyright (c) 2020, Samsung Electronics Co., Ltd.
   All Rights Reserved. */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   
   - Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
   
   - Neither the name of the copyright owner, nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _XEVEM_TYPE_H_
#define _XEVEM_TYPE_H_

#include "xeve_type.h"
#include "xevem_dra.h"

/*****************************************************************************
 * CORE information used for encoding process.
 *
 * The variables in this structure are very often used in encoding process.
 *****************************************************************************/
typedef struct _XEVEM_CORE
{
    XEVE_CORE           core;
    /* intra prediction mode */
    u8                  mpm_ext[8];
    u8                  pims[IPD_CNT]; /* probable intra mode set*/
    /* ibc flag for MODE_IBC */
    u8                  ibc_flag;
    /* history-based prediction buffer */
    XEVE_HISTORY_BUFFER tmp_mot_lut[NUM_CU_LOG2][NUM_CU_LOG2];
    XEVE_HISTORY_BUFFER best_mot_lut[NUM_CU_LOG2][NUM_CU_LOG2];
    XEVE_HISTORY_BUFFER history_buffer;
    /* mmvd_flag for MODE_INTER */
    u8                  mmvd_flag;
    /* affine flag for MODE_INTER */
    u8                  affine_flag;
    /* ats */
    u8                  ats_intra_cu;
    u8                  ats_mode;
    u8                  ats_inter_info;
    /* temporal pixel buffer for inter prediction */
    pel                 eif_tmp_buffer[(MAX_CU_SIZE + 2) * (MAX_CU_SIZE + 2)];
    u8                  eval_mvp_idx[MAX_NUM_MVP];
    u8                  dmvr_flag;
}XEVEM_CORE;

/******************************************************************************
 * CONTEXT used for encoding process.
 *
 * All have to be stored are in this structure.
 *****************************************************************************/
typedef struct _XEVEM_CTX
{
    XEVE_CTX bctx;

    DRA_CONTROL      * dra_control;
    SIG_PARAM_DRA    * dra_array;

    /* ibc prediction analysis */
    XEVE_PIBC          pibc[XEVE_MAX_TASK_CNT];
    XEVE_IBC_HASH    * ibc_hash;

    int   (*fn_pibc_init_lcu)(XEVE_CTX * ctx, XEVE_CORE * core);
    double(*fn_pibc_analyze_cu)(XEVE_CTX *ctx, XEVE_CORE *core, int x, int y, int log2_cuw, int log2_cuh, XEVE_MODE *mi, s16 coef[N_C][MAX_CU_DIM], pel *rec[N_C], int s_rec[N_C]);
    int   (*fn_pibc_set_complexity)(XEVE_CTX * ctx, int complexity);
    int   (*fn_pibc_init_frame)(XEVE_CTX * ctx);
    
    /* adaptive loop filter */
    XEVE_ALF         * enc_alf;

    int   (*fn_alf)(XEVE_CTX * ctx, XEVE_PIC * pic, XEVE_SH* sh, XEVE_APS* aps);
    
    /* affine map (width in SCU x height in SCU) of raster scan order in a frame */
    u32              * map_affine;

    /* map for ats intra */
    u8               * map_ats_intra_cu;
    u8               * map_ats_mode_h;
    u8               * map_ats_mode_v;
    u8               * map_ats_inter;

    u32              * ats_inter_pred_dist[XEVE_MAX_TASK_CNT];
    u8               * ats_inter_info_pred[XEVE_MAX_TASK_CNT];   //best-mode ats_inter info
    u8               * ats_inter_num_pred[XEVE_MAX_TASK_CNT];

}XEVEM_CTX;

#include "xevem_mode.h"
#include "xevem_eco.h"
#include "xevem_ipred.h"
#include "xevem_ibc_hash.h"
#include "xevem_alf.h"
#include "xevem_tq.h"
#include "xevem_pred.h"
#include "xevem_itdq.h"
#include "xevem_itdq_sse.h"
#include "xevem_picman.h"
#include "xevem_tbl.h"
#include "xevem_util.h"
#include "xevem_mc.h"
#include "xevem_mc_sse.h"
#if GRAB_STAT
#include "xevem_stat.h"
#endif
#endif /* _XEVE_TYPE_H_ */
