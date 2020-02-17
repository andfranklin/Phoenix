[Mesh]
  type = GeneratedMesh
  dim = 3
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 0.1
  nx = 10
  ny = 10
  nz = 1

  boundary_id =   '1 2 3 4 5 6'
  boundary_name = 'bottom right top left front back'
[]

[Variables]
  [./T]
    order = FIRST
    family = LAGRANGE
    initial_condition = 300
  [../]
[]

[Kernels]
  [./dt]
    type = HeatConductionTimeDerivative
    specific_heat = 1.0
    density_name = 1.0
    variable = T
  [../]
  [./diff]
    type = HeatConduction
    diffusion_coefficient = 1.0
    variable = T
  [../]
  [./q_dot]
    type = HeatSource
    value = 1000.0
    variable = T
  [../]
[]

[UserObjects]
  [./avg_rad_flux_helper]
    type = AverageRadiationFluxHelper
    variable = T
    boundary = 'right'
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = T
    boundary = 'left'
    value = 300.0
  [../]

  # 305K @ right and 427.5K @ center
  # @ 4s when q_dot=1000
  [./right]
    type = AverageRadiationEmissionBC
    avg_rad_flux_helper = avg_rad_flux_helper
    variable = T
    boundary = 'right'
  [../]
[]

[Executioner]
  type = Transient

  scheme = implicit-euler
  num_steps = 100
  dt = 4e-2

  solve_type = PJFNK

  l_tol = 1e-4
  l_max_its = 30

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-9
  nl_max_its = 30
[]

[Outputs]
  execute_on = 'initial timestep_end'
  exodus = true
[]
