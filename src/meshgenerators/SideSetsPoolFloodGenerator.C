
#include "SideSetsPoolFloodGenerator.h"
#include "MooseMeshUtils.h"
#include "CastUniquePointer.h"

#include "libmesh/enum_point_locator_type.h"
#include "libmesh/distributed_mesh.h"
#include "libmesh/elem.h"

registerMooseObject("PhoenixApp", SideSetsPoolFloodGenerator);

defineLegacyParams(SideSetsPoolFloodGenerator);

InputParameters
SideSetsPoolFloodGenerator::validParams()
{
  InputParameters params = SideSetsGeneratorBase::validParams();

  params.addRequiredParam<MeshGeneratorName>("input", "The mesh we want to modify");
  params.addClassDescription("Adds new sidesets starting at the specified point containing all "
                             "connected element faces that are a part of the specified pool "
                             "sideset. Analogous to the paint bucket in Microsoft Paint.");
  params.addRequiredParam<BoundaryName>("new_boundary", "The name of the boundary to create");
  params.addRequiredParam<Point>("point", "The point from which to start filling-in the new sidesets");

  return params;
}

SideSetsPoolFloodGenerator::SideSetsPoolFloodGenerator(const InputParameters & parameters)
  : SideSetsGeneratorBase(parameters),
    _input(getMesh("input")),
    _boundary_name(getParam<BoundaryName>("new_boundary")),
    _point(getParam<Point>("point"))
{
  if (typeid(_input).name() == typeid(std::unique_ptr<DistributedMesh>).name())
    mooseError("SideSetsPoolFloodGenerator only works with ReplicatedMesh.");
}

std::unique_ptr<MeshBase>
SideSetsPoolFloodGenerator::generate()
{
  std::unique_ptr<MeshBase> mesh = std::move(_input);

  BoundaryID pool_id = 0;
  BoundaryID new_id = MooseMeshUtils::getBoundaryIDs(*mesh, {_boundary_name}, true)[0];

  setup(*mesh);

  _visited.clear();
  fill_pool(_point, pool_id, new_id, *mesh);

  finalize();
  mesh->get_boundary_info().sideset_name(new_id) = _boundary_name;

  // print sideset info
  std::cout << *mesh << std::endl;

  return dynamic_pointer_cast<MeshBase>(mesh);
}

void
SideSetsPoolFloodGenerator::fill_pool(const Point & point,
                                          BoundaryID pool_id,
                                          BoundaryID new_id,
                                          MeshBase & mesh)
{
  std::unique_ptr<PointLocatorBase> pl = PointLocatorBase::build(TREE, mesh);
  const Elem * elem = (*pl)(point);

  if (elem)
  {
    for (unsigned int side_indx=0; side_indx < elem->n_sides(); ++side_indx)
    {
      // continue to the next side if it is not on the boundary
      if (elem->neighbor_ptr(side_indx)) continue;

      std::unique_ptr<const Elem> elem_side = elem->side_ptr(side_indx);
      if (side_in_pool(elem, side_indx, pool_id, mesh) && elem_side->contains_point(point))
        flood(elem, side_indx, pool_id, new_id, mesh);
    }
  }
  else
  {
    std::ostringstream ss;
    ss << "The point " << point << " is not located on the mesh.";
    mooseError(ss.str());
  }
}

bool
SideSetsPoolFloodGenerator::side_in_pool(const Elem * elem, unsigned int side_indx, BoundaryID pool_id, MeshBase & mesh)
{
  return mesh.get_boundary_info().has_boundary_id(elem, side_indx, pool_id);
}

void
SideSetsPoolFloodGenerator::flood(const Elem * elem,
                                      unsigned int side_indx,
                                      BoundaryID pool_id,
                                      BoundaryID new_id,
                                      MeshBase & mesh)
{
  if ((_visited[side_indx].find(elem) != _visited[side_indx].end())) return;
  _visited[side_indx].insert(elem);

  reassign_side(elem, side_indx, new_id, mesh);
  std::unique_ptr<const Elem> side = elem->side_ptr(side_indx);

  // loop through element's sides and flood adjacent, boundary sides.
  for (unsigned int next_side_indx=0; next_side_indx < elem->n_sides(); ++next_side_indx)
  {
    // continue to the next side if it is not on the boundary or the side is
    // the current side.
    if (elem->neighbor_ptr(next_side_indx) || next_side_indx == side_indx)
      continue;

    if (side_in_pool(elem, next_side_indx, pool_id, mesh))
    {
      // flood if next_side is connected to side via an edge (w/o loss of generality)
      std::unique_ptr<const Elem> next_side = elem->side_ptr(next_side_indx);
      if (next_side->contains_edge_of(side.get()))
        flood(elem, next_side_indx, pool_id, new_id, mesh);
    }
  }

  // loop through element's neighbors and flood neighbors' boundary sides
  // adjacent to the flooded side.
  for (unsigned int neighbor_indx = 0; neighbor_indx < elem->n_sides(); ++neighbor_indx)
  {
    const libMesh::Elem * neighbor = elem->neighbor_ptr(neighbor_indx);
    if (neighbor == nullptr) continue;

    for (unsigned int neighbor_side_indx = 0; neighbor_side_indx < neighbor->n_sides(); ++neighbor_side_indx)
    {
      // continue to the next side if it is not on the boundary
      if (neighbor->neighbor_ptr(neighbor_side_indx)) continue;

      if (side_in_pool(neighbor, neighbor_side_indx, pool_id, mesh))
      {
        std::unique_ptr<const Elem> neighbor_side = neighbor->side_ptr(neighbor_side_indx);
        if (neighbor_side->contains_vertex_of(side.get()))
          flood(neighbor, neighbor_side_indx, pool_id, new_id, mesh);
      }
    }
  }
}

void
SideSetsPoolFloodGenerator::reassign_side(const Elem * elem,
                                              unsigned int side_indx,
                                              BoundaryID new_id,
                                              MeshBase & mesh)
{
  if (_replace)
    mesh.get_boundary_info().remove_side(elem, side_indx);
  mesh.get_boundary_info().add_side(elem, side_indx, new_id);
}
