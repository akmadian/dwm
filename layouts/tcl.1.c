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

	mw = m->mfact * m->ww; // Monitor width
	sw = (m->ww - mw) / 2; // split width
	bdw = (2 * c->bw);
	resize(c,
	       n < 3 ? m->wx + gappoh : m->wx + sw + gappoh,
	       m->wy + gappov*oe,
	       n == 1 ? m->ww - bdw : mw - bdw - gappoh,
	       m->wh - bdw - gappov*oe,
	       False);

	if (--n == 0)
		return;

	w = (m->ww - mw) / ((n > 1) + 1);
	c = nexttiled(c->next);

	if (n > 1)
	{
		x = m->wx + ((n > 1) ? mw - gappoh * 2 + sw : mw - gappoh * 2);
		y = m->wy;
		h = m->wh / (n / 2);

		if (h < bh)
			h = m->wh;

		for (i = 0; c && i < n / 2; c = nexttiled(c->next), i++)
		{
			resize(c,
			       x + gappoh,
			       y + gappov*oe,
			       w - bdw - gappoh,
			       (i + 1 == n / 2) ? m->wy + m->wh - y - bdw - gappov*oe*i : h - bdw - gappov*oe,
			       False);

			if (h != m->wh)
				y = c->y + HEIGHT(c);
		}
	}

	x = (n + 1 / 2) == 1 ? mw : m->wx;
	y = m->wy;
	h = m->wh / ((n + 1) / 2);

	if (h < bh)
		h = m->wh;

	for (i = 0; c; c = nexttiled(c->next), i++)
	{
		resize(c,
		       x + gappoh,
		       y + gappov*oe,
		       (i + 1 == (n + 1) / 2) ? w - bdw : w - bdw,
		       (i + 1 == (n + 1) / 2) ? m->wy + m->wh - y - bdw - gappov*oe*i : h - bdw - gappov*oe,
		       False);

		if (h != m->wh)
			y = c->y + HEIGHT(c);
	}
}

