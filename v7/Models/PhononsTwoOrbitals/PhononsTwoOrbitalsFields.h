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

/*! \file PhononsTwoOrbitalsFields.h
 *
 *  Wrapper around classical spin, to say that only spin is a MC variable
 *
 */
#ifndef PHONONS_2ORB_FIELDS_H
#define PHONONS_2ORB_FIELDS_H
#include "../../ClassicalFields/SpinOperations.h"
#include "../../ClassicalFields/PhononOperations.h"
#include "loki/Typelist.h"

namespace Spf {
template<typename FieldType,typename GeometryType>
class PhononsTwoOrbitalsFields {

public:

	typedef ClassicalSpinOperations<GeometryType,FieldType> SpinOperationsType;
	typedef typename SpinOperationsType::SpinType SpinType;
	typedef PhononOperations<GeometryType,FieldType> PhononOperationsType;
	typedef typename PhononOperationsType::PhononType PhononType;

	typedef LOKI_TYPELIST_2(SpinOperationsType, PhononOperationsType) OperationsList;

	template<typename SomeParamsType>
	PhononsTwoOrbitalsFields(SizeType vol,const SomeParamsType& params)
	    : spin_(vol, params), phonon_(vol, params.dynvarsfile, params.options)
	{}

	PhononsTwoOrbitalsFields(const SpinType& spin,const PhononType& phonon) :
	    spin_(spin),phonon_(phonon)
	{
	}

	const PsimagLite::String& name(SizeType i) const { return name_[i]; }

	void getField(SpinType const** field,SizeType i) const
	{
		assert(i == 0);
		*field = &spin_;
	}

	void getField(PhononType const** field,SizeType i) const
	{
		assert(i == 1);
		*field = &phonon_;
	}

	void getField(SpinType** field,SizeType i)
	{
		assert(i == 0);
		*field = &spin_;
	}

	void getField(PhononType** field,SizeType i)
	{
		assert(i == 1);
		*field = &phonon_;
	}

	template<typename FieldType2,typename GeometryType2>
	friend std::ostream& operator<<(std::ostream& os,
	                                const PhononsTwoOrbitalsFields<FieldType2,GeometryType2>& f);

private:
	static PsimagLite::Vector<PsimagLite::String>::Type name_;
	SpinType spin_;
	PhononType phonon_;

}; // PhononsTwoOrbitalsFields

template<typename FieldType,typename GeometryType>
std::ostream& operator<<(std::ostream& os,
                         const PhononsTwoOrbitalsFields<FieldType,GeometryType>& f)
{
	os<<f.spin_;
	os<<f.phonon_;
	return os;
}
template<typename FieldType,typename GeometryType>
PsimagLite::Vector<PsimagLite::String>::Type PhononsTwoOrbitalsFields<FieldType,GeometryType>::name_(2);

} // namespace Spf

/*@}*/
#endif
