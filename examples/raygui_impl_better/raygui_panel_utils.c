#include "raygui.h"

#include "../../split_panel.h"

#define ANCHOR_BOX_CORNER_SIZE 20
#define EDGE_RESIZE_OFFSET 5

Rectangle GetPanelRectangle(Panel panel) {
    return (Rectangle){*panel.origin.x, *panel.origin.y, (*panel.end.x - *panel.origin.x), (*panel.end.y - *panel.origin.y)};
}

Rectangle GetPanelAchorRectangle(Panel panel) {
    return (Rectangle){*panel.end.x - ANCHOR_BOX_CORNER_SIZE, *panel.origin.y, ANCHOR_BOX_CORNER_SIZE, ANCHOR_BOX_CORNER_SIZE};
}

int IsPanelHovered(Panel panel) {
    return CheckCollisionPointRec((Vector2) {GetMouseX(), GetMouseY()}, GetPanelRectangle(panel));
}

int IsPanelAnchorHovered(Panel panel) {
    return CheckCollisionPointRec((Vector2) {GetMouseX(), GetMouseY()}, GetPanelAchorRectangle(panel));
}

int IsPanelReadyToResize(Panel panel) {
    Rectangle rect_out = GetPanelRectangle(panel);
    Rectangle rect_in = {rect_out.x + EDGE_RESIZE_OFFSET, rect_out.y + EDGE_RESIZE_OFFSET, rect_out.width - 2*EDGE_RESIZE_OFFSET, rect_out.height - 2*EDGE_RESIZE_OFFSET};
    Vector2 mouse = {GetMouseX(), GetMouseY()};
    
    return CheckCollisionPointRec(mouse, rect_out) && !CheckCollisionPointRec(mouse, rect_in);
}

typedef enum PanelEdge {
    PANEL_EDGE_LEFT,
    PANEL_EDGE_RIGHT,
    PANEL_EDGE_TOP,
    PANEL_EDGE_BOTTOM
} PanelEdge;

PanelEdge GetClosestEdgeToPointer(Panel panel) {
    Rectangle rect = GetPanelRectangle(panel);
    Vector2 point = GetMousePosition();

    double center_x = rect.x + rect.width / 2;
    double center_y = rect.y + rect.height / 2;

    double slope1 = rect.height / rect.width;
    double slope2 = -slope1;

    double intercept1 = center_y - slope1 * center_x;
    double intercept2 = center_y - slope2 * center_x;

    double y1 = slope1 * point.x + intercept1;
    double y2 = slope2 * point.x + intercept2;

    if (point.y < y1 && point.y < y2) {
        return PANEL_EDGE_TOP;
    } else if (point.y > y1 && point.y > y2) {
        return PANEL_EDGE_BOTTOM;
    } else if (point.y < y1 && point.y > y2) {
        return PANEL_EDGE_RIGHT;
    } else {
        return PANEL_EDGE_LEFT;
    }
}

