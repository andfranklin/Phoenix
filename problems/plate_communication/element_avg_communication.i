quadrature_type = 'GAUSS'
quadrature_order = 'FOURTH'
refinement_level = 1

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

  [./avg_rad_flux_helper]
    type = AverageRadiationFluxHelper
    variable = T
    boundary = 'lp_right rp_left'
  [../]
[]

[BCs]
  [./lp_left]
    type = DirichletBC
    variable = T
    boundary = 'lp_left'
    value = 300.0
  [../]

  [./lp_right_emission]
    type = AverageRadiationEmissionBC
    avg_rad_flux_helper = avg_rad_flux_helper
    variable = T
    boundary = lp_right
  [../]

  [./lp_right_irradiation]
    type = AverageIrradiationBC
    avg_rad_flux_helper = avg_rad_flux_helper
    variable = T
    boundary = lp_right
  [../]

  [./rp_left_emission]
    type = AverageRadiationEmissionBC
    avg_rad_flux_helper = avg_rad_flux_helper
    variable = T
    boundary = 'rp_left'
  [../]

  [./rp_left_irradiation]
    type = AverageIrradiationBC
    avg_rad_flux_helper = avg_rad_flux_helper
    variable = T
    boundary = rp_left
  [../]

  [./rp_right]
    type = DirichletBC
    variable = T
    boundary = 'rp_right'
    value = 300.0
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
