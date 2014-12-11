#include "Analysis.hpp"
#include <vector>
#include <cmath>
#include <memory>
using namespace std;

#include <iomanip>

// Based on arXiv:1405.7875

//Note: have not included the W signal regions

namespace Gambit {
  namespace ColliderBit {

    class Analysis_ATLAS_0LEP_20invfb : public Analysis {
    private:

      // Numbers passing cuts
      double _num2jl, _num2jm, _num2jt, _num3j,
        _num4jlm, _num4jl, _num4jm, _num4jt, _num5j, _num6jl,
	_num6jm,_num6jt,_num6jtp;

      vector<int> cutFlowVector;
      vector<string> cutFlowVector_str;
      int NCUTS; //=16;


    public:

      Analysis_ATLAS_0LEP_20invfb() {

	_num2jl=0; _num2jm=0; _num2jt=0; _num3j=0;
        _num4jlm=0; _num4jl=0; _num4jm=0; _num4jt=0; _num5j=0; _num6jl=0;
	_num6jm=0;_num6jt=0;_num6jtp=0;

	NCUTS=60;

	for(int i=0;i<NCUTS;i++){
          cutFlowVector.push_back(0);
          cutFlowVector_str.push_back("");
	}


      }


      void analyze(const Event* event) {
        Analysis::analyze(event);

        // Missing energy
        P4 ptot = event->missingmom();
        double met = event->met();

        // Now define vectors of baseline objects
        vector<Particle*> baselineElectrons;
        for (Particle* electron : event->electrons()) {
          if (electron->pT() > 10. && fabs(electron->eta()) < 2.47) baselineElectrons.push_back(electron);
        }
        vector<Particle*> baselineMuons;
        for (Particle* muon : event->muons()) {
          if (muon->pT() > 10. && fabs(muon->eta()) < 2.4) baselineMuons.push_back(muon);
        }
        vector<Jet*> baselineJets;
        for (Jet* jet : event->jets()) {
          if (jet->pT() > 20. && fabs(jet->eta()) < 4.5) baselineJets.push_back(jet);
        }

        // Overlap removal: only applied to jets with |eta|<2.8
        vector<Particle*> signalElectrons;
        vector<Particle*> signalMuons;
        vector<Jet*> signalJets;

        // Remove any jet within dR=0.2 of an electrons
        for (size_t iJet=0;iJet<baselineJets.size();iJet++) {
          bool overlap=false;
          P4 jetVec=baselineJets.at(iJet)->mom();
	  if(fabs(jetVec.eta())<2.8){
	    for (size_t iEl=0;iEl<baselineElectrons.size();iEl++) {
	      P4 elVec=baselineElectrons.at(iEl)->mom();
	      if (fabs(elVec.deltaR_eta(jetVec))<0.2)overlap=true;
	    }
	  }
          if (!overlap)signalJets.push_back(baselineJets.at(iJet));
        }

        // Remove electrons with dR=0.4 or surviving jets
        for (size_t iEl=0;iEl<baselineElectrons.size();iEl++) {
          bool overlap=false;
          P4 elVec=baselineElectrons.at(iEl)->mom();
          for (size_t iJet=0;iJet<signalJets.size();iJet++) {
            P4 jetVec=signalJets.at(iJet)->mom();
            if (fabs(elVec.deltaR_eta(jetVec))<0.4 && fabs(jetVec.eta())<2.8)overlap=true;
          }
          if (!overlap)signalElectrons.push_back(baselineElectrons.at(iEl));
        }

        // Remove muons with dR=0.4 or surviving jets
        for (size_t iMu=0;iMu<baselineMuons.size();iMu++) {
          bool overlap=false;
          P4 muVec=baselineMuons.at(iMu)->mom();
          for (size_t iJet=0;iJet<signalJets.size();iJet++) {
            P4 jetVec=signalJets.at(iJet)->mom();
            if (fabs(muVec.deltaR_eta(jetVec))<0.4 && fabs(jetVec.eta())<2.8)overlap=true;
          }
          if (!overlap)signalMuons.push_back(baselineMuons.at(iMu));
        }


        // We now have the signal electrons, muons and jets: move on to the 0 lepton 2012 analysis

        // Calculate common variables and cuts first
        int nElectrons = signalElectrons.size();
        int nMuons = signalMuons.size();
        int nJets = signalJets.size();

	bool leptonCut = (nElectrons == 0 && nMuons == 0);
        bool metCut = (met > 160.);
        float meff_incl = met;
        float HT=0;
        for (const Jet* j : signalJets)
          if (j->pT() > 40) {
            meff_incl += j->pT();
            HT  += j->pT();
          }

        // Do 2 jet regions
        float meff2j = 0;
        float dPhiMin2j = 0;
        if (nJets > 1) {
          if (signalJets[0]->pT()>130. && signalJets[1]->pT()>60.) {
            dPhiMin2j = SmallestdPhi(signalJets,ptot.phi());
            meff2j = met + signalJets[0]->pT() + signalJets[1]->pT();
            if (leptonCut && metCut && dPhiMin2j>0.4) {
              if (met/sqrt(HT)>8. && meff_incl>800.) _num2jl += 1;
              if (met/sqrt(HT)>15. && meff_incl>1200.) _num2jm += 1;
	      if (met/sqrt(HT)>15. && meff_incl>1600.) _num2jt += 1;
            }
	    
          }
	  
        }

        // Do the 3 jet regions
	float dPhiMin3j=0;
	float meff3j=0;
        if (nJets > 2) {
          if (signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60.) {
            dPhiMin3j = SmallestdPhi(signalJets,ptot.phi());
            meff3j = met + signalJets.at(0)->pT() + signalJets.at(1)->pT() + signalJets.at(2)->pT();
            if (leptonCut && metCut && dPhiMin3j > 0.4) {
              if (met/meff3j>0.3 && meff_incl>2200.) _num3j += 1;
            }
          }
        }

        // Do the 4 jet regions
	float dPhiMin4=0;
	float dPhiMin2=0;
	float meff4j=0;

        if (nJets > 3) {
          if (signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60.) {
            dPhiMin4 = SmallestdPhi(signalJets,ptot.phi());
            dPhiMin2 = SmallestRemainingdPhi(signalJets,ptot.phi());
            meff4j = met + signalJets.at(0)->pT() + signalJets.at(1)->pT() + signalJets.at(2)->pT() + signalJets.at(3)->pT();
            if (leptonCut && metCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2) {
              if(met/sqrt(HT)>10. && meff_incl>700.)_num4jlm += 1;
	      if(met/sqrt(HT)>10. && meff_incl>1000.)_num4jl += 1;
	      if (met/meff4j>0.4 && meff_incl>1300.) _num4jm += 1;
              if (met/meff4j>0.25 && meff_incl>2200.) _num4jt += 1;
            }
          }
        }

        // Do 5 jet region
        if (nJets > 4) {
          if (signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60.) {
            dPhiMin4 = SmallestdPhi(signalJets,ptot.phi());
            dPhiMin2 = SmallestRemainingdPhi(signalJets,ptot.phi());
            float meff5j = met + signalJets.at(0)->pT() + signalJets.at(1)->pT() + signalJets.at(2)->pT() + signalJets.at(3)->pT() + signalJets.at(4)->pT();
            if (leptonCut && metCut && dPhiMin4>0.4 && dPhiMin2>0.2) {
              if (met/meff5j>0.2 && meff_incl>1200.) _num5j += 1;
            }
          }
        }

        // Do the 6 jet regions
	float meff6j=0.;
        if (nJets > 5) {
          if (signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && signalJets.at(5)->pT()>60.) {
            dPhiMin4 = SmallestdPhi(signalJets,ptot.phi());
            dPhiMin2 = SmallestRemainingdPhi(signalJets,ptot.phi());
            meff6j = met + signalJets.at(0)->pT() + signalJets.at(1)->pT() + signalJets.at(2)->pT() + signalJets.at(3)->pT() + signalJets.at(4)->pT() + signalJets.at(5)->pT();
            if (leptonCut && metCut && dPhiMin4>0.4 && dPhiMin2>0.2) {
	      if (met/meff6j>0.2 && meff_incl>900.) _num6jl += 1;
              if (met/meff6j>0.2 && meff_incl>1200.) _num6jm += 1;
              if (met/meff6j>0.25 && meff_incl>1500.) _num6jt += 1;
	       if (met/meff6j>0.15 && meff_incl>1700.) _num6jtp += 1;
            }
          }
        }

	cutFlowVector_str[0] = "No cuts ";
	cutFlowVector_str[1] = "2j: MET > 160 GeV and jet pT ";
	cutFlowVector_str[2] = "2j: dPhiMin > 0.4 ";
	cutFlowVector_str[3] = "2j: met/sqrt(HT) > 15 ";
	cutFlowVector_str[4] = "2j: meff_incl > 1200 ";
	cutFlowVector_str[5] = "2j: meff_incl > 1600 ";
	cutFlowVector_str[6] = "3j: MET > 160 and jet pT ";
	cutFlowVector_str[7] = "3j: dPhiMin > 0.4 ";
	cutFlowVector_str[8] = "3j: met/meff3j > 0.3 ";
	cutFlowVector_str[9] = "3j: met/meff_incl > 2200. ";
	cutFlowVector_str[10] = "4jlm: MET > 160 and jet pT ";
	cutFlowVector_str[11] = "4jlm: dPhiMin > 0.4 ";
	cutFlowVector_str[12] = "4jlm: dPhiMin2 > 0.2 ";
	cutFlowVector_str[13] = "4jlm: met/sqrt(HT) > 10 ";
	cutFlowVector_str[14] = "4jlm: meff incl > 700 ";
	cutFlowVector_str[15] = "4jl: meff incl > 1000 ";
	cutFlowVector_str[16] = "4jt: met/meff4j > 0.25 ";
	cutFlowVector_str[17] = "4jt: meff incl > 2200 ";
	cutFlowVector_str[18] = "5j: MET > 160 and jet pT ";
	cutFlowVector_str[19] = "5j: dPhiMin > 0.4 ";
	cutFlowVector_str[20] = "5j: dPhiMin2 > 0.2 ";
	cutFlowVector_str[21] = "5j: met/meff5j > 0.2 ";
	cutFlowVector_str[22] = "5j: meff incl > 1200. ";
	cutFlowVector_str[23] = "6jl: MET >  160 and jet pT  ";
	cutFlowVector_str[24] = "6jl: dPhiMin > 0.4 ";
	cutFlowVector_str[25] = "6jl: dPhiMin2 > 0.2 ";
	cutFlowVector_str[26] = "6jl: met/meff6j > 0.2 ";
	cutFlowVector_str[27] = "6jl: meff incl > 900. ";
	cutFlowVector_str[28] = "6jt: met/meff6j > 0.25 ";
	cutFlowVector_str[29] = "6jt: meff incl > 1500. ";

	


	for(int j=0;j<NCUTS;j++){
          if(
             (j==0) ||
	     
	     (j==1 && signalJets.size()>1 && signalJets[0]->pT()>130. && signalJets[1]->pT()>60. && metCut && leptonCut) ||

	     (j==2 && signalJets.size()>1 && signalJets[0]->pT()>130. && signalJets[1]->pT()>60. && metCut && dPhiMin2j>0.4) ||
	     
	     (j==3 && signalJets.size()>1 && signalJets[0]->pT()>130. && signalJets[1]->pT()>60. && metCut && dPhiMin2j>0.4 && met/sqrt(HT)>15. ) ||

	     (j==4 && signalJets.size()>1 && signalJets[0]->pT()>130. && signalJets[1]->pT()>60. && metCut && dPhiMin2j>0.4 && met/sqrt(HT)>15. && meff_incl>1200.) ||

	     (j==5 && signalJets.size()>1 && signalJets[0]->pT()>130. && signalJets[1]->pT()>60. && metCut && dPhiMin2j>0.4 && met/sqrt(HT)>15. && meff_incl>1600.) ||

	     (j==6 && signalJets.size()>2 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && metCut && leptonCut) ||

	     (j==7 && signalJets.size()>2 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && metCut && leptonCut && dPhiMin3j > 0.4) ||

	     (j==8 && signalJets.size()>2 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && metCut && leptonCut && dPhiMin3j > 0.4 && met/meff3j>0.3) ||

	     (j==9 && signalJets.size()>2 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && metCut && leptonCut && dPhiMin3j > 0.4 && met/meff3j>0.3 && meff_incl>2200.) ||

	     (j==10 && signalJets.size() > 3 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && metCut) ||

	     (j==11 && signalJets.size() > 3 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && metCut && dPhiMin4 > 0.4) ||
	     
	     (j==12 && signalJets.size() > 3 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && metCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2) ||

	     (j==13 && signalJets.size() > 3 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && metCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2 && met/sqrt(HT) > 10.) ||

	     (j==14 && signalJets.size() > 3 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && metCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2 && met/sqrt(HT) > 10. && meff_incl > 700.) ||

	     (j==15 && signalJets.size() > 3 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && metCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2 && met/sqrt(HT) > 10. && meff_incl > 1000.) ||

	     (j==16 && signalJets.size() > 3 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && metCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2 && met/meff4j>0.25) ||

	     (j==17 && signalJets.size() > 3 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && metCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2 && met/meff4j>0.25 && meff_incl > 2200.) ||

	     //Start 5j signal regions

	     (j==18 && signalJets.size() > 4 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && metCut && leptonCut) ||

	     (j==19 && signalJets.size() > 4 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && metCut && leptonCut && dPhiMin4 > 0.4) ||

	     (j==20 && signalJets.size() > 4 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && metCut && leptonCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2) ||

	     (j==21 && signalJets.size() > 4 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && metCut && leptonCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2 && met/meff4j > 0.25) ||

	     (j==22 && signalJets.size() > 4 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && metCut && leptonCut && dPhiMin4 > 0.4 && dPhiMin2 > 0.2 && met/meff4j > 0.25 && meff_incl > 1200.) ||

	     //Start 6jl region

	     (j==23 && signalJets.size() > 5 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && signalJets.at(5)->pT()>60. && leptonCut && metCut) ||

	     (j==24 && signalJets.size() > 5 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && signalJets.at(5)->pT()>60. && leptonCut && metCut && dPhiMin4>0.4) ||

	     (j==25 && signalJets.size() > 5 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && signalJets.at(5)->pT()>60. && leptonCut && metCut && dPhiMin4>0.4 && dPhiMin2 > 0.2) ||

	     (j==26 && signalJets.size() > 5 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && signalJets.at(5)->pT()>60. && leptonCut && metCut && dPhiMin4>0.4 && dPhiMin2 > 0.2 && met/meff6j>0.2) ||

	     (j==27 && signalJets.size() > 5 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && signalJets.at(5)->pT()>60. && leptonCut && metCut && dPhiMin4>0.4 && dPhiMin2 > 0.2 && met/meff6j>0.2 && meff_incl > 900.) ||

	     (j==28 && signalJets.size() > 5 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && signalJets.at(5)->pT()>60. && leptonCut && metCut && dPhiMin4>0.4 && dPhiMin2 > 0.2 && met/meff6j>0.2 && meff_incl > 900. && met/meff6j>0.25) ||

	     (j==29 && signalJets.size() > 5 && signalJets.at(0)->pT()>130. && signalJets.at(1)->pT()>60. && signalJets.at(2)->pT()>60. && signalJets.at(3)->pT()>60. && signalJets.at(4)->pT()>60. && signalJets.at(5)->pT()>60. && leptonCut && metCut && dPhiMin4>0.4 && dPhiMin2 > 0.2 && met/meff6j>0.2 && meff_incl > 900. && met/meff6j>0.25 && meff_incl>1500.) 

	     ){

            cutFlowVector[j]++;
	    
          }
        }
	
	
      }
      

      void add(const Analysis* a) {
        const Analysis_ATLAS_0LEP_20invfb* aa = dynamic_cast<const Analysis_ATLAS_0LEP_20invfb*>(a);
        add_xsec(aa->xsec(), aa->xsec_err());
        // double tmp = _numAT;
        const double weight = (xsec() > 0) ? aa->xsec_per_event() / xsec_per_event() : 1;

        // cout << tmp << " -> " << _numAT << endl;
      }

      void finalize() {
        
	//Print cutflow here
	using namespace std;
	
        double scale_to = 1.; //0.0244*1000.*20.1; //sigma * L
        //double trigger_cleaning_eff = 0.90;
	
        cout << "------------------------------------------------------------------------------------------------------------------------------ "<<std::endl;
        cout << "CUT FLOW: ATLAS 0 Lep paper "<<std::endl;
        cout << "------------------------------------------------------------------------------------------------------------------------------"<<std::endl;
	
        std::cout<< right << setw(40) << "CUT" << setw(20) << "RAW" << setw(20) << "SCALED" << setw(20) << "%" << setw(20) << "clean adj RAW"<< setw(20) << "clean adj %" << std::endl;
        for(int j=0; j<NCUTS; j++) {
          std::cout << right << setw(40) << cutFlowVector_str[j].c_str() << setw(20) << cutFlowVector[j] << setw(20) << cutFlowVector[j]*scale_to/cutFlowVector[0] << setw(20) << 100.*cutFlowVector[j]/cutFlowVector[0] << "%" << setw(20) << cutFlowVector[j]*scale_to/cutFlowVector[0] << setw(20) << 100.*cutFlowVector[j]/cutFlowVector[0]<< "%" << endl;
        }
        cout << "------------------------------------------------------------------------------------------------------------------------------ "<<std::endl;

      }

      double loglikelihood() {
        return 0;
      }

      void collect_results() {
        /// @todo Implement!

	finalize();

        //Now fill a results object with the results for each SR
        //Numbers are taken from CONF note
        SignalRegionData results_2jl;
        results_2jl.set_observation(12315.);
        results_2jl.set_background(13000.);
        results_2jl.set_backgroundsys(1000.);
        results_2jl.set_signalsys(0.);
        results_2jl.set_signal(_num2jl);

	SignalRegionData results_2jm;
        results_2jm.set_observation(715.);
        results_2jm.set_background(760.);
        results_2jm.set_backgroundsys(50.);
        results_2jm.set_signalsys(0.);
        results_2jm.set_signal(_num2jm);

	SignalRegionData results_2jt;
        results_2jt.set_observation(133.);
        results_2jt.set_background(125.);
        results_2jt.set_backgroundsys(10.);
        results_2jt.set_signalsys(0.);
        results_2jt.set_signal(_num2jt);

	SignalRegionData results_3j;
        results_3j.set_observation(7.);
        results_3j.set_background(5.);
        results_3j.set_backgroundsys(1.2);
        results_3j.set_signalsys(0.);
        results_3j.set_signal(_num3j);

	SignalRegionData results_4jlm;
        results_4jlm.set_observation(2169.);
        results_4jlm.set_background(2120.);
        results_4jlm.set_backgroundsys(110.);
        results_4jlm.set_signalsys(0.);
        results_4jlm.set_signal(_num4jlm);

	SignalRegionData results_4jl;
        results_4jl.set_observation(608.);
        results_4jl.set_background(630.);
        results_4jl.set_backgroundsys(50.);
        results_4jl.set_signalsys(0.);
        results_4jl.set_signal(_num4jl);

	SignalRegionData results_4jm;
        results_4jm.set_observation(24.);
        results_4jm.set_background(37.);
        results_4jm.set_backgroundsys(6.);
        results_4jm.set_signalsys(0.);
        results_4jm.set_signal(_num4jm);
	
	SignalRegionData results_4jt;
        results_4jt.set_observation(0.);
        results_4jt.set_background(2.5);
        results_4jt.set_backgroundsys(1.);
        results_4jt.set_signalsys(0.);
        results_4jt.set_signal(_num4jt);

	SignalRegionData results_5j;
        results_5j.set_observation(121.);
        results_5j.set_background(126.);
        results_5j.set_backgroundsys(13.);
        results_5j.set_signalsys(0.);
        results_5j.set_signal(_num5j);

	SignalRegionData results_6jl;
        results_6jl.set_observation(121.);
        results_6jl.set_background(111.);
        results_6jl.set_backgroundsys(11.);
        results_6jl.set_signalsys(0.);
        results_6jl.set_signal(_num6jl);

	SignalRegionData results_6jm;
        results_6jm.set_observation(39.);
        results_6jm.set_background(33.);
        results_6jm.set_backgroundsys(6.);
        results_6jm.set_signalsys(0.);
        results_6jm.set_signal(_num6jm);

	SignalRegionData results_6jt;
        results_6jt.set_observation(5.);
        results_6jt.set_background(5.2);
        results_6jt.set_backgroundsys(1.4);
        results_6jt.set_signalsys(0.);
        results_6jt.set_signal(_num6jt);

	SignalRegionData results_6jtp;
        results_6jtp.set_observation(6.);
        results_6jtp.set_background(4.9);
        results_6jtp.set_backgroundsys(1.6);
        results_6jtp.set_signalsys(0.);
        results_6jtp.set_signal(_num6jt);

	add_result(results_2jl);
	add_result(results_2jm);
	add_result(results_2jt);
	add_result(results_3j);
	add_result(results_4jlm);
	add_result(results_4jl);
	add_result(results_4jm);
	add_result(results_4jt);
	add_result(results_5j);
	add_result(results_6jl);
	add_result(results_6jm);
	add_result(results_6jt);
	add_result(results_6jtp);

        return;
      }

      ///////////////////


      double SmallestdPhi(std::vector<Jet*> jets,double phi_met) {
        if (jets.size()<2) return(999);
        double dphi1 = std::acos(std::cos(jets.at(0)->phi()-phi_met));
        double dphi2 = std::acos(std::cos(jets.at(1)->phi()-phi_met));
        double dphi3 = 999;
        if (jets.size() > 2 && jets[2]->pT() > 40.)
          dphi3 = std::acos(std::cos(jets[2]->phi() - phi_met));
        double min1 = std::min(dphi1, dphi2);
        return std::min(min1, dphi3);
      }

      double SmallestRemainingdPhi(const std::vector<Jet *> jets,double phi_met) {
        double remainingDPhi = 999;
        double dphiMin = 999;
        for (size_t i = 0; i < jets.size(); i++) {
          if (i > 2 && jets[i]->pT() > 40.) { //< @todo Just start the loop at i = 3?
            remainingDPhi = std::acos(std::cos((jets[i]->phi() - phi_met)));
            dphiMin = std::min(remainingDPhi, dphiMin);
          }
        }
        return dphiMin;
      }


    };

    // Factory fn
    DEFINE_ANALYSIS_FACTORY(ATLAS_0LEP_20invfb)


  }
}