// conversational lathe menu
#include "../../inc/MarlinConfigPre.h"

//#if HAS_MARLINUI_MENU

#include "menu_item.h"
#include "menu_addon.h"

#include "../../module/motion.h"
#include "../../gcode/gcode.h" // for inch support
#include "../../gcode/queue.h"
#include "../../module/temperature.h"
#include "../../module/conversational.h"

#if ENABLED(DELTA)
  #include "../../module/delta.h"
#endif

// Always show configurable options regardless of FT Motion active
//#define FT_MOTION_NO_MENU_TOGGLE

constexpr bool has_large_area() {
  return TERN0(HAS_X_AXIS, (X_BED_SIZE) >= 1000) || TERN0(HAS_Y_AXIS, (Y_BED_SIZE) >= 1000) || TERN0(HAS_Z_AXIS, (Z_MAX_POS) >= 1000);
}


extern Conversational conversational; 
uint8_t Conversational::taper_angle; 
uint8_t Conversational::radius_radius; 
float Conversational::feed_speed; 
float Conversational::part_diameter; 
int8_t Conversational::x_offset; 
int8_t Conversational::y_offset; 

void on_offset_update(float offset, char axis){
    char offset_buf[30] = { 0 };
    char str_1[16];

    if (axis=='X') sprintf_P(offset_buf, PSTR("G92 X%s"), dtostrf(offset, 1, 0, str_1));
    else if (axis=='Y') sprintf_P(offset_buf, PSTR("G92 Y%s"), dtostrf(offset, 1, 0, str_1));

    gcode.process_subcommands_now(F(offset_buf));

    char offset_debug[30] = {0}; 
    sprintf_P(offset_debug, PSTR("M118 E1 %s"), offset_buf); 
    queue.inject(F(offset_debug)); 
}

void _menu_taper (){
    START_MENU(); 
    BACK_ITEM(MSG_TAPER); 
    EDIT_ITEM_FAST(uint8, MSG_TAPER_ANGLE, &conversational.taper_angle, 0, 90, conversational.on_taper_update); 
    EDIT_ITEM_FAST(float42_52, MSG_PART_DIAMETER, &conversational.part_diameter, 0, 2, conversational.on_taper_update); 
    EDIT_ITEM_FAST(float42_52, MSG_CONVERSATIONAL_FEED, &conversational.feed_speed, 0, 90, conversational.on_taper_update); 

    ACTION_ITEM(MSG_MAKE_TAPER, conversational.make_taper); 
    
    END_MENU(); 
}

void _menu_radius (){
    START_MENU(); 
    BACK_ITEM(MSG_RADIUS); 
    EDIT_ITEM_FAST(uint8, MSG_RADIUS_RADIUS, &conversational.radius_radius, 0, 90, conversational.on_radius_update); 
    EDIT_ITEM_FAST(float42_52, MSG_PART_DIAMETER, &conversational.part_diameter, 0, 2, conversational.on_radius_update); 
    EDIT_ITEM_FAST(float42_52, MSG_CONVERSATIONAL_FEED, &conversational.feed_speed, 0, 90, conversational.on_radius_update); 

    ACTION_ITEM(MSG_MAKE_RADIUS, conversational.make_radius); 

    END_MENU(); 
}

void _menu_offsets(){
    START_MENU(); 
    BACK_ITEM(MSG_SET_OFFSETS); 

    EDIT_ITEM_FAST(int8, MSG_X_OFFSET, &conversational.x_offset, -10, 10,  []{ on_offset_update(conversational.x_offset, 'X');}); 
    EDIT_ITEM_FAST(int8, MSG_Y_OFFSET, &conversational.y_offset, -10, 10,  []{ on_offset_update(conversational.y_offset, 'Y');}); 

    END_MENU(); 
}


/*
void make_taper(){

    // Get the new position
    const float diff = float(int32_t(ui.encoderPosition));
    //(void)ui.manual_move.apply_diff(axis, diff, 0, 90);
    //ui.manual_move.soon(axis);
    ui.refresh(LCDVIEW_REDRAW_NOW);

    // insert code to move motors here

    ui.encoderPosition = 0;
    if (ui.should_draw()) {
        MenuEditItemBase::itemIndex = axis;
        const float pos = ui.manual_move.axis_value(axis);
        MenuEditItemBase::draw_edit_screen(GET_TEXT_F(MSG_TAPER_ANGLE), ui.manual_move.menu_scale >= 0.1f ? (has_large_area() ? ftostr51sign(pos) : ftostr41sign(pos)) : ftostr63(pos));
    }
}

void make_radius(){
    // Get the new position
    const float diff = float(int32_t(ui.encoderPosition));
    //(void)ui.manual_move.apply_diff(axis, diff, -10, 10);
    //ui.manual_move.soon(axis);
    ui.refresh(LCDVIEW_REDRAW_NOW);
}
*/

// uint8_t taper_angle; 
// #define _EDIT_TAPER() do{ \
//     editable.uint8 = taper_angle; \
//     EDIT_ITEM_FAST(uint8, MSG_TAPER_ANGLE, &editable.uint8, 0, 90, on_taper_update);  \ 
// }while(0)

// uint8_t radius_radius; 
// #define _EDIT_RADIUS() do{ \
//     editable.uint8 = radius_radius; \
//     EDIT_ITEM_FAST(uint8, MSG_TAPER_ANGLE, &editable.uint8, 0, 90, on_radius_update);  \ 
// }while(0)

void menu_conversational() {
    START_MENU();
    BACK_ITEM(MSG_CONVERSATIONAL); 
    SUBMENU(MSG_SET_OFFSETS, _menu_offsets); 
    SUBMENU(MSG_TAPER, _menu_taper );
    SUBMENU(MSG_RADIUS, _menu_radius );
    END_MENU (); 
}
