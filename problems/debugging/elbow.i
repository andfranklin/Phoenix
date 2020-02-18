case = 'elbow'
refinement_level = 0
quadrature_type = 'GAUSS'
quadrature_order = 'FOURTH'

[Mesh]
  [raw_elbow]
    type = FileMeshGenerator
    file = 'elbow.msh'
  []

  [blocking_elbow]
    type = RenameBlockGenerator
    input = 'raw_elbow'
    old_block_id = '0'
    new_block_id = '1'
  []

  [elbow_partial]
    type = SideSetsFromPointsGenerator
    input = 'blocking_elbow'
    new_boundary = '1 2'
    points = ' 9.0 0.0  0.0
              20.0 0.0 11.0'
    fixed_normal = true
    replace = true
    variance = 1e-6
  []

  [elbow_unlabeled]
    type = SideSetsPoolFloodGenerator
    input = 'elbow_partial'
    new_boundaries = '3 4'
    points = ' 8.0 0.0 0.0
              10.0 0.0 0.0'
    replace = true
  []

  [elbow_blocks_labeled]
    type = RenameBlockGenerator
    input = 'elbow_unlabeled'
    old_block_id = '1'
    new_block_name = 'elbow'
  []

  [elbow]
    type = RenameBoundaryGenerator
    input = 'elbow_blocks_labeled'
    old_boundary_id = '1 2 3 4'
    new_boundary_name = 'left right inside outside'
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
  [./elbow_0]
    type = DirichletBC
    variable = diffused
    boundary = 'left'
    value = 0
  [../]

  [./elbow_1]
    type = DirichletBC
    variable = diffused
    boundary = 'right'
    value = 1
  [../]
[]

[UserObjects]
  [./view_factor_calculator]
    type = ViewFactorCalculator
    quadrature_order = ${quadrature_order}
    quadrature_type = ${quadrature_type}

    boundary = 'inside'

    occlusion_detection = NONE # BRUTE_FORCE_WITH_BBOX
    base_representation = APPROXIMATE
    collision_representation = APPROXIMATE
    quadrature_representation = APPROXIMATE

    precision = 40
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
[]

[Outputs]
  execute_on = 'timestep_end'
  exodus = true
  file_base = '${case}_${refinement_level}_${quadrature_order}_${quadrature_type}'
[]
