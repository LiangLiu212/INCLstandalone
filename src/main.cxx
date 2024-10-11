// ROOT header
//

#include "TRandom3.h"
#include "TH1D.h"
#include "TCanvas.h"


// INCL header 

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>

#include "G4INCLGeant4Compat.hh"
#include "G4INCLCascade.hh"

#include "G4INCLClustering.hh"
#include "G4INCLParticle.hh"
#include "G4INCLIAvatar.hh"
#include "G4INCLIPropagationModel.hh"
#include "G4INCLNucleus.hh"
#include "G4INCLStandardPropagationModel.hh"
#include "G4INCLRandom.hh"
#include "G4INCLRanecu.hh"
#include "G4INCLFinalState.hh"
#include "G4INCLParticleTable.hh"
#include "G4INCLKinematicsUtils.hh"

// signal handler (for Linux and GCC)
#include "G4INCLSignalHandling.hh"

// For I/O
#include "IWriter.hh"
#include "ASCIIWriter.hh"
#include "ProtobufWriter.hh"
#include "INCLTree.hh"
#include "ROOTWriter.hh"
#include "HDF5Writer.hh"

// For configuration
#include "G4INCLConfig.hh"

// For logging
#include "G4INCLLogger.hh"

// Generic de-excitation interface
#include "G4INCLIDeExcitation.hh"

// ABLA v3p de-excitation
#ifdef INCL_DEEXCITATION_ABLAXX
#include "G4INCLAblaInterface.hh"
#endif

// ABLACXX de-excitation
#ifdef INCL_DEEXCITATION_ABLACXX
#include "G4INCLAblaxxInterface.hh"
#endif

// ABLA07 de-excitation
#ifdef INCL_DEEXCITATION_ABLA07
#include "G4INCLAbla07Interface.hh"
#endif

// SMM de-excitation
#ifdef INCL_DEEXCITATION_SMM
#include "G4INCLSMMInterface.hh"
#endif

// GEMINIXX de-excitation
#ifdef INCL_DEEXCITATION_GEMINIXX
#include "G4INCLGEMINIXXInterface.hh"
#endif

#ifdef HAS_BOOST_DATE_TIME
#include <boost/date_time/posix_time/posix_time.hpp>
namespace bpt = boost::posix_time;
#endif

#ifdef HAS_BOOST_TIMER
#include <boost/timer/timer.hpp>
namespace bt = boost::timer;
#endif

using namespace G4INCL;
using namespace std;
int main(int argc, char *argv[]){
   // Set the random number generator algorithm. The system can support
    // multiple different generator algorithms in a completely
    // transparent way.


	// the configuration of INCL model
	G4INCL::Config *theConfig = new G4INCL::Config();
	theConfig->init();

	// define a argon nucleus
	ParticleSpecies targetSpecies = G4INCL::ParticleSpecies("Ar40");
	cout << targetSpecies.theA << endl;
	cout << targetSpecies.theZ << endl;
	cout << targetSpecies.theS << endl;
	theConfig->setTargetA(targetSpecies.theA);
	theConfig->setTargetZ(targetSpecies.theZ);
	theConfig->setTargetS(targetSpecies.theS);
	theConfig->setINCLXXDataFilePath("/root/inclxx/inclxx-v6.33.1-e5857a1/data");

//	cout << theConfig->summary() << endl;
//	cout << theConfig->getTargetA() << endl;
//	cout << theConfig->getProjectileType() << endl;
//	cout << theConfig->getINCLXXDataFilePath() << endl;


	// initialize INCL model
        Random::initialize(theConfig);
	Clustering::initialize(theConfig);
	ParticleTable::initialize(theConfig);
//	Cluster cc(18, 40, 0, true);
//	cc.initializeParticles();
//	cout << cc.print() << endl;

	// define a new argon Nucleus and initialize it
	G4INCL::Nucleus *p = new G4INCL::Nucleus(40, 18, 0, theConfig, ParticleTable::getMaximumNuclearRadius(Proton, 40, 18), NType);
	p->initializeParticles();
	cout << p->getStore()->getParticles().at(2)->getPotentialEnergy() << endl;
	cout << p->getStore()->getParticles().at(2)->getEnergy() -  p->getStore()->getParticles().at(2)->getPotentialEnergy()  << endl;
	cout << p->getStore()->getParticles().at(2)->getMomentum().print() << endl;
	cout << p->getStore()->getParticles().at(2)->getPosition().print() << endl;

	// reset the nucleus
	p->deleteParticles();
	p->getStore()->clear();
	p->initializeParticles();

//	cout << p->print() << endl;
	cout << p->getStore()->getParticles().at(2)->getPotentialEnergy() << endl;
	cout << p->getStore()->getParticles().at(2)->getEnergy() -  p->getStore()->getParticles().at(2)->getPotentialEnergy()  << endl;
	cout << p->getStore()->getParticles().at(2)->getMomentum().print() << endl;
	cout << p->getStore()->getParticles().at(2)->getPosition().print() << endl;
	cout << p->computeTotalEnergy() << endl;

//	cout << p->getStore()->getParticles().at(3)->getEnergy() << endl;
//	cout << p->getStore()->getParticles().at(3)->getMomentum().print() << endl;
//	cout << p->getStore()->getParticles().at(3)->getPosition().print() << endl;


	TRandom3 rdm;
	rdm.SetSeed(123);
	TH1D *h1 = new TH1D("h1", "h1", 20, 0, 400);
	TH1D *h2 = new TH1D("h2", "h2", 20, 0, 10);
	TH1D *h3 = new TH1D("h3", "h3", 50, 850, 940);
	for(size_t i = 0; i < 10000; i++){
	// reset the nucleus
		p->deleteParticles();
		p->getStore()->clear();
		p->initializeParticles();
		int i_n = rdm.Integer(22) + 18;
	//	cout << "reset : =========> " << i_n << endl;
		h1->Fill(p->getStore()->getParticles().at(i_n)->getMomentum().mag());
		h2->Fill(p->getStore()->getParticles().at(i_n)->getPosition().mag());
		h3->Fill(p->getStore()->getParticles().at(i_n)->getEnergy() - p->getStore()->getParticles().at(i_n)->getPotentialEnergy());
	//	cout << p->getStore()->getParticles().at(i_n)->getMomentum().print() << endl;
	}
	TCanvas *c1 = new TCanvas("c1", "c1", 600, 400);
	c1->cd();
	h1->Draw();
	h1->SetTitle(";Fermi mom (MeV); Events");
	c1->SaveAs("fermi_mom.png");
	TCanvas *c2 = new TCanvas("c2", "c2", 600, 400);
	c2->cd();
	h2->Draw();
	h2->SetTitle(";position (fm); Events");
	c2->SaveAs("fermi_posi.png");
	TCanvas *c3 = new TCanvas("c3", "c3", 600, 400);
	c3->cd();
	h3->Draw();
	h3->SetTitle(";E (MeV); Events");
	c3->SaveAs("fermi_energy.png");
	return 0;
}
