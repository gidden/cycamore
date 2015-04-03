#include "tariff_region.h"

namespace cyclus {

TariffRegion::TariffRegion(cyclus::Context* ctx) : cyclus::Region(ctx) {}

TariffRegion::~TariffRegion() {}

double TariffRegion::AdjustMatlPref(Request<Material>* req, Bid<Material>* bid,
                                    double pref, TradeSense sense) {
  int time = context()->time();
  Agent* other;
  std::string name;
  if (tariff_vals.find(time) != tariff_vals.end()) {
    other = sense == BID ?
            req->requester()->manager() : bid->bidder()->manager();
    other = other->parent()->parent(); // region is parent of parent
    std::map<std::string, double>& vals = tariff_vals[time];
    name = other->prototype();
    if (vals.find(name) != vals.end()) {
      pref *= vals[name];
    }
  }
  return pref;
}

extern "C" cyclus::Agent* ConstructTariffRegion(cyclus::Context* ctx) {
  return new TariffRegion(ctx);
}

}  // namespace cyclus
