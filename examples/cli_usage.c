/******************************************************************\
 * CLI usage of split panel system.
 *
 * How to use:
 *  - To create a split: type 'S'(uppercase) followed by panel-id
 *      then 'V'(uppercase) or 'H'(uppercase) for vertical or horizontal
 *      split. Wrong input for split type will result in no split.
 *     Example: 'S 2 H' Will split second panel horizontally.
 *
 *  - To modify a split: type 'M'(capital) folowed by panel-id
 *      then choose the point to modify 'o'(lowercase) for `origin`
 *      or 'e'(lowercase) for `end`. Then x or y for choosing
 *      'x' or 'y' coordinate. Followed by its value.
 *     Example: 'M 3 o y 10.0' Will set the y ordinate of thrid
 *      panel's origin to `10.0f` (float).
 *  - Look in end of this file for an example run.
 *
 * Authors:
 *  - Kirandeep Singh (@Kirandeep-Singh-Khehra)
 *
\******************************************************************/

#include <stdio.h>

#include "../split_panel.h"

/****** HELPERS ******/
void PrintPoint(Point v) {
  printf("(%f, %f)", *v.x, *v.y);
}

void PrintPanel(Panel p) {
  printf("[");
  PrintPoint(*p.origin);
  PrintPoint(*p.end);
  printf("]\n");
}

/****** DRIVER ******/
void cli_test() {
  Panel panels[10];
  panels[0] = CreatePanel(CreatePoint(0.0f, 0.0f), CreatePoint(20.0f, 20.0f));
  int panels_len = 1;

  while (1) {
    for (int i = 0; i < panels_len; i++) {
      printf("%d:", i);
      PrintPanel(panels[i]);
    }
    printf("-----\n - Add Split: S panel-id [H/V]\n - Modify Split: M panel-id [o/e] [x/y] amount\n > ");
    char choice;
    int panelid;
    char c2, c3;
    float amt;
    scanf(" %c", &choice);
    switch (choice) {
      case 'S':
        scanf("%d", &panelid);
        scanf(" %c", &c2);
        c2 = (c2 == 'V')? PANEL_SPLIT_TYPE_VERTICAL : (c2 == 'H')? PANEL_SPLIT_TYPE_HORIZONTAL : c2;
        Panel *p = SplitPanel(&panels[panelid], c2);
        if (p != NULL) {
          panels[panels_len++] = *p;
        }
        break;
      case 'M':
        scanf("%d", &panelid);
        scanf(" %c", &c2);
        scanf(" %c", &c3);
        scanf("%f", &amt);

        switch (c2) {
          case 'o':
                    switch (c3) {
                      case 'x':
                        *panels[panelid].origin->x = amt;
                        break;
                      case 'y':
                        *panels[panelid].origin->y = amt;
                        break;
                    }
          case 'e':
                    switch (c3) {
                      case 'x':
                        *panels[panelid].end->x = amt;
                        break;
                      case 'y':
                        *panels[panelid].end->y = amt;
                        break;
                    }
          
        }

        break;

      default:
        printf("Exiting ...\n");
        exit(EXIT_SUCCESS);
    }
  }
}

int main() {
  cli_test();
  return 0;
}

/*
0:[(0.000000, 0.000000)(20.000000, 20.000000)]
-----
 - Add Split: S panel-id [H/V]
 - Modify Split: M panel-id [o/e] [x/y] amount
 > S 0 H
0:[(0.000000, 0.000000)(20.000000, 10.000000)]
1:[(0.000000, 10.000000)(20.000000, 20.000000)]
-----
 - Add Split: S panel-id [H/V]
 - Modify Split: M panel-id [o/e] [x/y] amount
 > S 1 V
0:[(0.000000, 0.000000)(20.000000, 10.000000)]
1:[(0.000000, 10.000000)(10.000000, 20.000000)]
2:[(10.000000, 10.000000)(20.000000, 20.000000)]
-----
 - Add Split: S panel-id [H/V]
 - Modify Split: M panel-id [o/e] [x/y] amount
 > M 1 e x 12.0
0:[(0.000000, 0.000000)(20.000000, 10.000000)]
1:[(0.000000, 10.000000)(12.000000, 20.000000)]
2:[(12.000000, 10.000000)(20.000000, 20.000000)]
-----
 - Add Split: S panel-id [H/V]
 - Modify Split: M panel-id [o/e] [x/y] amount
 > e
Exiting ..
*/
