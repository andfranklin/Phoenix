#include "PhoenixApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
PhoenixApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  return params;
}

PhoenixApp::PhoenixApp(InputParameters parameters) : MooseApp(parameters)
{
  PhoenixApp::registerAll(_factory, _action_factory, _syntax);
}

PhoenixApp::~PhoenixApp() {}

void
PhoenixApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"PhoenixApp"});
  Registry::registerActionsTo(af, {"PhoenixApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
PhoenixApp::registerApps()
{
  registerApp(PhoenixApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
PhoenixApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  PhoenixApp::registerAll(f, af, s);
}
extern "C" void
PhoenixApp__registerApps()
{
  PhoenixApp::registerApps();
}
