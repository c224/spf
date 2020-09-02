/*
Copyright (c) 2009-2013, UT-Battelle, LLC
All rights reserved

[SPF, Version 7.0]
[by G.A., Oak Ridge National Laboratory]

UT Battelle Open Source Software License 11242008

OPEN SOURCE LICENSE

Subject to the conditions of this License, each
contributor to this software hereby grants, free of
charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), a
perpetual, worldwide, non-exclusive, no-charge,
royalty-free, irrevocable copyright license to use, copy,
modify, merge, publish, distribute, and/or sublicense
copies of the Software.

1. Redistributions of Software must retain the above
copyright and license notices, this list of conditions,
and the following disclaimer.  Changes or modifications
to, or derivative works of, the Software should be noted
with comments and the contributor and organization's
name.

2. Neither the names of UT-Battelle, LLC or the
Department of Energy nor the names of the Software
contributors may be used to endorse or promote products
derived from this software without specific prior written
permission of UT-Battelle.

3. The software and the end-user documentation included
with the redistribution, with or without modification,
must include the following acknowledgment:

"This product includes software produced by UT-Battelle,
LLC under Contract No. DE-AC05-00OR22725  with the
Department of Energy."

*********************************************************
DISCLAIMER

THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER, CONTRIBUTORS, UNITED STATES GOVERNMENT,
OR THE UNITED STATES DEPARTMENT OF ENERGY BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

NEITHER THE UNITED STATES GOVERNMENT, NOR THE UNITED
STATES DEPARTMENT OF ENERGY, NOR THE COPYRIGHT OWNER, NOR
ANY OF THEIR EMPLOYEES, REPRESENTS THAT THE USE OF ANY
INFORMATION, DATA, APPARATUS, PRODUCT, OR PROCESS
DISCLOSED WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*********************************************************


*/
/** \ingroup SPF */
/*@{*/

/*! \file InputCheck.h
 *
 *  InputChecking functions
 */
#ifndef INPUT_CHECK_H
#define INPUT_CHECK_H
#include <vector>
#include <stdexcept>
#include "Options.h"

namespace Spf {

class InputCheck {

	typedef PsimagLite::Options::Readable OptionsReadableType;

public:

	InputCheck() : optsReadable_(0) {}

	~InputCheck()
	{
		if (optsReadable_!=0) delete optsReadable_;
	}

	PsimagLite::String import() const
	{
		PsimagLite::String str = "";

		//str += "vector hubbardU;\n";
		return str;
	}

	bool check(const PsimagLite::String& label,
	           const PsimagLite::Vector<PsimagLite::String>::Type& vec,
	           SizeType line) const
	{
		if (label=="JMVALUES") {
			if (vec.size()!=2) return error1("JMVALUES",line);
			return true;
		} else if (label=="RAW_MATRIX") {
			SizeType row = atoi(vec[0].c_str());
			SizeType col = atoi(vec[1].c_str());
			SizeType n = row*col;
			if (vec.size()!=n+2) return error1("RAW_MATRIX",line);
			return true;
		} else if (label=="Connectors") {
			return true;
		} else if (label == "MagneticField") {
			return true;
		} else if (label=="FiniteLoops") {
			SizeType n = atoi(vec[0].c_str());
			if (vec.size()!=3*n+1)  return error1("FiniteLoops",line);
			return true;
		}
		return false;
	}

	void check(const PsimagLite::String& label,
	           const PsimagLite::String& val,
	           SizeType)
	{}

	void checkSimpleLabel(PsimagLite::String label, SizeType line)
	{}

	bool isSet(const PsimagLite::String& thisOption) const
	{
		return optsReadable_->isSet(thisOption);
	}

	void checkForThreads(SizeType nthreads) const
	{
		if (nthreads==1) return;

		PsimagLite::String message1(__FILE__);
		message1 += " FATAL: You are requesting nthreads>0 but you did not compile with USE_PTHREADS enabled\n";
		message1 += " Either set Threads=1 in the input file (you won't have threads though) or\n";
		message1 += " add -DUSE_PTHREADS to the CPP_FLAGS in your Makefile and recompile\n";
		throw PsimagLite::RuntimeError(message1.c_str());
	}

	void usageMain(const PsimagLite::String& name) const
	{
		std::cerr<<"USAGE is "<<name<<"\n";
	}

private:

	bool error1(const PsimagLite::String& message,SizeType line) const
	{
		PsimagLite::String s(__FILE__);
		s += " : Input error for label " + message + " near line " + ttos(line) + "\n";
		throw PsimagLite::RuntimeError(s.c_str());

	}

	OptionsReadableType* optsReadable_;

}; // class InputCheck
} // namespace Spf

/*@}*/
#endif
