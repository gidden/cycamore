#include "separations.h"

#include <gtest/gtest.h>
#include <sstream>
#include "cyclus.h"

using pyne::nucname::id;
using cyclus::Composition;
using cyclus::CompMap;
using cyclus::Material;
using cyclus::QueryResult;
using cyclus::Cond;
using cyclus::toolkit::MatQuery;

namespace cycamore {

TEST(SeparationsTests, SepMaterial) {
  CompMap comp;
  comp[id("U235")] = 10;
  comp[id("U238")] = 90;
  comp[id("Pu239")] = 1;
  comp[id("Pu240")] = 2;
  comp[id("Am241")] = 3;
  comp[id("Am242")] = 2.8;
  double qty = 100;
  Composition::Ptr c = Composition::CreateFromMass(comp);
  Material::Ptr mat = Material::CreateUntracked(qty, c);

  std::map<int, double> effs;
  effs[id("U")] = .7;
  effs[id("Pu")] = .4;
  effs[id("Am241")] = .4;

  Material::Ptr sep = SepMaterial(effs, mat);
  MatQuery mqorig(mat);
  MatQuery mqsep(sep);

  EXPECT_DOUBLE_EQ(effs[id("U")] * mqorig.mass("U235"), mqsep.mass("U235"));
  EXPECT_DOUBLE_EQ(effs[id("U")] * mqorig.mass("U238"), mqsep.mass("U238"));
  EXPECT_DOUBLE_EQ(effs[id("Pu")] * mqorig.mass("Pu239"), mqsep.mass("Pu239"));
  EXPECT_DOUBLE_EQ(effs[id("Pu")] * mqorig.mass("Pu240"), mqsep.mass("Pu240"));
  EXPECT_DOUBLE_EQ(effs[id("Am241")] * mqorig.mass("Am241"), mqsep.mass("Am241"));
  EXPECT_DOUBLE_EQ(0, mqsep.mass("Am242"));
}

TEST(SeparationsTests, SepMixElemAndNuclide) {
  std::string config =
      "<streams>"
      "    <item>"
      "        <commod>stream1</commod>"
      "        <info>"
      "            <buf_size>-1</buf_size>"
      "            <efficiencies>"
      "                <item><comp>U</comp> <eff>0.6</eff></item>"
      "                <item><comp>Pu239</comp> <eff>.7</eff></item>"
      "            </efficiencies>"
      "        </info>"
      "    </item>"
      "</streams>"
      ""
      "<leftover_commod>waste</leftover_commod>"
      "<throughput>100</throughput>"
      "<feedbuf_size>100</feedbuf_size>"
      "<feed_commods> <val>feed</val> </feed_commods>"
     ;

  CompMap m;
  m[id("u235")] = 0.08;
  m[id("u238")] = 0.9;
  m[id("Pu239")] = .01;
  m[id("Pu240")] = .01;
  Composition::Ptr c = Composition::CreateFromMass(m);

  int simdur = 2;
  cyclus::MockSim sim(cyclus::AgentSpec(":cycamore:Separations"), config, simdur);
  sim.AddSource("feed").recipe("recipe1").Finalize();
  sim.AddSink("stream1").capacity(100).Finalize();
  sim.AddRecipe("recipe1", c);
  int id = sim.Run();

  std::vector<Cond> conds;
  conds.push_back(Cond("SenderId", "==", id));
  int resid = sim.db().Query("Transactions", &conds).GetVal<int>("ResourceId");
  MatQuery mq (sim.GetMaterial(resid));
  EXPECT_DOUBLE_EQ(m[922350000]*0.6*100, mq.mass("U235"));
  EXPECT_DOUBLE_EQ(m[922380000]*0.6*100, mq.mass("U238"));
  EXPECT_DOUBLE_EQ(m[942390000]*0.7*100, mq.mass("Pu239"));
  EXPECT_DOUBLE_EQ(0, mq.mass("Pu240"));
}


TEST(SeparationsTests, Pause) {
  std::string config =
      "<streams>"
      "    <item>"
      "        <commod>stream1</commod>"
      "        <info>"
      "            <buf_size>-1</buf_size>"
      "            <efficiencies>"
      "                <item><comp>U</comp> <eff>0.6</eff></item>"
      "                <item><comp>Pu239</comp> <eff>.7</eff></item>"
      "            </efficiencies>"
      "        </info>"
      "    </item>"
      "</streams>"
      ""
      "<pauses><val><first>2</first><second>1</second></val></pauses>"
      ""
      "<leftover_commod>waste</leftover_commod>"
      "<throughput>100</throughput>"
      "<feedbuf_size>100</feedbuf_size>"
      "<feed_commods> <val>feed</val> </feed_commods>"
     ;

  CompMap m;
  m[id("u235")] = 0.08;
  m[id("u238")] = 0.9;
  m[id("Pu239")] = .01;
  m[id("Pu240")] = .01;
  Composition::Ptr c = Composition::CreateFromMass(m);

  int simdur = 4;
  cyclus::MockSim sim(cyclus::AgentSpec(":cycamore:Separations"), config, simdur);
  sim.AddSource("feed").recipe("recipe1").Finalize();
  sim.AddSink("stream1").capacity(100).Finalize();
  sim.AddRecipe("recipe1", c);
  int id = sim.Run();

  std::vector<Cond> conds;
  conds.push_back(Cond("SenderId", "==", id));
  int resid;
  
  conds.push_back(Cond("Time", "==", 1));
  resid = sim.db().Query("Transactions", &conds).GetVal<int>("ResourceId");
  MatQuery mq1(sim.GetMaterial(resid));
  EXPECT_DOUBLE_EQ(m[922350000]*0.6*100, mq1.mass("U235"));
  EXPECT_DOUBLE_EQ(m[922380000]*0.6*100, mq1.mass("U238"));
  EXPECT_DOUBLE_EQ(m[942390000]*0.7*100, mq1.mass("Pu239"));
  EXPECT_DOUBLE_EQ(0, mq1.mass("Pu240"));
  conds.pop_back();

  conds.push_back(Cond("Time", "==", 2));
  ASSERT_THROW(sim.db().Query("Transactions", &conds).GetVal<int>("ResourceId"),
               cyclus::StateError);
  conds.pop_back();
  
  conds.push_back(Cond("Time", "==", 3));
  resid = sim.db().Query("Transactions", &conds).GetVal<int>("ResourceId");
  MatQuery mq2(sim.GetMaterial(resid));
  EXPECT_DOUBLE_EQ(m[922350000]*0.6*100, mq2.mass("U235"));
  EXPECT_DOUBLE_EQ(m[922380000]*0.6*100, mq2.mass("U238"));
  EXPECT_DOUBLE_EQ(m[942390000]*0.7*100, mq2.mass("Pu239"));
  EXPECT_DOUBLE_EQ(0, mq2.mass("Pu240"));
  conds.pop_back();


}

} // namespace cycamore

