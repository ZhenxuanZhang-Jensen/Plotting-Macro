#ifndef _COMMONCONTROLPLOTVARS_CHS_H
#define _COMMONCONTROLPLOTVARS_CHS_H

#include "plotvar_t.h"

const plotVar_t commonplotvars_chs[] = {
////    plotvar NBINS	MINRange  MAXRange	xlabel outfile  slog drawleg
////
//------------    Photons  		------------------------
 { "gen_leading_photon_Pt", 50, 0., 300., "Leading photon p_{T}", "gen_leading_photon_Pt", 0, 1},
//------------	Quarks			------------------------
//------------	W+				------------------------
//------------	W-				------------------------
//------------	Other variables	------------------------
 { "OneAK8TwoAK4_minMass_ReconsH_dR_H1", 50, 0., 5., "#Delta R between photons", "OneAK8TwoAK4_minMass_ReconsH_dR_H1", 0, 1},
 { "OneAK8TwoAK4_minMass_ReconsH_dR_H2", 50, 0., 5., "#Delta R between photons", "OneAK8TwoAK4_minMass_ReconsH_dR_H2", 0, 1},

 // Don't remove below line. It is used to break the loop.
 { "", 0,0.0,0.,"","",0,0 }
};


#endif // _COMMONCONTROLPLOTVARS_CHS_H
