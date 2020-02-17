case = 'pri_pri'
refinement_level = 0
quadrature_type = 'GAUSS'
quadrature_order = 'FOURTH'

[Mesh]
  [raw_left_plate]
    type = FileMeshGenerator
    file = 'left_plate.msh'
  []

  [left_plate_blocked]
    type = RenameBlockGenerator
    input = 'raw_left_plate'
    old_block_id = '0'
    new_block_id = '1'
  []

  [left_plate]
    type = SideSetsFromNormalsGenerator
    input = 'left_plate_blocked'
    new_boundary = '1 2 3 4 5 6'
    normals = ' 1.0  0.0  0.0
               -1.0  0.0  0.0
                0.0  1.0  0.0
                0.0 -1.0  0.0
                0.0  0.0  1.0
                0.0  0.0 -1.0'
    replace = true
    variance = 1e-6
  []

  [raw_right_plate]
    type = FileMeshGenerator
    file = 'right_plate.msh'
  []

  [right_plate_blocked]
    type = RenameBlockGenerator
    input = 'raw_right_plate'
    old_block_id = '0'
    new_block_id = '2'
  []

  [right_plate]
    type = SideSetsFromNormalsGenerator
    input = 'right_plate_blocked'
    new_boundary = '7 8 9 10 11 12'
    normals = ' 1.0  0.0  0.0
               -1.0  0.0  0.0
                0.0  1.0  0.0
                0.0 -1.0  0.0
                0.0  0.0  1.0
                0.0  0.0 -1.0'
    replace = true
    variance = 1e-6
  []

  [combined_raw]
    type = CombinerGenerator
    inputs = 'left_plate right_plate'
  []

  [combined_blocks_renamed]
    type = RenameBlockGenerator
    input = 'combined_raw'
    old_block_id = '1 2'
    new_block_name = 'left_plate right_plate'
  []

  [combined]
    type = RenameBoundaryGenerator
    input = 'combined_blocks_renamed'
    old_boundary_id = '1 2 3 4 5 6 7 8 9 10 11 12'
    new_boundary_name = 'lp_right lp_left lp_front lp_back lp_top lp_bottom
                         rp_right rp_left rp_front rp_back rp_top rp_bottom'
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
 [./lp_bottom]
   type = DirichletBC
   variable = diffused
   boundary = 'lp_bottom'
   value = 1
 [../]

 [./lp_top]
   type = DirichletBC
   variable = diffused
   boundary = 'lp_top'
   value = 0
 [../]

 [./rp_bottom]
   type = DirichletBC
   variable = diffused
   boundary = 'rp_bottom'
   value = 0
 [../]

 [./rp_top]
   type = DirichletBC
   variable = diffused
   boundary = 'rp_top'
   value = 1
 [../]
[]

[UserObjects]
  [./view_factor_calculator]
    type = RadiationHeatTransferSetup
    quadrature_order = ${quadrature_order}
    quadrature_type = ${quadrature_type}

    boundary = 'lp_right rp_left'

    occlusion_detection = NONE
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
