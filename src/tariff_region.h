#ifndef CYCLUS_AGENTS_TARIFF_REGION_H_
#define CYCLUS_AGENTS_TARIFF_REGION_H_

#include <string>

#include "cyclus.h"

namespace cyclus {

class TariffRegion : public cyclus::Region {
 public:
  TariffRegion(cyclus::Context* ctx);
  virtual ~TariffRegion();

  #pragma cyclus
  
  #pragma cyclus note {"doc": "A region that applies tarrifs to trades with " \
                              "other regions."}

  virtual void Tick();
  
  /// adjust preferences for materials based on user input for tariffs
  virtual double AdjustMatlPref(Request<Material>* req, Bid<Material>* bid,
                                double pref, TradeSense sense);


 private:
  // todo: add docs, etc.
  // map of region_name: time: value
  #pragma cyclus var {}
  std::map<int, std::map<std::string, double> > region_tariffs;
  // this should be internal
  #pragma cyclus var{}
  std::map<std::string, double> current_region_tariffs_;
};

}  // namespace cyclus

#endif  // CYCLUS_AGENTS_TARIFF_REGION_H_
