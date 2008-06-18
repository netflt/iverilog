/*
 * Copyright (c) 1998-2008 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

# include "config.h"

# include  "Module.h"
# include  "PGate.h"
# include  "PWire.h"
# include  <assert.h>

/* n is a permallocated string. */
Module::Module(perm_string n)
: PScope(n)
{
      library_flag = false;
      default_nettype = NetNet::NONE;
}

Module::~Module()
{
}

void Module::add_gate(PGate*gate)
{
      gates_.push_back(gate);
}

void Module::add_task(perm_string name, PTask*task)
{
      tasks_[name] = task;
}

void Module::add_function(perm_string name, PFunction *func)
{
      funcs_[name] = func;
}

unsigned Module::port_count() const
{
      return ports.count();
}

/*
 * Return the array of PEIdent object that are at this port of the
 * module. If the port is internally unconnected, return an empty
 * array.
 */
const svector<PEIdent*>& Module::get_port(unsigned idx) const
{
      assert(idx < ports.count());
      static svector<PEIdent*> zero;

      if (ports[idx])
	    return ports[idx]->expr;
      else
	    return zero;
}

unsigned Module::find_port(const char*name) const
{
      assert(name != 0);
      for (unsigned idx = 0 ;  idx < ports.count() ;  idx += 1) {
	    if (ports[idx] == 0) {
		    /* It is possible to have undeclared ports. These
		       are ports that are skipped in the declaration,
		       for example like so: module foo(x ,, y); The
		       port between x and y is unnamed and thus
		       inaccessible to binding by name. */
		  continue;
	    }
	    assert(ports[idx]);
	    if (ports[idx]->name == name)
		  return idx;
      }

      return ports.count();
}


PGate* Module::get_gate(perm_string name)
{
      for (list<PGate*>::iterator cur = gates_.begin()
		 ; cur != gates_.end()
		 ; cur ++ ) {

	    if ((*cur)->get_name() == name)
		  return *cur;
      }

      return 0;
}

const list<PGate*>& Module::get_gates() const
{
      return gates_;
}

