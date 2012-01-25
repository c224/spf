
/** \ingroup SPF */
/*@{*/

/*! \file Spin.h
 *
 *  DynVars: theta and phi (classical spin)
 *
 */
#ifndef SPIN_H
#define SPIN_H
#include "IoSimple.h"
#include "Random48.h"
#include "TypeToString.h"

namespace Spf {
	template<typename FieldType_>
	struct Spin { // Do not add functions here, this is a struct!!
		typedef typename PsimagLite::IoSimple::In IoSimpleIn;
		typedef FieldType_ FieldType;

		template<typename SomeParamsType>
		Spin(size_t vol,const SomeParamsType& params,bool freeze=false,bool makeVoid=false) : 
				size(vol),theta(vol,0),phi(vol,0),isFrozen(freeze),isVoid(makeVoid)
		{
			if (params.dynvarsfile=="none") return;
			if (params.dynvarsfile=="random") {
				PsimagLite::Random48<FieldType> random48(params.randomSeed);
				for (size_t i=0;i<theta.size();i++) {
					theta[i] = random48.random()*M_PI;
					phi[i] = random48.random()*2.*M_PI;
				}
				return;
			}
			
			if (params.dynvarsfile=="ferro") {
				for (size_t i=0;i<theta.size();i++) {
					theta[i] = 0.0;
					phi[i] = 0.0;
				}
				return;
			}

			if (params.dynvarsfile=="pizero") {
				size_t l = size_t(sqrt(size));
				if (l*l!=size) {
					std::string s(__FILE__);
					s += " : " + ttos(__LINE__);
					s += ": Hi there, I'm the Spin class, I have no way of ";
					s += " knowing what geometry you are using, but ";
					s += " it sure doesn't appear to be a square lattice.\n";
					s += " \"pizero\" start type valid only for square lattice\n";
					throw std::runtime_error(s.c_str());
				}
				for (size_t i=0;i<theta.size();i++) {
					theta[i] = M_PI;
					phi[i] = (i % l) % 2 ? 0 : M_PI;
				}
				return;
			}

			IoSimpleIn ioin(params.dynvarsfile);
			(*this)<=ioin;
			if (theta.size()==0 || phi.size()==0) throw std::runtime_error("PRoblem\n");
		}
				
		//size_t size() const { return theta.size(); }
		
		size_t size;
		std::vector<FieldType> theta;
		std::vector<FieldType> phi;
		bool isFrozen;
		bool isVoid;
		
	}; // Spin
	
	template<typename FieldType>
	std::ostream& operator<<(std::ostream& os,const Spin<FieldType>& dynVars)
	{
		os<<"Theta\n";
		os<<dynVars.theta;
		os<<"Phi\n";
		os<<dynVars.phi;
		os<<"IsFrozen"<<dynVars.isFrozen<<"\n";
		return os;
	}
	
	//! Operator to read Dynvars from file
	template<typename FieldType>
	Spin<FieldType>& operator<=(
			Spin<FieldType>& dynVars,
			typename PsimagLite::IoSimple::In& ioin)
	{
		ioin.read(dynVars.theta,"Theta");
		ioin.read(dynVars.phi,"Phi");
		ioin.readline(dynVars.isFrozen,"IsFrozen");
		
		return dynVars;
	}
	
} // namespace Spf

/*@}*/
#endif
