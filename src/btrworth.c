/*

    Butterworth effect file for SoX
    Copyright (C) 1999 Jan Paul Schmidt <jps@fundament.org>

    This source code is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This source code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Code based on the butterworth implementation in

    Sound Processing Kit - A C++ Class Library for Audio Signal Processing
    Copyright (C) 1995-1998 Kai Lassfolk

    as described in

    Computer music: synthesis, composition, and performance
    Charles Dodge, Thomas A. Jerse
    [2nd ed.]
    Page 214

 */

#include <math.h>

#include "st.h"
#include "btrworth.h"

int st_butterworth_start (effp)
eff_t effp;
{
  butterworth_t butterworth = (butterworth_t) effp->priv;

  butterworth->x [0] = 0.0;
  butterworth->x [1] = 0.0;
  butterworth->y [0] = 0.0;
  butterworth->y [1] = 0.0;
  return (ST_SUCCESS);
}

int st_butterworth_flow (effp, ibuf, obuf, isamp, osamp)
eff_t effp;
LONG *ibuf, *obuf;
LONG *isamp, *osamp;
{
  butterworth_t butterworth = (butterworth_t) effp->priv;

  double in;
  double out;

  int len;
  int done;

  len = ((*isamp > *osamp) ? *osamp : *isamp);

  for (done = 0; done < len; done++) {
    in = *ibuf++;

    /*
     * Substituting butterworth->a [x] and butterworth->b [x] with
     * variables, which are set outside of the loop, did not increased
     * speed on my AMD Box. GCC seems to do a good job :o)
     */

    out =
      butterworth->a [0] * in +
      butterworth->a [1] * butterworth->x [0] +
      butterworth->a [2] * butterworth->x [1] -
      butterworth->b [0] * butterworth->y [0] -
      butterworth->b [1] * butterworth->y [1];

    butterworth->x [1] = butterworth->x [0];
    butterworth->x [0] = in;
    butterworth->y [1] = butterworth->y [0];
    butterworth->y [0] = out;

    if (out < -2147483647.0) {
      out = -2147483647.0;
    }
    else if (out > 2147483647.0) {
      out = 2147483647.0;
    }

    *obuf++ = out;
  }

  *isamp = len;
  *osamp = len;
  return (ST_SUCCESS);
}

