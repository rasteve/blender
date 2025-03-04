/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup bke
 *
 * Manage edit mesh cache: #EditMeshData
 */

#include "BLI_bounds.hh"
#include "BLI_math_vector.h"
#include "BLI_span.hh"

#include "BKE_editmesh.hh"
#include "BKE_editmesh_cache.hh" /* own include */

using blender::float3;
using blender::Span;

/* -------------------------------------------------------------------- */
/** \name Ensure Data (derived from coords)
 * \{ */

Span<float3> BKE_editmesh_cache_ensure_face_normals(BMEditMesh &em,
                                                    blender::bke::EditMeshData &emd)
{
  if (emd.vert_positions.is_empty() || !emd.face_normals.is_empty()) {
    return emd.face_normals;
  }
  BMesh *bm = em.bm;

  emd.face_normals.reinitialize(bm->totface);

  BM_mesh_elem_index_ensure(bm, BM_VERT);
  BMFace *efa;
  BMIter fiter;
  int i;
  BM_ITER_MESH_INDEX (efa, &fiter, bm, BM_FACES_OF_MESH, i) {
    BM_elem_index_set(efa, i); /* set_inline */
    BM_face_calc_normal_vcos(bm, efa, emd.face_normals[i], emd.vert_positions);
  }
  bm->elem_index_dirty &= ~BM_FACE;
  return emd.face_normals;
}

Span<float3> BKE_editmesh_cache_ensure_vert_normals(BMEditMesh &em,
                                                    blender::bke::EditMeshData &emd)
{
  if (emd.vert_positions.is_empty() || !emd.vert_normals.is_empty()) {
    return emd.vert_normals;
  }
  BMesh *bm = em.bm;

  /* Calculate vertex normals from face normals. */
  const Span<float3> face_normals = BKE_editmesh_cache_ensure_face_normals(em, emd);

  emd.vert_normals.reinitialize(bm->totvert);

  BM_mesh_elem_index_ensure(bm, BM_FACE);
  BM_verts_calc_normal_vcos(bm, face_normals, emd.vert_positions, emd.vert_normals);
  return emd.vert_normals;
}

Span<float3> BKE_editmesh_cache_ensure_face_centers(BMEditMesh &em,
                                                    blender::bke::EditMeshData &emd)
{
  if (!emd.face_centers.is_empty()) {
    return emd.face_centers;
  }
  BMesh *bm = em.bm;

  emd.face_centers.reinitialize(bm->totface);

  BMFace *efa;
  BMIter fiter;
  int i;
  if (emd.vert_positions.is_empty()) {
    BM_ITER_MESH_INDEX (efa, &fiter, bm, BM_FACES_OF_MESH, i) {
      BM_face_calc_center_median(efa, emd.face_centers[i]);
    }
  }
  else {
    BM_mesh_elem_index_ensure(bm, BM_VERT);
    BM_ITER_MESH_INDEX (efa, &fiter, bm, BM_FACES_OF_MESH, i) {
      BM_face_calc_center_median_vcos(bm, efa, emd.face_centers[i], emd.vert_positions);
    }
  }
  return emd.face_centers;
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Calculate Min/Max
 * \{ */

std::optional<blender::Bounds<blender::float3>> BKE_editmesh_cache_calc_minmax(
    const BMEditMesh &em, const blender::bke::EditMeshData &emd)
{
  using namespace blender;
  BMesh *bm = em.bm;
  if (bm->totvert == 0) {
    return std::nullopt;
  }

  if (emd.vert_positions.is_empty()) {
    BMVert *eve;
    BMIter iter;
    float3 min(std::numeric_limits<float>::max());
    float3 max(std::numeric_limits<float>::lowest());
    BM_ITER_MESH (eve, &iter, bm, BM_VERTS_OF_MESH) {
      minmax_v3v3_v3(min, max, eve->co);
    }
    return Bounds<float3>{min, max};
  }

  return bounds::min_max(emd.vert_positions.as_span());
}

/** \} */
