// Manage event handling inside a Canvas similar to the DOM Level 3 Event Model
//
// Copyright (C) 2012  Thomas Geymayer <tomgey@gmail.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA

#ifndef CANVAS_EVENT_MANAGER_HXX_
#define CANVAS_EVENT_MANAGER_HXX_

#include "canvas_fwd.hxx"
#include <deque>

namespace simgear
{
namespace canvas
{

  struct EventTarget
  {
    ElementWeakPtr  element;
    osg::Vec2f      local_pos,
                    local_delta;
  };
  typedef std::deque<EventTarget> EventPropagationPath;

  class EventManager
  {
    public:
      EventManager();

      bool handleEvent( const MouseEventPtr& event,
                        const EventPropagationPath& path );

    protected:
      struct StampedPropagationPath
      {
        StampedPropagationPath():
          time(0)
        {}

        StampedPropagationPath(const EventPropagationPath& path, double time):
          path(path),
          time(time)
        {}

        EventPropagationPath path;
        double time;
      };

      // TODO if we really need the paths modify to not copy around the paths
      //      that much.
      StampedPropagationPath _last_mouse_down,
                             _last_click;
      size_t _current_click_count;

      /**
       * Propagate click event and handle multi-click (eg. create dblclick)
       */
      void handleClick( const MouseEventPtr& event,
                        const EventPropagationPath& path );

      bool propagateEvent( const EventPtr& event,
                           const EventPropagationPath& path );

      /**
       * Check if two click events (either mousedown/up or two consecutive
       * clicks) are inside a maximum distance to still create a click or
       * dblclick event respectively.
       */
      bool checkClickDistance( const EventPropagationPath& path1,
                               const EventPropagationPath& path2 ) const;
      EventPropagationPath
      getCommonAncestor( const EventPropagationPath& path1,
                         const EventPropagationPath& path2 ) const;
  };

} // namespace canvas
} // namespace simgear

#endif /* CANVAS_EVENT_MANAGER_HXX_ */