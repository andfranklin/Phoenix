case = 'mesh_generated_plates'
refinement_level = 0
quadrature_type = 'GAUSS'
quadrature_order = 'FOURTH'

[Mesh]
  [raw_left_plate]
    type = FileMeshGenerator
    file = 'left_plate.msh'
  []

  [left_plate]
    type = RenameBlockGenerator
    input = 'raw_left_plate'
    old_block_id = '0'
    new_block_id = '1'
  []

  [raw_right_plate]
    type = FileMeshGenerator
    file = 'right_plate.msh'
  []

  [right_plate]
    type = RenameBlockGenerator
    input = 'raw_right_plate'
    old_block_id = '0'
    new_block_id = '2'
  []

  [cmbn]
    type = CombinerGenerator
    inputs = 'left_plate right_plate'
  []
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

#[BCs]
#  [./lp_bottom]
#    type = DirichletBC
#    variable = diffused
#    boundary = 'lp_bottom'
#    value = 1
#  [../]

#  [./lp_top]
#    type = DirichletBC
#    variable = diffused
#    boundary = 'lp_top'
#    value = 0
#  [../]

#  [./rp_bottom]
#    type = DirichletBC
#    variable = diffused
#    boundary = 'rp_bottom'
#    value = 0
#  [../]

#  [./rp_top]
#    type = DirichletBC
#    variable = diffused
#    boundary = 'rp_top'
#    value = 1
#  [../]
#[]

#[UserObjects]
#  [./view_factor_calculator]
#    type = RadiationHeatTransferSetup
#    quadrature_order = ${quadrature_order}
#    quadrature_type = ${quadrature_type}

#    execute_on = 'INITIAL'
#    boundary = 'lp_front rp_back'

#    occlusion_detection = NONE
#    base_representation = APPROXIMATE
#    collision_representation = APPROXIMATE
#    quadrature_representation = APPROXIMATE

#    precision = 40
#  [../]
#[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
[]

[Outputs]
  execute_on = 'timestep_end'
  exodus = true
  file_base = '${case}_${refinement_level}_${quadrature_order}_${quadrature_type}'
[]
