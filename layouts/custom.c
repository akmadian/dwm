#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Client *
nexttiledcust(Client *c) {
  for (; c && (c->isfloating || !ISVISIBLE(c)); c = c->next);
  return c;
}

void
custom(Monitor *m) {
  unsigned int i, n, h, my, ty;
  Client *c;

  //fprintf(stderr, "CUSTOM LAYOUT START\n");
  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
  //fprintf(stderr, "\tMONITOR %i INFO - mx: %i my: %i mw: %i mh: %i - n: %i - nmaster: %i\n", m->num, m->mx, m->my, m->mw, m->mh, n, m->nmaster);
  //fprintf(stderr, "\t                - wx: %i wy: %i ww: %i wh: %i - n: %i - nmaster: %i\n", m->wx, m->wy, m->ww, m->wh, n, m->nmaster);

  //fprintf(stderr, "\tMONITOR %i INFO - mx: %i my: %i mw: %i mh: %i - n: %i - nmaster: %i\n", m->next->num, m->next->mx, m->next->my, m->next->mw, m->mh, n, m->nmaster);
  //fprintf(stderr, "\tMONITOR %i quarter size: %i\n", m->num, monitor_quarter_size);

  int master_width = m->ww * mfact;
  int sides_width = m->ww * (mfact/2);

  if (n == 0)
   		return;

  int right_side_height_offset = m->wy;
  int left_side_height_offset = m->wy;

  int n_clients_non_master = n - 1;
  int n_clients_right = n_clients_non_master / 2;
  int n_clients_left = n_clients_non_master / 2;

  if (n_clients_right + n_clients_left + 1 != n) {
    n_clients_right++;
  }

  //fprintf(stderr, "\tSPLIT DATA - total: %i   %i | %i | %i\n", n, n_clients_left, 1, n_clients_right);
  //fprintf(stderr, "\t             factors: %d | %d | %d\n\n", sides_width, master_width, sides_width);

  int n_processed_clients_right = 0;
  int n_processed_clients_left  = 0;

  // resize(Client *c, int x, int y, int w, int h, int interact)
  for (i = my = ty = 0, c = nexttiledcust(m->clients); c; c = nexttiledcust(c->next), i++) {
    //fprintf(stderr, "\tCLIENT INFO - %s i: %i n: %i ismaster: %i\n", c->name, i, n, i == m->nmaster);
    if (i == m->nmaster) {
      resize(c,
          n == 2 ? m->wx + gappoh : m->wx + sides_width + gappoh,
          m->wy + gappov,
          n == 2 ? m->ww*mfact - gappoh : master_width*.80 + gappih,
          m->wh - gappov*2,
          0
          );
    } else {
      // RIGHT COLUMN
      if (n_processed_clients_right + 1 <= n_clients_right) {
        //fprintf(stderr, "\t\tProcessing non-master window to right: %s\n", c->name);
        h = m->wh / n_clients_right;
        resize(c,
          n == 2 ? m->wx + gappoh + m->mw*mfact : m->wx + sides_width + master_width*.80 + gappih*3,
          right_side_height_offset + gappov,
          n == 2 ? (m->ww*(1-mfact)) - gappoh*2 : sides_width - gappoh,
          h - gappov*2,
          0
          );
        right_side_height_offset += h;
        n_processed_clients_right++;
        //fprintf(stderr, "\t\tN Processed - left: %i right: %i\n", n_processed_clients_left, n_processed_clients_right);
        continue;
      }
        //else
          //fprintf(stderr, "\t\tFinished sending clients to right split.\n");

      // LEFT COLUMN
      if (n_processed_clients_left <= n_clients_left) {
        //fprintf(stderr, "\t\tProcessing non-master window to left: %s\n", c->name);
        h = m->wh / n_clients_left;
        resize(c,
          m->wx + gappoh,
          left_side_height_offset + gappov,
          sides_width - gappoh,
          h - gappov*2,
          0
          );
        left_side_height_offset += h;
        n_processed_clients_left++;
        //fprintf(stderr, "\t\tN Processed - left: %i right: %i\n", n_processed_clients_left, n_processed_clients_right);
        continue;
      }
        //else
          //fprintf(stderr, "\t\tFinished sending clients to left split.\n");

    }
  }
}

