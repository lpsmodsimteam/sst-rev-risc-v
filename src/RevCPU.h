//
// _RevCPU_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _SST_REVCPU_H_
#define _SST_REVCPU_H_

// -- Standard Headers
#include <vector>

// -- SST Headers
#include <sst/core/sst_config.h>
#include <sst/core/component.h>
#include <sst/core/interfaces/simpleNetwork.h>

// -- Rev Headers
#include "RevOpts.h"
#include "RevMem.h"
#include "RevLoader.h"
#include "RevProc.h"
#include "RevNIC.h"
#include "PanNet.h"

//using namespace SST::Interfaces;
//using namespace SST::RevCPU;

namespace SST {
  namespace RevCPU {
    class RevCPU : public SST::Component {

    public:
      /// RevCPU: top-level SST component constructor
      RevCPU( SST::ComponentId_t id, SST::Params& params );

      /// RevCPU: top-level SST component destructor
      ~RevCPU();

      /// RevCPU: standard SST component 'setup' function
      void setup();

      /// RevCPU: standard SST component 'finish' function
      void finish();

      /// RevCPU: standard SST component 'init' function
      void init( unsigned int phase );

      /// RevCPU: standard SST component clock function
      bool clockTick( SST::Cycle_t currentCycle );

      // -------------------------------------------------------
      // RevCPU Component Registration Data
      // -------------------------------------------------------
      /// RevCPU: Register the component with the SST core
      SST_ELI_REGISTER_COMPONENT(
                                  RevCPU,
                                  "revcpu",
                                  "RevCPU",
                                  SST_ELI_ELEMENT_VERSION( 1, 0, 0 ),
                                  "RISC-V SST CPU",
                                  COMPONENT_CATEGORY_PROCESSOR
                                )

      // -------------------------------------------------------
      // RevCPU Component Parameter Data
      // -------------------------------------------------------
      SST_ELI_DOCUMENT_PARAMS(
                              {"verbose",   "Sets the verbosity level of output",      "0" },
                              {"clock",     "Clock for the CPU",                       "1GHz" },
                              {"program",   "Sets the binary executable",              "a.out" },
                              {"args",      "Sets the argument list",                  ""},
                              {"numCores",  "Number of RISC-V cores to instantiate",   "1" },
                              {"memSize",   "Main memory size in bytes",               "1073741824"},
                              {"startAddr", "Starting PC of the target core",          "core:0x80000000"},
                              {"machine",   "RISC-V machine model of the target core", "core:G"},
                              {"memCost",   "Memory latency range in cycles min:max",  "core:0:10"},
                              {"table",     "Instruction cost table",                  "core:/path/to/table"},
                              {"enable_nic","Enable the internal RevNIC",              "0"},
                              {"enable_pan","Enable PAN network endpoint",             "0"},
                              {"splash",    "Display the splash logo",                 "0"}
                             )

      // -------------------------------------------------------
      // RevCPU Port Parameter Data
      // -------------------------------------------------------
      SST_ELI_DOCUMENT_PORTS(
                            )

      // -------------------------------------------------------
      // RevCPU SubComponent Parameter Data
      // -------------------------------------------------------
      SST_ELI_DOCUMENT_SUBCOMPONENT_SLOTS(
                                         {"nic", "Network interface", "SST::RevCPU::RevNIC"},
                                         {"pan_nic", "PAN Network interface", "SST::RevCPU::PanNet"}
                                         )

      // -------------------------------------------------------
      // RevCPU Component Statistics Data
      // -------------------------------------------------------
      SST_ELI_DOCUMENT_STATISTICS(
                                 )

    private:
      unsigned numCores;                  ///< RevCPU: number of RISC-V cores
      std::string Exe;                    ///< RevCPU: binary executable
      std::string Args;                   ///< RevCPU: argument list
      RevOpts *Opts;                      ///< RevCPU: Simulation options object
      RevMem *Mem;                        ///< RevCPU: RISC-V main memory object
      RevLoader *Loader;                  ///< RevCPU: RISC-V loader
      std::vector<RevProc *> Procs;       ///< RevCPU: RISC-V processor objects
      bool *Enabled;                      ///< RevCPU: Completion structure

      bool EnableNIC;                     ///< RevCPU: Flag for enabling the NIC
      bool EnablePAN;                     ///< RevCPU  flag for enabling the PAN operations

      TimeConverter* timeConverter;       ///< RevCPU: SST time conversion handler
      SST::Output output;                 ///< RevCPU: SST output handler

      nicAPI *Nic;                        ///< RevCPU: Network interface controller
      panNicAPI *PNic;                    ///< RevCPU: PAN network interface controller

      /// RevCPU: RevNIC message handler
      void handleMessage(SST::Event *ev);

      /// RevCPU: PAN NIC message handler
      void handlePANMessage(SST::Event *ev);

    }; // class RevCPU
  } // namespace RevCPU
} // namespace SST

#endif

// EOF
