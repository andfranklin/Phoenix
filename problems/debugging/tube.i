case = 'tube'
refinement_level = 0
quadrature_type = 'GAUSS'
quadrature_order = 'FOURTH'

[Mesh]
  [raw_tube]
    type = FileMeshGenerator
    file = 'tube.msh'
  []

  [blocking_tube]
    type = RenameBlockGenerator
    input = 'raw_tube'
    old_block_id = '0'
    new_block_id = '1'
  []

  [tube_partial]
    type = SideSetsFromPointsGenerator
    input = 'blocking_tube'
    new_boundary = '1 2'
    points = '9.0 0.0  0.0
              9.0 0.0 10.0'
    fixed_normal = true
    replace = true
    variance = 1e-6
  []

  [tube_unlabeled]
    type = SideSetsPoolFloodGenerator
    input = 'tube_partial'
    new_boundaries = '3 4'
    points = ' 8.0 0.0 0.0
              10.0 0.0 0.0'
    replace = true
  []

  [tube_blocks_labeled]
    type = RenameBlockGenerator
    input = 'tube_unlabeled'
    old_block_id = '1'
    new_block_name = 'tube'
  []

  [tube]
    type = RenameBoundaryGenerator
    input = 'tube_blocks_labeled'
    old_boundary_id = '1 2 3 4'
    new_boundary_name = 'bottom top inside outside'
  []

  uniform_refine = ${refinement_level}
[]

[Variables]
  [./diffused]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
 [./diff]
   type = Diffusion
   variable = diffused
 [../]
[]

[BCs]
  [./tube_0]
    type = DirichletBC
    variable = diffused
    boundary = 'inside'
    value = 0
  [../]

  [./tube_1]
    type = DirichletBC
    variable = diffused
    boundary = 'outside'
    value = 1
  [../]
[]

# [UserObjects]
#   [./view_factor_calculator]
#     type = RadiationHeatTransferSetup
#     quadrature_order = ${quadrature_order}
#     quadrature_type = ${quadrature_type}
#
#     execute_on = 'INITIAL'
#
#     boundary = 'w_tube_outside plate_front'
#
#     occlusion_detection = NONE # BRUTE_FORCE_WITH_BBOX
#     base_representation = APPROXIMATE
#     collision_representation = APPROXIMATE
#     quadrature_representation = APPROXIMATE
#
#     precision = 40
#   [../]
# []

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
[]

[Outputs]
  execute_on = 'timestep_end'
  exodus = true
  file_base = '${case}_${refinement_level}_${quadrature_order}_${quadrature_type}'
[]
