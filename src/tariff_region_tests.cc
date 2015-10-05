#include <gtest/gtest.h>

#include "tariff_region.h"

#include "agent_tests.h"
#include "region_tests.h"

using cyclus::TariffRegion;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class TariffRegionTest : public ::testing::Test {
 protected:
  cyclus::TestContext tc_;
  TariffRegion* src_region_;

  virtual void SetUp() {
    src_region_ = new TariffRegion(tc_.get());
  }

  virtual void TearDown() {}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(TariffRegionTest, clone) {
  TariffRegion* cloned_fac = dynamic_cast<TariffRegion*> (src_region_->Clone());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(TariffRegionTest, InitialState) {
  // Test things about the initial state of the region here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(TariffRegionTest, Print) {
  EXPECT_NO_THROW(std::string s = src_region_->str());
  // Test TariffRegion specific aspects of the print method here
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(TariffRegionTest, ReceiveMessage) {
  // Test TariffRegion specific behaviors of the receiveMessage function here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(TariffRegionTest, Tick) {
  int time = 1;
  EXPECT_NO_THROW(src_region_->Tick());
  // Test TariffRegion specific behaviors of the handleTick function here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(TariffRegionTest, Tock) {
  int time = 1;
  EXPECT_NO_THROW(src_region_->Tick());
  // Test TariffRegion specific behaviors of the handleTock function here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
cyclus::Agent* TariffRegionConstructor(cyclus::Context* ctx) {
  return new TariffRegion(ctx);
}

// Required to get functionality in cyclus agent unit tests library
#ifndef CYCLUS_AGENT_TESTS_CONNECTED
int ConnectAgentTests();
static int cyclus_agent_tests_connected = ConnectAgentTests();
#define CYCLUS_AGENT_TESTS_CONNECTED cyclus_agent_tests_connected
#endif  // CYCLUS_AGENT_TESTS_CONNECTED

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
INSTANTIATE_TEST_CASE_P(TariffRegion, RegionTests,
                        ::testing::Values(&TariffRegionConstructor));
INSTANTIATE_TEST_CASE_P(TariffRegion, AgentTests,
                        ::testing::Values(&TariffRegionConstructor));
