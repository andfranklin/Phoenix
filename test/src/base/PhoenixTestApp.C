//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "PhoenixTestApp.h"
#include "PhoenixApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
PhoenixTestApp::validParams()
{
  InputParameters params = PhoenixApp::validParams();
  return params;
}

PhoenixTestApp::PhoenixTestApp(InputParameters parameters) : MooseApp(parameters)
{
  PhoenixTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

PhoenixTestApp::~PhoenixTestApp() {}

void
PhoenixTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  PhoenixApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"PhoenixTestApp"});
    Registry::registerActionsTo(af, {"PhoenixTestApp"});
  }
}

void
PhoenixTestApp::registerApps()
{
  registerApp(PhoenixApp);
  registerApp(PhoenixTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
PhoenixTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  PhoenixTestApp::registerAll(f, af, s);
}
extern "C" void
PhoenixTestApp__registerApps()
{
  PhoenixTestApp::registerApps();
}
