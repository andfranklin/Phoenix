case = 'w_tube'
plate_mesh = ''
refinement_level = 0
quadrature_type = 'GAUSS'
quadrature_order = 'FIRST'

[Mesh]
  [raw_w_tube]
    type = FileMeshGenerator
    file = 'w_tube.msh'
  []

  [blocking_w_tube]
    type = RenameBlockGenerator
    input = 'raw_w_tube'
    old_block_id = '0'
    new_block_id = '1'
  []

  [w_tube_partial]
    type = SideSetsFromPointsGenerator
    input = 'blocking_w_tube'
    new_boundary = '1 2'
    points = '-591.5 469.0 0.0
               591.5 469.0 0.0'
    fixed_normal = true
    replace = true
    variance = 1e-6
  []

  [w_tube]
    type = SideSetsPoolFloodGenerator
    input = 'w_tube_partial'
    new_boundaries = '3 4'
    points = '579.0 469.0 0.0
              604.0 469.0 0.0'
    replace = true
  []

  [raw_plate]
    type = FileMeshGenerator
    file = '${plate_mesh}'
  []

  [plate_blocked]
    type = RenameBlockGenerator
    input = 'raw_plate'
    old_block_id = '0'
    new_block_id = '2'
  []

  [plate]
    type = SideSetsFromNormalsGenerator
    input = 'plate_blocked'
    new_boundary = '5 6 7 8 9 10'
    normals = ' 0  1  0
                0 -1  0
                1  0  0
               -1  0  0
                0  0  1
                0  0 -1'
    replace = true
    variance = 1e-6
  []

  [combined_raw]
    type = CombinerGenerator
    inputs = 'w_tube plate'
  []

  [combined_blocks_renamed]
    type = RenameBlockGenerator
    input = 'combined_raw'
    old_block_id = '1 2'
    new_block_name = 'w_tube plate'
  []

  [combined]
    type = RenameBoundaryGenerator
    input = 'combined_blocks_renamed'
    old_boundary_id = '1 2 3 4 5 6 7 8 9 10'
    new_boundary_name = 'w_tube_left w_tube_right w_tube_inside w_tube_outside
                         plate_top plate_bottom plate_right plate_left plate_front plate_back'
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
  [./w_tube_1]
    type = DirichletBC
    variable = diffused
    boundary = 'w_tube_inside'
    value = 1
  [../]

  [./w_tube_0]
    type = DirichletBC
    variable = diffused
    boundary = 'w_tube_outside'
    value = 0
  [../]

  [./plate_1]
    type = DirichletBC
    variable = diffused
    boundary = 'plate_left'
    value = 1
  [../]

  [./plate_0]
    type = DirichletBC
    variable = diffused
    boundary = 'plate_right'
    value = 0
  [../]
[]

[UserObjects]
  [./view_factor_calculator]
    type = RadiationHeatTransferSetup
    quadrature_order = ${quadrature_order}
    quadrature_type = ${quadrature_type}

    execute_on = 'INITIAL'

    boundary = 'w_tube_outside plate_front'

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
  nl_abs_tol = 1e3
[]

[Outputs]
  execute_on = 'timestep_end'
  exodus = true
  file_base = '${case}_${refinement_level}_${quadrature_order}_${quadrature_type}'
[]
