#ifndef CYCLUS_AGENTS_TARIFF_REGION_H_
#define CYCLUS_AGENTS_TARIFF_REGION_H_

#include <string>

#include "cyclus.h"

namespace cyclus {

class TariffRegion : public cyclus::Region {
 public:
  TariffRegion(cyclus::Context* ctx);
  virtual ~TariffRegion();

  /// #pragma cyclus

  #pragma cyclus clone
  #pragma cyclus initfromcopy
  #pragma cyclus initfromdb
  #pragma cyclus infiletodb
  #pragma cyclus schema
  #pragma cyclus annotations
  #pragma cyclus snapshot
  #pragma cyclus snapshotinv
  #pragma cyclus initinv
  
  #pragma cyclus note {"doc": "A region that applies tarrifs to trades with " \
                              "other regions."}

  /// adjust preferences for materials based on user input for tariffs
  virtual double AdjustMatlPref(Request<Material>* req, Bid<Material>* bid,
                                double pref, TradeSense sense);


 protected:
  // todo: add docs, etc.
  // map of region_name: time: value
  #pragma cyclus var {}
  std::map<int, std::map<std::string, double> > tariff_vals;
};

}  // namespace cyclus

#endif  // CYCLUS_AGENTS_TARIFF_REGION_H_
