quadrature_type = 'GAUSS'
quadrature_order = 'FOURTH'
refinement_level = 0

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
    old_boundary_id = '1 2 3 4 5 6
                       7 8 9 10 11 12'
    new_boundary_name = 'lp_right lp_left lp_front lp_back lp_top lp_bottom
                         rp_right rp_left rp_front rp_back rp_top rp_bottom'
  []

  uniform_refine = ${refinement_level}
[]

[Variables]
  [./T]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./left_plate_ic]
    type = ConstantIC
    value = 300
    variable = T
    block = 'left_plate'
  [../]

  [./right_plate_ic]
    type = ConstantIC
    value = 0
    variable = T
    block = 'right_plate'
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
    block = 'left_plate'
  [../]
[]

[UserObjects]
  [./view_factor_calculator]
    type = ViewFactorCalculator
    quadrature_order = ${quadrature_order}
    quadrature_type = ${quadrature_type}

    boundary = 'lp_right rp_left'

    occlusion_detection = NONE
    base_representation = APPROXIMATE
    collision_representation = APPROXIMATE
    quadrature_representation = APPROXIMATE

    precision = 40
  [../]

  [./avg_rad_flux_helper]
    type = AverageRadiationFluxHelper
    variable = T
    boundary = 'lp_right rp_left'
  [../]
[]

[BCs]
  [./lp_right_emission]
    type = ElemAvgRadiationEmissionBC
    avg_rad_flux_helper = avg_rad_flux_helper
    variable = T
    boundary = lp_right
  [../]

  [./lp_right_irradiation]
    type = ElemAvgIrradiationBC
    avg_rad_flux_helper = avg_rad_flux_helper
    view_factor_calculator = view_factor_calculator
    variable = T
    boundary = lp_right
  [../]

  [./rp_left_emission]
    type = ElemAvgRadiationEmissionBC
    avg_rad_flux_helper = avg_rad_flux_helper
    variable = T
    boundary = 'rp_left'
  [../]

  [./rp_left_irradiation]
    type = ElemAvgIrradiationBC
    avg_rad_flux_helper = avg_rad_flux_helper
    view_factor_calculator = view_factor_calculator
    variable = T
    boundary = rp_left
  [../]
[]

[Executioner]
  type = Transient

  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 0.05
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
  exodus = true
[]
