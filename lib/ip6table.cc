/*
 * ip6table.{cc,hh} -- a stupid IP6 routing table, best for small routing tables
 * Peilei Fan, Robert Morris
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>

#include <click/ip6table.hh>

IP6Table::IP6Table()
{
}

IP6Table::~IP6Table()
{
}

bool
IP6Table::lookup(const IP6Address &dst, IP6Address &gw, int &index) const
{
  int best = -1;

  for (int i = 0; i < _v.size(); i++)
    if (_v[i]._valid && dst.matches_prefix(_v[i]._dst, _v[i]._mask)) {
      if (best < 0 || _v[i]._mask.mask_more_specific(_v[best]._mask))
        best = i;
    }

  if (best < 0)
    return false;
  else {
    gw = _v[best]._gw;
    index = _v[best]._index;
    return true;
  }
}

void
IP6Table::add(const IP6Address &dst, const IP6Address &mask,
	      const IP6Address &gw, int index)
{
  struct Entry e;

  e._dst = dst;
  e._mask = mask;
  e._gw = gw;
  e._index = index;
  e._valid = 1;

  for (int i = 0; i < _v.size(); i++)
    if (!_v[i]._valid) {
      _v[i] = e;
      return;
    }
  _v.push_back(e);
}

void
IP6Table::del(const IP6Address &dst, const IP6Address &mask)
{
  for (int i = 0; i < _v.size(); i++)
    if (_v[i]._valid && (_v[i]._dst == dst) && (_v[i]._mask == mask))
      _v[i]._valid = 0;
}

// generate Vector template instance
#include <click/vector.cc>
template class Vector<IP6Table::Entry>;
