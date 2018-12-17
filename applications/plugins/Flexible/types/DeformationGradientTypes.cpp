/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#define FLEXIBLE_DeformationGradientTYPES_CPP

#include <Flexible/config.h>
#include "../types/DeformationGradientTypes.h"
#include <sofa/core/ObjectFactory.h>

#include <SofaBaseMechanics/MechanicalObject.inl>
#include <sofa/defaulttype/TemplatesAliases.h>
#include <sofa/core/State.inl>

namespace sofa
{

using namespace sofa::defaulttype;


namespace core
{

template class SOFA_Flexible_API State<F331dTypes>;
template class SOFA_Flexible_API State<F321dTypes>;
template class SOFA_Flexible_API State<F311dTypes>;
template class SOFA_Flexible_API State<F332dTypes>;
template class SOFA_Flexible_API State<F221dTypes>;

} // namespace core


namespace component
{
namespace container
{

// ==========================================================================
// Instanciation

int DefGradientMechanicalObjectClass = core::RegisterObject ( "mechanical state vectors" )
        .add< MechanicalObject<F331Types> >()
        .add< MechanicalObject<F321Types> >()
        .add< MechanicalObject<F311Types> >()
        .add< MechanicalObject<F332Types> >()
        .add< MechanicalObject<F221Types> >()
		;

template class SOFA_Flexible_API MechanicalObject<F331Types>;
template class SOFA_Flexible_API MechanicalObject<F321Types>;
template class SOFA_Flexible_API MechanicalObject<F311Types>;
template class SOFA_Flexible_API MechanicalObject<F332Types>;
template class SOFA_Flexible_API MechanicalObject<F221Types>;

static RegisterTemplateAlias alias0("F331", F331Types::Name() );
static RegisterTemplateAlias alias1("F332", F332Types::Name() );
static RegisterTemplateAlias alias2("F321", F321Types::Name() );
static RegisterTemplateAlias alias3("F311", F331Types::Name() );
static RegisterTemplateAlias alias4("F332", F332Types::Name() );
static RegisterTemplateAlias alias5("F221", F221Types::Name() );

} // namespace container
} // namespace component
} // namespace sofa
