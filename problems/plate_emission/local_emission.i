[Mesh]
  type = GeneratedMesh
  dim = 3
  xmin = 0
  xmax = 10
  ymin = 0
  ymax = 10
  zmin = 0
  zmax = 1.0
  nx = 40
  ny = 40
  nz = 4

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
    value = 10000.0
    variable = T
  [../]
[]

[BCs]
  [./front]
    type = LocalRadiationEmissionBC
    variable = T
    boundary = 'front'
  [../]
[]

[Executioner]
  type = Transient

  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 0.05
  [../]

  [./Quadrature]
    type = GAUSS

    element_order = SECOND
    side_order = FIFTH
  [../]

  steady_state_detection = true
  steady_state_tolerance = 1e-12

  scheme = implicit-euler

  solve_type = PJFNK

  l_abs_tol = 1e-5
  l_tol = 2e-3
  l_max_its = 5

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-9
  nl_max_its = 10
[]

[Outputs]
  execute_on = 'initial timestep_end'
  refinements = 2
  exodus = true
[]
