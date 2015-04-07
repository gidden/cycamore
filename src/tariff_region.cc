#include "tariff_region.h"

#include "prettyprint.hpp"

namespace cyclus {

TariffRegion::TariffRegion(cyclus::Context* ctx) : cyclus::Region(ctx) {}

TariffRegion::~TariffRegion() {}

void TariffRegion::Tick() {
  int time = context()->time();
  if (tariff_vals.find(time) != tariff_vals.end()) {
    std::map<std::string, double>& vals = tariff_vals[time];
    std::map<std::string, double>::iterator it;
    for (it = vals.begin(); it != vals.end(); ++it) {
      current_tariffs_[it->first] = it->second;
    }
  }
}

double TariffRegion::AdjustMatlPref(Request<Material>* req, Bid<Material>* bid,
                                    double pref, TradeSense sense) {
  Agent* other = sense == BID ?
                 req->requester()->manager() : bid->bidder()->manager();
  other = other->parent()->parent(); // region is parent of parent
  std::string name = other->prototype();
  if (current_tariffs_.find(name) != current_tariffs_.end()) {
    LOG(cyclus::LEV_INFO4, "TariffR") << current_tariffs_[name];
    pref *= current_tariffs_[name];
  }
  LOG(cyclus::LEV_INFO4, "TariffR") << "Final pref: " << pref;
  return pref;
}

extern "C" cyclus::Agent* ConstructTariffRegion(cyclus::Context* ctx) {
  return new TariffRegion(ctx);
}

}  // namespace cyclus
