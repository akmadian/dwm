void
tcl(Monitor * m)
{
	int x, y, h, w, mw, sw, bdw;
	int oe = enablegaps;
	int ie = enablegaps;
	unsigned int i, n;

	Client * c;

	for (n = 0, c = nexttiled(m->clients); c;
	        c = nexttiled(c->next), n++);

	if (n == 0)
		return;

	c = nexttiled(m->clients);

	mw = m->mfact * m->ww;
	sw = (m->ww - mw) / 2;
	bdw = (2 * c->bw);
	resize(c,                               // client
	       n < 3 ? m->wx + m->gappih*ie : m->wx + sw + m->gappih*ie,      // client x
	       m->wy + m->gappoh*oe,            // client y
	       n == 1 ? m->ww - bdw : mw - bdw, // client w
	       m->wh - bdw - m->gappov*oe - m->gappiv*ie,                     // client h
	       False);

	if (--n == 0)
		return;

	w = (m->ww - mw) / ((n > 1) + 1);
	c = nexttiled(c->next);

	if (n > 1)
	{
		x = m->wx + ((n > 1) ? mw + sw : mw);
		y = m->wy;
		h = m->wh / (n / 2);

		if (h < bh)
			h = m->wh;

		for (i = 0; c && i < n / 2; c = nexttiled(c->next), i++)
		{
			resize(c,                                                     // client
			       x + m->gappoh*oe + m->gappih*ie,																			// client x
			       y + m->gappov*oe,																			// client y
			       w - bdw - m->gappih*ie,                                // client w
			       (i + 1 == n / 2) ? m->wy + m->wh - y - bdw : h - bdw - m->gappiv*ie - m->gappov*oe,  // client h
			       False);

			if (h != m->wh)
				y = c->y + HEIGHT(c);
		}
	}

	x = (n + 1 / 2) == 1 ? mw : m->wx + m->gappih*ie;
	y = m->wy + m->gappih*ie;
	h = (m->wh / ((n + 1) / 2)) - m->;

	if (h < bh)
		h = m->wh;

	for (i = 0; c; c = nexttiled(c->next), i++)
	{
		resize(c,
		       x + m->gappoh*oe,
		       y + m->gappov*oe,
		       (i + 1 == (n + 1) / 2) ? w - bdw - m->gappih*ie + m->gappoh*oe : w - bdw - m->gappih*ie - m->gappoh*oe,
		       (i + 1 == (n + 1) / 2) ? m->wy + m->wh - y - bdw : h - bdw - m->gappiv*ie - m->gappov*oe,
		       False);

		if (h != m->wh)
			y = c->y + HEIGHT(c);
	}
}
