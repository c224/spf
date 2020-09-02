#include "AllocatorCpu.h"
#include <iostream>
void printLicense()
{
	const PsimagLite::String license_=
	        "Copyright (c) 2009-2020, UT-Battelle, LLC\n"
	        "All rights reserved\n"
	        "\n"
	        "[SPF, Version 7.]\n"
	        "\n"
	        "*********************************************************\n"
	        "THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND\n"
	        "CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED\n"
	        "WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n"
	        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A\n"
	        "PARTICULAR PURPOSE ARE DISCLAIMED. \n"
	        "\n"
	        "Please see full open source license included in file LICENSE.\n"
	        "*********************************************************\n"
	        "\n"
	        "\n";
	std::cout<<license_;
}

#ifndef USE_FLOAT
typedef double FieldType;
#else
typedef float FieldType;
#endif

#include "ParametersEngine.h"
#include "Engine.h"
#include <algorithm>
#include "Concurrency.h"
typedef PsimagLite::Concurrency MyConcurrencyType;
#include "Models/PnictidesMultiOrbitals/PnictidesMultiOrbitals.h"
#include "Models/DmsMultiOrbital/DmsMultiOrbital.h"
#include "Models/PhononsTwoOrbitals/PhononsTwoOrbitals.h"
#include "Models/HubbardOneOrbital/HubbardOneOrbital.h"
#include "Geometries/GeometrySquare.h"
#include "Geometries/ShastrySutherland.h"
#include "Geometries/GeometryCubic.h"
#include "Geometries/GeometryFcc.h"
#include "Geometries/GeometrySquare45Degrees.h"
#include "Random48.h"
#include "InputCheck.h"
#include "InputNg.h"
#include "ProgramGlobals.h"

typedef Spf::GeometrySquare<FieldType> GeometrySquareType;
typedef Spf::ShastrySutherland<FieldType> ShastrySutherlandType;
typedef Spf::GeometryCubic<FieldType> GeometryCubicType;
typedef Spf::GeometryFcc<FieldType> GeometryFccType;
typedef Spf::GeometrySquare45Degrees<FieldType> GeometrySquare45DegreesType;
typedef PsimagLite::InputNg<Spf::InputCheck> InputNgType;
typedef Spf::ParametersEngine<FieldType,InputNgType::Readable> ParametersEngineType;
typedef PsimagLite::Random48<FieldType> RandomNumberGeneratorType;

template<typename GeometryType,typename ModelType>
void mainLoop2(ParametersEngineType& engineParams,
               InputNgType::Readable& io,
               const GeometryType& geometry)
{
	typedef Spf::Engine<ParametersEngineType,
	                    ModelType,
	                    InputNgType::Readable,
	                    RandomNumberGeneratorType> EngineType;

	ModelType model(engineParams,io,geometry);

	EngineType engine(engineParams,model,io);

	engine.main();
}

template<typename GeometryType>
void mainLoop(ParametersEngineType& engineParams,
              InputNgType::Readable& io)
{
	typedef Spf::PnictidesMultiOrbitals<ParametersEngineType,
	                                    GeometryType> PnictidesMultiOrbitalsType;
	typedef Spf::DmsMultiOrbital<ParametersEngineType,GeometryType> DmsMultiOrbitalType;
	typedef Spf::PhononsTwoOrbitals<ParametersEngineType,
	                                GeometryType> PhononsTwoOrbitalsType;
	typedef Spf::HubbardOneOrbital<ParametersEngineType,
	                               GeometryType> HubbardOneOrbitalType;

	GeometryType geometry(engineParams.latticeLength);

	std::cout<<"Geometry\n";
	Spf::ProgramGlobals::printGeometry(std::cout,geometry);
	std::cout<<"---------------------------------------------------\n";

	if (engineParams.model=="DmsMultiOrbital") {
		mainLoop2<GeometryType,DmsMultiOrbitalType>(engineParams,io,geometry);
	} else if (engineParams.model=="PnictidesMultiOrbitals") {
		mainLoop2<GeometryType,PnictidesMultiOrbitalsType>(engineParams,io,geometry);
	} else if (engineParams.model=="PhononsTwoOrbitals") {
		mainLoop2<GeometryType,PhononsTwoOrbitalsType>(engineParams,io,geometry);
	} else if (engineParams.model=="HubbardOneOrbital") {
		mainLoop2<GeometryType,HubbardOneOrbitalType>(engineParams,io,geometry);
	} else {
		std::cerr<<"model="<<engineParams.model<<"\n";
		throw PsimagLite::RuntimeError("Unknown model");
	}
}

int main(int argc,char *argv[])
{
	MyConcurrencyType concurrency(&argc,&argv,1);

	Spf::InputCheck inputCheck;
	PsimagLite::String filename="";
	int opt = 0;
	PsimagLite::String strUsage(argv[0]);
	strUsage += " -f filename";
	while ((opt = getopt(argc, argv,"f:")) != -1) {
		switch (opt) {
		case 'f':
			filename = optarg;
			break;
		default:
			inputCheck.usageMain(strUsage);
			return 1;
		}
	}

	// sanity checks here
	if (filename=="") {
		inputCheck.usageMain(strUsage);
		return 1;
	}

	if (PsimagLite::Concurrency::root()) printLicense();

	InputNgType::Writeable ioWriteable(filename,inputCheck);
	InputNgType::Readable io(ioWriteable);

	ParametersEngineType engineParams(io);
	std::cout<<engineParams;

	PsimagLite::Concurrency::codeSectionParams.npthreads = engineParams.npthreads;

	if (engineParams.geometry=="ladder") {
		mainLoop<GeometrySquareType>(engineParams,io);
	} else if (engineParams.geometry=="cubic") {
		mainLoop<GeometryCubicType>(engineParams,io);
	} else if (engineParams.geometry=="fcc") {
		mainLoop<GeometryFccType>(engineParams,io);
	} else if (engineParams.geometry=="square45Degrees") {
		mainLoop<GeometrySquare45DegreesType>(engineParams,io);
	} else if (engineParams.geometry=="ShastrySutherland") {
		mainLoop<ShastrySutherlandType>(engineParams,io);
	} else {
		std::cerr<<"geometry="<<engineParams.geometry<<"\n";
		throw PsimagLite::RuntimeError("Unknown geometry\n");
	}
}

/* ####### End of spf.cpp ######## */

