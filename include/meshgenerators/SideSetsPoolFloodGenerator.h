#pragma once

#include "SideSetsGeneratorBase.h"

// Forward declarations
class SideSetsPoolFloodGenerator;

template <>
InputParameters validParams<SideSetsPoolFloodGenerator>();

/**
 * Adds new sidesets starting at the specified point containing all connected
 * element faces that are a part of the specified pool sideset. Analogous to
 * the paint bucket in Microsoft Paint.
 */
class SideSetsPoolFloodGenerator : public SideSetsGeneratorBase
{
public:
  static InputParameters validParams();

  SideSetsPoolFloodGenerator(const InputParameters & parameters);

  std::unique_ptr<MeshBase> generate() override;

protected:
  /**
   * This method implements a routine to construct a sideset from a pool of
   * sides and a starting point on the surface of the mesh.
   *
   * It is analogous to the paint bucket in Microsoft Paint.
   */
  void fill_pool(const Point & point, BoundaryID pool_id, BoundaryID new_id, MeshBase & mesh);

  /**
   * This method implements the recursive routine to flood a pool of sides, and
   * add them to the new boundary. All that is required is the starting element
   * and side that is apart of the pool.
   */
  void flood(const Elem * elem, unsigned int side_indx, BoundaryID pool_id, BoundaryID new_id, MeshBase & mesh);

  void reassign_side(const Elem * elem, unsigned int side_indx, BoundaryID new_id, MeshBase & mesh);

  bool side_in_pool(const Elem * elem, unsigned int side_indx, BoundaryID pool_id, MeshBase & mesh);

  std::unique_ptr<MeshBase> & _input;

  BoundaryName _pool_boundary_name;

  std::vector<BoundaryName> _boundary_names;

  std::vector<Point> _points;
};
