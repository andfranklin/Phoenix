#include "AverageRadiationBCInterface.h"
#include "BoundaryCondition.h"

defineLegacyParams(AverageRadiationBCInterface);

InputParameters
AverageRadiationBCInterface::validParams()
{
  InputParameters params = emptyInputParameters();
  params.addRequiredParam<UserObjectName>("avg_rad_flux_helper",
                                          "Computes the average radiation flux local to each element's face.");
  return params;
}

AverageRadiationBCInterface::AverageRadiationBCInterface(BoundaryCondition * bc)
  : _avg_rad_flux_helper(bc->getUserObjectTempl<AverageRadiationFluxHelper>("avg_rad_flux_helper"))
{
  if (!bc->isBoundarySubset(_avg_rad_flux_helper.boundaryIDs()))
  {
    std::string error = "The specified boundaries are not a subset of the ";
    error += "boundaries analized by the AverageRadiationFluxHelper. ";

    {
      error += "Specified boundaries: {";

      const auto & bns = bc->boundaryNames();
      unsigned int i = 1;
      for (auto bn: bns)
      {
        error += bn;
        if (i < bns.size()) error += ", ";
        ++i;
      }
      error += "}. ";
    }

    {
      error += "Possible boundaries: {";
      const auto & bns = _avg_rad_flux_helper.boundaryNames();
      unsigned int i = 1;
      for (auto bn: bns)
      {
        error += bn;
        if (i < bns.size()) error += ", ";
        ++i;
      }
      error += "}.";
    }

    bc->mooseError(error);
  }
}
