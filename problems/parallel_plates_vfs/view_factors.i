case = 'coarse_plates'
refinement_level = 0
quadrature_type = 'GAUSS'
quadrature_order = 'FOURTH'

[Mesh]
  file = '${case}.e'

  block_id = '1 2'
  block_name = 'left_plate right_plate'

  boundary_id =   '1 2 3 4 5 6 7 8 9 10 11 12'
  boundary_name = 'lp_top lp_bottom lp_left lp_right lp_front lp_back rp_top rp_bottom rp_left rp_right rp_front rp_back'
  second_order = false

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

    execute_on = 'INITIAL'
    boundary = 'lp_front rp_back'

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
