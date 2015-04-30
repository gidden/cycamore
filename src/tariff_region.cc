#include "tariff_region.h"

#include "prettyprint.hpp"

namespace cyclus {

TariffRegion::TariffRegion(cyclus::Context* ctx) : cyclus::Region(ctx) {}

TariffRegion::~TariffRegion() {}

void TariffRegion::Tick() {
  int time = context()->time();
  if (region_tariffs.find(time) != region_tariffs.end()) {
    std::map<std::string, double>& vals = region_tariffs[time];
    std::map<std::string, double>::iterator it;
    for (it = vals.begin(); it != vals.end(); ++it) {
      current_region_tariffs_[it->first] = it->second;
    }
  }
}

double TariffRegion::AdjustMatlPref(Request<Material>* req, Bid<Material>* bid,
                                    double pref, TradeSense sense) {
  // region tariff
  Agent* other = sense == BID ?
                 req->requester()->manager() : bid->bidder()->manager();
  other = other->parent()->parent(); // region is parent of parent
  std::string name = other->prototype();
  if (current_region_tariffs_.find(name) != current_region_tariffs_.end()) {
    LOG(cyclus::LEV_INFO4, "TariffR") << current_region_tariffs_[name];
    pref *= current_region_tariffs_[name];
  }
  
  LOG(cyclus::LEV_INFO4, "TariffR") << "Final pref: " << pref;
  return pref;
}

extern "C" cyclus::Agent* ConstructTariffRegion(cyclus::Context* ctx) {
  return new TariffRegion(ctx);
}

}  // namespace cyclus
