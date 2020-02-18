case = coarse_plates
refinement_level = 1

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
  [./temp]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Materials]
  [./left]
    type = HeatConductionMaterial
    temp = temp
    block = 'left_plate'
    specific_heat = 1.0
    thermal_conductivity = 1.0
  [../]

  [./left_rho]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0'
    block = 'left_plate'
  [../]

  [./right]
    type = HeatConductionMaterial
    temp = temp
    block = 'right_plate'
    specific_heat = 1.0
    thermal_conductivity = 1.0
  [../]

  [./right_rho]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0'
    block = 'right_plate'
  [../]
[]

[Kernels]
  [./dt]
    type = HeatConductionTimeDerivative
    variable = temp
  [../]
  [./diff]
    type = HeatConduction
    variable = temp
  [../]
[]

[ICs]
  [./lp_ic]
    type = ConstantIC
    value = 100.0
    variable = temp
    block = 'left_plate'
  [../]

  [./rp_ic]
    type = ConstantIC
    value = 100.0
    variable = temp
    block = 'right_plate'
  [../]
[]

[BCs]
  [./lp_bottom]
    type = DirichletBC
    variable = temp
    boundary = 'lp_bottom'
    value = 150.0
  [../]

  [./lp_top]
    type = DirichletBC
    variable = temp
    boundary = 'lp_top'
    value = 100.0
  [../]

  [./lp_front]
    type = LocalizedRadiationEmission
    variable = temp
    boundary = 'lp_front'
  [../]

  [./rp_bottom]
    type = DirichletBC
    variable = temp
    boundary = 'rp_bottom'
    value = 100.0
  [../]

  [./rp_top]
    type = DirichletBC
    variable = temp
    boundary = 'rp_top'
    value = 150.0
  [../]
[]

[UserObjects]
  [./view_factor_calculator]
    type = ViewFactorCalculator
    quadrature_order = TWENTIETH
    # quadrature_type = TRAP
    # quadrature_order = CONSTANT
    quadrature_type = GAUSS

    boundary = 'lp_front rp_back'

    occlusion_detection = NONE
    base_representation = APPROXIMATE
    collision_representation = APPROXIMATE
    quadrature_representation = APPROXIMATE

    precision = 20
  [../]
[]

[Executioner]
  type = Transient
  # start_time = 0
  # end_time = 10.0
  scheme = implicit-euler
  num_steps = 500
  dt = 1e-2

  # dtmin = 1.0e-10
  # dtmax = 0.5
  #
  # [./TimeStepper]
  #   type = SolutionTimeAdaptiveDT
  #   percent_change = 0.25
  #   dt = 0.5
  # [../]

  solve_type = PJFNK

  l_tol = 1e-5
  l_max_its = 30

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-9
  nl_max_its = 30
[]

[Outputs]
  execute_on = 'initial timestep_end'
  exodus = true
  file_base = '${case}_solution_${refinement_level}'
[]
