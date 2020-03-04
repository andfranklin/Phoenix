case = 'occluded'
refinement_level = 0
quadrature_type = 'GAUSS'
quadrature_order = 'FOURTH'

[Mesh]
  [raw_blocking_cylinder]
    type = FileMeshGenerator
    file = 'blocking_cylinder.msh'
  []

  [blocking_cylinder_blocked]
    type = RenameBlockGenerator
    input = 'raw_blocking_cylinder'
    old_block_id = '0'
    new_block_id = '1'
  []

  [blocking_cylinder_t_and_b]
    type = SideSetsFromNormalsGenerator
    input = 'blocking_cylinder_blocked'
    new_boundary = '1 2'
    normals = ' 0 0  1
                0 0 -1'
    fixed_normal = true
    replace = true
    variance = 1e-6
  []

  [blocking_cylinder]
    type = SideSetsPoolFloodGenerator
    input = 'blocking_cylinder_t_and_b'
    new_boundaries = '3 4'
    points = '1.0 0.0 1.0
              2.0 0.0 1.0'
    replace = true
  []

  [raw_bottom_ring]
    type = FileMeshGenerator
    file = 'bottom_ring.msh'
  []

  [bottom_ring_blocked]
    type = RenameBlockGenerator
    input = 'raw_bottom_ring'
    old_block_id = '0'
    new_block_id = '2'
  []

  [bottom_ring_split_sides]
    type = SideSetsFromNormalsGenerator
    input = 'bottom_ring_blocked'
    new_boundary = '1 2'
    normals = '0 0  1
               0 0 -1'
    replace = true
    variance = 1e-6
  []

  [bottom_ring]
    type = RenameBoundaryGenerator
    input = 'bottom_ring_split_sides'
    old_boundary_id = '0 1 2'
    new_boundary_id = '5 6 7'
  []

  [raw_top_ring]
    type = FileMeshGenerator
    file = 'top_ring.msh'
  []

  [top_ring_blocked]
    type = RenameBlockGenerator
    input = 'raw_top_ring'
    old_block_id = '0'
    new_block_id = '3'
  []

  [top_ring_split_sides]
    type = SideSetsFromNormalsGenerator
    input = 'top_ring_blocked'
    new_boundary = '1 2'
    normals = '0 0  1
               0 0 -1'
    replace = true
    variance = 1e-6
  []

  [top_ring]
    type = RenameBoundaryGenerator
    input = 'top_ring_split_sides'
    old_boundary_id = '0 1 2'
    new_boundary_id = '8 9 10'
  []

  [combined_raw]
    type = CombinerGenerator
    inputs = 'blocking_cylinder bottom_ring top_ring'
  []

  [combined_blocks_renamed]
    type = RenameBlockGenerator
    input = 'combined_raw'
    old_block_id = '1 2 3'
    new_block_name = 'blocking_cylinder bottom_ring top_ring'
  []

  [combined]
    type = RenameBoundaryGenerator
    input = 'combined_blocks_renamed'
    old_boundary_id = '1 2 3 4 5 6 7 8 9 10'
    new_boundary_name = 'cyl_top cyl_bottom cyl_inside cyl_outside
                         br_sides br_top br_bottom
                         tr_sides tr_top tr_bottom'
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
  [./br_bottom]
    type = DirichletBC
    variable = diffused
    boundary = 'br_bottom'
    value = 1
  [../]

  [./br_top]
    type = DirichletBC
    variable = diffused
    boundary = 'br_top'
    value = 0
  [../]

  [./tr_bottom]
    type = DirichletBC
    variable = diffused
    boundary = 'tr_bottom'
    value = 0
  [../]

  [./tr_top]
    type = DirichletBC
    variable = diffused
    boundary = 'tr_top'
    value = 1
  [../]
[]

[UserObjects]
  [./view_factor_calculator]
    type = ViewFactorCalculator
    quadrature_order = ${quadrature_order}
    quadrature_type = ${quadrature_type}

    boundary = 'br_top tr_bottom cyl_outside'

    occlusion_detection = BRUTE_FORCE_WITHOUT_BBOX
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
