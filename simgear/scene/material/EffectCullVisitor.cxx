// Copyright (C) 2008  Timothy Moore timoore@redhat.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <osg/StateSet>

#include "EffectCullVisitor.hxx"

#include "EffectGeode.hxx"
#include "Effect.hxx"
#include "Technique.hxx"

namespace simgear
{

using osgUtil::CullVisitor;

EffectCullVisitor::EffectCullVisitor()
{
}

EffectCullVisitor::EffectCullVisitor(const EffectCullVisitor& rhs) :
    CullVisitor(rhs)
{
}

CullVisitor* EffectCullVisitor::clone() const
{
    return new EffectCullVisitor(*this);
}

void EffectCullVisitor::apply(osg::Geode& node)
{
    if (isCulled(node))
        return;
    EffectGeode *eg = dynamic_cast<EffectGeode*>(&node);
    if (!eg) {
        CullVisitor::apply(node);
        return;
    }
    Technique* technique = eg->getEffect()->chooseTechnique(&getRenderInfo());
    if (!technique) {
        CullVisitor::apply(node);
        return;
    }
    // push the node's state.
    osg::StateSet* node_state = node.getStateSet();
    if (node_state)
        pushStateSet(node_state);
    for (EffectGeode::DrawablesIterator beginItr = eg->drawablesBegin(),
             e = eg->drawablesEnd();
         beginItr != e;
         beginItr = technique->processDrawables(beginItr, e, this,
                                                eg->isCullingActive()))
        ;
    // pop the node's state off the geostate stack.
    if (node_state)
        popStateSet();

}
}